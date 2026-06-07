// Hand-written smoke test for an MPC60 ALL-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC60ALL
#include "mpc60all_write.h"

#include <sstream>

namespace {

std::unique_ptr<mpc60all_write_t::u3le_t> make_u3le(uint32_t value) {
    std::unique_ptr<mpc60all_write_t::u3le_t> result(new mpc60all_write_t::u3le_t(nullptr));
    result->set_b12(static_cast<uint16_t>(value & 0xffff));
    result->set_b3(static_cast<uint8_t>((value >> 16) & 0xff));
    return result;
}

std::unique_ptr<mpc60all_write_t::track_header_t::body_t> make_track_header_body() {
    std::unique_ptr<mpc60all_write_t::track_header_t::body_t> body(new mpc60all_write_t::track_header_t::body_t(nullptr));
    body->set_user_track_number(1);
    body->set_track_mute(false);
    body->set_track_in_use(true);
    body->set_drum_track(false);
    body->set__unnamed4(0);
    std::unique_ptr<std::vector<bool>> channel_assignments(new std::vector<bool>(16, false));
    channel_assignments->at(0) = true;
    body->set_channel_assignments(std::move(channel_assignments));
    body->set_track_name(std::string("TRACK001", 8) + std::string(8, '\x00'));
    return body;
}

std::unique_ptr<mpc60all_write_t::track_header_t> make_track_header() {
    std::unique_ptr<mpc60all_write_t::track_header_t> track_header(new mpc60all_write_t::track_header_t(nullptr));
    track_header->set_absolute_track_number(1);
    track_header->set_body(make_track_header_body());
    return track_header;
}

std::unique_ptr<mpc60all_write_t::event_t> make_sysex_event() {
    std::unique_ptr<mpc60all_write_t::event_t> event(new mpc60all_write_t::event_t(true, 0xff, 43, nullptr));
    event->set_is_first_event(true);
    event->set_preparsed_status(0xff);
    event->set_remaining_byte_count(43);
    event->set_parsed_status(0xf0);
    event->set_track_number(0);
    std::unique_ptr<std::vector<uint8_t>> body(new std::vector<uint8_t>(39, 0x00));
    body->push_back(0xf7);
    event->set_system_exclusive_body(std::move(body));
    return event;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc60all_write_roundtrip) {
    mpc60all_write_t root(nullptr);
    root.set_file_id(std::string("\x03", 1));
    root.set_file_format_version(std::string("\x01", 1));
    root.set_total_number_of_bytes_in_all_sequences(256);

    std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::sequence_t>>> sequences(
        new std::vector<std::unique_ptr<mpc60all_write_t::sequence_t>>());
    std::unique_ptr<mpc60all_write_t::sequence_t> sequence(new mpc60all_write_t::sequence_t(nullptr));

    std::unique_ptr<mpc60all_write_t::sequence_header_t> sequence_header(new mpc60all_write_t::sequence_header_t(nullptr));
    sequence_header->set_sequence_number(0);
    sequence_header->set_sequence_length_in_bytes(make_u3le(6));
    sequence_header->set_offset_from_bottom_of_sequence_to_sequence_start(make_u3le(0));
    sequence_header->set_sequence_name(std::string("SEQ DEMO", 8) + std::string(8, '\x00'));
    sequence_header->set_loop_to_bar(mpc60all_write_t::OFF_ON_FALSE);
    sequence_header->set__unnamed5(0);
    sequence_header->set_loop_to_bar_number(0);
    sequence_header->set_number_of_bars(1);
    sequence_header->set_length_in_ticks(96);
    sequence_header->set_tempo(1200);

    std::unique_ptr<mpc60all_write_t::smpte_offset_t> smpte(new mpc60all_write_t::smpte_offset_t(nullptr));
    smpte->set_hundredth_frames(0);
    smpte->set_frames(0);
    smpte->set_seconds(0);
    smpte->set_minutes(0);
    smpte->set_hours(0);
    sequence_header->set_smpte_offset(std::move(smpte));

    sequence_header->set_stereo_mix(std::unique_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(32, 100)));
    sequence_header->set_stereo_pan(std::unique_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(32, 50)));
    sequence_header->set_echo_mix(std::unique_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(32, 0)));
    sequence_header->set_drum_tuning(std::unique_ptr<std::vector<uint16_t>>(new std::vector<uint16_t>(32, 0)));
    sequence_header->set_last_active_user_track(0);
    sequence_header->set_number_of_tempo_changes(0);
    sequence_header->set_number_of_active_track_headers(3);
    sequence->set_sequence_header(std::move(sequence_header));

    std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::track_header_t>>> track_headers(
        new std::vector<std::unique_ptr<mpc60all_write_t::track_header_t>>());
    track_headers->push_back(make_track_header());
    sequence->set_track_headers(std::move(track_headers));
    sequence->set_track_headers_terminator(std::string("\xff", 1));
    sequence->set_position_tracker(std::unique_ptr<mpc60all_write_t::position_tracker_t>(
        new mpc60all_write_t::position_tracker_t(nullptr)));
    sequence->set__unnamed4(std::string(5, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::event_t>>> events(
        new std::vector<std::unique_ptr<mpc60all_write_t::event_t>>());
    events->push_back(make_sysex_event());
    sequence->set_events(std::move(events));

    sequences->push_back(std::move(sequence));
    root.set_sequences(std::move(sequences));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc60all_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.file_id(), std::string("\x03", 1));
    BOOST_CHECK_EQUAL(parsed.file_format_version(), std::string("\x01", 1));
    BOOST_CHECK_EQUAL(parsed.total_number_of_bytes_in_all_sequences(), 256U);
    BOOST_REQUIRE_EQUAL(parsed.sequences()->size(), 1U);

    auto* parsed_sequence = parsed.sequences()->at(0).get();
    BOOST_REQUIRE(parsed_sequence->sequence_header() != nullptr);
    BOOST_CHECK_EQUAL(parsed_sequence->sequence_header()->sequence_length_in_bytes()->value(), 6);
    BOOST_CHECK_EQUAL(parsed_sequence->sequence_header()->sequence_name(), std::string("SEQ DEMO", 8) + std::string(8, '\x00'));
    BOOST_CHECK_EQUAL(parsed_sequence->sequence_header()->tempo(), 1200);
    BOOST_CHECK_EQUAL(parsed_sequence->sequence_header()->number_of_active_track_headers(), 3);

    BOOST_REQUIRE_EQUAL(parsed_sequence->track_headers()->size(), 1U);
    auto* parsed_track_header = parsed_sequence->track_headers()->at(0).get();
    BOOST_CHECK_EQUAL(parsed_track_header->absolute_track_number(), 1);
    BOOST_REQUIRE(parsed_track_header->body() != nullptr);
    BOOST_CHECK_EQUAL(parsed_track_header->body()->track_name(), std::string("TRACK001", 8) + std::string(8, '\x00'));
    BOOST_CHECK_EQUAL(parsed_track_header->body()->channel_assignments()->at(0), true);

    BOOST_REQUIRE_EQUAL(parsed_sequence->events()->size(), 1U);
    auto* parsed_event = parsed_sequence->events()->at(0).get();
    BOOST_CHECK_EQUAL(parsed_event->status(), 0xf0);
    BOOST_CHECK_EQUAL(parsed_event->track_number(), 0);
    BOOST_REQUIRE(parsed_event->system_exclusive_body() != nullptr);
    BOOST_REQUIRE_EQUAL(parsed_event->system_exclusive_body()->size(), 40U);
    BOOST_CHECK_EQUAL(parsed_event->system_exclusive_body()->back(), 0xf7);
}
#else
BOOST_AUTO_TEST_CASE(test_mpc60all_write_roundtrip) {
    BOOST_FAIL("mpc60all_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
