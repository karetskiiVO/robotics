#if defined(_WIN32) || defined(_WIN64)

/**
 * \file
 * \brief Contains udp input class implementation.
 */

// ============================================================================

#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

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
    
    private:
        fd_set readfds;
        std::map<SOCKET, Callback> callbacks_;
        std::mutex mutex_;
    
    public:
        Poller() {
            FD_ZERO(&readfds);
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                throw std::runtime_error("WSAStartup failed: " + std::to_string(WSAGetLastError()));
            }
        }
    
        void Register(SOCKET fd, Callback callback) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (fd == INVALID_SOCKET) {
                throw std::runtime_error("Invalid socket");
            }
            FD_SET(fd, &readfds);
            callbacks_[fd] = std::move(callback);
        }
    
        void Unregister(SOCKET fd) {
            std::lock_guard<std::mutex> lock(mutex_);
            FD_CLR(fd, &readfds);
            callbacks_.erase(fd);
            closesocket(fd);
        }
    
        size_t Wait() {
            fd_set tmpfds;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                tmpfds = readfds;
            }
    
            timeval tv = {0, 500};
            int result = select(0, &tmpfds, nullptr, nullptr, &tv);
            if (result == SOCKET_ERROR) throw std::runtime_error("select failed: " + std::to_string(WSAGetLastError()));
            if (result == 0) return 0;
    
            {
                std::lock_guard<std::mutex> lock(mutex_);
                for (int i = 0; i < result; i++) {
                    SOCKET sock = tmpfds.fd_array[i];
                    if (callbacks_.count(sock)) {
                        callbacks_[sock]();
                    }
                }
            }
            
            return static_cast<size_t>(result);
        }
    
        ~Poller() {
            WSACleanup();
        }
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

#endif