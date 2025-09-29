#pragma once

#ifndef EMULATEDLINE_h
#define EMULATEDLINE_h
#include <emulatedblock.h>
#include <emulationcore.h>
#include <modules.h>
#include <memory.h>

#include <string>
#include <istream>
#include <interfaces.h>
#include <exception>
#include <iostream>
#include <packet.h>
#include <map>

using namespace Components;

class EmulationLine {
public:
    // Tags for different modules
    enum class ConfigTag { ModulatorTag, BUCTag, LineUpTag, LineDownTag, LNBTag, DemodulatorTag };

    // Config for emulation line. Currently supports only liner models
    struct Config {
        Modulator::Config modulator;
        BUC::Config buc;
        Line::Config line_up;
        Line::Config line_down;
        LNB::Config lnb;
        Demodulator::Config demodulator;
        
        std::vector<ConfigTag> model;  // Holds tags for line

        Config() {
            GenerateDefault();  // Generating default for first setup
        }

        // Generate default emulation line config from simple examples
        void GenerateDefault() {
            model.push_back(ConfigTag::ModulatorTag);
            model.push_back(ConfigTag::BUCTag);
            model.push_back(ConfigTag::LineUpTag);
            model.push_back(ConfigTag::LineDownTag);
            model.push_back(ConfigTag::LNBTag);
            model.push_back(ConfigTag::DemodulatorTag);
        }

        // Append the model with tag
        void AddTag(ConfigTag tag) { model.push_back(tag); }

        // Clear all tags
        void Clear() { model.clear(); }

        size_t Size() { return model.size(); }
    };

private:
    EmulationCore* core_;        // Line of blocks to emulate
    Config config_;
    EmulatedBlock* satellite;
    EmulatedBlock* input;
    EmulatedBlock* output;
    std::vector<EmulatedBlock*> blocks_;

public:
    // Generate line from config
    EmulationLine(EmulationCore* core, Config cfg, EmulatedBlock* input, EmulatedBlock* satellite, EmulatedBlock* output) :
        core_(core),
        config_(std::move(cfg)),
        satellite(satellite),
        input(input),
        output(output)
    { ReloadLine(); }

    // Load new configuration
    void ReloadLine() {
        // Generate blocks for every tag, needed for future configuration
        for (size_t i = 0; i < config_.Size(); ++i) {
            auto new_block = core_->NewEmulatedBlock();
            blocks_.push_back(new_block);
        }

        input->GetComponent<UdpInput>()->SetNext(blocks_.front());

        // Connect blocks in line and add loggers
        for (size_t i = 0; i < config_.Size(); ++i) {
            // Need to rethink & compose all case statements into prettier code (maybe map?)
            switch (config_.model[i]) {
                case ConfigTag::ModulatorTag: {
                    blocks_[i]->AddComponent(new Modulator{
                        config_.modulator,
                        i + 1 == config_.Size() ? nullptr : blocks_[i + 1],
                    });
                    
                    // blocks_[i]->AddComponent(new Logger{
                    //     Logger::Config{std::make_shared<std::vector<Packet>>(), "modulator"},
                    //     nullptr
                    // });
                    
                    break;
                }
                case ConfigTag::BUCTag: {
                    blocks_[i]->AddComponent(new BUC{
                        config_.buc,
                        i + 1 == config_.Size() ? nullptr : blocks_[i + 1],
                    });

                    //blocks_[i]->AddComponent(new Logger{Logger::Config{std::make_shared<std::vector<Packet>>(), "buc"}, nullptr});
                    break;
                }
                case ConfigTag::LineUpTag: {
                    blocks_[i]->AddComponent(new Line{
                        config_.line_up,
                        satellite,
                    });

                    //blocks_[i]->AddComponent(new Logger{Logger::Config{std::make_shared<std::vector<Packet>>(), "lineup"}, nullptr});
                    
                    break;
                }
                case ConfigTag::LineDownTag: {
                    satellite->GetComponent<Satellite>()->RegisterNewHop(blocks_[i]);

                    blocks_[i]->AddComponent(new Line{
                        config_.line_down,
                        i + 1 == config_.Size() ? nullptr : blocks_[i + 1],
                    });

                    //blocks_[i]->AddComponent(new Logger{Logger::Config{std::make_shared<std::vector<Packet>>(), "linedown"}, nullptr});
                    
                    break;
                }
                case ConfigTag::LNBTag: {
                    blocks_[i]->AddComponent(new LNB{
                        config_.lnb,
                        i + 1 == config_.Size() ? nullptr : blocks_[i + 1],
                    });
                    
                    //blocks_[i]->AddComponent(new Logger{Logger::Config{std::make_shared<std::vector<Packet>>(), "lnb"}, nullptr});
                    
                    break;
                }
                case ConfigTag::DemodulatorTag: {
                    blocks_[i]->AddComponent(new Demodulator{
                        config_.demodulator,
                        output,
                    });

                    //blocks_[i]->AddComponent(new Logger{Logger::Config{std::make_shared<std::vector<Packet>>(), "demodulator"}, nullptr});
                    
                    break;
                }
            }
        }
    }

    void ChangeConfig(Config config) { config_ = config; }

    // Find index of block in line corresponding to needed module
    size_t Findblock(ConfigTag module_tag) {
        size_t index = 0;
        while ((config_.model[index] != module_tag) && (index < config_.model.size())) {
            ++index;
        }

        if (index == config_.model.size()) {
            throw std::out_of_range("Didn't find module with needed tag in line");
        }

        return index;
    }

    // Get block corresponding to module
    EmulatedBlock* GetBlock(ConfigTag module_tag) { return blocks_[Findblock(module_tag)]; }

    // Return logs from logger corresponding to module tag
    std::shared_ptr<std::vector<Packet>> GetLog(ConfigTag module_tag) { return blocks_[Findblock(module_tag)]->GetComponent<Logger>()->GetLog(); }

    // Write all logs collected in FileLogger in every block
    void WriteLogs() {
        for (size_t i = 0; i < config_.Size(); ++i) {
            blocks_[i]->GetComponent<FileLogger>()->WriteLog();
        }
    }
};

#endif