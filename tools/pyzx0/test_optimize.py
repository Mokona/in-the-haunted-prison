import unittest

from optimize import elias_gamma_needed_bits


class TestEliasGammaBits(unittest.TestCase):
    def test_elias_gamma_bits(self):
        self.assertEqual(elias_gamma_needed_bits(1), 1)
        self.assertEqual(elias_gamma_needed_bits(2), 3)
        self.assertEqual(elias_gamma_needed_bits(3), 3)
        self.assertEqual(elias_gamma_needed_bits(4), 5)
        self.assertEqual(elias_gamma_needed_bits(5), 5)
        self.assertEqual(elias_gamma_needed_bits(6), 5)
        self.assertEqual(elias_gamma_needed_bits(7), 5)
        self.assertEqual(elias_gamma_needed_bits(8), 7)
        self.assertEqual(elias_gamma_needed_bits(9), 7)
        self.assertEqual(elias_gamma_needed_bits(10), 7)
        self.assertEqual(elias_gamma_needed_bits(11), 7)
        self.assertEqual(elias_gamma_needed_bits(12), 7)
        self.assertEqual(elias_gamma_needed_bits(13), 7)
        self.assertEqual(elias_gamma_needed_bits(14), 7)
        self.assertEqual(elias_gamma_needed_bits(15), 7)
        self.assertEqual(elias_gamma_needed_bits(16), 9)
        self.assertEqual(elias_gamma_needed_bits(17), 9)
