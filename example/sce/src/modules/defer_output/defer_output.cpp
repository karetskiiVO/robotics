/**
 * \file
 * \brief Contains DeferOutput module implementation.
 */

// ============================================================================

#include <chrono>

#include <modules/defer_output/defer_output.h>

#ifdef USE_QT_GUI

#include <QTextEdit>

#endif

// ============================================================================

using namespace Components;

// ============================================================================

bool HasCollision(const Packet& packet1, const Packet& packet2) {
    // WORKAROUND: Если в эмуляции будет два демодулятора с одинаковой
    // принимаемой частотой, то они примут один и тот же пакет и добавят его
    // в локальную очередь Worker. Тогда он будет уничтожен из-за коллизии.
    // Сравнение id позволяет выявить один и тот же пакет и избежать этого.

    if (packet1.header.id == packet2.header.id) {
        return false;
    }

    // TODO: Нужно добавить hashmap по частоте с определенным шагом.
    // Тогда эту проверку можно будет убрать.

    if (packet1.header.carrier_frequency != packet2.header.carrier_frequency) {
        return false;
    }

    if (packet1.header.timepos > packet2.header.timepos) {
        return HasCollision(packet2, packet1);
    }

    auto l1 = packet1.header.timepos;
    auto r1 = l1 + packet1.PacketDuration();

    auto l2 = packet2.header.timepos;
    
    return r1 >= l2;
}

// MSVC moment
#undef GetCurrentTime
MeasureUnits::Time GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto dur = now.time_since_epoch();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(dur);
    return MeasureUnits::Time(ns.count());
}

// ============================================================================

class Worker {
  public:
    Worker() {
        worker_ = std::thread([this] { while (Timeout()); });
    }

    void AddPacket(DeferOutput* host, Packet pkg) {
        std::lock_guard lock(queue_lock_);

        if (inbound_.empty()) {
            on_update_.notify_one();
        }

        inbound_.push_back({std::move(pkg), host});
    }

    bool Timeout() {
        {
            std::unique_lock lock(queue_lock_);
    
            if (local_.empty()) {
                while (inbound_.empty()) {
                    if (!is_running_) {
                        return false;
                    }
    
                    on_update_.wait(lock);
                }
            }
    
            // TODO: Убрать проверку коллизии из критической секции.
    
            for (auto it = inbound_.begin(); it != inbound_.end(); it++) {
                bool has_collision = false;
    
                for (auto jt = local_.begin(); jt != local_.end();) {
                    if (HasCollision(it->pkg, jt->pkg)) {
                        jt = local_.erase(jt);
                        has_collision = true;
                    } else {
                        jt++;
                    }
                }
    
                if (!has_collision) {
                    local_.insert(std::move(*it));
                }
            }
    
            inbound_.clear();
        }
    
        size_t deleted = 0;
        MeasureUnits::Duration new_delay(0);
    
        // TODO: Итератор std::set позволяет удалять прямо во время цикла.
    
        for (auto it = local_.begin(); it != local_.end(); it++) {
            auto curr = GetCurrentTime();
            auto expected = it->pkg.header.timepos;
    
            if (expected <= curr) {
                it->host->OnSend(it->pkg);
                deleted++;
            } else {
                new_delay = expected - curr;
                break;
            }
        }
    
        if (new_delay.asNanoseconds() == 0) {
            local_.clear();
        } else {
            for (size_t i = 0; i < deleted; i++) {
                local_.erase(local_.begin());
            }
        }
    
        std::this_thread::sleep_for(
            std::chrono::nanoseconds(new_delay.asNanoseconds()));
    
        return true;
    }

    ~Worker() {
        {
            std::lock_guard lock(queue_lock_);
    
            is_running_ = false;
    
            on_update_.notify_one();
        }
    
        worker_.join();
    }

  private:
    struct Wrap {
        Packet pkg;
        DeferOutput* host;
    };

    struct PacketCmp {
        bool operator()(const Wrap& a, const Wrap& b) const {
            return a.pkg.header.timepos < b.pkg.header.timepos;
        }
    };

    bool is_running_ = true;

    /// Packages not yet taken by worker.
    std::vector<Wrap> inbound_;

    /// Local batch of the worker.
    std::set<Wrap, PacketCmp> local_;

    /// Worker thread.
    std::thread worker_;

    /// For inbound_ access syncronization.
    mutable std::mutex queue_lock_;

    /// For blocking on empty inbound_ queue.
    std::condition_variable on_update_;
};

// ============================================================================

static Worker worker;

// ============================================================================

DeferOutput::DeferOutput(Config cfg)
    : client_(cfg.host), remote_(cfg.remote) {}

void DeferOutput::OnReceive(Packet pkg) {
    worker.AddPacket(this, std::move(pkg));
}

void DeferOutput::OnSend(const Packet& pkg) {
    std::lock_guard lock(config_lock_);

    client_.Send(pkg, remote_);
}

void DeferOutput::SetConfig(const nlohmann::json& cfg) {
    auto addr = cfg["host_addr"].get<std::string>();

    if (cfg.contains("host_port")) {
        throw std::runtime_error("Invalid JSON format.");
    }

    auto new_host = UDPAddr(addr);

    std::lock_guard lock(config_lock_);

    client_.Rebind(new_host);

    addr = cfg["remote_addr"].get<std::string>();

    remote_ = UDPAddr(addr);
}

nlohmann::json DeferOutput::GetConfig() const {
    std::lock_guard lock(config_lock_);

    sockaddr_in host = client_.GetHostAddress();

    nlohmann::json config = {
        {"host_addr", UDPAddr(host).ToString()},
        {"remote_addr", UDPAddr(remote_).ToString()},
    };

    return config;
}

#ifdef USE_QT_GUI

DefaultSettingsWidget* DeferOutput::GetSettingsWidget() {
    DefaultSettingsWidget* widget = new DefaultSettingsWidget();
    widget->setObjectName("config");
    return widget;
}

QWidget* DeferOutput::GetStatsWidget() {
    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");
    return stat_widget;
}

#endif
