/**
 * \file
 * \brief Contains input class implementation.
 */

// ============================================================================

#include <cassert>

#include <modules/udp_output/udp_output.h>

#ifdef USE_QT_GUI

#include <QTextEdit>

#endif

// ============================================================================

using namespace Components;

// ============================================================================

UdpOutput::UdpOutput(sockaddr_in host, sockaddr_in remote)
    : client_(host), remote_(remote) {}

void UdpOutput::OnReceive(Packet pkg) { client_.Send(pkg, remote_); }

void UdpOutput::SetConfig(const nlohmann::json& cfg) {
    auto addr = cfg["host_addr"].get<std::string>();
    if (cfg.contains("host_port")) throw std::runtime_error("Переделайте это легаси");

    auto new_host = UDPAddr(addr);

    client_.Rebind(new_host);

    addr = cfg["remote_addr"].get<std::string>();

    remote_ = UDPAddr(addr);
}

nlohmann::json UdpOutput::GetConfig() const {
    sockaddr_in host = client_.GetHostAddress();

    nlohmann::json config = {
        {"host_addr", UDPAddr(host).ToString()},
        {"remote_addr", UDPAddr(remote_).ToString()},
    };

    return config;
}

#ifdef USE_QT_GUI

DefaultSettingsWidget* UdpOutput::GetSettingsWidget() {
    DefaultSettingsWidget* widget = new DefaultSettingsWidget();
    widget->setObjectName("config");
    return widget;
}

QWidget* UdpOutput::GetStatsWidget() {
    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");
    return stat_widget;
}

#endif
