// Hand-written smoke test for a fixed-layout generated C++ read-write fixture.

#include <boost/test/unit_test.hpp>
#include <sstream>

#ifdef KS_HAS_RW_FIXED_RECORD
#include "fixed_record_write.h"
#include "kaitai/exceptions.h"

BOOST_AUTO_TEST_CASE(test_fixed_record_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    fixed_record_write_t writer(&ks_write);

    writer.set_magic(std::string("KSTR", 4));
    writer.set_version(0x1234);
    writer.set_kind(fixed_record_write_t::RECORD_KIND_ALPHA);
    writer.set_name("ABCD");
    writer.set_payload(std::string("xyz", 3));
    writer._check();
    writer._write();

    io.seekg(0);
    kaitai::kstream ks_read(&io);
    fixed_record_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.magic(), std::string("KSTR", 4));
    BOOST_CHECK_EQUAL(reader.version(), 0x1234);
    BOOST_CHECK_EQUAL(reader.kind(), fixed_record_write_t::RECORD_KIND_ALPHA);
    BOOST_CHECK_EQUAL(reader.name(), "ABCD");
    BOOST_CHECK_EQUAL(reader.payload(), std::string("xyz", 3));
}

BOOST_AUTO_TEST_CASE(test_fixed_record_write_check_rejects_bad_magic) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    fixed_record_write_t writer(&ks_write);

    writer.set_magic(std::string("FAIL", 4));
    writer.set_version(0x1234);
    writer.set_kind(fixed_record_write_t::RECORD_KIND_BETA);
    writer.set_name("WXYZ");
    writer.set_payload(std::string("123", 3));

    BOOST_CHECK_THROW(
        writer._check(),
        kaitai::validation_not_equal_error<std::string>
    );
}
#else
BOOST_AUTO_TEST_CASE(test_fixed_record_write_roundtrip) {
    BOOST_FAIL("fixed_record_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
