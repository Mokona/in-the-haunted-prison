import unittest

from compress import compress_data


class TestCompress(unittest.TestCase):
    def test_compress_abcdef(self):
        input_data = bytearray(b"abcdef")
        output_data, delta = compress_data(input_data, skip=0,
                                           backwards_mode=False, classic_mode=False, quick_mode=False)

        expected_output_data = bytearray(b'MabcdefUV')
        self.assertEqual(expected_output_data, output_data)
        self.assertEqual(2, delta)

    def test_compress_abcabc(self):
        input_data = bytearray(b"abcabc")
        output_data, delta = compress_data(input_data, skip=0,
                                           backwards_mode=False, classic_mode=False, quick_mode=False)

        expected_output_data = bytearray(b'{abc\xfaUU\x80')
        self.assertEqual(expected_output_data, output_data)
        self.assertEqual(3, delta)
