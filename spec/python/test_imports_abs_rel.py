import unittest

from imports_abs_rel import ImportsAbsRel

class TestImportsAbsRel(unittest.TestCase):
    def test_imports_abs_rel(self):
        with ImportsAbsRel.from_file('src/fixed_struct.bin') as r:

            self.assertEqual(r.one, 80)
            self.assertEqual(r.two.one, 65)
            self.assertEqual(r.two.two.one, 67)
