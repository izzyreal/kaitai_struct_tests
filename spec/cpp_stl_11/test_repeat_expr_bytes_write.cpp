// Hand-written smoke test for repeated fixed-size string/bytes C++ read-write fixtures.

#include <boost/test/unit_test.hpp>
#include <memory>
#include <sstream>
#include <vector>

#ifdef KS_HAS_RW_REPEAT_EXPR_BYTES
#include "repeat_expr_bytes_write.h"

BOOST_AUTO_TEST_CASE(test_repeat_expr_bytes_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_expr_bytes_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::string>> names(new std::vector<std::string>());
    names->push_back("ABCD");
    names->push_back("WXYZ");
    std::unique_ptr<std::vector<std::string>> payloads(new std::vector<std::string>());
    payloads->push_back(std::string("one", 3));
    payloads->push_back(std::string("two", 3));

    writer.set_count(2);
    writer.set_names(std::move(names));
    writer.set_payloads(std::move(payloads));
    writer._check();
    writer._write();

    io.seekg(0);
    kaitai::kstream ks_read(&io);
    repeat_expr_bytes_write_t reader(&ks_read);
    reader._read();

    BOOST_REQUIRE(reader.names() != nullptr);
    BOOST_REQUIRE(reader.payloads() != nullptr);
    BOOST_CHECK_EQUAL(reader.names()->at(0), "ABCD");
    BOOST_CHECK_EQUAL(reader.names()->at(1), "WXYZ");
    BOOST_CHECK_EQUAL(reader.payloads()->at(0), std::string("one", 3));
    BOOST_CHECK_EQUAL(reader.payloads()->at(1), std::string("two", 3));
}

BOOST_AUTO_TEST_CASE(test_repeat_expr_bytes_write_check_rejects_bad_item_size) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_expr_bytes_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::string>> names(new std::vector<std::string>());
    names->push_back("ABC");
    std::unique_ptr<std::vector<std::string>> payloads(new std::vector<std::string>());
    payloads->push_back(std::string("one", 3));

    writer.set_count(1);
    writer.set_names(std::move(names));
    writer.set_payloads(std::move(payloads));

    BOOST_CHECK_THROW(
        writer._check(),
        std::runtime_error
    );
}
#else
BOOST_AUTO_TEST_CASE(test_repeat_expr_bytes_write_roundtrip) {
    BOOST_FAIL("repeat_expr_bytes_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
