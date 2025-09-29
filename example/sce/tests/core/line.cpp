/**
 * \file
 * \brief Contains tests for EmulationLine.
 */

// ============================================================================

#include <sstream>
#include <thread>

#include <gtest/gtest.h>

#include <emulation.h>

// ============================================================================

using namespace Components;

// ============================================================================

static constexpr size_t PACKAGES = 10;

static constexpr size_t PAYLOAD_SIZE = 256;

// ============================================================================

/// Returns current time in nanoseconds.
#undef GetCurrentTime
static MeasureUnits::Time GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto dur = now.time_since_epoch();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(dur);
    return MeasureUnits::Time(ns.count());
}

// ============================================================================

TEST(SingleLine, Basic) {
    std::vector<uint8_t> payload(PAYLOAD_SIZE);

    UDPClient sender(UDPAddr("127.0.0.1:0"));
    UDPClient receiver(UDPAddr("127.0.0.1:0"));

    EmulationCore core;
    LineConfig line_cfg;

    line_cfg.output.remote = receiver.GetHostAddress();

    core.CreateLine("line", std::move(line_cfg));

    auto input_addr = core.GetBlock("line", Module::INPUT)->GetComponent<UdpInput>()->GetHostAddress();

    Packet pkg(PacketHeader(0));
    pkg.header.size = PAYLOAD_SIZE;
    pkg.SetPayload(payload);
    pkg.header.carrier_frequency = MeasureUnits::hertz(2100);

    Packet pkg_received(PacketHeader(0));

    for (size_t i = 0; i < PACKAGES; i++) {
        pkg.header.id = i;
        pkg.header.timepos = GetCurrentTime();

        sender.Send(pkg, input_addr);

        ASSERT_EQ(1, UdpInput::Run());

        ASSERT_TRUE(receiver.Receive(pkg_received));

        ASSERT_EQ(i, pkg_received.header.id);
        ASSERT_EQ(payload, *pkg_received.payload);
    }
}

TEST(SingleLine, PacketDrop) {
    std::vector<uint8_t> payload(PAYLOAD_SIZE);

    UDPClient sender(UDPAddr("127.0.0.1:0"));
    UDPClient receiver(UDPAddr("127.0.0.1:0"));

    EmulationCore core;
    LineConfig line_cfg;

    line_cfg.output.remote = receiver.GetHostAddress();

    core.CreateLine("line", std::move(line_cfg));

    auto input_addr = core.GetBlock("line", Module::INPUT)->GetComponent<UdpInput>()->GetHostAddress();

    std::thread sender_thread([&sender, input_addr, &payload]() {
        Packet pkg(PacketHeader(0));
        pkg.header.size = PAYLOAD_SIZE;
        pkg.SetPayload(payload);
        pkg.header.carrier_frequency = MeasureUnits::hertz(2100);
        pkg.header.timepos = GetCurrentTime();

        for (size_t i = 0; i < PACKAGES; i++) {
            pkg.header.id = i;

            sender.Send(pkg, input_addr);
        }
    });

    std::thread emulation_thread([]() {
        for (size_t i = 0; i < PACKAGES;) {
            i += UdpInput::Run();
        }
    });

    Packet pkg_received(PacketHeader(0));

    receiver.SetRecvTimeout(1'000'000UL);
    ASSERT_FALSE(receiver.Receive(pkg_received));

    sender_thread.join();
    emulation_thread.join();
}

TEST(MultiLine, Basic) {
    std::vector<uint8_t> payload(PAYLOAD_SIZE);

    UDPClient sender(UDPAddr("127.0.0.1:0"));
    UDPClient receiver(UDPAddr("127.0.0.1:0"));

    EmulationCore core;
    LineConfig line_configs[2];

    line_configs[0].GenerateDefault(MeasureUnits::hertz(2100));
    line_configs[1].GenerateDefault(MeasureUnits::hertz(4200));

    for (size_t i = 0; i < 2; i++) {
        line_configs[i].output.remote = receiver.GetHostAddress();
    }

    core.CreateLine("line1", std::move(line_configs[0]));
    core.CreateLine("line2", std::move(line_configs[1]));

    sockaddr_in inputs_addr[2] = {
        core.GetBlock("line1", Module::INPUT)->GetComponent<UdpInput>()->GetHostAddress(),
        core.GetBlock("line2", Module::INPUT)->GetComponent<UdpInput>()->GetHostAddress()
    };

    Packet pkg(PacketHeader(0));
    pkg.header.size = PAYLOAD_SIZE;
    pkg.SetPayload(payload);

    Packet pkg_received(PacketHeader(0));

    for (size_t i = 0; i < PACKAGES; i++) {
        pkg.header.id = i;

        pkg.header.timepos = GetCurrentTime();

        pkg.header.id = i;

        if (i % 2 == 0) {
            pkg.header.carrier_frequency = MeasureUnits::hertz(2100);
        } else {
            pkg.header.carrier_frequency = MeasureUnits::hertz(4200);
        }

        sender.Send(pkg, inputs_addr[i % 2]);

        ASSERT_EQ(1, UdpInput::Run());

        receiver.Receive(pkg_received);

        ASSERT_EQ(i, pkg_received.header.id);
        ASSERT_EQ(payload, *pkg_received.payload);
    }
}

TEST(MultiLine, PacketDrop) {
    std::vector<uint8_t> payload(PAYLOAD_SIZE);

    UDPClient sender(UDPAddr("127.0.0.1:0"));
    UDPClient receiver(UDPAddr("127.0.0.1:0"));

    EmulationCore core;
    LineConfig line_configs[2];

    line_configs[0].GenerateDefault(MeasureUnits::hertz(2100));
    line_configs[1].GenerateDefault(MeasureUnits::hertz(4200));

    for (size_t i = 0; i < 2; i++) {
        line_configs[i].output.remote = receiver.GetHostAddress();
    }

    core.CreateLine("line1", std::move(line_configs[0]));
    core.CreateLine("line2", std::move(line_configs[1]));

    sockaddr_in inputs_addr[2] = {
        core.GetBlock("line1", Module::INPUT)->GetComponent<UdpInput>()->GetHostAddress(),
        core.GetBlock("line2", Module::INPUT)->GetComponent<UdpInput>()->GetHostAddress()
    };

    std::thread sender_thread([&sender, &inputs_addr, &payload]() {
        Packet pkg(PacketHeader(0));
        pkg.header.size = PAYLOAD_SIZE;
        pkg.SetPayload(payload);
        pkg.header.timepos = GetCurrentTime();

        for (size_t i = 0; i < PACKAGES; i++) {
            pkg.header.id = i;

            pkg.header.carrier_frequency = MeasureUnits::hertz(2100);

            sender.Send(pkg, inputs_addr[i % 2]);
        }
    });

    std::thread emulation_thread([&receiver]() {
        for (size_t i = 0; i < PACKAGES;) {
            i += UdpInput::Run();
        }
    });

    Packet pkg_received(PacketHeader(0));

    receiver.SetRecvTimeout(1'000'000UL);

    ASSERT_FALSE(receiver.Receive(pkg_received));

    sender_thread.join();
    emulation_thread.join();
}
