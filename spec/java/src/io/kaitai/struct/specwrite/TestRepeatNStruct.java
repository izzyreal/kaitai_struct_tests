package io.kaitai.struct.specwrite;

import io.kaitai.struct.KaitaiStruct.ReadWrite;
import io.kaitai.struct.testwrite.RepeatNStruct;
import org.testng.annotations.Test;

public class TestRepeatNStruct extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return RepeatNStruct.class;
    }

    @Override
    protected String getSrcFilename() {
        return "repeat_n_struct.bin";
    }
}
