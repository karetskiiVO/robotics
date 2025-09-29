#include "packet.h"

PacketHeader::PacketHeader(Id id, ModCod::Type mod_cod) : 
    id(id), 
    mod_cod(mod_cod),
    timepos(0),
    carrier_frequency(100), 
    clock_frequency(1),
    power(0),
    size(0) {}

Packet::Packet(PacketHeader pk_header) : 
    header(pk_header),
    payload() {}
