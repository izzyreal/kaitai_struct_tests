package io.kaitai.struct.specwrite;

import io.kaitai.struct.KaitaiStruct.ReadWrite;
import io.kaitai.struct.testwrite.SwitchManualInt;
import org.testng.annotations.Test;

public class TestSwitchManualInt extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return SwitchManualInt.class;
    }

    @Override
    protected String getSrcFilename() {
        return "switch_opcodes.bin";
    }
}
