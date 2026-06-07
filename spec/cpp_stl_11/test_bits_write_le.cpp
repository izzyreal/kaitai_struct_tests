// Hand-written smoke test for little-endian bitfield generated C++ read-write fixtures.

#include <boost/test/unit_test.hpp>
#include <sstream>

#ifdef KS_HAS_RW_BITS_WRITE_LE
#include "bits_write_le.h"

BOOST_AUTO_TEST_CASE(test_bits_write_le_roundtrip_and_bytes) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    bits_write_le_t writer(&ks_write);

    writer.set_bits_a(true);
    writer.set_bits_b(5);
    writer.set_bits_c(10);
    writer.set_large_bits_1(682);
    writer.set_spacer(3);
    writer.set_large_bits_2(938);
    writer.set_after(0x7e);
    writer._check();
    writer._write();

    BOOST_CHECK_EQUAL(io.str(), std::string("\xab\xaa\x4e\x75\x7e", 5));

    io.seekg(0);
    kaitai::kstream ks_read(&io);
    bits_write_le_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.bits_a(), true);
    BOOST_CHECK_EQUAL(reader.bits_b(), 5);
    BOOST_CHECK_EQUAL(reader.bits_c(), 10);
    BOOST_CHECK_EQUAL(reader.large_bits_1(), 682);
    BOOST_CHECK_EQUAL(reader.spacer(), 3);
    BOOST_CHECK_EQUAL(reader.large_bits_2(), 938);
    BOOST_CHECK_EQUAL(reader.after(), 0x7e);
}
#else
BOOST_AUTO_TEST_CASE(test_bits_write_le_roundtrip_and_bytes) {
    BOOST_FAIL("bits_write_le read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
