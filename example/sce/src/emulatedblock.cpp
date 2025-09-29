#include <emulatedblock.h>

// ============================================================================

void Send (IBlockCommunicator* block, Packet packet) {
    block->OnReceive(packet);
}
