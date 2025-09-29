#include <modules/logger/logger.h>

using namespace Components;

Logger::Logger(Config config, IBlockCommunicator*)
: config(config) {}

void Logger::OnReceive(Packet packet) {
  // Next line is for demonstrtion purposes only
  std::cout << "Packet with id " << packet.header.id << " is on module " << config.module_name << "\n";
  std ::cout << ".timepos " << packet.header.timepos.asNanoseconds() << '\n';
  config.logger_space->push_back(packet);
}

std::shared_ptr<std::vector<Packet>> Logger::GetLog() {
  return config.logger_space;
}