/**
* \file
* \brief Contains tests for line module with satellite movement simulation.
*/
#include <gtest/gtest.h>
#include <modules/line/line.h>
#include <utils/MeasureUnits/MeasureUnits.h>

using namespace Components;
using namespace MeasureUnits;

namespace {

// Вспомогательная функция для создания тестовых эфемерид
std::vector<Line::SatelliteData> CreateTestEphemeris() {
    return {
        {
            MeasureUnits::Time(0),
            Point<int64_t>{35'786'000, 0, 0}, 
            Point<int64_t>{0, 3'074, 0} 
        },
        {
            MeasureUnits::Time(1'000'000'000), 
            Point<int64_t>{35'786'000, 3'074, 0},
            Point<int64_t>{0, 3'074, 0}
        },
        {
            MeasureUnits::Time(2'000'000'000), 
            Point<int64_t>{35'786'000, 6'148, 0},
            Point<int64_t>{0, 3'074, 0}
        }
    };
}

// Фиктивный блок для тестирования
struct DummyBlock final : IBlockCommunicator {
    void OnReceive(Packet packet) override {
        received_packets.push_back(packet);
    }
    
    std::vector<Packet> received_packets;
};

} // namespace

TEST(Line, PacketDelayCalculation) {
    Line::Config config {
        .attenuation = MeasureUnits::dB(1.0),
        .coord = GeoCoordinate(55'700'000, 37'300'000, 0), 
        .satellite_longitude = 0,
        .satellite_ephemeris = CreateTestEphemeris()
    };
    
    DummyBlock dummy;
    Line line(config, &dummy);

    Packet packet;
    packet.header.timepos = MeasureUnits::Time(0);
    packet.header.carrier_frequency = MeasureUnits::Frequency(1'000'000'000);
    
    line.OnReceive(packet);
    ASSERT_EQ(dummy.received_packets.size(), 1);
    
    auto delay_ns = dummy.received_packets[0].header.timepos.asMilliseconds();
    EXPECT_NEAR(delay_ns, 125, 25);  
    
    // Отправляем пакет в момент времени 1 секунда
    packet.header.timepos = MeasureUnits::Time(1'000'000'000);
    line.OnReceive(packet);
    ASSERT_EQ(dummy.received_packets.size(), 2);

    // Проверяем что задержка для второго пакета также в допустимом диапазоне
    auto second_delay_ns = dummy.received_packets[1].header.timepos.asMilliseconds() - 1'000;
    EXPECT_NEAR(second_delay_ns, 125, 25);
}


TEST(Line, NoEphemerisUsesDefault) {
    Line::Config config {
        .attenuation = MeasureUnits::dB(1.0),
        .coord = GeoCoordinate(55'700'000, 37'300'000, 0),
        .satellite_longitude = 45'000'000,
        .satellite_ephemeris = {}
    };
    
    DummyBlock dummy;
    Line line(config, &dummy);
    
    // Должен использовать геостационарную орбиту
    auto pos = line.CalculateSatellitePosition(MeasureUnits::Time(0));
    EXPECT_EQ(pos.longitude, config.satellite_longitude);
    EXPECT_EQ(pos.altitude, 35'786'000);
}