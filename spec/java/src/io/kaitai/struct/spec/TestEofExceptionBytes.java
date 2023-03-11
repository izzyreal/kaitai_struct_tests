// Autogenerated from KST: please remove this line if doing any edits by hand!

package io.kaitai.struct.spec;

import io.kaitai.struct.testformats.EofExceptionBytes;
import org.testng.annotations.Test;
import static org.testng.Assert.*;
public class TestEofExceptionBytes extends CommonSpec {

    @Test
    public void testEofExceptionBytes() throws Exception {
        assertThrowsEofError(new ThrowingRunnable() {
            @Override
            public void run() throws Throwable {
                EofExceptionBytes r = EofExceptionBytes.fromFile(SRC_DIR + "term_strz.bin");
            }
        });
    }
}
