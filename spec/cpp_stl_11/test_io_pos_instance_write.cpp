// Hand-written smoke test for parse instances written via explicit io + pos.

#include <boost/test/unit_test.hpp>
#include <sstream>

#ifdef KS_HAS_RW_IO_POS_INSTANCE
#include "io_pos_instance_write.h"

BOOST_AUTO_TEST_CASE(test_io_pos_instance_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    io_pos_instance_write_t writer(&ks_write);

    writer.set_marker(0x78);
    writer.set_tail(0x9a);
    writer._check();
    writer._write();

    const std::string data = io.str();
    BOOST_CHECK_EQUAL(data, std::string("\x78\x9a", 2));

    kaitai::kstream ks_read(data);
    io_pos_instance_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.marker(), 0x78);
    BOOST_CHECK_EQUAL(reader.tail(), 0x9a);
}
#else
BOOST_AUTO_TEST_CASE(test_io_pos_instance_write_roundtrip) {
    BOOST_FAIL("io_pos_instance_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
