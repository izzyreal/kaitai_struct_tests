// Hand-written smoke test for parsing and rewriting realistic MIDI track bytes.

#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_STANDARD_MIDI_FILE_WITH_RUNNING_STATUS
#include "standard_midi_file_with_running_status_write.h"

#include <sstream>
#include <vector>

namespace {

void append_u2be(std::string& out, uint16_t value) {
    out.push_back(static_cast<char>((value >> 8) & 0xFF));
    out.push_back(static_cast<char>(value & 0xFF));
}

void append_u4be(std::string& out, uint32_t value) {
    out.push_back(static_cast<char>((value >> 24) & 0xFF));
    out.push_back(static_cast<char>((value >> 16) & 0xFF));
    out.push_back(static_cast<char>((value >> 8) & 0xFF));
    out.push_back(static_cast<char>(value & 0xFF));
}

void append_delta(std::string& out, uint8_t value) {
    out.push_back(static_cast<char>(value));
}

void append_meta_event(std::string& out, uint8_t delta, uint8_t meta_type, const std::string& body) {
    append_delta(out, delta);
    out.push_back(static_cast<char>(0xFF));
    out.push_back(static_cast<char>(meta_type));
    out.push_back(static_cast<char>(body.size()));
    out.append(body);
}

void append_note_on(std::string& out, uint8_t delta, uint8_t note, uint8_t velocity) {
    append_delta(out, delta);
    out.push_back(static_cast<char>(0x90));
    out.push_back(static_cast<char>(note));
    out.push_back(static_cast<char>(velocity));
}

std::string build_sample_midi() {
    std::string track;
    append_meta_event(track, 0x00, 0x04, "        ");
    append_meta_event(track, 0x00, 0x01, "TRACK DATA:00E1006403  020102   ");
    append_meta_event(track, 0x00, 0x03, "TRACKNAME1234567");
    append_note_on(track, 0x00, 0x25, 0x7F);
    append_note_on(track, 0x0A, 0x25, 0x00);
    append_meta_event(track, 0x00, 0x2F, "");

    std::string file;
    file.append("MThd", 4);
    append_u4be(file, 6);
    append_u2be(file, 1);
    append_u2be(file, 1);
    append_u2be(file, 96);
    file.append("MTrk", 4);
    append_u4be(file, static_cast<uint32_t>(track.size()));
    file.append(track);
    return file;
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_standard_midi_file_with_running_status_write_parse_rewrite_realistic_track) {
    const std::string original = build_sample_midi();

    std::stringstream in(original, std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    standard_midi_file_with_running_status_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_CHECK_EQUAL(parsed.hdr()->magic(), "MThd");
    BOOST_CHECK_EQUAL(parsed.hdr()->num_tracks(), 1);
    BOOST_REQUIRE_EQUAL(parsed.tracks()->size(), 1U);

    auto* track = parsed.tracks()->at(0).get();
    BOOST_CHECK_EQUAL(track->magic(), "MTrk");
    BOOST_REQUIRE(track->events() != nullptr);
    BOOST_REQUIRE_EQUAL(track->events()->event()->size(), 6U);

    auto* instrument = track->events()->event()->at(0).get();
    BOOST_REQUIRE(instrument->meta_event_body() != nullptr);
    BOOST_CHECK_EQUAL(
        instrument->meta_event_body()->meta_type(),
        standard_midi_file_with_running_status_write_t::meta_event_body_t::META_TYPE_ENUM_INSTRUMENT_NAME
    );
    BOOST_CHECK_EQUAL(instrument->meta_event_body()->body(), "        ");

    auto* track_data = track->events()->event()->at(1).get();
    BOOST_REQUIRE(track_data->meta_event_body() != nullptr);
    BOOST_CHECK_EQUAL(
        track_data->meta_event_body()->meta_type(),
        standard_midi_file_with_running_status_write_t::meta_event_body_t::META_TYPE_ENUM_TEXT_EVENT
    );
    BOOST_CHECK_EQUAL(track_data->meta_event_body()->body(), "TRACK DATA:00E1006403  020102   ");

    auto* name = track->events()->event()->at(2).get();
    BOOST_REQUIRE(name->meta_event_body() != nullptr);
    BOOST_CHECK_EQUAL(
        name->meta_event_body()->meta_type(),
        standard_midi_file_with_running_status_write_t::meta_event_body_t::META_TYPE_ENUM_SEQUENCE_TRACK_NAME
    );
    BOOST_CHECK_EQUAL(name->meta_event_body()->body(), "TRACKNAME1234567");

    auto* note_on = track->events()->event()->at(3).get();
    BOOST_CHECK_EQUAL(note_on->event_type(), 0x90);
    auto* note_on_body =
        dynamic_cast<standard_midi_file_with_running_status_write_t::note_on_event_t*>(note_on->event_body());
    BOOST_REQUIRE(note_on_body != nullptr);
    BOOST_CHECK_EQUAL(note_on_body->note(), 0x25);
    BOOST_CHECK_EQUAL(note_on_body->velocity(), 0x7F);

    auto* note_off = track->events()->event()->at(4).get();
    BOOST_CHECK_EQUAL(note_off->event_type(), 0x90);
    auto* note_off_body =
        dynamic_cast<standard_midi_file_with_running_status_write_t::note_on_event_t*>(note_off->event_body());
    BOOST_REQUIRE(note_off_body != nullptr);
    BOOST_CHECK_EQUAL(note_off_body->note(), 0x25);
    BOOST_CHECK_EQUAL(note_off_body->velocity(), 0x00);

    auto* end = track->events()->event()->at(5).get();
    BOOST_REQUIRE(end->meta_event_body() != nullptr);
    BOOST_CHECK_EQUAL(
        end->meta_event_body()->meta_type(),
        standard_midi_file_with_running_status_write_t::meta_event_body_t::META_TYPE_ENUM_END_OF_TRACK
    );

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream out_io(&out);
    parsed._set_io(&out_io);
    parsed._check();
    parsed._write();

    BOOST_CHECK_EQUAL(out.str(), original);
}
#else
BOOST_AUTO_TEST_CASE(test_standard_midi_file_with_running_status_write_parse_rewrite_realistic_track) {
    BOOST_FAIL("standard_midi_file_with_running_status_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
