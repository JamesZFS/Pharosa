import json
import os
import subprocess
import sys
import time

import cv2
from jsmin import jsmin

if len(sys.argv) < 2:
    print('usage: python renderer_helper.py <config_json_path>')
    exit(1)

EXE_PATH = '/Users/james/Test/CG/Pharosa/cmake-build-debug/Pharosa'

config_path = sys.argv[1]

with open(config_path, 'r') as f:
    j = jsmin(f.read())

min_path = '.'.join(config_path.split('.')[:-1]) + ' - min.json'
with open(min_path, 'w') as f:
    f.write(j)

with open(min_path, 'r') as f:
    config = json.load(f)

save_path = config['save_path']

del config, j

print('#### Pharosa starts ####')
pharosa = subprocess.Popen([EXE_PATH, min_path])  # execute rendering

while True:
    if os.path.exists(save_path) or pharosa.poll() != None: break
    time.sleep(1)

time.sleep(1)
window_name = 'Rendering Progress (press key to refresh)'
cv2.namedWindow(window_name, cv2.WINDOW_AUTOSIZE)

while pharosa.poll() == None:  # keep refreshing window
    img = cv2.imread(save_path)
    cv2.imshow(window_name, img)
    cv2.waitKey()

print("#### Pharosa return code = %d  Press any key to exit ####" % pharosa.returncode)
cv2.waitKey()
cv2.destroyAllWindows()
os.remove(min_path)
