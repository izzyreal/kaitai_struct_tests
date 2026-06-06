// Hand-written smoke test for nested generated C++ read-write fixtures.

#include <boost/test/unit_test.hpp>
#include <memory>
#include <sstream>

#ifdef KS_HAS_RW_NESTED_RECORD
#include "nested_record_write.h"
#include "kaitai/exceptions.h"

BOOST_AUTO_TEST_CASE(test_nested_record_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    nested_record_write_t writer(&ks_write);

    std::unique_ptr<nested_record_write_t::entry_t> entry(new nested_record_write_t::entry_t(nullptr));
    entry->set_tag(std::string("ENTR", 4));
    entry->set_value(0x3456);

    writer.set_header(std::string("ROOT", 4));
    writer.set_version(0x12);
    writer.set_entry(std::move(entry));
    writer._check();
    writer._write();

    io.seekg(0);
    kaitai::kstream ks_read(&io);
    nested_record_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.header(), std::string("ROOT", 4));
    BOOST_CHECK_EQUAL(reader.version(), 0x12);
    BOOST_REQUIRE(reader.entry() != nullptr);
    BOOST_CHECK_EQUAL(reader.entry()->tag(), std::string("ENTR", 4));
    BOOST_CHECK_EQUAL(reader.entry()->value(), 0x3456);
}

BOOST_AUTO_TEST_CASE(test_nested_record_write_check_rejects_bad_child_contents) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    nested_record_write_t writer(&ks_write);

    std::unique_ptr<nested_record_write_t::entry_t> entry(new nested_record_write_t::entry_t(nullptr));
    entry->set_tag(std::string("FAIL", 4));
    entry->set_value(0x3456);

    writer.set_header(std::string("ROOT", 4));
    writer.set_version(0x12);
    writer.set_entry(std::move(entry));

    BOOST_CHECK_THROW(
        writer._check(),
        kaitai::validation_not_equal_error<std::string>
    );
}
#else
BOOST_AUTO_TEST_CASE(test_nested_record_write_roundtrip) {
    BOOST_FAIL("nested_record_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
