<?php
namespace Kaitai\Struct\Tests;

class ImportsAbsRelTest extends TestCase {
    public function testImportsAbsRel() {
        $r = ImportsAbsRel::fromFile(self::SRC_DIR_PATH . '/fixed_struct.bin');

        $this->assertSame(80, $r->one());
        $this->assertSame(65, $r->two()->one());
        $this->assertSame(67, $r->two()->two()->one());
    }
}
