// Hand-written smoke test for an MPC3000 ALL-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC3000ALL
#include "mpc3000all_write.h"

#include <sstream>

namespace {

std::unique_ptr<mpc3000seq_write_t::u3le_t> make_u3le(uint32_t value) {
    std::unique_ptr<mpc3000seq_write_t::u3le_t> result(
        new mpc3000seq_write_t::u3le_t(nullptr));
    result->set_b12(static_cast<uint16_t>(value & 0xffff));
    result->set_b3(static_cast<uint8_t>((value >> 16) & 0xff));
    return result;
}

std::unique_ptr<mpc3000seq_write_t::track_header_t> make_track_header() {
    std::unique_ptr<mpc3000seq_write_t::track_header_t> header(
        new mpc3000seq_write_t::track_header_t(nullptr));
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

std::unique_ptr<mpc3000seq_write_t::smpte_offset_t> make_smpte_offset() {
    std::unique_ptr<mpc3000seq_write_t::smpte_offset_t> smpte(
        new mpc3000seq_write_t::smpte_offset_t(nullptr));
    smpte->set_hundredth_frames(0);
    smpte->set_frames(0);
    smpte->set_seconds(0);
    smpte->set_minutes(0);
    smpte->set_hours(0);
    return smpte;
}

std::unique_ptr<mpc3000seq_write_t::mixer_t> make_mixer() {
    std::unique_ptr<mpc3000seq_write_t::mixer_t> mixer(
        new mpc3000seq_write_t::mixer_t(nullptr));
    mixer->set_stereo_mix(100);
    mixer->set_stereo_pan(50);
    mixer->set_individual_out_mix(0);
    mixer->set_individual_out(0);
    mixer->set_follow_stereo(true);
    return mixer;
}

std::unique_ptr<mpc3000seq_write_t::delays_t> make_delays() {
    std::unique_ptr<mpc3000seq_write_t::delays_t> delays(
        new mpc3000seq_write_t::delays_t(nullptr));
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
    return delays;
}

std::unique_ptr<mpc3000seq_write_t::tempo_change_t> make_tempo_change() {
    std::unique_ptr<mpc3000seq_write_t::tempo_change_t> tempo_change(
        new mpc3000seq_write_t::tempo_change_t(nullptr));
    tempo_change->set_ticks_from_sequence_start(0);
    tempo_change->set_factor1(0);
    tempo_change->set_factor2(1);
    return tempo_change;
}

std::unique_ptr<mpc3000seq_write_t::event_t> make_note_event() {
    std::unique_ptr<mpc3000seq_write_t::event_t> event(
        new mpc3000seq_write_t::event_t(nullptr));
    event->set_tick(0);
    event->set_duration_bits_1(0);
    event->set_track(0);
    event->set_duration_bits_2(0);
    event->set_id(60);
    std::unique_ptr<mpc3000seq_write_t::note_event_t> note(
        new mpc3000seq_write_t::note_event_t(nullptr));
    note->set_duration_bits_3(24);
    note->set_velocity(100);
    note->set_variation_type_bit_1(false);
    note->set_variation_value(0);
    note->set_variation_type_bit_2(false);
    event->set_note_event(std::move(note));
    return event;
}

std::unique_ptr<mpc3000seq_write_t::event_t> make_terminator_event() {
    std::unique_ptr<mpc3000seq_write_t::event_t> event(
        new mpc3000seq_write_t::event_t(nullptr));
    event->set_tick(0xfffff);
    event->set_terminator(std::string(5, '\x00'));
    return event;
}

std::unique_ptr<mpc3000all_write_t::sequence_t> make_sequence() {
    std::unique_ptr<mpc3000all_write_t::sequence_t> sequence(new mpc3000all_write_t::sequence_t(nullptr));

    std::unique_ptr<mpc3000all_write_t::misc_chunks_t> misc(new mpc3000all_write_t::misc_chunks_t(nullptr));
    std::unique_ptr<mpc3000seq_write_t::sequence_header_t> sequence_header(
        new mpc3000seq_write_t::sequence_header_t(nullptr));
    sequence_header->set_sequence_number(0);
    sequence_header->set_sequence_length_in_bytes(make_u3le(1));
    sequence_header->set__unnamed2(std::string(5, '\x00'));
    sequence_header->set_sequence_name(std::string("SEQ DEMO", 8) + std::string(8, '\x00'));
    sequence_header->set__unnamed4(std::string(1, '\x00'));
    sequence_header->set_loop_to_bar(false);
    sequence_header->set__unnamed6(0);
    sequence_header->set_loop_to_bar_number(0);
    sequence_header->set_number_of_bars(1);
    sequence_header->set_length_in_ticks(96);
    sequence_header->set_tempo(1200);
    misc->set_sequence_header(std::move(sequence_header));

    misc->set_smpte_offset(make_smpte_offset());

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::mixer_t>>> mixers(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::mixer_t>>());
    for (int i = 0; i < 64; ++i) {
        mixers->push_back(make_mixer());
    }
    misc->set_mixer(std::move(mixers));
    misc->set__unnamed3(std::string(2, '\x00'));
    misc->set_delays(make_delays());
    misc->set__unnamed5(std::string(3, '\x00'));
    misc->set__unnamed6(std::string(16, '\x00'));
    misc->set_last_active_track(0);
    misc->set_number_of_tempo_changes(1);
    misc->set_number_of_active_track_headers(1);

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::track_header_t>>> track_headers(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::track_header_t>>());
    track_headers->push_back(make_track_header());
    misc->set_track_headers(std::move(track_headers));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::tempo_change_t>>> tempo_changes(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::tempo_change_t>>());
    tempo_changes->push_back(make_tempo_change());
    misc->set_tempo_changes(std::move(tempo_changes));
    sequence->set_misc_chunks(std::move(misc));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000seq_write_t::event_t>>> events(
        new std::vector<std::unique_ptr<mpc3000seq_write_t::event_t>>());
    events->push_back(make_note_event());
    events->push_back(make_terminator_event());
    sequence->set_events(std::move(events));

    return sequence;
}

std::unique_ptr<mpc3000all_write_t::song_t> make_song() {
    std::unique_ptr<mpc3000all_write_t::song_t> song(new mpc3000all_write_t::song_t(nullptr));
    song->set_number_of_steps(1);

    std::unique_ptr<mpc3000all_write_t::song_t::song_body_t> body(new mpc3000all_write_t::song_t::song_body_t(nullptr));
    body->set_song_number(0);
    body->set_end_status(0);
    body->set_loop_back_step_number(0);
    body->set_song_name("SONG");
    body->set_smpte_offset(make_smpte_offset());

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000all_write_t::song_t::song_body_t::step_t>>> steps(
        new std::vector<std::unique_ptr<mpc3000all_write_t::song_t::song_body_t::step_t>>());
    std::unique_ptr<mpc3000all_write_t::song_t::song_body_t::step_t> step(
        new mpc3000all_write_t::song_t::song_body_t::step_t(nullptr));
    step->set_sequence_number(0);
    step->set_repetition_count(1);
    steps->push_back(std::move(step));
    body->set_steps(std::move(steps));
    song->set_song_body(std::move(body));
    return song;
}

std::unique_ptr<mpc3000all_write_t::song_t> make_song_terminator() {
    std::unique_ptr<mpc3000all_write_t::song_t> song(new mpc3000all_write_t::song_t(nullptr));
    song->set_number_of_steps(0);
    return song;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc3000all_write_roundtrip) {
    mpc3000all_write_t root(nullptr);
    std::unique_ptr<mpc3000all_write_t::sequence_t> sequence = make_sequence();
    std::stringstream sequence_out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream sequence_ks(&sequence_out);
    sequence->_set_io(&sequence_ks);
    sequence->_write();

    std::unique_ptr<mpc3000all_write_t::all_file_header_t> header(new mpc3000all_write_t::all_file_header_t(nullptr));
    header->set_file_id(std::string("\x04", 1));
    header->set_file_version(std::string("\x03", 1));
    header->set_total_number_of_bytes_in_all_sequences(static_cast<uint32_t>(sequence_out.str().size() + 1));
    root.set_all_file_header(std::move(header));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000all_write_t::sequence_t>>> sequences(
        new std::vector<std::unique_ptr<mpc3000all_write_t::sequence_t>>());
    sequences->push_back(std::move(sequence));
    root.set_sequences(std::move(sequences));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000all_write_t::song_t>>> songs(
        new std::vector<std::unique_ptr<mpc3000all_write_t::song_t>>());
    songs->push_back(make_song());
    songs->push_back(make_song_terminator());
    root.set_songs(std::move(songs));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc3000all_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_REQUIRE(parsed.all_file_header() != nullptr);
    BOOST_CHECK_EQUAL(parsed.all_file_header()->file_id(), std::string("\x04", 1));
    BOOST_CHECK_EQUAL(parsed.all_file_header()->file_version(), std::string("\x03", 1));
    BOOST_CHECK_EQUAL(parsed.all_file_header()->total_number_of_bytes_in_all_sequences(), static_cast<uint32_t>(sequence_out.str().size() + 1));

    BOOST_REQUIRE_EQUAL(parsed.sequences()->size(), 1U);
    auto* parsed_sequence = parsed.sequences()->at(0).get();
    BOOST_REQUIRE(parsed_sequence->misc_chunks() != nullptr);
    BOOST_REQUIRE(parsed_sequence->misc_chunks()->sequence_header() != nullptr);
    BOOST_CHECK_EQUAL(parsed_sequence->misc_chunks()->sequence_header()->sequence_name(), std::string("SEQ DEMO", 8) + std::string(8, '\x00'));
    BOOST_CHECK_EQUAL(parsed_sequence->misc_chunks()->number_of_active_track_headers(), 1);
    BOOST_REQUIRE_EQUAL(parsed_sequence->misc_chunks()->track_headers()->size(), 1U);
    BOOST_CHECK_EQUAL(parsed_sequence->misc_chunks()->track_headers()->at(0)->track_name(), std::string("TRACK001", 8) + std::string(8, '\x00'));
    BOOST_REQUIRE_EQUAL(parsed_sequence->events()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed_sequence->events()->at(0)->tick(), 0U);
    BOOST_CHECK_EQUAL(parsed_sequence->events()->at(0)->id(), 60);
    BOOST_REQUIRE(parsed_sequence->events()->at(0)->note_event() != nullptr);
    BOOST_CHECK_EQUAL(parsed_sequence->events()->at(0)->note_event()->velocity(), 100U);
    BOOST_CHECK_EQUAL(parsed_sequence->events()->at(1)->tick(), 0xfffffU);

    BOOST_REQUIRE_EQUAL(parsed.songs()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed.songs()->at(0)->number_of_steps(), 1U);
    BOOST_REQUIRE(parsed.songs()->at(0)->song_body() != nullptr);
    BOOST_CHECK_EQUAL(parsed.songs()->at(0)->song_body()->song_name(), "SONG");
    BOOST_REQUIRE_EQUAL(parsed.songs()->at(0)->song_body()->steps()->size(), 1U);
    BOOST_CHECK_EQUAL(parsed.songs()->at(0)->song_body()->steps()->at(0)->sequence_number(), 0U);
    BOOST_CHECK_EQUAL(parsed.songs()->at(1)->number_of_steps(), 0U);
    BOOST_CHECK(parsed.songs()->at(1)->_is_null_song_body());
}
#else
BOOST_AUTO_TEST_CASE(test_mpc3000all_write_roundtrip) {
    BOOST_FAIL("mpc3000all_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
