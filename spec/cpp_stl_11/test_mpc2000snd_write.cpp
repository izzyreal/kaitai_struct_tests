// Hand-written smoke test for an MPC2000 SND-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC2000SND
#include "mpc2000snd_write.h"

#include <sstream>

BOOST_AUTO_TEST_CASE(test_mpc2000snd_write_roundtrip) {
    mpc2000snd_write_t root(nullptr);
    root.set_magic(std::string("\x01\x04", 2));
    root.set_name(std::string("KICK_01", 7) + std::string(10, '\x00'));
    root.set_level(100);
    root.set_tune(-2);
    root.set_stereo(false);
    root.set_start(0);
    root.set_end(2);
    root.set_frame_count(2);
    root.set_loop_frame_count(0);
    root.set_loop_enabled(false);
    root.set_beat_count(4);
    root.set_sample_rate(44100);

    std::unique_ptr<std::vector<int16_t>> frames(new std::vector<int16_t>());
    frames->push_back(1234);
    frames->push_back(-1234);
    root.set_frames(std::move(frames));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._check();
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc2000snd_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.magic(), std::string("\x01\x04", 2));
    BOOST_CHECK_EQUAL(parsed.name(), std::string("KICK_01", 7) + std::string(10, '\x00'));
    BOOST_CHECK_EQUAL(parsed.level(), 100);
    BOOST_CHECK_EQUAL(parsed.tune(), -2);
    BOOST_CHECK_EQUAL(parsed.stereo(), false);
    BOOST_CHECK_EQUAL(parsed.start(), 0U);
    BOOST_CHECK_EQUAL(parsed.end(), 2U);
    BOOST_CHECK_EQUAL(parsed.frame_count(), 2U);
    BOOST_CHECK_EQUAL(parsed.loop_frame_count(), 0U);
    BOOST_CHECK_EQUAL(parsed.loop_enabled(), false);
    BOOST_CHECK_EQUAL(parsed.beat_count(), 4);
    BOOST_CHECK_EQUAL(parsed.sample_rate(), 44100);
    BOOST_REQUIRE_EQUAL(parsed.frames()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed.frames()->at(0), 1234);
    BOOST_CHECK_EQUAL(parsed.frames()->at(1), -1234);
}
#else
BOOST_AUTO_TEST_CASE(test_mpc2000snd_write_roundtrip) {
    BOOST_FAIL("mpc2000snd_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
