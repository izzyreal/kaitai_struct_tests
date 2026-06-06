// Hand-written smoke test for the generated C++ read-write prototype.

#include <boost/test/unit_test.hpp>
#include <sstream>

#ifdef KS_HAS_RW_HELLO_WORLD
#include "hello_world_write.h"

BOOST_AUTO_TEST_CASE(test_hello_world_write) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    hello_world_write_t writer(&ks_write);

    writer.set_one(0x50);
    writer._check();
    writer._write();

    io.seekg(0);
    kaitai::kstream ks_read(&io);
    hello_world_write_t reader(&ks_read);
    reader._read();

    BOOST_CHECK_EQUAL(reader.one(), 80);
}
#else
BOOST_AUTO_TEST_CASE(test_hello_world_write) {
    BOOST_FAIL("hello_world_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
