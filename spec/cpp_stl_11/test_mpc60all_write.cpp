// Hand-written smoke test for an MPC60 ALL-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC60ALL
#include "mpc60all_write.h"

#include <cstdint>
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

std::unique_ptr<mpc60all_write_t::song_step_t> make_song_step(uint8_t sequence_number, uint8_t repeats) {
    std::unique_ptr<mpc60all_write_t::song_step_t> step(new mpc60all_write_t::song_step_t(nullptr));
    step->set_sequence_number(sequence_number);
    step->set_repeats(repeats);
    return step;
}

std::unique_ptr<mpc60all_write_t::song_t> make_song() {
    std::unique_ptr<mpc60all_write_t::song_t> song(new mpc60all_write_t::song_t(nullptr));
    song->set_step_count(2);
    std::unique_ptr<mpc60all_write_t::song_body_t> body(new mpc60all_write_t::song_body_t(nullptr));
    body->set_song_number(1);
    body->set_reserved_1(std::string("\x00", 1));
    body->set_reserved_2(std::string("\x01", 1));
    body->set_song_name(std::string("SONG01", 6) + std::string(10, ' '));
    body->set_reserved_3(std::string(5, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::song_step_t>>> steps(
        new std::vector<std::unique_ptr<mpc60all_write_t::song_step_t>>());
    steps->push_back(make_song_step(1, 1));
    steps->push_back(make_song_step(1, 1));
    body->set_steps(std::move(steps));
    song->set_body(std::move(body));
    return song;
}

std::unique_ptr<mpc60all_write_t::song_t> make_song_terminator() {
    std::unique_ptr<mpc60all_write_t::song_t> song(new mpc60all_write_t::song_t(nullptr));
    song->set_step_count(0);
    std::unique_ptr<mpc60all_write_t::empty_song_t> body(new mpc60all_write_t::empty_song_t(nullptr));
    song->set_body(std::move(body));
    return song;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc60all_write_roundtrip) {
    mpc60all_write_t root(nullptr);
    std::unique_ptr<mpc60all_write_t::sequence_t> sequence(new mpc60all_write_t::sequence_t(nullptr));
    std::stringstream sequence_out(std::ios::in | std::ios::out | std::ios::binary);
    try {
        root.set_file_id(std::string("\x04", 1));
        root.set_file_format_version(mpc60all_write_t::ALL_WRAPPER_VERSION_V2);

    std::unique_ptr<mpc60all_write_t::sequence_header_t> sequence_header(new mpc60all_write_t::sequence_header_t(nullptr));
    sequence_header->set_sequence_number(0);
    sequence_header->set_sequence_length_in_bytes(make_u3le(1));
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

    kaitai::kstream sequence_ks(&sequence_out);
    sequence->_set_io(&sequence_ks);
    sequence->_write();

    root.set_total_number_of_bytes_in_all_sequences(static_cast<uint32_t>(sequence_out.str().size() + 1));
    root.set_sequences_terminator(std::string("\xff", 1));

    std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::sequence_t>>> sequences(
        new std::vector<std::unique_ptr<mpc60all_write_t::sequence_t>>());
    sequences->push_back(std::move(sequence));
    root.set_sequences(std::move(sequences));

    std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::song_t>>> songs(
        new std::vector<std::unique_ptr<mpc60all_write_t::song_t>>());
    songs->push_back(make_song());
    songs->push_back(make_song_terminator());
    root.set_songs(std::move(songs));
    } catch (const std::exception& e) {
        BOOST_FAIL(std::string("construction failed: ") + e.what());
    }

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    try {
        root._set_io(&ks);
        root._write();
    } catch (const std::exception& e) {
        BOOST_FAIL(std::string("write failed: ") + e.what());
    }

    const std::string bytes = out.str();
    auto byte_at = [&](std::size_t index) -> uint8_t {
        return static_cast<uint8_t>(static_cast<unsigned char>(bytes.at(index)));
    };
    const uint32_t counted_sequence_bytes =
        static_cast<uint32_t>(byte_at(2)) |
        (static_cast<uint32_t>(byte_at(3)) << 8) |
        (static_cast<uint32_t>(byte_at(4)) << 16) |
        (static_cast<uint32_t>(byte_at(5)) << 24);

    BOOST_REQUIRE_GE(bytes.size(), static_cast<std::size_t>(32));
    BOOST_CHECK_EQUAL(byte_at(0), 0x04);
    BOOST_CHECK_EQUAL(byte_at(1), 0x02);
    BOOST_CHECK_EQUAL(counted_sequence_bytes, static_cast<uint32_t>(sequence_out.str().size() + 1));
    BOOST_CHECK_EQUAL(byte_at(counted_sequence_bytes + 5), 0xff);
    BOOST_CHECK_EQUAL(byte_at(bytes.size() - 1), 0x00);

    const std::size_t song_offset = counted_sequence_bytes + 6;
    BOOST_CHECK_EQUAL(byte_at(song_offset + 0), 0x02);
    BOOST_CHECK_EQUAL(byte_at(song_offset + 1), 0x01);
    BOOST_CHECK_EQUAL(byte_at(song_offset + 2), 0x00);
    BOOST_CHECK_EQUAL(byte_at(song_offset + 3), 0x01);
    BOOST_CHECK_EQUAL(bytes.substr(song_offset + 4, 16), std::string("SONG01", 6) + std::string(10, ' '));
    BOOST_CHECK_EQUAL(byte_at(song_offset + 25), 0x01);
    BOOST_CHECK_EQUAL(byte_at(song_offset + 26), 0x01);
    BOOST_CHECK_EQUAL(byte_at(song_offset + 27), 0x01);
    BOOST_CHECK_EQUAL(byte_at(song_offset + 28), 0x01);
    BOOST_CHECK_EQUAL(byte_at(song_offset + 29), 0x00);

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc60all_write_t parsed(&parsed_io);
    try {
        parsed._read();
    } catch (const std::exception& e) {
        BOOST_FAIL(std::string("reparse failed: ") + e.what());
    }

    BOOST_CHECK_EQUAL(parsed.file_id(), std::string("\x04", 1));
    BOOST_CHECK_EQUAL(parsed.file_format_version(), mpc60all_write_t::ALL_WRAPPER_VERSION_V2);
    BOOST_CHECK_EQUAL(parsed.total_number_of_bytes_in_all_sequences(), static_cast<uint32_t>(sequence_out.str().size() + 1));
    BOOST_CHECK_EQUAL(parsed.sequences_terminator(), std::string("\xff", 1));
    BOOST_REQUIRE_EQUAL(parsed.sequences()->size(), 1U);
    BOOST_REQUIRE_EQUAL(parsed.songs()->size(), 2U);

    auto* parsed_sequence = parsed.sequences()->at(0).get();
    BOOST_REQUIRE(parsed_sequence->sequence_header() != nullptr);
    BOOST_CHECK_EQUAL(parsed_sequence->sequence_header()->sequence_length_in_bytes()->value(), 1);
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

    auto* parsed_song = parsed.songs()->at(0).get();
    BOOST_CHECK_EQUAL(parsed_song->step_count(), 2);
    auto* parsed_song_body = dynamic_cast<mpc60all_write_t::song_body_t*>(parsed_song->body());
    BOOST_REQUIRE(parsed_song_body != nullptr);
    BOOST_CHECK_EQUAL(parsed_song_body->song_number(), 1);
    BOOST_CHECK_EQUAL(parsed_song_body->song_name(), std::string("SONG01", 6) + std::string(10, ' '));
    BOOST_CHECK_EQUAL(parsed_song_body->reserved_1(), std::string("\x00", 1));
    BOOST_CHECK_EQUAL(parsed_song_body->reserved_2(), std::string("\x01", 1));
    BOOST_CHECK_EQUAL(parsed_song_body->reserved_3(), std::string(5, '\x00'));
    BOOST_REQUIRE_EQUAL(parsed_song_body->steps()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed_song_body->steps()->at(0)->sequence_number(), 1);
    BOOST_CHECK_EQUAL(parsed_song_body->steps()->at(0)->repeats(), 1);
    BOOST_CHECK_EQUAL(parsed_song_body->steps()->at(1)->sequence_number(), 1);
    BOOST_CHECK_EQUAL(parsed_song_body->steps()->at(1)->repeats(), 1);

    auto* parsed_song_terminator = parsed.songs()->at(1).get();
    BOOST_CHECK_EQUAL(parsed_song_terminator->step_count(), 0);
    auto* parsed_empty_song = dynamic_cast<mpc60all_write_t::empty_song_t*>(parsed_song_terminator->body());
    BOOST_REQUIRE(parsed_empty_song != nullptr);
}

BOOST_AUTO_TEST_CASE(test_mpc60all_write_seq_only_roundtrip) {
    mpc60all_write_t root(nullptr);
    std::unique_ptr<mpc60all_write_t::sequence_t> sequence(new mpc60all_write_t::sequence_t(nullptr));
    std::stringstream sequence_out(std::ios::in | std::ios::out | std::ios::binary);
    try {
        root.set_file_id(std::string("\x04", 1));
        root.set_file_format_version(mpc60all_write_t::ALL_WRAPPER_VERSION_V2);

        std::unique_ptr<mpc60all_write_t::sequence_header_t> sequence_header(new mpc60all_write_t::sequence_header_t(nullptr));
        sequence_header->set_sequence_number(0);
        sequence_header->set_sequence_length_in_bytes(make_u3le(1));
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

        kaitai::kstream sequence_ks(&sequence_out);
        sequence->_set_io(&sequence_ks);
        sequence->_write();

        root.set_total_number_of_bytes_in_all_sequences(static_cast<uint32_t>(sequence_out.str().size() + 1));
        root.set_sequences_terminator(std::string("\xff", 1));

        std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::sequence_t>>> sequences(
            new std::vector<std::unique_ptr<mpc60all_write_t::sequence_t>>());
        sequences->push_back(std::move(sequence));
        root.set_sequences(std::move(sequences));

        std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::song_t>>> songs(
            new std::vector<std::unique_ptr<mpc60all_write_t::song_t>>());
        songs->push_back(make_song_terminator());
        root.set_songs(std::move(songs));
    } catch (const std::exception& e) {
        BOOST_FAIL(std::string("construction failed: ") + e.what());
    }

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    try {
        root._set_io(&ks);
        root._write();
    } catch (const std::exception& e) {
        BOOST_FAIL(std::string("write failed: ") + e.what());
    }

    const std::string bytes = out.str();
    auto byte_at = [&](std::size_t index) -> uint8_t {
        return static_cast<uint8_t>(static_cast<unsigned char>(bytes.at(index)));
    };
    const uint32_t counted_sequence_bytes =
        static_cast<uint32_t>(byte_at(2)) |
        (static_cast<uint32_t>(byte_at(3)) << 8) |
        (static_cast<uint32_t>(byte_at(4)) << 16) |
        (static_cast<uint32_t>(byte_at(5)) << 24);

    BOOST_REQUIRE_GE(bytes.size(), static_cast<std::size_t>(8));
    BOOST_CHECK_EQUAL(byte_at(0), 0x04);
    BOOST_CHECK_EQUAL(byte_at(1), 0x02);
    BOOST_CHECK_EQUAL(counted_sequence_bytes, static_cast<uint32_t>(sequence_out.str().size() + 1));
    BOOST_CHECK_EQUAL(byte_at(counted_sequence_bytes + 5), 0xff);
    BOOST_CHECK_EQUAL(byte_at(bytes.size() - 1), 0x00);
    BOOST_CHECK_EQUAL(bytes.size(), sequence_out.str().size() + 8);

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc60all_write_t parsed(&parsed_io);
    try {
        parsed._read();
    } catch (const std::exception& e) {
        BOOST_FAIL(std::string("reparse failed: ") + e.what());
    }

    BOOST_REQUIRE_EQUAL(parsed.songs()->size(), 1U);
    BOOST_CHECK_EQUAL(parsed.songs()->at(0)->step_count(), 0);
    auto* parsed_empty_song = dynamic_cast<mpc60all_write_t::empty_song_t*>(parsed.songs()->at(0)->body());
    BOOST_REQUIRE(parsed_empty_song != nullptr);
}

BOOST_AUTO_TEST_CASE(test_mpc60all_write_v3_seq_only_roundtrip) {
    mpc60all_write_t root(nullptr);
    std::unique_ptr<mpc60all_write_t::sequence_t> sequence(new mpc60all_write_t::sequence_t(nullptr));
    std::stringstream sequence_out(std::ios::in | std::ios::out | std::ios::binary);
    try {
        root.set_file_id(std::string("\x04", 1));
        root.set_file_format_version(mpc60all_write_t::ALL_WRAPPER_VERSION_V3);

        std::unique_ptr<mpc60all_write_t::sequence_header_t> sequence_header(new mpc60all_write_t::sequence_header_t(nullptr));
        sequence_header->set_sequence_number(1);
        sequence_header->set_sequence_length_in_bytes(make_u3le(1));
        sequence_header->set_offset_from_bottom_of_sequence_to_sequence_start(make_u3le(0));
        sequence_header->set_sequence_name(std::string("SEQ01", 5) + std::string(11, ' '));
        sequence_header->set_loop_to_bar(mpc60all_write_t::OFF_ON_TRUE);
        sequence_header->set__unnamed5(0);
        sequence_header->set_loop_to_bar_number(9);
        sequence_header->set_number_of_bars(1);
        sequence_header->set_length_in_ticks(384);
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
        sequence_header->set_number_of_tempo_changes(1);
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

        kaitai::kstream sequence_ks(&sequence_out);
        sequence->_set_io(&sequence_ks);
        sequence->_write();

        root.set_total_number_of_bytes_in_all_sequences(static_cast<uint32_t>(sequence_out.str().size() + 1));
        root.set_sequences_terminator(std::string("\xff", 1));

        std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::sequence_t>>> sequences(
            new std::vector<std::unique_ptr<mpc60all_write_t::sequence_t>>());
        sequences->push_back(std::move(sequence));
        root.set_sequences(std::move(sequences));

        std::unique_ptr<std::vector<std::unique_ptr<mpc60all_write_t::song_t>>> songs(
            new std::vector<std::unique_ptr<mpc60all_write_t::song_t>>());
        songs->push_back(make_song_terminator());
        root.set_songs(std::move(songs));
    } catch (const std::exception& e) {
        BOOST_FAIL(std::string("construction failed: ") + e.what());
    }

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    try {
        root._set_io(&ks);
        root._write();
    } catch (const std::exception& e) {
        BOOST_FAIL(std::string("write failed: ") + e.what());
    }

    const std::string bytes = out.str();
    auto byte_at = [&](std::size_t index) -> uint8_t {
        return static_cast<uint8_t>(static_cast<unsigned char>(bytes.at(index)));
    };
    const uint32_t counted_sequence_bytes =
        static_cast<uint32_t>(byte_at(2)) |
        (static_cast<uint32_t>(byte_at(3)) << 8) |
        (static_cast<uint32_t>(byte_at(4)) << 16) |
        (static_cast<uint32_t>(byte_at(5)) << 24);

    BOOST_REQUIRE_GE(bytes.size(), static_cast<std::size_t>(8));
    BOOST_CHECK_EQUAL(byte_at(0), 0x04);
    BOOST_CHECK_EQUAL(byte_at(1), 0x03);
    BOOST_CHECK_EQUAL(counted_sequence_bytes, static_cast<uint32_t>(sequence_out.str().size() + 1));
    BOOST_CHECK_EQUAL(byte_at(counted_sequence_bytes + 5), 0xff);
    BOOST_CHECK_EQUAL(byte_at(bytes.size() - 1), 0x00);

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc60all_write_t parsed(&parsed_io);
    try {
        parsed._read();
    } catch (const std::exception& e) {
        BOOST_FAIL(std::string("reparse failed: ") + e.what());
    }

    BOOST_CHECK_EQUAL(parsed.file_id(), std::string("\x04", 1));
    BOOST_CHECK_EQUAL(parsed.file_format_version(), mpc60all_write_t::ALL_WRAPPER_VERSION_V3);
    BOOST_CHECK_EQUAL(parsed.total_number_of_bytes_in_all_sequences(), static_cast<uint32_t>(sequence_out.str().size() + 1));
    BOOST_CHECK_EQUAL(parsed.sequences_terminator(), std::string("\xff", 1));
    BOOST_REQUIRE_EQUAL(parsed.sequences()->size(), 1U);
    BOOST_REQUIRE_EQUAL(parsed.songs()->size(), 1U);
    BOOST_CHECK_EQUAL(parsed.sequences()->at(0)->sequence_header()->sequence_number(), 1);
    BOOST_CHECK_EQUAL(parsed.sequences()->at(0)->sequence_header()->sequence_name(), std::string("SEQ01", 5) + std::string(11, ' '));
    BOOST_CHECK_EQUAL(parsed.sequences()->at(0)->sequence_header()->loop_to_bar(), mpc60all_write_t::OFF_ON_TRUE);
    BOOST_CHECK_EQUAL(parsed.sequences()->at(0)->sequence_header()->loop_to_bar_number(), 9);
    BOOST_CHECK_EQUAL(parsed.sequences()->at(0)->sequence_header()->number_of_tempo_changes(), 1);
    auto* parsed_empty_song_v3 = dynamic_cast<mpc60all_write_t::empty_song_t*>(parsed.songs()->at(0)->body());
    BOOST_REQUIRE(parsed_empty_song_v3 != nullptr);
}
#else
BOOST_AUTO_TEST_CASE(test_mpc60all_write_roundtrip) {
    BOOST_FAIL("mpc60all_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
