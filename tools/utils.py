def output_asm_data(bytestream):
    result = ''
    values_8_by_8 = zip(*[iter(bytestream)] * 8)
    for values in values_8_by_8:
        result += ' ' * 4 + 'db      ' + ', '.join(['${:02x}'.format(byte) for byte in values]) + '\n'
    return result
