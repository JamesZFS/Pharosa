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

skipset = {'l', 'm', 'o', 's', 'u', '#'}
ord_pattern = re.compile('(\d+)//\d+')

os.chdir(src_dir)

with open(src_name, 'r') as f:
    lines = f.readlines()

new = []

for line in lines:
    if line[0] in skipset: continue
    if line.startswith('vn'): continue

    if line[0] == 'v':
        new.append(line.strip())
    elif line[0] == 'f':
        mt = ord_pattern.findall(line)
        assert len(mt) == 3
        new.append('f %s %s %s' % (mt[0], mt[1], mt[2]))
    else:
        raise ValueError(line[0])

with open(trg_name, 'w') as f:
    f.write('\n'.join(new) + '\n')

print('writen to "%s"' % os.path.abspath(trg_name))
