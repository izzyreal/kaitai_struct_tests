// Autogenerated from KST: please remove this line if doing any edits by hand!

package io.kaitai.struct.spec;

import io.kaitai.struct.testformats.FloatToI;
import org.testng.annotations.Test;
import static org.testng.Assert.*;
public class TestFloatToI extends CommonSpec {

    @Test
    public void testFloatToI() throws Exception {
        FloatToI r = FloatToI.fromFile(SRC_DIR + "floating_points.bin");

        assertEquals(r.singleValue(), 0.5, 1e-6);
        assertEquals(r.doubleValue(), 0.25, 1e-6);
        assertEquals(r.singleValueIf(), 0.5, 1e-6);
        assertEquals(r.doubleValueIf(), 0.25, 1e-6);
        assertIntEquals(r.singleI(), 0);
        assertIntEquals(r.doubleI(), 0);
        assertIntEquals(r.singleIfI(), 0);
        assertIntEquals(r.doubleIfI(), 0);
        assertIntEquals(r.float1I(), 1);
        assertIntEquals(r.float2I(), 1);
        assertIntEquals(r.float3I(), 1);
        assertIntEquals(r.float4I(), -2);
        assertIntEquals(r.calcIfI(), 13);
    }
}
