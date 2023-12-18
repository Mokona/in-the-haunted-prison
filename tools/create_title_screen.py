import PIL.Image
import os.path
from data_path import data_folder
from pyzx0.compress import compress_data

# Load the image from the data folder
image = PIL.Image.open(os.path.join(data_folder, 'title-dithered-320x200.png'))

# Get the pixels as grayscale values. Everything under 128 is black, everything above is white.
pixels = image.convert('L').load()

# Pack the pixels 8 by 8 into bytes
packed = []
for y in range(0, image.size[1], 8):
    for x in range(0, image.size[0], 8):
        byte = 0
        for dy in range(8):
            for dx in range(8):
                if pixels[x + dx, y + dy] > 128:
                    byte |= 1 << (7 - dx)
        packed.append(byte)

output_data, delta = compress_data(packed, 0, False, False, False)

# Write the output file as a C array
with open(os.path.join(data_folder, 'title-screen.c'), 'w') as output_file:
    output_file.write('const unsigned char title_screen[] = {\n')
    for i in range(0, len(output_data), 16):
        line = []
        for j in range(16):
            if i + j < len(output_data):
                line.append(f'0x{output_data[i + j]:02x}')
        line.append('\n')
        output_file.write('    ')
        output_file.write(','.join(line))
    output_file.write('};\n')
