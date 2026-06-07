// Hand-written smoke test for parse instances written via explicit pos.

#include <boost/test/unit_test.hpp>
#include <sstream>

#ifdef KS_HAS_RW_POS_INSTANCE
#include "pos_instance_write.h"

BOOST_AUTO_TEST_CASE(test_pos_instance_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    pos_instance_write_t writer(&ks_write);

    writer.set_marker(0x34);
    writer.set_tail(0x56);
    writer._check();
    writer._write();

    const std::string data = io.str();
    BOOST_CHECK_EQUAL(data, std::string("\x34\x56", 2));

    kaitai::kstream ks_read(data);
    pos_instance_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.marker(), 0x34);
    BOOST_CHECK_EQUAL(reader.tail(), 0x56);
}
#else
BOOST_AUTO_TEST_CASE(test_pos_instance_write_roundtrip) {
    BOOST_FAIL("pos_instance_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
