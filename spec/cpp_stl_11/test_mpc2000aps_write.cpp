// Hand-written smoke test for an MPC2000 APS-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC2000APS
#include "mpc2000aps_write.h"

#include <sstream>

namespace {

std::unique_ptr<mpc2000pgm_write_t::slider_t> make_pgm_slider() {
    std::unique_ptr<mpc2000pgm_write_t::slider_t> slider(new mpc2000pgm_write_t::slider_t(nullptr));
    slider->set_note(60);
    slider->set_tune_low(-6);
    slider->set_tune_high(6);
    slider->set_decay_low(-4);
    slider->set_decay_high(4);
    slider->set_attack_low(-3);
    slider->set_attack_high(3);
    slider->set_filter_low(-2);
    slider->set_filter_high(2);
    return slider;
}

std::unique_ptr<mpc2000pgm_write_t::pad_mixer_t> make_pad_mixer(uint8_t pan) {
    std::unique_ptr<mpc2000pgm_write_t::pad_mixer_t> mixer(new mpc2000pgm_write_t::pad_mixer_t(nullptr));
    mixer->set_fx_output(mpc2000pgm_write_t::FX_OUTPUT_NONE);
    mixer->set_volume(100);
    mixer->set_pan(pan);
    mixer->set_volume_individual(90);
    mixer->set_output(0);
    mixer->set_effects_send_level(32);
    return mixer;
}

std::unique_ptr<mpc2000aps_write_t::note_t> make_aps_note(uint8_t sound_index, int16_t tune, uint8_t velocity_to_pitch) {
    std::unique_ptr<mpc2000aps_write_t::note_t> note(new mpc2000aps_write_t::note_t(nullptr));
    note->set_sound_index(sound_index);
    note->set__unnamed1(std::string(1, '\x00'));
    note->set_sound_generation_mode(mpc2000pgm_write_t::SOUND_GENERATION_MODE_NORMAL);
    note->set_velocity_range_lower(1);
    note->set_also_play_use_note_1(255);
    note->set_velocity_range_upper(127);
    note->set_also_play_use_note_2(255);
    note->set_voice_overlap_mode(mpc2000pgm_write_t::VOICE_OVERLAP_MODE_POLY);
    note->set_mute_assign_1(0);
    note->set_mute_assign_2(0);
    note->set_tune(tune);
    note->set_attack(10);
    note->set_decay(20);
    note->set_decay_mode(mpc2000pgm_write_t::DECAY_MODE_END);
    note->set_cutoff(80);
    note->set_resonance(25);
    note->set_velocity_envelope_to_filter_attack(5);
    note->set_velocity_envelope_to_filter_decay(6);
    note->set_velocity_envelope_to_filter_amount(7);
    note->set_velocity_to_level(8);
    note->set_velocity_to_attack(9);
    note->set_velocity_to_start(10);
    note->set_velocity_to_cutoff(11);
    note->set_slider_parameter(mpc2000pgm_write_t::SLIDER_PARAMETER_TUNING);
    note->set_velocity_to_pitch(velocity_to_pitch);
    return note;
}

std::unique_ptr<mpc2000aps_write_t::drum_t> make_drum(uint8_t program, uint8_t base_pan) {
    std::unique_ptr<mpc2000aps_write_t::drum_t> drum(new mpc2000aps_write_t::drum_t(nullptr));
    std::unique_ptr<std::vector<std::unique_ptr<mpc2000pgm_write_t::pad_mixer_t>>> pad_mixers(
        new std::vector<std::unique_ptr<mpc2000pgm_write_t::pad_mixer_t>>());
    for (int i = 0; i < 64; ++i) {
        pad_mixers->push_back(make_pad_mixer(static_cast<uint8_t>(base_pan + (i % 8))));
    }
    drum->set_pad_mixers(std::move(pad_mixers));
    drum->set__unnamed1(std::string(2, '\x00'));
    drum->set_receive_program_change(mpc2000aps_write_t::NO_YES_TRUE);
    drum->set_receive_midi_volume(mpc2000aps_write_t::NO_YES_TRUE);
    drum->set__unnamed4(std::string(1, '\x00'));
    drum->set_program(program);
    drum->set_receive_program_change_duplicate(mpc2000aps_write_t::NO_YES_TRUE);
    drum->set_receive_midi_volume_duplicate(mpc2000aps_write_t::NO_YES_TRUE);
    return drum;
}

std::unique_ptr<mpc2000aps_write_t::aps_program_body_t> make_program_body() {
    std::unique_ptr<mpc2000aps_write_t::aps_program_body_t> body(new mpc2000aps_write_t::aps_program_body_t(nullptr));
    body->set__unnamed0(std::string(5, '\x00'));
    body->set_name(std::string("APS_PROG_A", 10) + std::string(7, '\x00'));
    body->set_slider(make_pgm_slider());
    body->set_program_change(12);
    body->set__unnamed4(std::string(5, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000aps_write_t::note_t>>> note_parameters(
        new std::vector<std::unique_ptr<mpc2000aps_write_t::note_t>>());
    for (int i = 0; i < 64; ++i) {
        note_parameters->push_back(make_aps_note(static_cast<uint8_t>(i % 2), static_cast<int16_t>(i - 32), static_cast<uint8_t>(i)));
    }
    body->set_note_parameters(std::move(note_parameters));
    body->set__unnamed6(std::string(1, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000pgm_write_t::pad_mixer_t>>> pad_mixers(
        new std::vector<std::unique_ptr<mpc2000pgm_write_t::pad_mixer_t>>());
    for (int i = 0; i < 64; ++i) {
        pad_mixers->push_back(make_pad_mixer(static_cast<uint8_t>(64 + (i % 16))));
    }
    body->set_pad_mixers(std::move(pad_mixers));
    body->set__unnamed8(std::string(3, '\x00'));

    std::unique_ptr<std::vector<int8_t>> pad_to_note_mapping(new std::vector<int8_t>());
    for (int i = 0; i < 64; ++i) {
        pad_to_note_mapping->push_back(static_cast<int8_t>(35 + (i % 8)));
    }
    body->set_pad_to_note_mapping(std::move(pad_to_note_mapping));
    body->set__unnamed10(std::string(200, '\x00'));
    return body;
}

std::unique_ptr<mpc2000aps_write_t::aps_program_meta_t> make_program_meta() {
    std::unique_ptr<mpc2000aps_write_t::aps_program_meta_t> meta(new mpc2000aps_write_t::aps_program_meta_t(nullptr));
    meta->set_index(0);
    meta->set_body(make_program_body());
    std::unique_ptr<std::vector<uint8_t>> tail(new std::vector<uint8_t>());
    meta->set__unnamed2(std::move(tail));
    return meta;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc2000aps_write_roundtrip) {
    mpc2000aps_write_t root(nullptr);
    root.set_magic(std::string("\x0a\x05", 2));
    root.set_sound_count(2);
    root.set__unnamed2(std::string(1, '\x00'));

    std::unique_ptr<std::vector<std::string>> sound_names(new std::vector<std::string>());
    sound_names->push_back(std::string("KICK_01", 7) + std::string(10, '\x00'));
    sound_names->push_back(std::string("SNARE_01", 8) + std::string(9, '\x00'));
    root.set_sound_names(std::move(sound_names));

    root.set__unnamed4(std::string(1, '\x00'));
    root.set_name(std::string("APS_SET_A", 9) + std::string(8, '\x00'));
    root.set__unnamed6(std::string(1, '\x00'));

    std::unique_ptr<mpc2000aps_write_t::global_parameters_t> global_parameters(new mpc2000aps_write_t::global_parameters_t(nullptr));
    global_parameters->set__unnamed0(0);
    global_parameters->set_pad_to_internal_sound(mpc2000aps_write_t::NO_YES_TRUE);
    global_parameters->set__unnamed2(0);
    global_parameters->set_pad_assign(mpc2000aps_write_t::PAD_ASSIGN_PROGRAM);
    global_parameters->set__unnamed4(0);
    global_parameters->set_indiv_fx_source(mpc2000aps_write_t::MIX_SOURCE_PROGRAM);
    global_parameters->set_stereo_mix_source(mpc2000aps_write_t::MIX_SOURCE_DRUM);
    global_parameters->set__unnamed7(0);
    global_parameters->set_record_mix_changes(mpc2000aps_write_t::NO_YES_FALSE);
    global_parameters->set_copy_pgm_mix_to_drum(mpc2000aps_write_t::NO_YES_TRUE);
    global_parameters->set__unnamed10(0);
    global_parameters->set_fx_drum(3);
    global_parameters->set__unnamed12(std::string(1, '\x00'));
    global_parameters->set_master_level(100);
    root.set_global_parameters(std::move(global_parameters));

    root.set__unnamed8(std::string(1, '\x00'));
    std::unique_ptr<std::vector<int8_t>> master_pad_to_note_mapping(new std::vector<int8_t>());
    for (int i = 0; i < 64; ++i) {
        master_pad_to_note_mapping->push_back(static_cast<int8_t>(35 + (i % 8)));
    }
    root.set_master_pad_to_note_mapping(std::move(master_pad_to_note_mapping));
    root.set__unnamed10(std::string(7, '\x00'));
    root.set_drum1(make_drum(0, 64));
    root.set__unnamed12(std::string(4, '\x00'));
    root.set_drum2(make_drum(1, 72));
    root.set__unnamed14(std::string(4, '\x00'));
    root.set_drum3(make_drum(2, 80));
    root.set__unnamed16(std::string(4, '\x00'));
    root.set_drum4(make_drum(3, 88));
    root.set__unnamed18(std::string(1, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000aps_write_t::aps_program_meta_t>>> aps_programs(
        new std::vector<std::unique_ptr<mpc2000aps_write_t::aps_program_meta_t>>());
    aps_programs->push_back(make_program_meta());
    root.set_aps_programs(std::move(aps_programs));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._check();
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc2000aps_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.magic(), std::string("\x0a\x05", 2));
    BOOST_CHECK_EQUAL(parsed.sound_count(), 2);
    BOOST_REQUIRE_EQUAL(parsed.sound_names()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed.sound_names()->at(0), std::string("KICK_01", 7) + std::string(10, '\x00'));
    BOOST_CHECK_EQUAL(parsed.name(), std::string("APS_SET_A", 9) + std::string(8, '\x00'));
    BOOST_REQUIRE(parsed.global_parameters() != nullptr);
    BOOST_CHECK_EQUAL(parsed.global_parameters()->pad_to_internal_sound(), mpc2000aps_write_t::NO_YES_TRUE);
    BOOST_CHECK_EQUAL(parsed.global_parameters()->stereo_mix_source(), mpc2000aps_write_t::MIX_SOURCE_DRUM);
    BOOST_CHECK_EQUAL(parsed.global_parameters()->master_level(), 100);

    BOOST_REQUIRE_EQUAL(parsed.master_pad_to_note_mapping()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.master_pad_to_note_mapping()->at(0), 35);
    BOOST_CHECK_EQUAL(parsed.master_pad_to_note_mapping()->at(7), 42);

    BOOST_REQUIRE(parsed.drum1() != nullptr);
    BOOST_CHECK_EQUAL(parsed.drum1()->program(), 0);
    BOOST_CHECK_EQUAL(parsed.drum4()->program(), 3);
    BOOST_REQUIRE_EQUAL(parsed.drum2()->pad_mixers()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.drum2()->pad_mixers()->at(0)->pan(), 72);

    BOOST_REQUIRE_EQUAL(parsed.aps_programs()->size(), 1U);
    BOOST_CHECK_EQUAL(parsed.aps_programs()->at(0)->index(), 0);
    BOOST_REQUIRE(parsed.aps_programs()->at(0)->body() != nullptr);
    BOOST_CHECK_EQUAL(parsed.aps_programs()->at(0)->body()->program_change(), 12);
    BOOST_CHECK_EQUAL(parsed.aps_programs()->at(0)->body()->slider()->note(), 60);
    BOOST_REQUIRE_EQUAL(parsed.aps_programs()->at(0)->body()->note_parameters()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.aps_programs()->at(0)->body()->note_parameters()->at(0)->sound_index(), 0);
    BOOST_CHECK_EQUAL(parsed.aps_programs()->at(0)->body()->note_parameters()->at(63)->velocity_to_pitch(), 63);
}
#else
BOOST_AUTO_TEST_CASE(test_mpc2000aps_write_roundtrip) {
    BOOST_FAIL("mpc2000aps_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
