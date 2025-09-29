#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <interfaces.h>
#include <emulatedblock.h>
#include <packet.h>

#include <utils/MeasureUnits/MeasureUnits.h>

#include <memory>
#include <vector>
#include <iostream>

namespace Components {

class Logger : public IReceiver, public Component {
 public:
  struct Config {
    std::shared_ptr<std::vector<Packet>> logger_space; // Placeholder for packets
    std::string module_name; // Name of module that logger corresponds to

    bool operator== (const Config &other) const = default;
  };

  Logger(Config config, IBlockCommunicator*);
  void OnReceive(Packet packet) override;
  std::shared_ptr<std::vector<Packet>> GetLog();

 private:
  Config config;
};

};

#endif