// Hand-written smoke test for repeated user-type C++ read-write fixtures.

#include <boost/test/unit_test.hpp>
#include <memory>
#include <sstream>
#include <vector>

#ifdef KS_HAS_RW_REPEAT_EXPR_USER
#include "repeat_expr_user_write.h"
#include "kaitai/exceptions.h"

BOOST_AUTO_TEST_CASE(test_repeat_expr_user_write_roundtrip) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_expr_user_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::unique_ptr<repeat_expr_user_write_t::entry_t>>> entries(
        new std::vector<std::unique_ptr<repeat_expr_user_write_t::entry_t>>()
    );
    std::unique_ptr<repeat_expr_user_write_t::entry_t> e1(new repeat_expr_user_write_t::entry_t(nullptr));
    e1->set_value(0x1234);
    e1->set_code(std::string("OK", 2));
    entries->push_back(std::move(e1));
    std::unique_ptr<repeat_expr_user_write_t::entry_t> e2(new repeat_expr_user_write_t::entry_t(nullptr));
    e2->set_value(0x5678);
    e2->set_code(std::string("OK", 2));
    entries->push_back(std::move(e2));

    writer.set_count(2);
    writer.set_entries(std::move(entries));
    writer._check();
    writer._write();

    io.seekg(0);
    kaitai::kstream ks_read(&io);
    repeat_expr_user_write_t reader(&ks_read);
    reader._read();

    BOOST_REQUIRE(reader.entries() != nullptr);
    BOOST_REQUIRE_EQUAL(reader.entries()->size(), 2U);
    BOOST_CHECK_EQUAL(reader.entries()->at(0)->value(), 0x1234);
    BOOST_CHECK_EQUAL(reader.entries()->at(1)->value(), 0x5678);
}

BOOST_AUTO_TEST_CASE(test_repeat_expr_user_write_check_rejects_bad_child_contents) {
    std::stringstream io(std::ios::in | std::ios::out | std::ios::binary);
    kaitai::kstream ks_write(&io);
    repeat_expr_user_write_t writer(&ks_write);

    std::unique_ptr<std::vector<std::unique_ptr<repeat_expr_user_write_t::entry_t>>> entries(
        new std::vector<std::unique_ptr<repeat_expr_user_write_t::entry_t>>()
    );
    std::unique_ptr<repeat_expr_user_write_t::entry_t> e1(new repeat_expr_user_write_t::entry_t(nullptr));
    e1->set_value(0x1234);
    e1->set_code(std::string("NO", 2));
    entries->push_back(std::move(e1));

    writer.set_count(1);
    writer.set_entries(std::move(entries));

    BOOST_CHECK_THROW(
        writer._check(),
        kaitai::validation_not_equal_error<std::string>
    );
}
#else
BOOST_AUTO_TEST_CASE(test_repeat_expr_user_write_roundtrip) {
    BOOST_FAIL("repeat_expr_user_write read-write fixture was not generated; run tests/build-formats cpp_stl_11 first");
}
#endif
