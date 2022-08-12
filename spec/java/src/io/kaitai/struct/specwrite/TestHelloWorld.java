package io.kaitai.struct.specwrite;

import io.kaitai.struct.KaitaiStruct.ReadWrite;
import io.kaitai.struct.testwrite.HelloWorld;
import org.testng.annotations.Test;

public class TestHelloWorld extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return HelloWorld.class;
    }

    @Override
    protected String getSrcFilename() {
        return "fixed_struct.bin";
    }
}
