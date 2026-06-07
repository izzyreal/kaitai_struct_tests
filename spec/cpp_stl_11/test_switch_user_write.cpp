#include <boost/test/unit_test.hpp>

#if KS_HAS_RW_SWITCH_USER
#include "switch_user_write.h"

#include <sstream>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(test_switch_user_write_roundtrip) {
    switch_user_write_t root(nullptr);
    root.set_kind(2);

    std::unique_ptr<switch_user_write_t::body_two_t> body(new switch_user_write_t::body_two_t(nullptr, &root, &root));
    body->set_value(0x1234);
    root.set_body(std::move(body));

    root._check();

    std::stringstream out(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks(&out);
    root._set_io(&ks);
    root._write();

    BOOST_CHECK_EQUAL(out.str(), std::string("\x02\x34\x12", 3));

    std::stringstream in(out.str(), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream parsed_io(&in);
    switch_user_write_t parsed(&parsed_io);
    parsed._read();

    BOOST_REQUIRE(parsed.body() != nullptr);
    auto* parsed_body = dynamic_cast<switch_user_write_t::body_two_t*>(parsed.body());
    BOOST_REQUIRE(parsed_body != nullptr);
    BOOST_CHECK_EQUAL(parsed_body->value(), 0x1234);
}

BOOST_AUTO_TEST_CASE(test_switch_user_write_type_mismatch) {
    switch_user_write_t root(nullptr);
    root.set_kind(1);

    std::unique_ptr<switch_user_write_t::body_two_t> wrong_body(new switch_user_write_t::body_two_t(nullptr, &root, &root));
    wrong_body->set_value(0x4321);
    root.set_body(std::move(wrong_body));

    BOOST_CHECK_THROW(root._check(), std::runtime_error);
}
#endif
