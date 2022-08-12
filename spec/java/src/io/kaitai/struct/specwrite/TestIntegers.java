package io.kaitai.struct.specwrite;

import io.kaitai.struct.KaitaiStruct.ReadWrite;
import io.kaitai.struct.testwrite.Integers;
import org.testng.annotations.Test;

public class TestIntegers extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return Integers.class;
    }

    @Override
    protected String getSrcFilename() {
        return "fixed_struct.bin";
    }
}
