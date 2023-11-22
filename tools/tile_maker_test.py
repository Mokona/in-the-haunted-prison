import unittest

from tile_maker import get_component_as_bytes


class TestGetComponentAsBytes(unittest.TestCase):
    def test_as_a_length_of_input_divisible_by_8(self):
        component = [255, 255, 255, 255, 255, 255, 255, 255]
        result = get_component_as_bytes(component)
        self.assertEqual(len(result), len(component) // 8)

    def test_returns_255_if_input_is_8_times_255(self):
        red_component = [255, 255, 255, 255, 255, 255, 255, 255]
        expected_result = [255]
        result = get_component_as_bytes(red_component)
        self.assertEqual(result, expected_result)

    def test_returns_0_if_input_is_8_times_0(self):
        red_component = [0, 0, 0, 0, 0, 0, 0, 0]
        expected_result = [0]
        result = get_component_as_bytes(red_component)
        self.assertEqual(result, expected_result)

    def test_returns_the_correct_sequence_for_an_input_pattern(self):
        red_component = [255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255]
        expected_result = [0x80, 0x01]
        result = get_component_as_bytes(red_component)
        self.assertEqual(result, expected_result)


if __name__ == '__main__':
    unittest.main()
