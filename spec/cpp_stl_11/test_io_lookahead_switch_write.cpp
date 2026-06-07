#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_IO_LOOKAHEAD_SWITCH
#include "io_lookahead_switch_write.h"

#include <sstream>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(test_io_lookahead_switch_write_roundtrip) {
    io_lookahead_switch_write_t root(nullptr);
    root.set_count(2);

    std::unique_ptr<std::vector<std::unique_ptr<io_lookahead_switch_write_t::event_t>>> events(
        new std::vector<std::unique_ptr<io_lookahead_switch_write_t::event_t>>());

    std::unique_ptr<io_lookahead_switch_write_t::event_t> first(
        new io_lookahead_switch_write_t::event_t(0xff, nullptr, &root, &root));
    first->set_event_header(0x81);
    first->set_status_byte_lookahead(0x81);
    std::unique_ptr<io_lookahead_switch_write_t::body_one_t> first_body(
        new io_lookahead_switch_write_t::body_one_t(nullptr, first.get(), &root));
    first_body->set_value(0x11);
    first->set_body(std::move(first_body));
    events->push_back(std::move(first));

    std::unique_ptr<io_lookahead_switch_write_t::event_t> second(
        new io_lookahead_switch_write_t::event_t(0x81, nullptr, &root, &root));
    second->set_status_byte_lookahead(0x22);
    std::unique_ptr<io_lookahead_switch_write_t::body_one_t> second_body(
        new io_lookahead_switch_write_t::body_one_t(nullptr, second.get(), &root));
    second_body->set_value(0x22);
    second->set_body(std::move(second_body));
    events->push_back(std::move(second));

    root.set_events(std::move(events));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._check();
    root._write();

    BOOST_CHECK_EQUAL(out.str(), std::string("\x02\x81\x11\x22", 4));

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    io_lookahead_switch_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_REQUIRE_EQUAL(parsed.events()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed.events()->at(1)->using_prev(), true);
    BOOST_CHECK_EQUAL(parsed.events()->at(1)->actual_kind(), 0x81);
    auto* second_parsed_body = dynamic_cast<io_lookahead_switch_write_t::body_one_t*>(parsed.events()->at(1)->body());
    BOOST_REQUIRE(second_parsed_body != nullptr);
    BOOST_CHECK_EQUAL(second_parsed_body->value(), 0x22);
}

BOOST_AUTO_TEST_CASE(test_io_lookahead_switch_write_wrong_cached_lookahead) {
    io_lookahead_switch_write_t root(nullptr);
    root.set_count(1);

    std::unique_ptr<std::vector<std::unique_ptr<io_lookahead_switch_write_t::event_t>>> events(
        new std::vector<std::unique_ptr<io_lookahead_switch_write_t::event_t>>());

    std::unique_ptr<io_lookahead_switch_write_t::event_t> event(
        new io_lookahead_switch_write_t::event_t(0x82, nullptr, &root, &root));
    event->set_status_byte_lookahead(0x33);
    std::unique_ptr<io_lookahead_switch_write_t::body_one_t> wrong_body(
        new io_lookahead_switch_write_t::body_one_t(nullptr, event.get(), &root));
    wrong_body->set_value(0x44);
    event->set_body(std::move(wrong_body));
    events->push_back(std::move(event));

    root.set_events(std::move(events));

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    BOOST_CHECK_THROW(root._check(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_io_lookahead_switch_write_invalidate_cached_values_on_setter) {
    io_lookahead_switch_write_t::event_t event(0x82, nullptr, nullptr, nullptr);
    event.set_status_byte_lookahead(0x22);

    BOOST_CHECK_EQUAL(event.using_prev(), true);
    BOOST_CHECK_EQUAL(event.actual_kind(), 0x82);

    event.set_status_byte_lookahead(0x81);
    event.set_event_header(0x81);
    BOOST_CHECK_EQUAL(event.using_prev(), false);
    BOOST_CHECK_EQUAL(event.actual_kind(), 0x81);
}
#endif
