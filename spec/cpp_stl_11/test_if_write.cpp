// Hand-written smoke test for conditional-field C++ read-write fixtures.

#include <boost/test/unit_test.hpp>
#include <memory>
#include <sstream>

#ifdef KS_HAS_RW_IF
#include "if_write.h"

BOOST_AUTO_TEST_CASE(test_if_write_roundtrip_present_and_absent) {
    {
        std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
        kaitai::kstream ks_write(&io);
        if_write_t writer(&ks_write);

        std::unique_ptr<if_write_t::payload_t> payload(new if_write_t::payload_t(nullptr));
        payload->set_magic(std::string("OK", 2));
        payload->set_value(0x7f);

        writer.set_has_label(1);
        writer.set_has_payload(1);
        writer.set_label("TEST");
        writer.set_payload(std::move(payload));
        writer._check();
        writer._write();

        io.seekg(0);
        kaitai::kstream ks_read(&io);
        if_write_t reader(&ks_read);
        reader._read();

        BOOST_CHECK_EQUAL(reader.label(), "TEST");
        BOOST_REQUIRE(reader.payload() != nullptr);
        BOOST_CHECK_EQUAL(reader.payload()->value(), 0x7f);
    }

    {
        std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
        kaitai::kstream ks_write(&io);
        if_write_t writer(&ks_write);

        writer.set_has_label(0);
        writer.set_has_payload(0);
        writer._check();
        writer._write();

        io.seekg(0);
        kaitai::kstream ks_read(&io);
        if_write_t reader(&ks_read);
        reader._read();

        BOOST_CHECK(reader._is_null_label());
        BOOST_CHECK(reader._is_null_payload());
    }
}

BOOST_AUTO_TEST_CASE(test_if_write_check_rejects_presence_mismatch) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    if_write_t writer(&ks_write);

    writer.set_has_label(0);
    writer.set_label("TEST");

    BOOST_CHECK_THROW(
        writer._check(),
        std::runtime_error
    );
}
#else
BOOST_AUTO_TEST_CASE(test_if_write_roundtrip_present_and_absent) {
    BOOST_FAIL("if_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
