// Hand-written smoke test for repeat: eos raw substream writeback.

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <stdexcept>
#include <vector>

#ifdef KS_HAS_RW_REPEAT_EOS_RAW_SUBSTREAM
#include "repeat_eos_raw_substream_write.h"

BOOST_AUTO_TEST_CASE(test_repeat_eos_raw_substream_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_eos_raw_substream_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::unique_ptr<repeat_eos_raw_substream_write_t::entry_t>>> entries(
        new std::vector<std::unique_ptr<repeat_eos_raw_substream_write_t::entry_t>>()
    );

    std::unique_ptr<repeat_eos_raw_substream_write_t::entry_t> first(new repeat_eos_raw_substream_write_t::entry_t(nullptr, &writer, &writer));
    first->set_left(0x31);
    first->set_right(0x32);
    entries->push_back(std::move(first));

    std::unique_ptr<repeat_eos_raw_substream_write_t::entry_t> second(new repeat_eos_raw_substream_write_t::entry_t(nullptr, &writer, &writer));
    second->set_left(0x41);
    second->set_right(0x42);
    entries->push_back(std::move(second));

    writer.set_entry_size(2);
    writer.set_entries(std::move(entries));
    writer._check();
    writer._write();

    const std::string data = io.str();
    BOOST_CHECK_EQUAL(data, std::string("\x02\x31\x32\x41\x42", 5));

    kaitai::kstream ks_read(data);
    repeat_eos_raw_substream_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.entry_size(), 0x02);
    BOOST_CHECK_EQUAL(reader.entries()->size(), static_cast<std::size_t>(2));
    BOOST_CHECK_EQUAL(reader.entries()->at(0)->left(), 0x31);
    BOOST_CHECK_EQUAL(reader.entries()->at(0)->right(), 0x32);
    BOOST_CHECK_EQUAL(reader.entries()->at(1)->left(), 0x41);
    BOOST_CHECK_EQUAL(reader.entries()->at(1)->right(), 0x42);
}
#else
BOOST_AUTO_TEST_CASE(test_repeat_eos_raw_substream_write_roundtrip) {
    BOOST_FAIL("repeat_eos_raw_substream_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
