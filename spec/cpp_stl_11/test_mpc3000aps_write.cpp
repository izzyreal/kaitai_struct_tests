// Hand-written smoke test for an MPC3000 APS-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC3000APS
#include "mpc3000aps_write.h"

#include <sstream>

namespace {

std::unique_ptr<mpc3000aps_write_t::note_variation_t> make_note_variation() {
    std::unique_ptr<mpc3000aps_write_t::note_variation_t> value(new mpc3000aps_write_t::note_variation_t(nullptr));
    value->set_note_number_assignment(60);
    value->set_tuning_low_range(58);
    value->set_tuning_hi_range(62);
    value->set_attack_low_range(10);
    value->set_attack_hi_range(20);
    value->set_decay_low_range(30);
    value->set_decay_hi_range(40);
    value->set_filter_low_range(50);
    value->set_filter_hi_range(60);
    return value;
}

std::unique_ptr<mpc3000aps_write_t::effects_settings_t> make_effects() {
    std::unique_ptr<mpc3000aps_write_t::effects_settings_t> effects(new mpc3000aps_write_t::effects_settings_t(nullptr));
    effects->set_effects_on(true);
    effects->set_unnamed1(0);
    effects->set_unnamed2(0);
    effects->set_delay_volume_tap1(10);
    effects->set_delay_volume_tap2(20);
    effects->set_delay_volume_tap3(30);
    effects->set_delay_pan_tap1(40);
    effects->set_delay_pan_tap2(50);
    effects->set_delay_pan_tap3(60);
    effects->set_delay_msecs_tap1(100);
    effects->set_delay_msecs_tap2(200);
    effects->set_delay_msecs_tap3(300);
    effects->set_unnamed12(std::string(6, '\x00'));
    effects->set_delay_feedback_tap1(1);
    effects->set_delay_feedback_tap2(2);
    effects->set_delay_feedback_tap3(3);
    effects->set_unnamed16(std::string(13, '\x00'));
    return effects;
}

std::unique_ptr<mpc3000aps_write_t::mixer_settings_t> make_mixer(uint8_t pan) {
    std::unique_ptr<mpc3000aps_write_t::mixer_settings_t> mixer(new mpc3000aps_write_t::mixer_settings_t(nullptr));
    mixer->set_stereo_mix_volume(100);
    mixer->set_stereo_mix_pan(pan);
    mixer->set_echo_volume(20);
    mixer->set_out_assign_fol_ster(0);
    return mixer;
}

std::unique_ptr<mpc3000aps_write_t::sound_assignment_t> make_sound_assignment(uint8_t sound_number, uint16_t tune) {
    std::unique_ptr<mpc3000aps_write_t::sound_assignment_t> value(new mpc3000aps_write_t::sound_assignment_t(nullptr));
    value->set_sound_number(sound_number);
    value->set_sound_generator_mode(mpc3000aps_write_t::SOUND_GENERATOR_MODE_NORMAL);
    value->set_if_over1(1);
    value->set_use_also_plays1(0xff);
    value->set_if_over2(127);
    value->set_use_also_plays2(0xff);
    value->set_poly(mpc3000aps_write_t::POLY_MODE_POLY);
    value->set_cutoff1(0);
    value->set_cutoff2(127);
    value->set_tune(tune);
    value->set_attack(10);
    value->set_decay(20);
    value->set_decay_mode(mpc3000aps_write_t::DECAY_MODE_START);
    value->set_filter_frequency(90);
    value->set_filter_resonance(25);
    value->set_filter_envel_attack(5);
    value->set_filter_envel_decay(6);
    value->set_filter_envel_amount(7);
    value->set_veloc_mod_of_volume(8);
    value->set_veloc_mod_of_attack(9);
    value->set_veloc_mod_of_soft_start(10);
    value->set_veloc_mod_of_filter_freq(11);
    value->set_param(mpc3000aps_write_t::NOTE_VARIATION_TYPE_TUNE);
    return value;
}

std::unique_ptr<mpc3000aps_write_t::program_t::pad_note_number_assignment_t> make_pad_assignment(uint8_t note) {
    std::unique_ptr<mpc3000aps_write_t::program_t::pad_note_number_assignment_t> value(
        new mpc3000aps_write_t::program_t::pad_note_number_assignment_t(nullptr));
    value->set_note_number(note);
    return value;
}

std::unique_ptr<mpc3000aps_write_t::program_t> make_program(const std::string& name) {
    std::unique_ptr<mpc3000aps_write_t::program_t> program(new mpc3000aps_write_t::program_t(nullptr));
    program->set_program_name(name);
    program->set_note_variation(make_note_variation());
    program->set_effects_settings(make_effects());

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000aps_write_t::sound_assignment_t>>> sound_assignments(
        new std::vector<std::unique_ptr<mpc3000aps_write_t::sound_assignment_t>>());
    for (int i = 0; i < 64; ++i) {
        sound_assignments->push_back(make_sound_assignment(static_cast<uint8_t>(i % 8), static_cast<uint16_t>(i)));
    }
    program->set_sound_assignments(std::move(sound_assignments));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000aps_write_t::mixer_settings_t>>> mixer_settings(
        new std::vector<std::unique_ptr<mpc3000aps_write_t::mixer_settings_t>>());
    for (int i = 0; i < 64; ++i) {
        mixer_settings->push_back(make_mixer(static_cast<uint8_t>(64 + (i % 16))));
    }
    program->set_mixer_settings(std::move(mixer_settings));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000aps_write_t::program_t::pad_note_number_assignment_t>>> pad_assignments(
        new std::vector<std::unique_ptr<mpc3000aps_write_t::program_t::pad_note_number_assignment_t>>());
    for (int i = 0; i < 64; ++i) {
        pad_assignments->push_back(make_pad_assignment(static_cast<uint8_t>(35 + (i % 8))));
    }
    program->set_pad_note_number_assignments(std::move(pad_assignments));
    return program;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc3000aps_write_roundtrip) {
    mpc3000aps_write_t root(nullptr);
    root.set_file_id(std::string("\x0a", 1));
    root.set_file_version(std::string("\x00", 1));
    root.set_num_programs(2);
    root.set_total_samples(123456);
    root.set_file_name("APS3000");
    root.set_active_program_number(1);
    root.set_stereo_mix_source(mpc3000aps_write_t::MIX_SOURCE_MASTER);
    root.set_indiv_out_echo_send_mix_source(mpc3000aps_write_t::MIX_SOURCE_SEQUENCE);
    root.set_effects_source(mpc3000aps_write_t::MIX_SOURCE_PROGRAM);
    root.set_record_live_mix_changes(1);
    root.set_center_pad_16_levels_if_param_tuning(60);
    root.set_audio_trigger_assign(35);
    root.set_effects_settings(make_effects());

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000aps_write_t::mixer_settings_t>>> mixer_settings(
        new std::vector<std::unique_ptr<mpc3000aps_write_t::mixer_settings_t>>());
    for (int i = 0; i < 64; ++i) {
        mixer_settings->push_back(make_mixer(static_cast<uint8_t>(64 + (i % 16))));
    }
    root.set_mixer_settings(std::move(mixer_settings));

    std::unique_ptr<std::vector<std::unique_ptr<mpc3000aps_write_t::program_t>>> programs(
        new std::vector<std::unique_ptr<mpc3000aps_write_t::program_t>>());
    programs->push_back(make_program("PROG_A"));
    programs->push_back(make_program("PROG_B"));
    root.set_programs(std::move(programs));

    std::unique_ptr<std::vector<std::string>> sound_names(new std::vector<std::string>());
    for (int i = 0; i < 128; ++i) {
        sound_names->push_back((i == 0) ? "KICK_01" : ((i == 1) ? "SNARE_01" : ("SND" + std::to_string(i))));
    }
    root.set_sound_names(std::move(sound_names));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._check();
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc3000aps_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.file_id(), std::string("\x0a", 1));
    BOOST_CHECK_EQUAL(parsed.file_version(), std::string("\x00", 1));
    BOOST_CHECK_EQUAL(parsed.num_programs(), 2);
    BOOST_CHECK_EQUAL(parsed.total_samples(), 123456U);
    BOOST_CHECK_EQUAL(parsed.file_name(), "APS3000");
    BOOST_CHECK_EQUAL(parsed.active_program_number(), 1);
    BOOST_CHECK_EQUAL(parsed.stereo_mix_source(), mpc3000aps_write_t::MIX_SOURCE_MASTER);
    BOOST_CHECK_EQUAL(parsed.effects_source(), mpc3000aps_write_t::MIX_SOURCE_PROGRAM);
    BOOST_REQUIRE(parsed.effects_settings() != nullptr);
    BOOST_CHECK_EQUAL(parsed.effects_settings()->effects_on(), true);
    BOOST_REQUIRE_EQUAL(parsed.mixer_settings()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.mixer_settings()->at(15)->stereo_mix_pan(), 79);
    BOOST_REQUIRE_EQUAL(parsed.programs()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed.programs()->at(0)->program_name(), "PROG_A");
    BOOST_CHECK_EQUAL(parsed.programs()->at(1)->program_name(), "PROG_B");
    BOOST_REQUIRE_EQUAL(parsed.programs()->at(0)->sound_assignments()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.programs()->at(0)->sound_assignments()->at(63)->tune(), 63);
    BOOST_REQUIRE_EQUAL(parsed.programs()->at(0)->pad_note_number_assignments()->size(), 64U);
    BOOST_CHECK_EQUAL(parsed.programs()->at(0)->pad_note_number_assignments()->at(0)->note_number(), 35);
    BOOST_REQUIRE_EQUAL(parsed.sound_names()->size(), 128U);
    BOOST_CHECK_EQUAL(parsed.sound_names()->at(0), "KICK_01");
    BOOST_CHECK_EQUAL(parsed.sound_names()->at(1), "SNARE_01");
}
#else
BOOST_AUTO_TEST_CASE(test_mpc3000aps_write_roundtrip) {
    BOOST_FAIL("mpc3000aps_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
