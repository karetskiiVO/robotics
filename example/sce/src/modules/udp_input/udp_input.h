/**
 * \file
 * \brief Contains input class declaration.
 */

#pragma once

// ============================================================================

#include <mutex>

#include <interfaces.h>
#include <packet.h>
#include <emulatedblock.h>

#include <utils/udp_client/udp_client.h>

#ifdef USE_QT_GUI

#include <qtinterfaces.h>

#endif

// ============================================================================

namespace Components {

/// Reads packages from UDP connection.
class UdpInput final :
#ifdef USE_QT_GUI
public IQTWidgetConfigurator,
#endif
public Component, public IConfigurator {
 public:
  struct Config {
    sockaddr_in host;
  };

  UdpInput(Config cfg, IBlockCommunicator* next);

  sockaddr_in GetHostAddress() const;

  static size_t Run();

  void SetNext(IBlockCommunicator* next);

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

  ~UdpInput();

 private:
  void OnReceive();

 private:
  UDPClient client_;
  IBlockCommunicator* next_;

  /// For client_ access syncronization.
  mutable std::mutex config_lock_;
};

}  // namespace Components
