/**
 * @file Decode.cpp
 * @author paul
 * @date 14.05.19
 * @brief Decode @TODO
 */

#include <gtest/gtest.h>

extern "C" {
#include "../rc_lib.h"
}

TEST(Decode, Simple1) {
    std::vector<uint8_t> buf{0xc9, 0x1, 0x0, 0x1d, 0x1, 0x8, 0x30, 0x0, 0x1, 0x5, 0x18, 0x70, 0x0, 0x2, 0x4b, 0x93};
    rc_lib_package_t pkg;
    rc_lib_init_rx(&pkg);
    for (const auto &b : buf) {
        ASSERT_EQ(rc_lib_decode(&pkg, b), b == 0x93);
    }

    ASSERT_EQ(pkg.uid, 1);
    ASSERT_EQ(pkg.tid, 0);

    ASSERT_EQ(pkg.mesh, false);
    ASSERT_EQ(pkg.routing_length, 0);
    ASSERT_EQ(pkg.error, false);

    ASSERT_EQ(pkg.channel_count, 8);
    ASSERT_EQ(pkg.resolution, 1024);
    ASSERT_EQ(pkg.channel_data[0], 1);
    ASSERT_EQ(pkg.channel_data[1], 2);
    ASSERT_EQ(pkg.channel_data[2], 3);
    ASSERT_EQ(pkg.channel_data[3], 4);
    ASSERT_EQ(pkg.channel_data[4], 5);
    ASSERT_EQ(pkg.channel_data[5], 6);
    ASSERT_EQ(pkg.channel_data[6], 7);
    ASSERT_EQ(pkg.channel_data[7], 8);
}

TEST(Decode, Simple2) {
    std::vector<uint8_t> buf{0xc9, 0x1, 0x0, 0x12, 0x1, 0xad, 0xec, 0xf, 0x5c, 0x93};
    rc_lib_package_t pkg;
    rc_lib_init_rx(&pkg);
    for (const auto &b : buf) {
        ASSERT_EQ(rc_lib_decode(&pkg, b), b == 0x93);
    }

    ASSERT_EQ(pkg.uid, 1);
    ASSERT_EQ(pkg.tid, 0);

    ASSERT_EQ(pkg.mesh, false);
    ASSERT_EQ(pkg.routing_length, 0);
    ASSERT_EQ(pkg.error, false);

    ASSERT_EQ(pkg.channel_count, 4);
    ASSERT_EQ(pkg.resolution, 128);
    ASSERT_EQ(pkg.channel_data[0], 1);
    ASSERT_EQ(pkg.channel_data[1], 90);
    ASSERT_EQ(pkg.channel_data[2], 50);
    ASSERT_EQ(pkg.channel_data[3], 127);
}

TEST(Decode, Simple3) {
    std::vector<uint8_t> buf{0xc9, 0x1, 0x0, 0x16, 0x1, 0xd0, 0x82, 0xc, 0xfe, 0x0, 0xb6, 0x93};
    rc_lib_package_t pkg;
    rc_lib_init_rx(&pkg);
    for (const auto &b : buf) {
        ASSERT_EQ(rc_lib_decode(&pkg, b), b == 0x93);
    }

    ASSERT_EQ(pkg.uid, 1);
    ASSERT_EQ(pkg.tid, 0);

    ASSERT_EQ(pkg.mesh, false);
    ASSERT_EQ(pkg.routing_length, 0);
    ASSERT_EQ(pkg.error, false);

    ASSERT_EQ(pkg.channel_count, 4);
    ASSERT_EQ(pkg.resolution, 2048);
    ASSERT_EQ(pkg.channel_data[0], 1);
    ASSERT_EQ(pkg.channel_data[1], 90);
    ASSERT_EQ(pkg.channel_data[2], 50);
    ASSERT_EQ(pkg.channel_data[3], 127);
}

TEST(Decode, Mesh) {
    std::vector<uint8_t> buf{0xc9, 0x3, 0x0, 0x92, 0x1, 0x1, 0xad, 0xec, 0xf, 0xdf, 0x93};
    rc_lib_package_t pkg;
    rc_lib_init_rx(&pkg);
    for (const auto &b : buf) {
        ASSERT_EQ(rc_lib_decode(&pkg, b), b == 0x93);
    }

    ASSERT_EQ(pkg.uid, 3);
    ASSERT_EQ(pkg.tid, 0);

    ASSERT_EQ(pkg.mesh, true);
    ASSERT_EQ(pkg.routing_length, 1);
    ASSERT_EQ(pkg.error, false);

    ASSERT_EQ(pkg.channel_count, 4);
    ASSERT_EQ(pkg.resolution, 128);
    ASSERT_EQ(pkg.channel_data[0], 1);
    ASSERT_EQ(pkg.channel_data[1], 90);
    ASSERT_EQ(pkg.channel_data[2], 50);
    ASSERT_EQ(pkg.channel_data[3], 127);
}

TEST(Decode, ErrorBit) {
    std::vector<uint8_t> buf{0xc9, 0x3, 0x0, 0xd2, 0x1, 0x1, 0xad, 0xec, 0xf, 0xdf, 0x93};
    rc_lib_package_t pkg;
    rc_lib_init_rx(&pkg);
    for (const auto &b : buf) {
        ASSERT_EQ(rc_lib_decode(&pkg, b), b == 0x93);
    }

    ASSERT_EQ(pkg.uid, 3);
    ASSERT_EQ(pkg.tid, 0);

    ASSERT_EQ(pkg.mesh, true);
    ASSERT_EQ(pkg.routing_length, 1);
    ASSERT_EQ(pkg.error, true);

    ASSERT_EQ(pkg.channel_count, 4);
    ASSERT_EQ(pkg.resolution, 128);
    ASSERT_EQ(pkg.channel_data[0], 1);
    ASSERT_EQ(pkg.channel_data[1], 90);
    ASSERT_EQ(pkg.channel_data[2], 50);
    ASSERT_EQ(pkg.channel_data[3], 127);
}

TEST(Decode, FullRes) {
    std::vector<uint8_t> buf{0xc9, 0x0, 0x0, 0x17, 0xff, 0x4f, 0x6, 0x0, 0xf2, 0xff, 0xac, 0x93};
    rc_lib_package_t pkg;
    rc_lib_init_rx(&pkg);
    for (const auto &b : buf) {
        ASSERT_EQ(rc_lib_decode(&pkg, b), b == 0x93);
    }

    ASSERT_EQ(pkg.uid, 0);
    ASSERT_EQ(pkg.tid, 0);

    ASSERT_EQ(pkg.mesh, false);
    ASSERT_EQ(pkg.routing_length, 0);
    ASSERT_EQ(pkg.error, false);

    ASSERT_EQ(pkg.channel_count, 4);
    ASSERT_EQ(pkg.resolution, 4096);
    ASSERT_EQ(pkg.channel_data[0], 4095);
    ASSERT_EQ(pkg.channel_data[1], 100);
    ASSERT_EQ(pkg.channel_data[2], 512);
    ASSERT_EQ(pkg.channel_data[3], 4095);
}

