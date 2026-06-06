// Hand-written smoke test for repeat-eos generated C++ read-write fixtures.

#include <boost/test/unit_test.hpp>
#include <memory>
#include <sstream>
#include <vector>

#ifdef KS_HAS_RW_REPEAT_EOS
#include "repeat_eos_write.h"

BOOST_AUTO_TEST_CASE(test_repeat_eos_write_roundtrip_appendable_stream) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_eos_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::string>> records(new std::vector<std::string>());
    records->push_back(std::string("one", 3));
    records->push_back(std::string("two", 3));

    writer.set_records(std::move(records));
    writer._check();
    writer._write();

    io.seekg(0);
    kaitai::kstream ks_read(&io);
    repeat_eos_write_t reader(&ks_read);
    reader._read();

    BOOST_REQUIRE(reader.records() != nullptr);
    BOOST_REQUIRE_EQUAL(reader.records()->size(), 2U);
    BOOST_CHECK_EQUAL(reader.records()->at(0), std::string("one", 3));
    BOOST_CHECK_EQUAL(reader.records()->at(1), std::string("two", 3));
}

BOOST_AUTO_TEST_CASE(test_repeat_eos_write_rejects_fixed_capacity_mismatch) {
    std::stringstream io(std::string(9, '\0'), std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_eos_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::string>> records(new std::vector<std::string>());
    records->push_back(std::string("one", 3));
    records->push_back(std::string("two", 3));

    writer.set_records(std::move(records));
    writer._check();

    BOOST_CHECK_THROW(
        writer._write(),
        std::runtime_error
    );
}
#else
BOOST_AUTO_TEST_CASE(test_repeat_eos_write_roundtrip_appendable_stream) {
    BOOST_FAIL("repeat_eos_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
