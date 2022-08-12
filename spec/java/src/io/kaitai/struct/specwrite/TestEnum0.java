package io.kaitai.struct.specwrite;

import io.kaitai.struct.KaitaiStruct.ReadWrite;
import io.kaitai.struct.testwrite.Enum0;
import org.testng.annotations.Test;

public class TestEnum0 extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return Enum0.class;
    }

    @Override
    protected String getSrcFilename() {
        return "enum_0.bin";
    }
}
