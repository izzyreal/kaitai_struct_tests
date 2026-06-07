// Hand-written smoke test for an MPC2000 MID-like real format.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_MPC2000MID
#include "mpc2000mid_write.h"

#include <sstream>

namespace {

std::unique_ptr<vlq_base128_be_write_t> make_vlq0() {
    std::unique_ptr<vlq_base128_be_write_t> value(new vlq_base128_be_write_t(nullptr));
    std::unique_ptr<std::vector<std::unique_ptr<vlq_base128_be_write_t::group_t>>> groups(
        new std::vector<std::unique_ptr<vlq_base128_be_write_t::group_t>>());
    std::unique_ptr<vlq_base128_be_write_t::group_t> group(new vlq_base128_be_write_t::group_t(nullptr, value.get(), value.get()));
    group->set_has_next(false);
    group->set_value(0);
    groups->push_back(std::move(group));
    value->set_groups(std::move(groups));
    return value;
}

std::unique_ptr<standard_midi_file_with_running_status_write_t::track_event_t> make_note_on_event(
    uint8_t previous_event_type,
    uint8_t status,
    uint8_t note,
    uint8_t velocity,
    standard_midi_file_with_running_status_write_t::track_events_t* parent,
    standard_midi_file_with_running_status_write_t* root
) {
    std::unique_ptr<standard_midi_file_with_running_status_write_t::track_event_t> event(
        new standard_midi_file_with_running_status_write_t::track_event_t(previous_event_type, nullptr, parent, root));
    event->set_v_time(make_vlq0());
    event->set_status_byte_lookahead(status);
    event->set_event_header(status);
    std::unique_ptr<standard_midi_file_with_running_status_write_t::note_on_event_t> body(
        new standard_midi_file_with_running_status_write_t::note_on_event_t(nullptr, event.get(), root));
    body->set_note(note);
    body->set_velocity(velocity);
    event->set_event_body(std::move(body));
    return event;
}

std::unique_ptr<standard_midi_file_with_running_status_write_t::track_event_t> make_end_of_track_event(
    uint8_t previous_event_type,
    standard_midi_file_with_running_status_write_t::track_events_t* parent,
    standard_midi_file_with_running_status_write_t* root
) {
    std::unique_ptr<standard_midi_file_with_running_status_write_t::track_event_t> event(
        new standard_midi_file_with_running_status_write_t::track_event_t(previous_event_type, nullptr, parent, root));
    event->set_v_time(make_vlq0());
    event->set_status_byte_lookahead(0xff);
    event->set_event_header(0xff);
    std::unique_ptr<standard_midi_file_with_running_status_write_t::meta_event_body_t> body(
        new standard_midi_file_with_running_status_write_t::meta_event_body_t(nullptr, event.get(), root));
    body->set_meta_type(standard_midi_file_with_running_status_write_t::meta_event_body_t::META_TYPE_ENUM_END_OF_TRACK);
    body->set_len(make_vlq0());
    body->set_body("");
    event->set_meta_event_body(std::move(body));
    return event;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_mpc2000mid_write_roundtrip) {
    mpc2000mid_write_t root(nullptr);
    root.set_sequencer_enabled(false);

    std::unique_ptr<standard_midi_file_with_running_status_write_t::header_t> hdr(
        new standard_midi_file_with_running_status_write_t::header_t(nullptr));
    hdr->set_magic("MThd");
    hdr->set_len_header(6);
    hdr->set_format(1);
    hdr->set_num_tracks(1);
    hdr->set_division(480);
    root.set_hdr(std::move(hdr));

    std::unique_ptr<std::vector<std::unique_ptr<standard_midi_file_with_running_status_write_t::track_t>>> tracks(
        new std::vector<std::unique_ptr<standard_midi_file_with_running_status_write_t::track_t>>());
    std::unique_ptr<standard_midi_file_with_running_status_write_t::track_t> track(
        new standard_midi_file_with_running_status_write_t::track_t(nullptr));
    track->set_magic("MTrk");
    track->set_len_events(8);

    std::unique_ptr<standard_midi_file_with_running_status_write_t::track_events_t> events(
        new standard_midi_file_with_running_status_write_t::track_events_t(nullptr, track.get(), nullptr));
    std::unique_ptr<std::vector<std::unique_ptr<standard_midi_file_with_running_status_write_t::track_event_t>>> event_list(
        new std::vector<std::unique_ptr<standard_midi_file_with_running_status_write_t::track_event_t>>());
    event_list->push_back(make_note_on_event(0xff, 0x90, 60, 100, events.get(), nullptr));
    event_list->push_back(make_end_of_track_event(0x90, events.get(), nullptr));
    events->set_event(std::move(event_list));
    track->set_events(std::move(events));
    tracks->push_back(std::move(track));
    root.set_tracks(std::move(tracks));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._check();
    root._write();

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    mpc2000mid_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.hdr()->magic(), "MThd");
    BOOST_CHECK_EQUAL(parsed.hdr()->num_tracks(), 1);
    BOOST_REQUIRE_EQUAL(parsed.tracks()->size(), 1U);
    BOOST_CHECK_EQUAL(parsed.tracks()->at(0)->magic(), "MTrk");
    BOOST_CHECK_EQUAL(parsed.tracks()->at(0)->len_events(), 8);
    BOOST_REQUIRE(parsed.tracks()->at(0)->events() != nullptr);
    BOOST_REQUIRE_EQUAL(parsed.tracks()->at(0)->events()->event()->size(), 2U);

    auto* first = parsed.tracks()->at(0)->events()->event()->at(0).get();
    BOOST_CHECK_EQUAL(first->event_type(), 0x90);
    auto* first_body = dynamic_cast<standard_midi_file_with_running_status_write_t::note_on_event_t*>(first->event_body());
    BOOST_REQUIRE(first_body != nullptr);
    BOOST_CHECK_EQUAL(first_body->note(), 60);
    BOOST_CHECK_EQUAL(first_body->velocity(), 100);

    auto* second = parsed.tracks()->at(0)->events()->event()->at(1).get();
    BOOST_CHECK_EQUAL(second->event_header(), 0xff);
    BOOST_REQUIRE(second->meta_event_body() != nullptr);
    BOOST_CHECK_EQUAL(
        second->meta_event_body()->meta_type(),
        standard_midi_file_with_running_status_write_t::meta_event_body_t::META_TYPE_ENUM_END_OF_TRACK
    );
    BOOST_CHECK_EQUAL(second->meta_event_body()->len()->value(), 0);
}
#else
BOOST_AUTO_TEST_CASE(test_mpc2000mid_write_roundtrip) {
    BOOST_FAIL("mpc2000mid_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
