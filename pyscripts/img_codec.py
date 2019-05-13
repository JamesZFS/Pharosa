import sys

import numpy as np
from PIL import Image

if len(sys.argv) < 2:
    print("usage: python img_codec.py <src_path>")
    exit(1)

src_path = sys.argv[1]
dst_path = '.'.join(src_path.split('.')[:-1]) + '.ppm'

img = Image.open(sys.argv[1])
width, height = img.size
data = img.getdata()
data = np.array(data, dtype='int')
a = np.reshape(data, (width, height, 3))

# any image to ppm
with open(dst_path, 'w', encoding='utf-8') as f:
    f.write('P3\n%d %d\n255\n' % a.shape[:2])  # head
    f.write(' '.join(map(str, a.flatten())) + '\n')  # body
