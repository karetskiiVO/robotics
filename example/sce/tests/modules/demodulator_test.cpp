#include <gtest/gtest.h>

#include <modules/demodulator/demodulator.h>

using namespace Components;

TEST(Demodulator, ConfigJSON) {

  auto demodulator = Demodulator(
    Demodulator::Config {
      MeasureUnits::Frequency(100)
    },
    nullptr
  );

  nlohmann::json config;
  config["DemodulatorTest"] = demodulator.config;

  ASSERT_EQ(config["DemodulatorTest"].get<Demodulator::Config>(), demodulator.config);

  Demodulator::Config testConfig;
  testConfig.frequencies = std::vector<MeasureUnits::Frequency>{ MeasureUnits::Frequency(100), MeasureUnits::Frequency(345) };

  config["DemodulatorTest"] = testConfig;

  demodulator.config = config["DemodulatorTest"].get<Demodulator::Config>();

  ASSERT_EQ(config["DemodulatorTest"].get<Demodulator::Config>(), testConfig);
}
