#ifdef __linux__

/**
 * \file
 * \brief Contains udp input class implementation.
 */

// ============================================================================

#include <sys/epoll.h>

#include <map>
#include <functional>
#include <cassert>

#include <modules/udp_input/udp_input.h>

#ifdef USE_QT_GUI

#include <QTextEdit>

#endif

// ============================================================================

using namespace Components;

// ============================================================================

class Poller {
  public:
    using Callback = std::function<void()>;

  public:
    Poller() {
        epoll_fd = epoll_create1(0);

        assert(epoll_fd != -1);
    }

    void Register(int fd, Callback callback) {
        epoll_event event;

        event.events = EPOLLIN;
        event.data.fd = fd;

        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);

        callbacks_.insert({fd, std::move(callback)});
    }

    void Unregister(int fd) {
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        
        callbacks_.erase(fd);
    }

    size_t Wait() {
        epoll_event events[10];

        auto ready_fds = epoll_wait(epoll_fd, events, 10, 500);
        assert(ready_fds != -1);

        for (int i = 0; i < ready_fds; i++) {
          callbacks_[events[i].data.fd]();
        }

        return ready_fds;
    }

    ~Poller() {
        close(epoll_fd);
    }

  private:
    int epoll_fd;
    std::map<int, Callback> callbacks_;
};

// ============================================================================

static Poller poller;

// ============================================================================

UdpInput::UdpInput(Config cfg, IBlockCommunicator* next)
: client_(cfg.host), next_(next) {
  client_.SetRecvTimeout(0);
  
  poller.Register(client_.GetSocket(), [this](){
    OnReceive();
  });
}

sockaddr_in UdpInput::GetHostAddress() const { return client_.GetHostAddress(); }

size_t UdpInput::Run() {
    return poller.Wait();
}

void UdpInput::SetNext(IBlockCommunicator* next) {
    next_ = next;
}

void UdpInput::SetConfig(const nlohmann::json& cfg) {
    auto addr = cfg["host_addr"].get<std::string>();
    if (cfg.contains("host_port")) throw std::runtime_error("Переделайте это легаси");

    auto new_host = UDPAddr(addr);

    poller.Unregister(client_.GetSocket());

    {
        std::lock_guard lock(config_lock_);
        client_.Rebind(new_host);

        poller.Register(client_.GetSocket(), [this](){
            OnReceive();
        });
    }
}

nlohmann::json UdpInput::GetConfig () const {
    std::lock_guard lock(config_lock_);

    sockaddr_in host = client_.GetHostAddress();

    nlohmann::json config = {
        {"host_addr", UDPAddr(host).ToString()},
    };

    return config;
}

void UdpInput::OnReceive() {
    Packet pkg(PacketHeader(0));

    {
        std::lock_guard lock(config_lock_);
        client_.Receive(pkg);
    }

    Send(next_, std::move(pkg));
}

UdpInput::~UdpInput() {
    poller.Unregister(client_.GetSocket());
}

#ifdef USE_QT_GUI

DefaultSettingsWidget* UdpInput::GetSettingsWidget() {
    DefaultSettingsWidget* widget = new DefaultSettingsWidget();
    widget->setObjectName("config");
    return widget;
}

QWidget* UdpInput::GetStatsWidget() {
    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");
    return stat_widget;
}

#endif
#endif // Linux only