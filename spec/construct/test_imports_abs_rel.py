import unittest

from imports_abs_rel import _schema

class TestImportsAbsRel(unittest.TestCase):
    def test_imports_abs_rel(self):
        r = _schema.parse_file('src/fixed_struct.bin')

        self.assertEqual(r.one, 80)
        self.assertEqual(r.two.one, 65)
        self.assertEqual(r.two.two.one, 67)
