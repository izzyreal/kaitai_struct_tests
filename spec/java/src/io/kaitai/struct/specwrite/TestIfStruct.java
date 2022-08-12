package io.kaitai.struct.specwrite;

import io.kaitai.struct.testwrite.IfStruct;
import io.kaitai.struct.KaitaiStruct.ReadWrite;
import org.testng.annotations.Test;

public class TestIfStruct extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return IfStruct.class;
    }

    @Override
    protected String getSrcFilename() {
        return "if_struct.bin";
    }
}
