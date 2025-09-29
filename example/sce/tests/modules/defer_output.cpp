/**
 * \file
 * \brief Contains tests for DeferOutput module.
 */

// ============================================================================

#include <chrono>  // std::chrono::system_clock
#include <thread>  // std::thread

#include <gtest/gtest.h>

#include <emulation.h>

#include <utils/waitgroup.h>

// ============================================================================

using namespace Components;

// ============================================================================

static constexpr size_t DELAY = 250'000'000ULL;

static constexpr size_t PACKAGES = 1000;

// ============================================================================

TEST(DeferOutput, Basic) {
    Packet pkg_origin(PacketHeader(0));
    pkg_origin.header.size = 256;
    pkg_origin.SetPayload(std::vector<uint8_t>(pkg_origin.header.size));

    UDPClient client(UDPAddr("127.0.0.1:0"));

    auto* output =
        new DeferOutput({UDPAddr("127.0.0.1:0"), client.GetHostAddress()});

    for (size_t i = 0; i < PACKAGES; i++) {
        Packet pkg_copy(pkg_origin);
        pkg_copy.header.id = i;
        pkg_copy.header.carrier_frequency = MeasureUnits::Frequency((i + 10) * 10);

        auto now = std::chrono::system_clock::now();
        auto dur = now.time_since_epoch();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(dur);

        pkg_origin.header.timepos = MeasureUnits::Time(ns.count() + DELAY);
        output->OnReceive(std::move(pkg_copy));
    }

    Packet pkg_received(PacketHeader(0));

    for (size_t i = 0; i < PACKAGES; i++) {
        client.Receive(pkg_received);

        ASSERT_EQ(i, pkg_received.header.id);
        ASSERT_EQ(*pkg_origin.payload, *pkg_received.payload);
    }

    delete output;
}
