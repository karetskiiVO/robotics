#pragma once

#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <interfaces.h>
#include <emulatedblock.h>
#include <packet.h>

#include <utils/MeasureUnits/MeasureUnits.h>

#include <memory>
#include <vector>
#include <iostream>

namespace Components {

class FileLogger : public IReceiver, public Component {
 public:
  struct Config {
    std::shared_ptr<std::vector<Packet>> logger_space; // Placeholder for packets
    std::string module_name; // Name of module that logger corresponds to
    std::string logs_path; // Path to write logs

    bool operator== (const Config &other) const = default;
  };

  FileLogger(Config config, IBlockCommunicator*);
  void OnReceive(Packet packet) override;
  void WriteLog();

 private:
  Config config;
};

};

#endif