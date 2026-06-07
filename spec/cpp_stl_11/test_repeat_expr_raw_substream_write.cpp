// Hand-written smoke test for repeated raw substream writeback.

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <stdexcept>
#include <vector>

#ifdef KS_HAS_RW_REPEAT_EXPR_RAW_SUBSTREAM
#include "repeat_expr_raw_substream_write.h"

BOOST_AUTO_TEST_CASE(test_repeat_expr_raw_substream_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_expr_raw_substream_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::unique_ptr<repeat_expr_raw_substream_write_t::entry_t>>> entries(
        new std::vector<std::unique_ptr<repeat_expr_raw_substream_write_t::entry_t>>()
    );

    std::unique_ptr<repeat_expr_raw_substream_write_t::entry_t> first(new repeat_expr_raw_substream_write_t::entry_t(nullptr, &writer, &writer));
    first->set_left(0x10);
    first->set_right(0x11);
    entries->push_back(std::move(first));

    std::unique_ptr<repeat_expr_raw_substream_write_t::entry_t> second(new repeat_expr_raw_substream_write_t::entry_t(nullptr, &writer, &writer));
    second->set_left(0x20);
    second->set_right(0x21);
    entries->push_back(std::move(second));

    writer.set_count(2);
    writer.set_entry_size(2);
    writer.set_entries(std::move(entries));
    writer._check();
    writer._write();

    const std::string data = io.str();
    BOOST_CHECK_EQUAL(data, std::string("\x02\x02\x10\x11\x20\x21", 6));

    kaitai::kstream ks_read(data);
    repeat_expr_raw_substream_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.count(), 0x02);
    BOOST_CHECK_EQUAL(reader.entry_size(), 0x02);
    BOOST_CHECK_EQUAL(reader.entries()->size(), static_cast<std::size_t>(2));
    BOOST_CHECK_EQUAL(reader.entries()->at(0)->left(), 0x10);
    BOOST_CHECK_EQUAL(reader.entries()->at(0)->right(), 0x11);
    BOOST_CHECK_EQUAL(reader.entries()->at(1)->left(), 0x20);
    BOOST_CHECK_EQUAL(reader.entries()->at(1)->right(), 0x21);
}

BOOST_AUTO_TEST_CASE(test_repeat_expr_raw_substream_write_rejects_size_mismatch) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_expr_raw_substream_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::unique_ptr<repeat_expr_raw_substream_write_t::entry_t>>> entries(
        new std::vector<std::unique_ptr<repeat_expr_raw_substream_write_t::entry_t>>()
    );

    std::unique_ptr<repeat_expr_raw_substream_write_t::entry_t> first(new repeat_expr_raw_substream_write_t::entry_t(nullptr, &writer, &writer));
    first->set_left(0x10);
    first->set_right(0x11);
    entries->push_back(std::move(first));

    writer.set_count(1);
    writer.set_entry_size(3);
    writer.set_entries(std::move(entries));
    writer._check();

    BOOST_CHECK_EXCEPTION(
        writer._write(),
        std::runtime_error,
        [](const std::runtime_error& e) {
            return std::string(e.what()).find("serialized size mismatch") != std::string::npos;
        }
    );
}
#else
BOOST_AUTO_TEST_CASE(test_repeat_expr_raw_substream_write_roundtrip) {
    BOOST_FAIL("repeat_expr_raw_substream_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
