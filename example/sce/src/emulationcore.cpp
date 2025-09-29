#include <stdexcept>    // std::runtime_error

#include <emulationcore.h>

// ============================================================================

using namespace Components;

// ============================================================================

LineConfig::LineConfig() {
    GenerateDefault(MeasureUnits::hertz(2100));
}

void LineConfig::GenerateDefault(MeasureUnits::Frequency carrier_frequency) {
    input = {
        sockaddr_in(UDPAddr("127.0.0.1:0"))
    };

    modulator = {
        carrier_frequency,
        MeasureUnits::hertz(100)
    };

    buc = {
        MeasureUnits::milliwatt(100),
        MeasureUnits::milliwatt(100),
        MeasureUnits::hertz(100)
    };

    line_up = {
        MeasureUnits::dB(1.0),
        GeoCoordinate(0, 0, 0),
        0,
        {}
    };

    line_down = Line::Config {
        MeasureUnits::dB(-1.0),
        GeoCoordinate(0, 0, 0),
        0, 
        {}
    };

    lnb = LNB::Config {
        MeasureUnits::milliwatt(100),
        MeasureUnits::milliwatt(100),
        MeasureUnits::hertz(100)
    };

    demodulator = Demodulator::Config {
        carrier_frequency
    };

    output = {
        sockaddr_in(UDPAddr("127.0.0.1:0")),
        sockaddr_in(UDPAddr("0.0.0.0:0"))
    };
}

// ============================================================================

EmulationCore::EmulationCore() {
    // TODO: Change satellite config.

    satellite_.AddComponent(new Satellite{
        Satellite::Config{},
    });
}

EmulatedBlock* EmulationCore::NewEmulatedBlock () {
    return new EmulatedBlock();
}

void EmulationCore::CreateLine(const std::string& line, LineConfig cfg) {
    if (auto res = blocks_.find(line); res != blocks_.end()) {
        throw std::runtime_error("Line already exists.");
    }

    auto& modules = blocks_[line];

    modules[static_cast<size_t>(Module::INPUT)].AddComponent(new UdpInput{
        cfg.input,
        &modules[static_cast<size_t>(Module::MODULATOR)],
    });

    modules[static_cast<size_t>(Module::MODULATOR)].AddComponent(new Modulator{
        cfg.modulator,
        &modules[static_cast<size_t>(Module::BUC)],
    });

    modules[static_cast<size_t>(Module::BUC)].AddComponent(new BUC{
        cfg.buc,
        &modules[static_cast<size_t>(Module::LINE_UP)],
    });

    modules[static_cast<size_t>(Module::LINE_UP)].AddComponent(new Line{
        cfg.line_up,
        &satellite_,
    });

    satellite_.GetComponent<Satellite>()->RegisterNewHop(
        &modules[static_cast<size_t>(Module::LINE_DOWN)]
    );

    modules[static_cast<size_t>(Module::LINE_DOWN)].AddComponent(new Line{
        cfg.line_down,
        &modules[static_cast<size_t>(Module::LNB)],
    });

    modules[static_cast<size_t>(Module::LNB)].AddComponent(new LNB{
        cfg.lnb,
        &modules[static_cast<size_t>(Module::DEMODULATOR)],
    });

    modules[static_cast<size_t>(Module::DEMODULATOR)].AddComponent(new Demodulator{
        cfg.demodulator,
        &modules[static_cast<size_t>(Module::OUTPUT)],
    });
    
    modules[static_cast<size_t>(Module::OUTPUT)].AddComponent(new DeferOutput{
        cfg.output
    });
}

EmulatedBlock* EmulationCore::GetBlock(const std::string& line, Module module) {
    if (auto res = blocks_.find(line); res != blocks_.end()) {
        return &res->second[static_cast<size_t>(module)];
    }

    throw std::runtime_error("Line not found.");
}

void EmulationCore::RemoveLine(const std::string& line) {
    if (auto res = blocks_.find(line); res != blocks_.end()) {
        blocks_.erase(res);
    }

    throw std::runtime_error("Line not found.");
}
