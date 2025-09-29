/**
 * \file
 * \brief Contains DeferOutput module declaration.
 */

#pragma once

// ============================================================================

#include <emulatedblock.h>
#include <interfaces.h>
#include <packet.h>

#include <utils/udp_client/udp_client.h>

#include <condition_variable>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

#ifdef USE_QT_GUI

#include <qtinterfaces.h>

#endif

// ============================================================================

namespace Components {

// ============================================================================

/// Sends received packages via UDP connection and emulates accumulated delay.
class DeferOutput final :
#ifdef USE_QT_GUI
    public IQTWidgetConfigurator,
#endif
    public Component,
    public IReceiver,
    public IConfigurator {
  public:
    struct Config {
        sockaddr_in host;
        sockaddr_in remote;
    };

    DeferOutput(Config cfg);

    void OnReceive(Packet pkg) override;

    /// Configures module via JSON.
    void SetConfig(const nlohmann::json& config) override;

    /// Returns configuration in JSON format.
    nlohmann::json GetConfig() const override;

    void OnSend(const Packet& pkg);

#ifdef USE_QT_GUI
    /// Returns settings widget.
    DefaultSettingsWidget* GetSettingsWidget() override;

    /// Returns widget for displaying statistics.
    QWidget* GetStatsWidget() override;
#endif

  private:
    struct PacketCmp {
        bool operator()(const Packet& a, const Packet& b) const {
            return a.header.timepos < b.header.timepos;
        }
    };

  private:
    UDPClient client_;
    sockaddr_in remote_;

    /// For client_ and remote_ access syncronization.
    mutable std::mutex config_lock_;
};

}  // namespace Components
