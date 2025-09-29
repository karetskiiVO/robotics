/**
 * \file
 * \brief Contains tests for input module.
 */

// ============================================================================

#include <gtest/gtest.h>

#include <emulation.h>

#include <utils/waitgroup.h>

// ============================================================================

using namespace Components;

// ============================================================================

static constexpr size_t PACKAGES = 100;

// ============================================================================

class Dummy final : public Component, public IReceiver {
  public:
    Dummy(const Packet& pkg, Waitgroup& wg) : pkg_(pkg), wg_(wg) {}

    void OnReceive(Packet pkg) override {
        ASSERT_EQ(pkg.header.id, expected_id_);
        ASSERT_EQ(*pkg.payload, *pkg_.payload);
        ASSERT_EQ(pkg.header.timepos, MeasureUnits::Time(0));

        expected_id_++;
        wg_.Done();
    }

  private:
    size_t expected_id_ = 0;
    const Packet& pkg_;
    Waitgroup& wg_;
};

// ============================================================================

TEST(UdpInput, Basic) {
    Packet pkg(PacketHeader(0));
    pkg.header.size = 256;
    pkg.SetPayload(std::vector<uint8_t>(pkg.header.size));

    Waitgroup wg;

    auto core = EmulationCore();
    auto* output_block = core.NewEmulatedBlock();

    output_block->AddComponent(new Dummy(pkg, wg));

    UdpInput* input = new UdpInput({UDPAddr("127.0.0.1:0")}, output_block);

    UDPClient client(UDPAddr("127.0.0.1:0"));

    wg.Add(PACKAGES);

    for (size_t i = 0; i < PACKAGES; i++) {
        pkg.header.id = i;
        client.Send(pkg, input->GetHostAddress());
        
        ASSERT_EQ(1, UdpInput::Run());
    }

    wg.Wait();

    delete input;
}
