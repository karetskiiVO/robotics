#define _CRT_SECURE_NO_WARNINGS

#include <modules/file_logger/file_logger.h>

#include <chrono>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>

using namespace Components;

FileLogger::FileLogger(Config config, IBlockCommunicator*)
: config(config) {}

void FileLogger::OnReceive(Packet packet) {
  // Next line is for demonstrtion purposes only
  std::cout << "Packet with id " << packet.header.id << " is on module " << config.module_name << "\n";
  std ::cout << ".timepos " << packet.header.timepos.asNanoseconds() << '\n';
  config.logger_space->push_back(packet);
}

void FileLogger::WriteLog() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm tm = *std::localtime(&now_time);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
  std::string time_str = oss.str();

  std::ofstream out_file(std::filesystem::path(config.logs_path) / std::filesystem::path(time_str + "-" + config.module_name + ".txt"));

  for (auto& packet : *(config.logger_space)) {
    out_file << packet.ToString();
  }

  return;
}