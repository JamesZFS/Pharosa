import os
from PIL import Image
import numpy as np

os.chdir('..')
img = Image.open('texture.jpg')
width, height = img.size
data = img.getdata()
data = np.array(data, dtype='int')
a = np.reshape(data, (width, height, 3))

# any image to ppm
with open('test.ppm', 'w', encoding='utf-8') as f:
    f.write('P3\n%d %d\n255\n' % a.shape[:2])  # head
    f.write(' '.join(map(str, a.flatten())) + '\n') # body
