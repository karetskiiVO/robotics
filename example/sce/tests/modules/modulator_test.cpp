#include <gtest/gtest.h>

#include <modules/modulator/modulator.h>

using namespace Components;

TEST(Modulator, ConfigJSON) {
  auto modulator = Modulator(
    Modulator::Config {
      MeasureUnits::Frequency(100),
      MeasureUnits::Frequency(2100),
    },
    nullptr
  );

  nlohmann::json config;
  config["ModulatorTest"] = modulator.config;

  ASSERT_EQ(config["ModulatorTest"].get<Modulator::Config>(), modulator.config);

  modulator.config = {

    .carrier_frequency = MeasureUnits::Frequency(345),
    .clock_frequency = MeasureUnits::Frequency(1200),
  };

  config["ModulatorTest"] = modulator.config;

  ASSERT_EQ(config["ModulatorTest"].get<Modulator::Config>(), modulator.config);
}
