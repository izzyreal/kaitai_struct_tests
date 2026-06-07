// Hand-written smoke test for an MPC3000 SEQ-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC3000SEQ
#include "mpc3000seq_write.h"

#include <sstream>

namespace {

std::unique_ptr<mpc3000seq_write_t::u3le_t> make_u3le(uint32_t value) {
    std::unique_ptr<mpc3000seq_write_t::u3le_t> result(new mpc3000seq_write_t::u3le_t(nullptr));
    result->set_b12(static_cast<uint16_t>(value & 0xffff));
    result->set_b3(static_cast<uint8_t>((value >> 16) & 0xff));
    return result;
}

std::unique_ptr<mpc3000seq_write_t::track_header_t> make_track_header() {
    std::unique_ptr<mpc3000seq_write_t::track_header_t> header(new mpc3000seq_write_t::track_header_t(nullptr));
    header->set_absolute_recorded_track_number(1);
    header->set_user_track_number(1);
    header->set_track_mute(false);
    header->set_track_in_use(true);
    header->set_drum_track(false);
    header->set__unnamed6(0);
    header->set_primary_port_channel_assignment(0x10);
    header->set_secondary_port_channel_assignment(-1);
    header->set_track_name(std::string("TRACK001", 8) + std::string(8, '\x00'));
    header->set_track_volume(100);
    header->set_program_change_number(12);
    header->set__unnamed12(std::string(1, '\x00'));
    return header;
}

std::unique_ptr<mpc3000seq_write_t::bar_t> make_bar(int index) {
    std::unique_ptr<mpc3000seq_write_t::bar_t> bar(new mpc3000seq_write_t::bar_t(index, nullptr));
    bar->set_ticks_per_beat(96);
    bar->set_last_tick(static_cast<uint64_t>((index + 1) * 96));
    return bar;
}

std::unique_ptr<mpc3000seq_write_t::event_t> make_note_event() {
    std::unique_ptr<mpc3000seq_write_t::event_t> event(new mpc3000seq_write_t::event_t(nullptr));
    event->set_tick(0);
    event->set_duration_bits_1(0);
    event->set_track(0);
    event->set_duration_bits_2(0);
    event->set_id(60);
    std::unique_ptr<mpc3000seq_write_t::note_event_t> body(new mpc3000seq_write_t::note_event_t(nullptr));
    body->set_duration_bits_3(24);
    body->set_velocity(100);
    body->set_variation_type_bit_1(false);
    body->set_variation_value(0);
    body->set_variation_type_bit_2(false);
    event->set_note_event(std::move(body));
    return event;
}

std::unique_ptr<mpc3000seq_write_t::event_t> make_terminator_event() {
    std::unique_ptr<mpc3000seq_write_t::event_t> event(new mpc3000seq_write_t::event_t(nullptr));
    event->set_tick(0xfffff);
    event->set_terminator(std::string(5, '\x00'));
    return event;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc3000seq_write_roundtrip) {
    mpc3000seq_write_t root(nullptr);
    root.set_file_id(std::string("\x03", 1));
    root.set_file_format_version(std::string("\x03", 1));

    std::unique_ptr<mpc3000seq_write_t::sequence_header_t> sequence_header(new mpc3000seq_write_t::sequence_header_t(nullptr));
    sequence_header->set_sequence_number(0);
    sequence_header->set_sequence_length_in_bytes(make_u3le(0));
    sequence_header->set__unnamed2(std::string(5, '\x00'));
    sequence_header->set_sequence_name(std::string("SEQ DEMO", 8) + std::string(8, '\x00'));
    sequence_header->set__unnamed4(std::string(1, '\x00'));
    sequence_header->set_loop_to_bar(false);
    sequence_header->set__unnamed6(0);
    sequence_header->set_loop_to_bar_number(0);
    sequence_header->set_number_of_bars(999);
    sequence_header->set_length_in_ticks(999 * 96);
    sequence_header->set_tempo(1200);
    root.set_sequence_header(std::move(sequence_header));

    std::unique_ptr<mpc3000seq_write_t::smpte_offset_t> smpte(new mpc3000seq_write_t::smpte_offset_t(nullptr));
    smpte->set_hundredth_frames(0);
    smpte->set_frames(0);
    smpte->set_seconds(0);
    smpte->set_minutes(0);
    smpte->set_hours(0);
    root.set_smpte_offset(std::move(smpte));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::mixer_t>>> mixers(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::mixer_t>>());
    for (int i = 0; i < 64; ++i) {
        std::unique_ptr<mpc3000seq_write_t::mixer_t> mixer(new mpc3000seq_write_t::mixer_t(nullptr));
        mixer->set_stereo_mix(100);
        mixer->set_stereo_pan(50);
        mixer->set_individual_out_mix(0);
        mixer->set_individual_out(0);
        mixer->set_follow_stereo(true);
        mixers->push_back(std::move(mixer));
    }
    root.set_mixer(std::move(mixers));
    root.set__unnamed5(std::string(2, '\x00'));

    std::unique_ptr<mpc3000seq_write_t::delays_t> delays(new mpc3000seq_write_t::delays_t(nullptr));
    delays->set_volume1(0);
    delays->set_volume2(0);
    delays->set_volume3(0);
    delays->set_pan1(50);
    delays->set_pan2(50);
    delays->set_pan3(50);
    delays->set_time1(0);
    delays->set_time2(0);
    delays->set_time3(0);
    delays->set_feedback1(0);
    delays->set_feedback2(0);
    delays->set_feedback3(0);
    root.set_delays(std::move(delays));
    root.set__unnamed7(std::string(3, '\x00'));
    root.set__unnamed8(std::string(16, '\x00'));
    root.set_last_active_track(0);
    root.set_number_of_tempo_changes(1);
    root.set_number_of_active_track_headers(1);

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::track_header_t>>> root_track_headers(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::track_header_t>>());
    root_track_headers->push_back(make_track_header());
    root.set_track_headers(std::move(root_track_headers));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::tempo_change_t>>> tempo_changes(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::tempo_change_t>>());
    std::unique_ptr<mpc3000seq_write_t::tempo_change_t> tempo_change(new mpc3000seq_write_t::tempo_change_t(nullptr));
    tempo_change->set_ticks_from_sequence_start(0);
    tempo_change->set_factor1(0);
    tempo_change->set_factor2(1);
    tempo_changes->push_back(std::move(tempo_change));
    root.set_tempo_changes(std::move(tempo_changes));

    std::unique_ptr<mpc3000seq_write_t::sequence_t> sequence(new mpc3000seq_write_t::sequence_t(nullptr));
    sequence->set_name_part_1("SEQPART1");
    sequence->set_name_part_2("SEQPART2");

    std::unique_ptr<mpc3000seq_write_t::sequence_body_t> body(new mpc3000seq_write_t::sequence_body_t(nullptr));
    body->set_is_used(1);
    body->set_index(1);
    body->set__unnamed2(std::string(7, '\x00'));
    body->set_bar_count(999);
    body->set_last_tick(999 * 96);
    body->set__unnamed5(std::string(16, '\x00'));
    body->set_loop_start_bar_index(0);
    body->set_loop_end_bar_index(998);
    body->set_loop_enabled(false);
    body->set__unnamed9(0);
    std::unique_ptr<mpc3000seq_write_t::sequence_body_t::start_time_t> start_time(
        new mpc3000seq_write_t::sequence_body_t::start_time_t(nullptr));
    start_time->set_hours(0);
    start_time->set_minutes(0);
    start_time->set_seconds(0);
    start_time->set_frames(0);
    start_time->set_frame_decimals(0);
    body->set_start_time(std::move(start_time));
    body->set__unnamed11(std::string(6, '\x00'));
    body->set_last_tick2(999 * 96);
    body->set__unnamed13(std::string(52, '\x00'));

    std::unique_ptr<std::vector<std::string>> device_names(new std::vector<std::string>());
    device_names->push_back(std::string(8, '\x00'));
    for (int i = 1; i < 33; ++i) {
        std::string name = "DEV" + std::to_string(i);
        device_names->push_back(name + std::string(8 - name.size(), '\x00'));
    }
    body->set_device_names(std::move(device_names));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::track_header_t>>> body_track_headers(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::track_header_t>>());
    body_track_headers->push_back(make_track_header());
    body->set_track_headers(std::move(body_track_headers));
    body->set__unnamed16(std::string(3587, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::bar_t>>> bars(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::bar_t>>());
    for (int i = 0; i < 999; ++i) {
        bars->push_back(make_bar(i));
    }
    body->set_bars(std::move(bars));
    body->set__unnamed18(std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::bar_t>>>(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::bar_t>>()));
    body->set__unnamed19(std::string(865, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::event_t>>> events(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::event_t>>());
    events->push_back(make_note_event());
    events->push_back(make_terminator_event());
    body->set_events(std::move(events));

    sequence->set_body(std::move(body));
    root.set_sequence(std::move(sequence));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._check();
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc3000seq_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.file_id(), std::string("\x03", 1));
    BOOST_CHECK_EQUAL(parsed.file_format_version(), std::string("\x03", 1));
    BOOST_CHECK_EQUAL(parsed.sequence_header()->sequence_name(), std::string("SEQ DEMO", 8) + std::string(8, '\x00'));
    BOOST_REQUIRE_EQUAL(parsed.mixer()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.mixer()->at(0)->stereo_mix(), 100);
    BOOST_CHECK_EQUAL(parsed.number_of_active_track_headers(), 1);
    BOOST_REQUIRE_EQUAL(parsed.track_headers()->size(), 1U);
    BOOST_CHECK_EQUAL(parsed.track_headers()->at(0)->track_name(), std::string("TRACK001", 8) + std::string(8, '\x00'));

    BOOST_REQUIRE(parsed.sequence() != nullptr);
    BOOST_CHECK_EQUAL(parsed.sequence()->name_part_1(), "SEQPART1");
    BOOST_CHECK_EQUAL(parsed.sequence()->name_part_2(), "SEQPART2");
    BOOST_REQUIRE(parsed.sequence()->body() != nullptr);
    BOOST_CHECK_EQUAL(parsed.sequence()->body()->bar_count(), 999);
    BOOST_REQUIRE_EQUAL(parsed.sequence()->body()->bars()->size(), 999U);
    BOOST_CHECK_EQUAL(parsed.sequence()->body()->bars()->at(998)->last_tick(), 999U * 96U);
    BOOST_REQUIRE_EQUAL(parsed.sequence()->body()->events()->size(), 2U);

    auto* first_event = parsed.sequence()->body()->events()->at(0).get();
    BOOST_CHECK_EQUAL(first_event->tick(), 0U);
    BOOST_CHECK_EQUAL(first_event->id(), 60);
    BOOST_REQUIRE(first_event->note_event() != nullptr);
    BOOST_CHECK_EQUAL(first_event->note_event()->velocity(), 100U);
    BOOST_CHECK_EQUAL(first_event->note_event()->duration_bits_3(), 24);

    auto* terminator = parsed.sequence()->body()->events()->at(1).get();
    BOOST_CHECK_EQUAL(terminator->tick(), 0xfffffU);
    BOOST_CHECK_EQUAL(terminator->terminator(), std::string(5, '\x00'));
}
#else
BOOST_AUTO_TEST_CASE(test_mpc3000seq_write_roundtrip) {
    BOOST_FAIL("mpc3000seq_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
