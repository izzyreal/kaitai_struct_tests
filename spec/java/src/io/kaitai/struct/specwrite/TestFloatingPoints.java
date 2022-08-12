package io.kaitai.struct.specwrite;

import io.kaitai.struct.KaitaiStruct.ReadWrite;
import io.kaitai.struct.testwrite.FloatingPoints;
import org.testng.annotations.Test;

public class TestFloatingPoints extends CommonSpec {
    @Override
    protected Class<? extends ReadWrite> getStructClass() {
        return FloatingPoints.class;
    }

    @Override
    protected String getSrcFilename() {
        return "floating_points.bin";
    }
}
