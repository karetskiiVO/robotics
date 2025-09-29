#include <modules/line/line.h>
#include <algorithm>

#ifdef USE_QT_GUI
#include <QTextEdit>
#include <gui/qt/LineSettingsWidget/linesettingswidget.h>
#endif

const int64_t kGeostationaryOrbit = 35'786'000; 
const int64_t kLightSpeed = 299'792'458;       

using namespace Components;

Line::Line(Config config, IBlockCommunicator* nextHop) 
    : config(std::move(config)), nextHop(nextHop) 
{
    std::sort(this->config.satellite_ephemeris.begin(), 
              this->config.satellite_ephemeris.end(),
              [](const auto& a, const auto& b) { 
                  return a.time.asNanoseconds() < b.time.asNanoseconds(); 
              });
}

void Line::OnReceive(Packet packet) {
    if (!nextHop) return;

    // attenuate signal
    packet.header.carrier_frequency *= 1 / config.attenuation.asTimes();
    
    GeoCoordinate satellite_pos = CalculateSatellitePosition(packet.header.timepos);
    int64_t distance = config.coord.distance(satellite_pos);
    
    int64_t delay = (distance * 1'000'000'000) / kLightSpeed;
    packet.header.timepos += MeasureUnits::Duration(delay);
 
    Send(nextHop, packet);
}

GeoCoordinate Line::CalculateSatellitePosition(MeasureUnits::Time time) const {
    if (config.satellite_ephemeris.empty()) {
        return GeoCoordinate(0, config.satellite_longitude, kGeostationaryOrbit);
    }

    auto it = std::lower_bound(config.satellite_ephemeris.begin(),
                               config.satellite_ephemeris.end(),
                               time);

    if (it == config.satellite_ephemeris.begin()) {
        return ExtrapolatePosition(*it, time);
    }
    if (it == config.satellite_ephemeris.end()) {
        return ExtrapolatePosition(config.satellite_ephemeris.back(), time);
    }

    return InterpolatePosition(*(it - 1), *it, time);
}

GeoCoordinate Line::ExtrapolatePosition(const SatelliteData& data, 
                                      MeasureUnits::Time currentT_time) const {
    double delta = (currentT_time - data.time).asSeconds();
    
    Point<double> position(
        data.position.x + data.velocity.x * delta,
        data.position.y + data.velocity.y * delta,
        data.position.z + data.velocity.z * delta
    );
    
    return CartesianToGeo(position);
}

GeoCoordinate Line::InterpolatePosition(const SatelliteData& prev, const SatelliteData& next, 
                                        MeasureUnits::Time currentT_time) const {
    int64_t interval = (next.time - prev.time).asNanoseconds();
    int64_t position = (currentT_time - prev.time).asNanoseconds();
    double factor = static_cast<double>(position) / interval;
    
    Point<double> pos(
        prev.position.x + factor * (next.position.x - prev.position.x),
        prev.position.y + factor * (next.position.y - prev.position.y),
        prev.position.z + factor * (next.position.z - prev.position.z)
    );

    Point<double> vel(
        prev.velocity.x + factor * (next.velocity.x - prev.velocity.x),
        prev.velocity.y + factor * (next.velocity.y - prev.velocity.y),
        prev.velocity.z + factor * (next.velocity.z - prev.velocity.z)
    );

    double delta = (currentT_time - prev.time).asSeconds();
    pos.x += vel.x * delta;
    pos.y += vel.y * delta;
    pos.z += vel.z * delta;

    return CartesianToGeo(pos);
}

void Line::SetConfig(const nlohmann::json& cfg) {
    config.attenuation = cfg["attenuation"].get<MeasureUnits::dB>();
    config.coord.latitude = cfg["latitude"].get<int64_t>();
    config.coord.longitude = cfg["longitude"].get<int64_t>();
    config.satellite_longitude = cfg["satellite_longitude"].get<int64_t>();

    if (cfg.contains("satellite_ephemeris")) {
        config.satellite_ephemeris = cfg["satellite_ephemeris"].get<std::vector<SatelliteData>>();
        std::sort(config.satellite_ephemeris.begin(), config.satellite_ephemeris.end(),
            [](const auto& a, const auto& b) { return a.time < b.time; });
    }
}

nlohmann::json Line::GetConfig() const {
    return {
        {"attenuation", config.attenuation},
        {"latitude", config.coord.latitude},
        {"longitude", config.coord.longitude},
        {"satellite_longitude", config.satellite_longitude},
        {"satellite_ephemeris", config.satellite_ephemeris}
    };
}

#ifdef USE_QT_GUI
AbstractSettingsWidget* Line::GetSettingsWidget() {
    auto* widget = new LineSettingsWidget();
    widget->setObjectName("config");
    return widget;
}

QWidget* Line::GetStatsWidget() {
    auto* statWidget = new QTextEdit();
    statWidget->setReadOnly(true);
    statWidget->setObjectName("statWidget");
    return statWidget;
}
#endif