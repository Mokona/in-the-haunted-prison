import unittest

from tools.utils import output_asm_data


class TestOutputAsmData(unittest.TestCase):
    def test_output_asm_data_with_8_values(self):
        bytestream = [170, 170, 170, 170, 170, 170, 170, 170]
        expected_result = '    db      $aa, $aa, $aa, $aa, $aa, $aa, $aa, $aa\n'
        self.assertEqual(output_asm_data(bytestream), expected_result)

    def test_output_asm_data_with_16_values(self):
        bytestream = [170, 170, 170, 170, 170, 170, 170, 170,
                      170, 170, 170, 170, 170, 170, 170, 170]
        expected_result = '    db      $aa, $aa, $aa, $aa, $aa, $aa, $aa, $aa\n' + \
                          '    db      $aa, $aa, $aa, $aa, $aa, $aa, $aa, $aa\n'
        self.assertEqual(output_asm_data(bytestream), expected_result)


if __name__ == '__main__':
    unittest.main()
