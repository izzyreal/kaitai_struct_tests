// Hand-written smoke test for an MPC2000 PGM-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC2000PGM
#include "mpc2000pgm_write.h"

#include <sstream>

namespace {

std::unique_ptr<mpc2000pgm_write_t::slider_t> make_slider() {
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

std::unique_ptr<mpc2000pgm_write_t::note_t> make_note(uint8_t sound_index, int16_t tune, uint8_t velocity_to_pitch) {
    std::unique_ptr<mpc2000pgm_write_t::note_t> note(new mpc2000pgm_write_t::note_t(nullptr));
    note->set_sound_index(sound_index);
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

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc2000pgm_write_roundtrip) {
    mpc2000pgm_write_t root(nullptr);
    root.set_magic(std::string("\x07\x04", 2));
    root.set_sound_count(2);

    std::unique_ptr<std::vector<std::string>> sound_names(new std::vector<std::string>());
    sound_names->push_back(std::string("KICK_01", 7) + std::string(10, '\x00'));
    sound_names->push_back(std::string("SNARE_01", 8) + std::string(9, '\x00'));
    root.set_sound_names(std::move(sound_names));

    root.set__unnamed3(std::string(1, '\x00'));
    root.set_name(std::string("KIT_A", 5) + std::string(11, '\x00'));
    root.set__unnamed5(std::string(2, '\x00'));
    root.set_slider(make_slider());
    root.set_program_change(12);
    root.set__unnamed8(std::string(5, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000pgm_write_t::note_t>>> note_parameters(
        new std::vector<std::unique_ptr<mpc2000pgm_write_t::note_t>>());
    for (int i = 0; i < 64; ++i) {
        note_parameters->push_back(make_note(static_cast<uint8_t>(i % 2), static_cast<int16_t>(i - 32), static_cast<uint8_t>(i)));
    }
    root.set_note_parameters(std::move(note_parameters));

    root.set__unnamed10(std::string(1, '\x00'));

    std::unique_ptr<std::vector<std::unique_ptr<mpc2000pgm_write_t::pad_mixer_t>>> pad_mixers(
        new std::vector<std::unique_ptr<mpc2000pgm_write_t::pad_mixer_t>>());
    for (int i = 0; i < 64; ++i) {
        pad_mixers->push_back(make_pad_mixer(static_cast<uint8_t>(64 + (i % 16))));
    }
    root.set_pad_mixers(std::move(pad_mixers));

    root.set__unnamed12(std::string(3, '\x00'));

    std::unique_ptr<std::vector<int8_t>> pad_to_note_mapping(new std::vector<int8_t>());
    for (int i = 0; i < 64; ++i) {
        pad_to_note_mapping->push_back(static_cast<int8_t>(35 + (i % 8)));
    }
    root.set_pad_to_note_mapping(std::move(pad_to_note_mapping));

    root.set__unnamed14(std::string(200, '\x00'));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._check();
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc2000pgm_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.magic(), std::string("\x07\x04", 2));
    BOOST_CHECK_EQUAL(parsed.sound_count(), 2);
    BOOST_REQUIRE_EQUAL(parsed.sound_names()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed.sound_names()->at(0), std::string("KICK_01", 7) + std::string(10, '\x00'));
    BOOST_CHECK_EQUAL(parsed.sound_names()->at(1), std::string("SNARE_01", 8) + std::string(9, '\x00'));
    BOOST_CHECK_EQUAL(parsed.name(), std::string("KIT_A", 5) + std::string(11, '\x00'));
    BOOST_REQUIRE(parsed.slider() != nullptr);
    BOOST_CHECK_EQUAL(parsed.slider()->note(), 60);
    BOOST_CHECK_EQUAL(parsed.slider()->tune_low(), -6);
    BOOST_CHECK_EQUAL(parsed.program_change(), 12);

    BOOST_REQUIRE_EQUAL(parsed.note_parameters()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.note_parameters()->at(0)->sound_index(), 0);
    BOOST_CHECK_EQUAL(parsed.note_parameters()->at(0)->tune(), -32);
    BOOST_CHECK_EQUAL(parsed.note_parameters()->at(63)->sound_index(), 1);
    BOOST_CHECK_EQUAL(parsed.note_parameters()->at(63)->velocity_to_pitch(), 63);

    BOOST_REQUIRE_EQUAL(parsed.pad_mixers()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.pad_mixers()->at(0)->pan(), 64);
    BOOST_CHECK_EQUAL(parsed.pad_mixers()->at(15)->pan(), 79);

    BOOST_REQUIRE_EQUAL(parsed.pad_to_note_mapping()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.pad_to_note_mapping()->at(0), 35);
    BOOST_CHECK_EQUAL(parsed.pad_to_note_mapping()->at(7), 42);
    BOOST_CHECK_EQUAL(parsed.pad_to_note_mapping()->at(63), 42);
}
#else
BOOST_AUTO_TEST_CASE(test_mpc2000pgm_write_roundtrip) {
    BOOST_FAIL("mpc2000pgm_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
