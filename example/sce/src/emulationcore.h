#pragma once

// ============================================================================

#include <map>
#include <string>
#include <array>

#include <modules.h>
#include <emulatedblock.h>

// ============================================================================

struct LineConfig {
public:
    LineConfig();

    void GenerateDefault(MeasureUnits::Frequency carrier_frequency);

public:
    Components::UdpInput::Config input;
    Components::Modulator::Config modulator;
    Components::BUC::Config buc;
    Components::Line::Config line_up;
    Components::Line::Config line_down;
    Components::LNB::Config lnb;
    Components::Demodulator::Config demodulator;
    Components::DeferOutput::Config output;
};

// ============================================================================

enum class Module {
    INPUT       = 0,
    MODULATOR   = 1,
    BUC         = 2,
    LINE_UP     = 3,
    LINE_DOWN   = 4,
    LNB         = 5,
    DEMODULATOR = 6,
    OUTPUT      = 7,
};

// ============================================================================

class EmulationCore {
public:
    EmulationCore();

    /// Allocates new emulated block.
    /// \warning Deprecated. Kept for backward compatibility.
    EmulatedBlock* NewEmulatedBlock();

    /// Creates new emulation line.
    /// \throws runtime_error if line already exists.
    void CreateLine(const std::string& line, LineConfig cfg);

    /// Returns specific block from emulation line.
    /// \throws runtime_error if line not found.
    EmulatedBlock* GetBlock(const std::string& line, Module module);

    /// Removes emulation line.
    /// \throws runtime_error if line not found.
    void RemoveLine(const std::string& line);

private:
    /// Map of emulation lines.
    std::map<std::string, std::array<EmulatedBlock, 8>> blocks_;

    /// Satellite shared by all emulation lines.
    EmulatedBlock satellite_;
};
