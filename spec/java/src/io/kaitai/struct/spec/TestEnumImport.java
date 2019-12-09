// Autogenerated from KST: please remove this line if doing any edits by hand!

package io.kaitai.struct.spec;

import io.kaitai.struct.testformats.EnumImport;
import org.testng.annotations.Test;
import static org.testng.Assert.*;
public class TestEnumImport extends CommonSpec {

    @Test
    public void testEnumImport() throws Exception {
        EnumImport r = EnumImport.fromFile(SRC_DIR + "enum_0.bin");

        assertEquals(r.pet1(), Enum0.Animal.CAT);
        assertEquals(r.pet2(), EnumDeep.Container1.Container2.Animal.HARE);
    }
}
