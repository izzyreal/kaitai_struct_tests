// Hand-written smoke test for an MPC60 SET-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC60SET
#include "mpc60set_write.h"

#include <sstream>

namespace {

std::unique_ptr<mpc60set_write_t::u3le_t> make_u3le(uint32_t value) {
    std::unique_ptr<mpc60set_write_t::u3le_t> result(new mpc60set_write_t::u3le_t(nullptr));
    result->set_b12(static_cast<uint16_t>(value & 0xffff));
    result->set_b3(static_cast<uint8_t>((value >> 16) & 0xff));
    return result;
}

std::unique_ptr<mpc60set_write_t::sound_directory_entry_t> make_sound_directory_entry(int index) {
    std::unique_ptr<mpc60set_write_t::sound_directory_entry_t> entry(
        new mpc60set_write_t::sound_directory_entry_t(nullptr));
    std::string name = "SND" + std::to_string(index);
    entry->set_name(name);
    entry->set_shared_sound_link(static_cast<uint8_t>(index));
    entry->set_start_address_in_memory(static_cast<uint32_t>(index * 100));
    entry->set_length_in_samples(2);
    entry->set_read_write_pointer(static_cast<uint32_t>(index * 100));
    entry->set_start_address_for_playing(static_cast<uint32_t>(index * 100));
    entry->set_time_from_start_of_data_to_start_play_msec(0);
    entry->set_time_from_start_of_data_to_end_of_play_msec(10);
    entry->set_attack_time_msec(0);
    entry->set_decay_time_msec(0);
    entry->set_pitch_factor(0);
    entry->set_attack_rate_sent_to_sound_generator(0);
    entry->set_decay_rate_sent_to_sound_generator(0);
    entry->set_decay_start_time(0);
    entry->set_sound_duration(10);
    entry->set_analog_output_jack(mpc60set_write_t::ANALOG_OUTPUT_JACK_JACK_1);
    entry->set_requested_stereo_mix_volume(100);
    entry->set_requested_stereo_mix_pan(64);
    entry->set_left_level_sent_to_sound_generator(100);
    entry->set_right_level_sent_to_sound_generator(100);
    entry->set_echo_level_sent_to_sound_generator(0);

    std::unique_ptr<mpc60set_write_t::sound_directory_entry_t::sound_characteristics_t> characteristics(
        new mpc60set_write_t::sound_directory_entry_t::sound_characteristics_t(nullptr));
    characteristics->set_normal_or_hihat_sound(mpc60set_write_t::NORMAL_OR_HIHAT_NORMAL);
    characteristics->set_save_required(false);
    characteristics->set_filter(mpc60set_write_t::FILTER_RANGE_LESS_THAN_OR_EQUALS_47_KHZ);
    entry->set_sound_characteristics(std::move(characteristics));
    return entry;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc60set_write_roundtrip) {
    mpc60set_write_t root(nullptr);
    root.set_file_id(std::string("\x02", 1));
    root.set_file_format_version(std::string("\x00", 1));
    root.set_total_number_of_sample_words(make_u3le(2));

    std::unique_ptr<std::vector<std::unique_ptr<mpc60set_write_t::sound_directory_entry_t>>> entries(
        new std::vector<std::unique_ptr<mpc60set_write_t::sound_directory_entry_t>>());
    for (int i = 0; i < 34; ++i) {
        entries->push_back(make_sound_directory_entry(i));
    }
    root.set_sound_directory_entry(std::move(entries));

    std::unique_ptr<std::vector<mpc60set_write_t::physical_drum_t>> sound_map(
        new std::vector<mpc60set_write_t::physical_drum_t>());
    for (int i = 0; i < 34; ++i) {
        sound_map->push_back(static_cast<mpc60set_write_t::physical_drum_t>(i));
    }
    root.set_sound_map(std::move(sound_map));
    root.set_use_master_mix_data(mpc60set_write_t::USE_MASTER_MIX_DATA_USE);
    root.set_master_stereo_mix(std::string(32, '\x64'));
    root.set_master_stereo_pan(std::string(32, '\x40'));
    root.set_master_echo_mix(std::string(32, '\x00'));
    root.set_master_drum_tuning(std::string(64, '\x00'));
    root.set_double_play_assignments(std::string(32, '\x00'));
    root.set_velocity_switch_on_off_for_each_double_play_assign(std::string(32, '\x00'));
    root.set_velocity_switch_threshold_value_for_each_double_play_assign(std::string(32, '\x00'));
    root.set__unnamed13(std::string(770, '\x00'));

    std::unique_ptr<std::vector<uint64_t>> sound_samples(new std::vector<uint64_t>());
    sound_samples->push_back(0x123);
    sound_samples->push_back(0x456);
    root.set_sound_samples(std::move(sound_samples));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc60set_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.file_id(), std::string("\x02", 1));
    BOOST_CHECK_EQUAL(parsed.file_format_version(), std::string("\x00", 1));
    BOOST_REQUIRE(parsed.total_number_of_sample_words() != nullptr);
    BOOST_CHECK_EQUAL(parsed.total_number_of_sample_words()->value(), 2U);
    BOOST_REQUIRE_EQUAL(parsed.sound_directory_entry()->size(), 34U);
    BOOST_CHECK_EQUAL(parsed.sound_directory_entry()->at(0)->name(), "SND0");
    BOOST_CHECK_EQUAL(parsed.sound_directory_entry()->at(33)->shared_sound_link(), 33U);
    BOOST_REQUIRE_EQUAL(parsed.sound_map()->size(), 34U);
    BOOST_CHECK_EQUAL(parsed.sound_map()->at(0), mpc60set_write_t::PHYSICAL_DRUM_HIHT_CLSD);
    BOOST_CHECK_EQUAL(parsed.sound_map()->at(33), mpc60set_write_t::PHYSICAL_DRUM_DR16);
    BOOST_CHECK_EQUAL(parsed.use_master_mix_data(), mpc60set_write_t::USE_MASTER_MIX_DATA_USE);
    BOOST_REQUIRE_EQUAL(parsed.sound_samples()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed.sound_samples()->at(0), 0x123U);
    BOOST_CHECK_EQUAL(parsed.sound_samples()->at(1), 0x456U);
}
#else
BOOST_AUTO_TEST_CASE(test_mpc60set_write_roundtrip) {
    BOOST_FAIL("mpc60set_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
