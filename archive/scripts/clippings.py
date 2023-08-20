import shutil
import os

src = r'k:\documents\My Clippings.txt'
dst = r'C:\Users\caloni\blog\themes\blog\static\txt\clippings.txt'
bkp = r'C:\Users\caloni\clippings.txt'
vim = r'C:\Users\caloni\blog\scripts\clippings.so'

if os.path.exists(src):
  shutil.move(src, dst)
  print('clippings updated')
else:
  print('clippings NOT updated')

# todo: parse clippings to clip lines
lines = []
with open(dst, encoding='utf-8-sig') as file:
  lines = file.readlines()

with open(bkp, 'a', encoding='utf-8-sig') as file:
  for line in lines:
    file.write(line)

clips = []
lineIdx = 0
while lineIdx < len(lines):
  clip = {
    'src': lines[lineIdx].strip(),
    'ref': lines[lineIdx+1].strip(),
    'lines': [ lines[lineIdx+3].strip() ]
    }
  lineIdx = lineIdx + 4
  while lines[lineIdx].strip() != '==========':
    clip['lines'].append(lines[lineIdx].strip())
    lineIdx = lineIdx + 1
  lineIdx = lineIdx + 1
  clips.append(clip)

with open(vim, 'w', encoding='utf-8-sig') as f:
  for clip in clips:
    for line in clip['lines']:
      match = 'call matchadd("search", "' + line.replace('"', "\\\"") + '")\n'
      f.write(match)

