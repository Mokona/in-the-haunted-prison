import os
import sys

if len(sys.argv) > 1:
    output_base = sys.argv[1]  # Absolutely not safe
else:
    output_base = os.path.join(os.path.dirname(__file__), '..')

data_folder = os.path.join(os.path.dirname(__file__), '..', 'data')
output_folder = os.path.join(output_base, 'generated')
