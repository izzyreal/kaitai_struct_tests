package io.kaitai.struct.specwrite;

import io.kaitai.struct.KaitaiStruct.ReadWrite;
import io.kaitai.struct.testwrite.TermBytes;
import org.testng.annotations.Test;

public class TestTermBytes extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return TermBytes.class;
    }

    @Override
    protected String getSrcFilename() {
        return "term_strz.bin";
    }
}
