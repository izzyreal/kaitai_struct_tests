// Hand-written smoke test for repeat: until raw substream writeback.

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <stdexcept>
#include <vector>

#ifdef KS_HAS_RW_REPEAT_UNTIL_RAW_SUBSTREAM
#include "repeat_until_raw_substream_write.h"

BOOST_AUTO_TEST_CASE(test_repeat_until_raw_substream_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_until_raw_substream_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::unique_ptr<repeat_until_raw_substream_write_t::entry_t>>> entries(
        new std::vector<std::unique_ptr<repeat_until_raw_substream_write_t::entry_t>>()
    );

    std::unique_ptr<repeat_until_raw_substream_write_t::entry_t> first(new repeat_until_raw_substream_write_t::entry_t(nullptr, &writer, &writer));
    first->set_is_last(0);
    first->set_value(0x55);
    entries->push_back(std::move(first));

    std::unique_ptr<repeat_until_raw_substream_write_t::entry_t> second(new repeat_until_raw_substream_write_t::entry_t(nullptr, &writer, &writer));
    second->set_is_last(1);
    second->set_value(0x66);
    entries->push_back(std::move(second));

    writer.set_entry_size(2);
    writer.set_entries(std::move(entries));
    writer._check();
    writer._write();

    const std::string data = io.str();
    BOOST_CHECK_EQUAL(data, std::string("\x02\x00\x55\x01\x66", 5));

    kaitai::kstream ks_read(data);
    repeat_until_raw_substream_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.entry_size(), 0x02);
    BOOST_CHECK_EQUAL(reader.entries()->size(), static_cast<std::size_t>(2));
    BOOST_CHECK_EQUAL(reader.entries()->at(0)->is_last(), 0);
    BOOST_CHECK_EQUAL(reader.entries()->at(0)->value(), 0x55);
    BOOST_CHECK_EQUAL(reader.entries()->at(1)->is_last(), 1);
    BOOST_CHECK_EQUAL(reader.entries()->at(1)->value(), 0x66);
}

BOOST_AUTO_TEST_CASE(test_repeat_until_raw_substream_write_rejects_bad_terminator) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_until_raw_substream_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::unique_ptr<repeat_until_raw_substream_write_t::entry_t>>> entries(
        new std::vector<std::unique_ptr<repeat_until_raw_substream_write_t::entry_t>>()
    );

    std::unique_ptr<repeat_until_raw_substream_write_t::entry_t> first(new repeat_until_raw_substream_write_t::entry_t(nullptr, &writer, &writer));
    first->set_is_last(0);
    first->set_value(0x55);
    entries->push_back(std::move(first));

    writer.set_entry_size(2);
    writer.set_entries(std::move(entries));

    BOOST_CHECK_EXCEPTION(
        writer._check(),
        std::runtime_error,
        [](const std::runtime_error& e) {
            return std::string(e.what()).find("repeat-until condition mismatch") != std::string::npos;
        }
    );
}
#else
BOOST_AUTO_TEST_CASE(test_repeat_until_raw_substream_write_roundtrip) {
    BOOST_FAIL("repeat_until_raw_substream_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
