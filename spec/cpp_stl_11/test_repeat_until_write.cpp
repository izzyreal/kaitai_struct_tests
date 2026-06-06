// Hand-written smoke test for repeat-until generated C++ read-write fixtures.

#include <boost/test/unit_test.hpp>
#include <memory>
#include <sstream>
#include <vector>

#ifdef KS_HAS_RW_REPEAT_UNTIL
#include "repeat_until_write.h"

BOOST_AUTO_TEST_CASE(test_repeat_until_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_until_write_t writer(&ks_write);

    std::unique_ptr<std::vector<int32_t>> entries(new std::vector<int32_t>());
    entries->push_back(-3);
    entries->push_back(275000);
    entries->push_back(-1);

    writer.set_entries(std::move(entries));
    writer._check();
    writer._write();

    io.seekg(0);
    kaitai::kstream ks_read(&io);
    repeat_until_write_t reader(&ks_read);
    reader._read();

    BOOST_REQUIRE(reader.entries() != nullptr);
    BOOST_REQUIRE_EQUAL(reader.entries()->size(), 3U);
    BOOST_CHECK_EQUAL(reader.entries()->at(0), -3);
    BOOST_CHECK_EQUAL(reader.entries()->at(1), 275000);
    BOOST_CHECK_EQUAL(reader.entries()->at(2), -1);
}

BOOST_AUTO_TEST_CASE(test_repeat_until_write_check_rejects_no_terminator) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_until_write_t writer(&ks_write);

    std::unique_ptr<std::vector<int32_t>> entries(new std::vector<int32_t>());
    entries->push_back(-3);
    entries->push_back(275000);
    entries->push_back(0);

    writer.set_entries(std::move(entries));

    BOOST_CHECK_THROW(
        writer._check(),
        std::runtime_error
    );
}
#else
BOOST_AUTO_TEST_CASE(test_repeat_until_write_roundtrip) {
    BOOST_FAIL("repeat_until_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
