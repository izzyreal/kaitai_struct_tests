#include <boost/test/unit_test.hpp>

#include <instance_std_array.h>

#include <iostream>
#include <fstream>
#include <vector>

BOOST_AUTO_TEST_CASE(test_instance_std_array) {
    std::ifstream ifs("src/instance_std_array.bin", std::ifstream::binary);
    kaitai::kstream ks(&ifs);
    instance_std_array_t* r = new instance_std_array_t(&ks);

    BOOST_CHECK_EQUAL(r->ofs(), 0x10);
    BOOST_CHECK_EQUAL(r->qty_entries(), 3);
    BOOST_CHECK_EQUAL(r->entry_size(), 4);

    BOOST_CHECK_EQUAL(r->entries()->size(), 3);
    BOOST_CHECK_EQUAL(r->entries()->at(0), std::string("\x11\x11\x11\x11"));
    BOOST_CHECK_EQUAL(r->entries()->at(1), std::string("\x22\x22\x22\x22"));
    BOOST_CHECK_EQUAL(r->entries()->at(2), std::string("\x33\x33\x33\x33"));

    delete r;
}
