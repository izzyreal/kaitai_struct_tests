package io.kaitai.struct.specwrite;

import io.kaitai.struct.KaitaiStruct.ReadWrite;
import io.kaitai.struct.testwrite.StrEos;
import org.testng.annotations.Test;

public class TestStrEos extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return StrEos.class;
    }

    @Override
    protected String getSrcFilename() {
        return "term_strz.bin";
    }
}
