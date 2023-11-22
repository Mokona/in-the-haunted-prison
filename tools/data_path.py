import os
import sys

output_base = sys.argv[1]  # Absolutely not safe

data_folder = os.path.join(os.path.dirname(__file__), '..', 'data')
output_folder = os.path.join(output_base, 'generated')
