from PIL import Image
import os

from utils import output_asm_data

# Get the data path
data_folder = os.path.join(os.path.dirname(__file__), '..', 'data')


def get_component_as_bytes(component):
    # Get red_component 8 by 8 values at a time using the zip function
    component_8_by_8 = zip(*[iter(component)] * 8)

    component_in_bytes = []
    for group_of_value in component_8_by_8:
        # Turn the 8 values of group_of_value into a byte
        byte = 0
        for value in group_of_value:
            byte <<= 1
            byte += 1 if value > 128 else 0

        component_in_bytes.append(byte)

    return component_in_bytes


def get_image_as_bytes(image):
    width, height = image.size

    # Prepare an array for four components of each pixel
    red_component = []
    green_component = []
    blue_component = []
    for x in range(width):
        for y in range(height):
            # Extract the RGBA components of the pixel
            r, g, b, _ = image.getpixel((y, x))
            red_component.append(r)
            green_component.append(g)
            blue_component.append(b)

    red_as_bytes = get_component_as_bytes(red_component)
    green_as_bytes = get_component_as_bytes(green_component)
    blue_as_bytes = get_component_as_bytes(blue_component)

    return red_as_bytes, green_as_bytes, blue_as_bytes


def image_as_asm_data(filename, image, result):
    red_as_bytes, green_as_bytes, blue_as_bytes = get_image_as_bytes(image)
    label = os.path.splitext(filename)[0]
    result += label + ':' + '\n'
    result += output_asm_data(blue_as_bytes)
    result += output_asm_data(red_as_bytes)
    result += output_asm_data(green_as_bytes)
    return label, result

