/**
 * \file
 * \brief Contains output class declaration.
*/

#pragma once

// ============================================================================

#include <vector>

#include <interfaces.h>
#include <emulatedblock.h>
#include <packet.h>

#include <utils/udp_client/udp_client.h>

#ifdef USE_QT_GUI

#include <qtinterfaces.h>

#endif

// ============================================================================

namespace Components {

/// Sends received packages via UDP connection.
class UdpOutput final :
#ifdef USE_QT_GUI
public IQTWidgetConfigurator,
#endif
public Component, public IReceiver, public IConfigurator {
  public:
    UdpOutput(sockaddr_in host, sockaddr_in remote);

    void OnReceive(Packet pkg) override;

    /// Configures module via JSON.
    void SetConfig (const nlohmann::json& config) override;
    
    /// Returns configuration in JSON format.
    nlohmann::json GetConfig () const override;

#ifdef USE_QT_GUI
    /// Returns settings widget.
    DefaultSettingsWidget* GetSettingsWidget() override;

    /// Returns widget for displaying statistics.
    QWidget* GetStatsWidget() override;
#endif

 private:
  UDPClient client_;
  sockaddr_in remote_;
};

}   // namespace Components
