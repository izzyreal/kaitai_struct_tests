package io.kaitai.struct.specwrite;

import io.kaitai.struct.KaitaiStruct.ReadWrite;
import io.kaitai.struct.testwrite.NestedTypes;
import org.testng.annotations.Test;

public class TestNestedTypes extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return NestedTypes.class;
    }

    @Override
    protected String getSrcFilename() {
        return "fixed_struct.bin";
    }
}
