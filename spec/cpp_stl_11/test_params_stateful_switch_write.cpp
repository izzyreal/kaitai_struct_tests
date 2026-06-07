#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_PARAMS_STATEFUL_SWITCH
#include "params_stateful_switch_write.h"

#include <sstream>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(test_params_stateful_switch_write_roundtrip) {
    params_stateful_switch_write_t root(nullptr);
    root.set_count(2);

    std::unique_ptr<std::vector<std::unique_ptr<params_stateful_switch_write_t::event_t>>> events(
        new std::vector<std::unique_ptr<params_stateful_switch_write_t::event_t>>());

    std::unique_ptr<params_stateful_switch_write_t::event_t> first(
        new params_stateful_switch_write_t::event_t(0xff, nullptr, &root, &root));
    first->set_use_prev(0);
    first->set_kind(1);
    std::unique_ptr<params_stateful_switch_write_t::body_one_t> first_body(
        new params_stateful_switch_write_t::body_one_t(nullptr, first.get(), &root));
    first_body->set_value(0x11);
    first->set_body(std::move(first_body));
    events->push_back(std::move(first));

    std::unique_ptr<params_stateful_switch_write_t::event_t> second(
        new params_stateful_switch_write_t::event_t(1, nullptr, &root, &root));
    second->set_use_prev(1);
    std::unique_ptr<params_stateful_switch_write_t::body_one_t> second_body(
        new params_stateful_switch_write_t::body_one_t(nullptr, second.get(), &root));
    second_body->set_value(0x22);
    second->set_body(std::move(second_body));
    events->push_back(std::move(second));

    root.set_events(std::move(events));
    root._check();

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._write();

    BOOST_CHECK_EQUAL(out.str(), std::string("\x02\x00\x01\x11\x01\x22", 6));

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    params_stateful_switch_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_REQUIRE_EQUAL(parsed.events()->size(), 2U);
    BOOST_CHECK_EQUAL(parsed.events()->at(0)->next_kind(), 1);
    BOOST_CHECK_EQUAL(parsed.events()->at(1)->actual_kind(), 1);
    auto* second_parsed_body = dynamic_cast<params_stateful_switch_write_t::body_one_t*>(parsed.events()->at(1)->body());
    BOOST_REQUIRE(second_parsed_body != nullptr);
    BOOST_CHECK_EQUAL(second_parsed_body->value(), 0x22);
}

BOOST_AUTO_TEST_CASE(test_params_stateful_switch_write_param_mismatch) {
    params_stateful_switch_write_t root(nullptr);
    root.set_count(1);

    std::unique_ptr<std::vector<std::unique_ptr<params_stateful_switch_write_t::event_t>>> events(
        new std::vector<std::unique_ptr<params_stateful_switch_write_t::event_t>>());

    std::unique_ptr<params_stateful_switch_write_t::event_t> event(
        new params_stateful_switch_write_t::event_t(2, nullptr, &root, &root));
    event->set_use_prev(1);
    std::unique_ptr<params_stateful_switch_write_t::body_one_t> wrong_body(
        new params_stateful_switch_write_t::body_one_t(nullptr, event.get(), &root));
    wrong_body->set_value(0x55);
    event->set_body(std::move(wrong_body));
    events->push_back(std::move(event));

    root.set_events(std::move(events));
    BOOST_CHECK_THROW(root._check(), std::runtime_error);
}
#endif
