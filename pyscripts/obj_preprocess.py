import sys
import os
import re

if len(sys.argv) < 2:
    print('用法：python obj_preprocess.py <src_obj_path>')
    exit(1)

src_path = sys.argv[1]
src_name = os.path.basename(src_path)
src_dir = os.path.dirname(src_path)
trg_name = src_name.replace('.', ' - done.')

skipset = {'vn', 'vt', 'l ', 'mt', 'o ', 's ', 'us', 'g '}
ord_pattern = re.compile('(\d+)/\d*/\d+')   # v / vt / vn

os.chdir(src_dir)

with open(src_name, 'r') as f:
    lines = f.readlines()

new = []

for line in lines:
    if line.startswith('#'): continue
    mark = line[:2]
    if mark in skipset: continue

    if mark == 'v ':
        new.append(line.strip())
    elif mark == 'f ':
        mt = ord_pattern.findall(line)
        try:
            assert len(mt) == 3
        except AssertionError as e:
            print(line)
            raise e
        new.append('f %s %s %s' % (mt[0], mt[1], mt[2]))
    else:
        raise ValueError(mark)

with open(trg_name, 'w') as f:
    f.write('\n'.join(new) + '\n')

print('writen to "%s"' % os.path.abspath(trg_name))
