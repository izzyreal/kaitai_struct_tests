// Hand-written smoke test for size-expr raw substream writeback.

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <stdexcept>

#ifdef KS_HAS_RW_RAW_SUBSTREAM_SIZED
#include "raw_substream_sized_write.h"

BOOST_AUTO_TEST_CASE(test_raw_substream_sized_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    raw_substream_sized_write_t writer(&ks_write);

    std::unique_ptr<raw_substream_sized_write_t::payload_type_t> payload(new raw_substream_sized_write_t::payload_type_t(nullptr, &writer, &writer));
    payload->set_left(0x44);
    payload->set_right(0x55);

    writer.set_payload_len(2);
    writer.set_payload(std::move(payload));
    writer._check();
    writer._write();

    const std::string data = io.str();
    BOOST_CHECK_EQUAL(data, std::string("\x02\x44\x55", 3));

    kaitai::kstream ks_read(data);
    raw_substream_sized_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.payload_len(), 0x02);
    BOOST_CHECK_EQUAL(reader.payload()->left(), 0x44);
    BOOST_CHECK_EQUAL(reader.payload()->right(), 0x55);
}

BOOST_AUTO_TEST_CASE(test_raw_substream_sized_write_rejects_size_mismatch) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    raw_substream_sized_write_t writer(&ks_write);

    std::unique_ptr<raw_substream_sized_write_t::payload_type_t> payload(new raw_substream_sized_write_t::payload_type_t(nullptr, &writer, &writer));
    payload->set_left(0x44);
    payload->set_right(0x55);

    writer.set_payload_len(3);
    writer.set_payload(std::move(payload));
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
BOOST_AUTO_TEST_CASE(test_raw_substream_sized_write_roundtrip) {
    BOOST_FAIL("raw_substream_sized_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
