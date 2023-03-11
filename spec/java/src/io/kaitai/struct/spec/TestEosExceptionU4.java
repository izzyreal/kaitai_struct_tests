// Autogenerated from KST: please remove this line if doing any edits by hand!

package io.kaitai.struct.spec;

import io.kaitai.struct.testformats.EosExceptionU4;
import org.testng.annotations.Test;
import static org.testng.Assert.*;
public class TestEosExceptionU4 extends CommonSpec {

    @Test
    public void testEosExceptionU4() throws Exception {
        assertThrowsEofError(new ThrowingRunnable() {
            @Override
            public void run() throws Throwable {
                EosExceptionU4 r = EosExceptionU4.fromFile(SRC_DIR + "term_strz.bin");
            }
        });
    }
}
