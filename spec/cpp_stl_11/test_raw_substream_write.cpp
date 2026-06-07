// Hand-written smoke test for fixed-size raw substream writeback.

#include <boost/test/unit_test.hpp>
#include <sstream>

#ifdef KS_HAS_RW_RAW_SUBSTREAM
#include "raw_substream_write.h"

BOOST_AUTO_TEST_CASE(test_raw_substream_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    raw_substream_write_t writer(&ks_write);

    std::unique_ptr<raw_substream_write_t::payload_type_t> payload(new raw_substream_write_t::payload_type_t(nullptr, &writer, &writer));
    payload->set_left(0x22);
    payload->set_right(0x33);

    writer.set_prefix(0x11);
    writer.set_payload(std::move(payload));
    writer._check();
    writer._write();

    const std::string data = io.str();
    BOOST_CHECK_EQUAL(data, std::string("\x11\x22\x33", 3));

    kaitai::kstream ks_read(data);
    raw_substream_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.prefix(), 0x11);
    BOOST_CHECK_EQUAL(reader.payload()->left(), 0x22);
    BOOST_CHECK_EQUAL(reader.payload()->right(), 0x33);
}
#else
BOOST_AUTO_TEST_CASE(test_raw_substream_write_roundtrip) {
    BOOST_FAIL("raw_substream_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
