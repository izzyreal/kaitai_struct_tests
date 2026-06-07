// Hand-written smoke test for an MPC2000XL ALL-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC2000XLALL
#include "mpc2000xlall_write.h"

#include <sstream>

namespace {

std::unique_ptr<std::vector<std::string>> make_ascii_strings(std::size_t count, std::size_t width, const std::string& prefix) {
    std::unique_ptr<std::vector<std::string>> values(new std::vector<std::string>());
    values->reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        std::string value = (i == 0 && prefix.empty()) ? "" : prefix + std::to_string(i);
        if (value.size() > width) {
            value.resize(width);
        } else {
            value += std::string(width - value.size(), '\x00');
        }
        values->push_back(value);
    }
    return values;
}

std::unique_ptr<std::vector<uint8_t>> make_u1s(std::size_t count, uint8_t value) {
    return std::unique_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(count, value));
}

std::unique_ptr<std::vector<bool>> make_bools(std::size_t count, bool value) {
    return std::unique_ptr<std::vector<bool>>(new std::vector<bool>(count, value));
}

std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::track_status_t>>> make_track_statuses(std::size_t count) {
    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::track_status_t>>> statuses(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::track_status_t>>());
    for (std::size_t i = 0; i < count; ++i) {
        std::unique_ptr<mpc2000xlall_write_t::track_status_t> status(new mpc2000xlall_write_t::track_status_t(nullptr));
        status->set_unused_or_used(true);
        status->set_off_or_on(true);
        status->set_transmit_program_changes(false);
        status->set__unnamed3(0);
        statuses->push_back(std::move(status));
    }
    return statuses;
}

std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::midi_switch_t>>> make_midi_switches() {
    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::midi_switch_t>>> switches(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::midi_switch_t>>());
    for (int i = 0; i < 4; ++i) {
        std::unique_ptr<mpc2000xlall_write_t::midi_switch_t> sw(new mpc2000xlall_write_t::midi_switch_t(nullptr));
        sw->set_controller(static_cast<uint8_t>(64 + i));
        sw->set_function(0);
        switches->push_back(std::move(sw));
    }
    return switches;
}

std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::location_t>>> make_locations() {
    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::location_t>>> locations(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::location_t>>());
    for (int i = 0; i < 9; ++i) {
        std::unique_ptr<mpc2000xlall_write_t::location_t> location(new mpc2000xlall_write_t::location_t(nullptr));
        location->set_bar(static_cast<uint16_t>(i));
        location->set_beat(0);
        location->set_clock(0);
        locations->push_back(std::move(location));
    }
    return locations;
}

std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::sequence_meta_t>>> make_sequence_metas() {
    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::sequence_meta_t>>> metas(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::sequence_meta_t>>());
    for (int i = 0; i < 99; ++i) {
        std::unique_ptr<mpc2000xlall_write_t::sequence_meta_t> meta(new mpc2000xlall_write_t::sequence_meta_t(nullptr));
        std::string name = (i == 0) ? "SEQ DEMO" : "";
        if (name.size() < 16) {
            name += std::string(16 - name.size(), '\x00');
        }
        meta->set_name(name);
        meta->set_is_used(i == 0 ? 1 : 0);
        metas->push_back(std::move(meta));
    }
    return metas;
}

std::unique_ptr<mpc2000xlall_write_t::song_t> make_song(int index) {
    std::unique_ptr<mpc2000xlall_write_t::song_t> song(new mpc2000xlall_write_t::song_t(nullptr));
    std::string name = index == 0 ? "SONG DEMO" : "";
    if (name.size() < 16) {
        name += std::string(16 - name.size(), '\x00');
    }
    song->set_name(name);

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::song_step_t>>> steps(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::song_step_t>>());
    for (int i = 0; i < 250; ++i) {
        std::unique_ptr<mpc2000xlall_write_t::song_step_t> step(new mpc2000xlall_write_t::song_step_t(nullptr));
        step->set_sequence_index(0);
        step->set_repeat_count(1);
        steps->push_back(std::move(step));
    }
    song->set_steps(std::move(steps));
    song->set__unnamed2(std::string(2, '\x00'));
    song->set_is_used(index == 0);
    song->set_loop_first_step(0);
    song->set_loop_last_step(0);
    song->set_is_loop_enabled(false);
    song->set__unnamed7(std::string(6, '\x00'));
    return song;
}

std::unique_ptr<mpc2000xlall_write_t::bar_t> make_bar(int index, uint64_t last_tick) {
    std::unique_ptr<mpc2000xlall_write_t::bar_t> bar(new mpc2000xlall_write_t::bar_t(index, nullptr));
    bar->set_ticks_per_beat(96);
    bar->set_last_tick(last_tick);
    return bar;
}

std::unique_ptr<mpc2000xlall_write_t::event_t> make_note_event() {
    std::unique_ptr<mpc2000xlall_write_t::event_t> event(new mpc2000xlall_write_t::event_t(nullptr));
    event->set_tick(0);
    event->set_duration_bits_1(0);
    event->set_track(0);
    event->set_duration_bits_2(0);
    event->set_id(60);
    std::unique_ptr<mpc2000xlall_write_t::note_event_t> note(new mpc2000xlall_write_t::note_event_t(nullptr));
    note->set_duration_bits_3(24);
    note->set_velocity(100);
    note->set_variation_type_bit_1(false);
    note->set_variation_value(0);
    note->set_variation_type_bit_2(false);
    event->set_note_event(std::move(note));
    return event;
}

std::unique_ptr<mpc2000xlall_write_t::event_t> make_control_change_event() {
    std::unique_ptr<mpc2000xlall_write_t::event_t> event(new mpc2000xlall_write_t::event_t(nullptr));
    event->set_tick(24);
    event->set_duration_bits_1(0);
    event->set_track(0);
    event->set_duration_bits_2(0);
    event->set_id(0xb0);
    std::unique_ptr<mpc2000xlall_write_t::control_change_event_t> cc(new mpc2000xlall_write_t::control_change_event_t(nullptr));
    cc->set_controller(7);
    cc->set_value(100);
    cc->set__unnamed2(std::string(1, '\x00'));
    event->set_control_change(std::move(cc));
    return event;
}

std::unique_ptr<mpc2000xlall_write_t::event_t> make_pitch_bend_event() {
    std::unique_ptr<mpc2000xlall_write_t::event_t> event(new mpc2000xlall_write_t::event_t(nullptr));
    event->set_tick(48);
    event->set_duration_bits_1(0);
    event->set_track(0);
    event->set_duration_bits_2(0);
    event->set_id(0xe0);
    std::unique_ptr<mpc2000xlall_write_t::pitch_bend_event_t> bend(new mpc2000xlall_write_t::pitch_bend_event_t(nullptr));
    bend->set_amount_bits_1(0);
    bend->set_amount_bits_2(64);
    bend->set__unnamed2(std::string(1, '\x00'));
    event->set_pitch_bend(std::move(bend));
    return event;
}

std::unique_ptr<mpc2000xlall_write_t::event_t> make_program_change_event() {
    std::unique_ptr<mpc2000xlall_write_t::event_t> event(new mpc2000xlall_write_t::event_t(nullptr));
    event->set_tick(72);
    event->set_duration_bits_1(0);
    event->set_track(0);
    event->set_duration_bits_2(0);
    event->set_id(0xc0);
    std::unique_ptr<mpc2000xlall_write_t::program_change_event_t> program(new mpc2000xlall_write_t::program_change_event_t(nullptr));
    program->set_program(12);
    program->set__unnamed1(std::string(2, '\x00'));
    event->set_program_change(std::move(program));
    return event;
}

std::unique_ptr<mpc2000xlall_write_t::event_t> make_ch_pressure_event() {
    std::unique_ptr<mpc2000xlall_write_t::event_t> event(new mpc2000xlall_write_t::event_t(nullptr));
    event->set_tick(84);
    event->set_duration_bits_1(0);
    event->set_track(0);
    event->set_duration_bits_2(0);
    event->set_id(0xd0);
    std::unique_ptr<mpc2000xlall_write_t::ch_pressure_event_t> pressure(new mpc2000xlall_write_t::ch_pressure_event_t(nullptr));
    pressure->set_pressure(70);
    pressure->set__unnamed1(std::string(2, '\x00'));
    event->set_ch_pressure(std::move(pressure));
    return event;
}

std::unique_ptr<mpc2000xlall_write_t::event_t> make_poly_pressure_event() {
    std::unique_ptr<mpc2000xlall_write_t::event_t> event(new mpc2000xlall_write_t::event_t(nullptr));
    event->set_tick(90);
    event->set_duration_bits_1(0);
    event->set_track(0);
    event->set_duration_bits_2(0);
    event->set_id(0xa0);
    std::unique_ptr<mpc2000xlall_write_t::poly_pressure_event_t> pressure(new mpc2000xlall_write_t::poly_pressure_event_t(nullptr));
    pressure->set_note(64);
    pressure->set_pressure(80);
    pressure->set__unnamed2(std::string(1, '\x00'));
    event->set_poly_pressure(std::move(pressure));
    return event;
}

std::unique_ptr<mpc2000xlall_write_t::event_t> make_exclusive_event() {
    std::unique_ptr<mpc2000xlall_write_t::event_t> event(new mpc2000xlall_write_t::event_t(nullptr));
    event->set_tick(96);
    event->set_duration_bits_1(0);
    event->set_track(0);
    event->set_duration_bits_2(0);
    event->set_id(0xf0);
    std::unique_ptr<mpc2000xlall_write_t::exclusive_event_t> exclusive(new mpc2000xlall_write_t::exclusive_event_t(nullptr));
    exclusive->set__unnamed0(std::string(3, '\x00'));
    exclusive->set_bytes(std::string("\xf0\x47", 2));
    std::unique_ptr<mpc2000xlall_write_t::mixer_event_t> mixer(new mpc2000xlall_write_t::mixer_event_t(nullptr));
    mixer->set__unnamed0(std::string(3, '\x00'));
    mixer->set_param(3);
    mixer->set_pad_index(4);
    mixer->set_value(99);
    mixer->set__unnamed4(std::string(2, '\x00'));
    exclusive->set_mixer(std::move(mixer));
    exclusive->set__unnamed3(std::string(14, '\x00'));
    event->set_exclusive(std::move(exclusive));
    return event;
}

std::unique_ptr<mpc2000xlall_write_t::event_t> make_terminator_event() {
    std::unique_ptr<mpc2000xlall_write_t::event_t> event(new mpc2000xlall_write_t::event_t(nullptr));
    event->set_tick(0xfffff);
    event->set_terminator(std::string(5, '\x00'));
    return event;
}

std::unique_ptr<mpc2000xlall_write_t::tracks_t> make_tracks() {
    std::unique_ptr<mpc2000xlall_write_t::tracks_t> tracks(new mpc2000xlall_write_t::tracks_t(nullptr));
    tracks->set_names(make_ascii_strings(64, 16, "TRK"));
    tracks->set_device(make_u1s(64, 1));
    tracks->set_bus(make_u1s(64, 0));
    tracks->set_program_change(make_u1s(64, 0));
    tracks->set_velocity_ratio(make_u1s(64, 100));
    tracks->set_status(make_track_statuses(64));
    tracks->set_unknown(std::string(64, '\x00'));
    return tracks;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc2000xlall_write_roundtrip) {
    mpc2000xlall_write_t root(nullptr);
    root.set_magic("MPC2KXL ALL 1.00");

    std::unique_ptr<mpc2000xlall_write_t::defaults_t> defaults(new mpc2000xlall_write_t::defaults_t(nullptr));
    defaults->set_sequence_name(std::string("SEQ DEMO", 8) + std::string(8, '\x00'));
    defaults->set__unnamed1(std::string(6, '\x00'));
    defaults->set_tempo(1200);
    defaults->set_numerator(4);
    defaults->set_denominator(4);
    defaults->set_bar_count(1);
    defaults->set_tick_count(96);
    defaults->set_unknown1(std::unique_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>(4, 0)));
    defaults->set_unknown2(std::string(74, '\x00'));
    defaults->set_device_names(make_ascii_strings(33, 8, "DEV"));
    defaults->set_track_names(make_ascii_strings(64, 16, "TRK"));
    defaults->set_devices(make_u1s(64, 1));
    defaults->set_buses(make_u1s(64, 0));
    defaults->set_programs(make_u1s(64, 0));
    defaults->set_track_velocities(make_u1s(64, 100));
    defaults->set_track_statuses(make_track_statuses(64));
    defaults->set__unnamed16(std::string(64, '\x00'));
    root.set_defaults(std::move(defaults));

    std::unique_ptr<mpc2000xlall_write_t::sequencer_t> sequencer(new mpc2000xlall_write_t::sequencer_t(nullptr));
    sequencer->set_active_sequence(0);
    sequencer->set__unnamed1(std::string(1, '\x00'));
    sequencer->set_active_track(0);
    sequencer->set__unnamed3(std::string(1, '\x00'));
    sequencer->set_master_tempo(1200);
    sequencer->set_tempo_source_is_sequence(true);
    sequencer->set__unnamed6(0);
    sequencer->set_timing_correct(0);
    sequencer->set_time_display_style(false);
    sequencer->set__unnamed9(0);
    sequencer->set_second_sequence_enabled(false);
    sequencer->set__unnamed11(0);
    sequencer->set_sequence_sequence_index(0);
    root.set_sequencer(std::move(sequencer));
    root.set__unnamed3(std::string(10, '\x00'));

    std::unique_ptr<mpc2000xlall_write_t::count_t> count(new mpc2000xlall_write_t::count_t(nullptr));
    count->set_enabled(false);
    count->set__unnamed1(0);
    count->set_count_in_mode(0);
    count->set_click_volume(100);
    count->set_rate(0);
    count->set_enabled_in_play(false);
    count->set__unnamed6(0);
    count->set_enabled_in_rec(false);
    count->set__unnamed8(0);
    count->set_click_output(0);
    count->set_wait_for_key(false);
    count->set__unnamed11(0);
    count->set_sound_source(0);
    count->set_accent_pad_index(0);
    count->set_normal_pad_index(1);
    count->set_accent_velo(127);
    count->set_normal_velo(100);
    root.set_count(std::move(count));

    std::unique_ptr<mpc2000xlall_write_t::midi_output_t> midi_output(new mpc2000xlall_write_t::midi_output_t(nullptr));
    midi_output->set_soft_thru_mode(0);
    root.set_midi_output(std::move(midi_output));

    std::unique_ptr<mpc2000xlall_write_t::midi_input_t> midi_input(new mpc2000xlall_write_t::midi_input_t(nullptr));
    midi_input->set_receive_channel(0);
    midi_input->set_sustain_pedal_to_duration(false);
    midi_input->set__unnamed2(0);
    midi_input->set_filter_enabled(false);
    midi_input->set__unnamed4(0);
    midi_input->set_filter_type(0);
    midi_input->set_multi_rec_enabled(false);
    midi_input->set__unnamed7(0);
    midi_input->set_multi_rec_destination_tracks(make_u1s(34, 0));
    midi_input->set_note_pass_enabled(true);
    midi_input->set__unnamed10(0);
    midi_input->set_pitch_bend_pass_enabled(true);
    midi_input->set__unnamed12(0);
    midi_input->set_pgm_change_pass_enabled(true);
    midi_input->set__unnamed14(0);
    midi_input->set_ch_pressure_pass_enabled(true);
    midi_input->set__unnamed16(0);
    midi_input->set_poly_pressure_pass_enabled(true);
    midi_input->set__unnamed18(0);
    midi_input->set_exclusive_pass_enabled(true);
    midi_input->set__unnamed20(0);
    midi_input->set_cc_pass_enabled(make_bools(128, true));
    root.set_midi_input(std::move(midi_input));

    std::unique_ptr<mpc2000xlall_write_t::midi_sync_t> midi_sync(new mpc2000xlall_write_t::midi_sync_t(nullptr));
    midi_sync->set_in_mode(0);
    midi_sync->set_out_mode(0);
    midi_sync->set_shift_early(0);
    midi_sync->set_send_mmc_enabled(false);
    midi_sync->set__unnamed4(0);
    midi_sync->set_frame_rate(0);
    midi_sync->set_input(0);
    midi_sync->set_output(0);
    root.set_midi_sync(std::move(midi_sync));

    std::unique_ptr<mpc2000xlall_write_t::song_global_t> song_global(new mpc2000xlall_write_t::song_global_t(nullptr));
    song_global->set_default_song_name(std::string("SONG DEMO", 9) + std::string(7, '\x00'));
    song_global->set_ignore_tempo_change_events_in_sequence(false);
    song_global->set__unnamed2(0);
    root.set_song_global(std::move(song_global));
    root.set__unnamed9(std::string(5, '\x00'));
    root.set_locations(make_locations());

    std::unique_ptr<mpc2000xlall_write_t::misc_t> misc(new mpc2000xlall_write_t::misc_t(nullptr));
    misc->set_tap_averaging(0);
    misc->set_midi_sync_in_receive_mmc_enabled(false);
    misc->set_midi_switch(make_midi_switches());
    root.set_misc(std::move(misc));
    root.set__unnamed12(std::string(3, '\x00'));

    std::unique_ptr<mpc2000xlall_write_t::step_edit_options_t> step_edit_options(new mpc2000xlall_write_t::step_edit_options_t(nullptr));
    step_edit_options->set_auto_step_increment(false);
    step_edit_options->set_duration_of_recorded_notes(0);
    step_edit_options->set_tc_value_percentage(50);
    root.set_step_edit_options(std::move(step_edit_options));
    root.set_prog_change_to_seq(false);
    root.set__unnamed15(std::string(78, '\x00'));
    root.set_sequences_metas(make_sequence_metas());

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::song_t>>> songs(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::song_t>>());
    for (int i = 0; i < 20; ++i) {
        songs->push_back(make_song(i));
    }
    root.set_songs(std::move(songs));

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::sequence_t>>> sequences(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::sequence_t>>());
    std::unique_ptr<mpc2000xlall_write_t::sequence_t> sequence(new mpc2000xlall_write_t::sequence_t(nullptr));
    sequence->set_name_part_1("SEQPART1");
    sequence->set_name_part_2("SEQPART2");

    std::unique_ptr<mpc2000xlall_write_t::sequence_body_t> body(new mpc2000xlall_write_t::sequence_body_t(nullptr));
    body->set_is_used(1);
    body->set_index(1);
    body->set__unnamed2(std::string(7, '\x00'));
    body->set_bar_count(1);
    body->set_last_tick(96);
    body->set__unnamed5(std::string(16, '\x00'));
    body->set_loop_start_bar_index(0);
    body->set_loop_end_bar_index(0);
    body->set_loop_enabled(false);
    body->set__unnamed9(0);
    std::unique_ptr<mpc2000xlall_write_t::sequence_body_t::start_time_t> start_time(
        new mpc2000xlall_write_t::sequence_body_t::start_time_t(nullptr));
    start_time->set_hours(0);
    start_time->set_minutes(0);
    start_time->set_seconds(0);
    start_time->set_frames(0);
    start_time->set_frame_decimals(0);
    body->set_start_time(std::move(start_time));
    body->set__unnamed11(std::string(6, '\x00'));
    body->set_last_tick2(96);
    body->set__unnamed13(std::string(52, '\x00'));
    body->set_device_names(make_ascii_strings(33, 8, "DEV"));
    body->set_tracks(make_tracks());
    body->set__unnamed16(std::string(3587, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::bar_t>>> bars(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::bar_t>>());
    bars->push_back(make_bar(0, 96));
    body->set_bars(std::move(bars));

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::bar_t>>> remaining_bars(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::bar_t>>());
    for (int i = 1; i < 999; ++i) {
        remaining_bars->push_back(make_bar(i, 96));
    }
    body->set__unnamed18(std::move(remaining_bars));
    body->set__unnamed19(std::string(865, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000xlall_write_t::event_t>>> events(
        new std::vector<std::unique_ptr<mpc2000xlall_write_t::event_t>>());
    events->push_back(make_note_event());
    events->push_back(make_control_change_event());
    events->push_back(make_pitch_bend_event());
    events->push_back(make_program_change_event());
    events->push_back(make_ch_pressure_event());
    events->push_back(make_poly_pressure_event());
    events->push_back(make_exclusive_event());
    events->push_back(make_terminator_event());
    body->set_events(std::move(events));

    sequence->set_body(std::move(body));
    sequences->push_back(std::move(sequence));
    root.set_sequences(std::move(sequences));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._check();
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc2000xlall_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.magic(), "MPC2KXL ALL 1.00");
    BOOST_CHECK_EQUAL(parsed.defaults()->sequence_name(), std::string("SEQ DEMO", 8) + std::string(8, '\x00'));
    BOOST_CHECK_EQUAL(parsed.defaults()->tempo(), 1200);
    BOOST_REQUIRE_EQUAL(parsed.defaults()->device_names()->size(), 33U);
    BOOST_REQUIRE_EQUAL(parsed.defaults()->track_names()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.sequencer()->master_tempo(), 1200);
    BOOST_REQUIRE_EQUAL(parsed.locations()->size(), 9U);
    BOOST_CHECK_EQUAL(parsed.locations()->at(0)->bar(), 0);
    BOOST_REQUIRE_EQUAL(parsed.sequences_metas()->size(), 99U);
    BOOST_CHECK_EQUAL(parsed.sequences_metas()->at(0)->is_used(), 1);
    BOOST_REQUIRE_EQUAL(parsed.songs()->size(), 20U);
    BOOST_CHECK(parsed.songs()->at(0)->is_used());

    BOOST_REQUIRE_EQUAL(parsed.sequences()->size(), 1U);
    auto* parsed_sequence = parsed.sequences()->at(0).get();
    BOOST_CHECK_EQUAL(parsed_sequence->name_part_1(), "SEQPART1");
    BOOST_CHECK_EQUAL(parsed_sequence->name_part_2(), "SEQPART2");
    BOOST_REQUIRE(parsed_sequence->body() != nullptr);
    BOOST_CHECK_EQUAL(parsed_sequence->body()->bar_count(), 1);
    BOOST_REQUIRE_EQUAL(parsed_sequence->body()->bars()->size(), 1U);
    BOOST_CHECK_EQUAL(parsed_sequence->body()->bars()->at(0)->last_tick(), 96U);
    BOOST_REQUIRE(parsed_sequence->body()->tracks() != nullptr);
    BOOST_CHECK_EQUAL(parsed_sequence->body()->tracks()->names()->at(0), std::string("TRK0", 4) + std::string(12, '\x00'));
    BOOST_REQUIRE_EQUAL(parsed_sequence->body()->events()->size(), 8U);

    auto* first_event = parsed_sequence->body()->events()->at(0).get();
    BOOST_CHECK_EQUAL(first_event->tick(), 0U);
    BOOST_CHECK_EQUAL(first_event->id(), 60);
    BOOST_REQUIRE(first_event->note_event() != nullptr);
    BOOST_CHECK_EQUAL(first_event->note_event()->duration_bits_3(), 24);
    BOOST_CHECK_EQUAL(first_event->note_event()->velocity(), 100U);

    auto* second_event = parsed_sequence->body()->events()->at(1).get();
    BOOST_CHECK_EQUAL(second_event->id(), 0xb0);
    BOOST_REQUIRE(second_event->control_change() != nullptr);
    BOOST_CHECK_EQUAL(second_event->control_change()->controller(), 7);
    BOOST_CHECK_EQUAL(second_event->control_change()->value(), 100);

    auto* third_event = parsed_sequence->body()->events()->at(2).get();
    BOOST_CHECK_EQUAL(third_event->id(), 0xe0);
    BOOST_REQUIRE(third_event->pitch_bend() != nullptr);
    BOOST_CHECK_EQUAL(third_event->pitch_bend()->amount_bits_1(), 0U);
    BOOST_CHECK_EQUAL(third_event->pitch_bend()->amount_bits_2(), 64U);

    auto* fourth_event = parsed_sequence->body()->events()->at(3).get();
    BOOST_CHECK_EQUAL(fourth_event->id(), 0xc0);
    BOOST_REQUIRE(fourth_event->program_change() != nullptr);
    BOOST_CHECK_EQUAL(fourth_event->program_change()->program(), 12);

    auto* fifth_event = parsed_sequence->body()->events()->at(4).get();
    BOOST_CHECK_EQUAL(fifth_event->id(), 0xd0);
    BOOST_REQUIRE(fifth_event->ch_pressure() != nullptr);
    BOOST_CHECK_EQUAL(fifth_event->ch_pressure()->pressure(), 70);

    auto* sixth_event = parsed_sequence->body()->events()->at(5).get();
    BOOST_CHECK_EQUAL(sixth_event->id(), 0xa0);
    BOOST_REQUIRE(sixth_event->poly_pressure() != nullptr);
    BOOST_CHECK_EQUAL(sixth_event->poly_pressure()->note(), 64);
    BOOST_CHECK_EQUAL(sixth_event->poly_pressure()->pressure(), 80);

    auto* seventh_event = parsed_sequence->body()->events()->at(6).get();
    BOOST_CHECK_EQUAL(seventh_event->id(), 0xf0);
    BOOST_REQUIRE(seventh_event->exclusive() != nullptr);
    BOOST_CHECK_EQUAL(seventh_event->exclusive()->bytes(), std::string("\xf0\x47", 2));
    BOOST_REQUIRE(seventh_event->exclusive()->mixer() != nullptr);
    BOOST_CHECK_EQUAL(seventh_event->exclusive()->mixer()->param(), 3);
    BOOST_CHECK_EQUAL(seventh_event->exclusive()->mixer()->pad_index(), 4);
    BOOST_CHECK_EQUAL(seventh_event->exclusive()->mixer()->value(), 99);

    auto* terminator = parsed_sequence->body()->events()->at(7).get();
    BOOST_CHECK_EQUAL(terminator->tick(), 0xfffffU);
    BOOST_CHECK_EQUAL(terminator->terminator(), std::string(5, '\x00'));
}
#else
BOOST_AUTO_TEST_CASE(test_mpc2000xlall_write_roundtrip) {
    BOOST_FAIL("mpc2000xlall_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
