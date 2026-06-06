// Hand-written smoke test for repeated primitive generated C++ read-write fixtures.

#include <boost/test/unit_test.hpp>
#include <memory>
#include <sstream>
#include <vector>

#ifdef KS_HAS_RW_REPEAT_EXPR
#include "repeat_expr_write.h"

BOOST_AUTO_TEST_CASE(test_repeat_expr_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_expr_write_t writer(&ks_write);

    std::unique_ptr<std::vector<uint8_t>> values(new std::vector<uint8_t>());
    values->push_back(0x11);
    values->push_back(0x22);
    values->push_back(0x33);

    writer.set_count(3);
    writer.set_values(std::move(values));
    writer._check();
    writer._write();

    io.seekg(0);
    kaitai::kstream ks_read(&io);
    repeat_expr_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.count(), 3);
    BOOST_REQUIRE(reader.values() != nullptr);
    BOOST_REQUIRE_EQUAL(reader.values()->size(), 3U);
    BOOST_CHECK_EQUAL(reader.values()->at(0), 0x11);
    BOOST_CHECK_EQUAL(reader.values()->at(1), 0x22);
    BOOST_CHECK_EQUAL(reader.values()->at(2), 0x33);
}

BOOST_AUTO_TEST_CASE(test_repeat_expr_write_check_rejects_size_mismatch) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_expr_write_t writer(&ks_write);

    std::unique_ptr<std::vector<uint8_t>> values(new std::vector<uint8_t>());
    values->push_back(0x11);
    values->push_back(0x22);

    writer.set_count(3);
    writer.set_values(std::move(values));

    BOOST_CHECK_THROW(
        writer._check(),
        std::runtime_error
    );
}
#else
BOOST_AUTO_TEST_CASE(test_repeat_expr_write_roundtrip) {
    BOOST_FAIL("repeat_expr_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
