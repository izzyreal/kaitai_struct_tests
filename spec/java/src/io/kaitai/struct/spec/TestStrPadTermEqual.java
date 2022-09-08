// Autogenerated from KST: please remove this line if doing any edits by hand!

package io.kaitai.struct.spec;

import io.kaitai.struct.testformats.StrPadTermEqual;
import org.testng.annotations.Test;
import static org.testng.Assert.*;
public class TestStrPadTermEqual extends CommonSpec {

    @Test
    public void testStrPadTermEqual() throws Exception {
        StrPadTermEqual r = StrPadTermEqual.fromFile(SRC_DIR + "str_pad_term.bin");

        assertEquals(r.s1(), "str1");
        assertEquals(r.s2(), "str2foo@");
        assertEquals(r.s3(), "str");
        assertEquals(r.s4(), "str4baz@.");
    }
}
