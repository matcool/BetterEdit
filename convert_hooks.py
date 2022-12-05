import glob
import re

files = glob.glob('hooks/*.cpp') + glob.glob('tools/**/*.cpp', recursive=True) + glob.glob('utils/*.cpp')

total = 0
for file_name in files:
    with open(file_name, 'r') as file:
        lines = file.read().split('\n')
    remove = []
    last = None
    pending = None
    it = iter(enumerate(lines))
    changed = False
    for i, line in it:
        if line.startswith('GDMAKE_HOOK'):
            m = re.search(r'GDMAKE_HOOK\((.+?),', line)
            if m is None: continue
            pending = m.group(1)
            remove.append(i)
            changed = True

            _, decl = next(it)
            # print(repr(decl))
            name = re.search(r'__.+? (.+?)\(', decl).group(1)
            if name is None:
                print(decl)
                exit(1)
            last = name
            lines[i + 1] = re.sub(r'edx_t\s+edx\s*,', '', decl.replace('__fastcall', ''))
        elif last is not None:
            if 'edx_t' in line:
                lines[i] = re.sub(r'edx_t\s+edx\s*,', '', line)
            elif line.startswith('}'):
                lines[i] = f'}} MAT_GDMAKE_HOOK({pending}, {last});'
                # print(lines[i])
                last = None
            else:
                orig = f'matdash::orig<&{last}>'
                line = line.replace('GDMAKE_ORIG_V', orig).replace('GDMAKE_ORIG_P', orig).replace('GDMAKE_ORIG', orig)
                line = re.sub(r'edx\s*,', '', line)
                lines[i] = line
    if not changed: continue
    for i in reversed(remove):
        lines.pop(i)
    print(f'writing to {file_name}')
    with open(file_name, 'w') as file:
        file.write('\n'.join(lines))
