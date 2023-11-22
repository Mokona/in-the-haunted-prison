from PIL import Image, ImageDraw, ImageFont, ImageColor
import os

from data_path import data_folder, output_folder
from utils import output_asm_data

BLACK = ImageColor.getrgb('black')
WHITE = ImageColor.getrgb('white')

CHAR_PALETTE = " !'-./0123456789:?éABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
CHAR_NUM = len(CHAR_PALETTE)

WIDTH_PER_CHAR = 8
HEIGHT_PER_CHAR = 8

SURFACE_WIDTH = CHAR_NUM * WIDTH_PER_CHAR
SURFACE_HEIGHT = HEIGHT_PER_CHAR

Y_SHIFT = 0


def create_font():
    font = ImageFont.truetype(os.path.join(data_folder, 'PublicPixel.ttf'), size=8)

    # Create a surface with the size of 8 by 8 times the number of characters
    surface = Image.new('RGBA', (SURFACE_WIDTH, SURFACE_HEIGHT))
    surface_draw = ImageDraw.Draw(surface)

    surface_draw.rectangle((0, 0, SURFACE_WIDTH, SURFACE_HEIGHT), fill=BLACK)

    for index, char in enumerate(CHAR_PALETTE):
        x_pos = index * WIDTH_PER_CHAR
        surface_draw.text((x_pos, Y_SHIFT), char, fill=WHITE, font=font)

    return surface


def get_bytes(index, surface):
    x_pos = index * WIDTH_PER_CHAR

    character = surface.crop((x_pos, 0, x_pos + WIDTH_PER_CHAR, HEIGHT_PER_CHAR))

    character_bytes = []
    for y in range(HEIGHT_PER_CHAR):
        byte = 0
        for x in range(WIDTH_PER_CHAR):
            r, g, b, _ = character.getpixel((x, y))
            byte <<= 1
            byte += 1 if r == 255 else 0  # Only look at the red component

        character_bytes.append(byte)

    return character_bytes


def main():
    surface = create_font()
    os.makedirs(os.path.join(output_folder), exist_ok=True)
    surface.save(os.path.join(output_folder, 'font_control.png'))  # Save a control image

    result = ''
    result += "        PUBLIC _font_data\n\n"
    result += '_font_data:\n'

    # Get each character of the font and read it's 8 bytes vertically
    for i in range(CHAR_NUM):
        character_bytes = get_bytes(i, surface)

        result += output_asm_data(character_bytes)

    output_filename = os.path.join(output_folder, 'font_data.asm')

    with open(output_filename, 'w') as f:
        f.write(result)


if __name__ == '__main__':
    main()
