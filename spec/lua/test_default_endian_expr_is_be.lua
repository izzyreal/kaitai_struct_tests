-- Autogenerated from KST: please remove this line if doing any edits by hand!

local luaunit = require("luaunit")

require("default_endian_expr_is_be")

TestDefaultEndianExprIsBe = {}

function TestDefaultEndianExprIsBe:test_default_endian_expr_is_be()
    local r = DefaultEndianExprIsBe:from_file("src/endian_expr.bin")

    luaunit.assertEquals(r.docs[1].indicator, "\073\073")
    luaunit.assertEquals(r.docs[1].main.some_int, 66)
    luaunit.assertEquals(r.docs[1].main.some_int_be, 66)
    luaunit.assertEquals(r.docs[1].main.some_int_le, 66)
    luaunit.assertEquals(r.docs[1].main.inst_int, 66)
    luaunit.assertEquals(r.docs[1].main.inst_sub.foo, 66)
    luaunit.assertEquals(r.docs[2].indicator, "\077\077")
    luaunit.assertEquals(r.docs[2].main.some_int, 66)
    luaunit.assertEquals(r.docs[2].main.some_int_be, 66)
    luaunit.assertEquals(r.docs[2].main.some_int_le, 66)
    luaunit.assertEquals(r.docs[2].main.inst_int, 1107296256)
    luaunit.assertEquals(r.docs[2].main.inst_sub.foo, 1107296256)
    luaunit.assertEquals(r.docs[3].indicator, "\088\088")
    luaunit.assertEquals(r.docs[3].main.some_int, 1107296256)
    luaunit.assertEquals(r.docs[3].main.some_int_be, 66)
    luaunit.assertEquals(r.docs[3].main.some_int_le, 66)
    luaunit.assertEquals(r.docs[3].main.inst_int, 66)
    luaunit.assertEquals(r.docs[3].main.inst_sub.foo, 66)
end
