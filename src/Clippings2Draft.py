import shutil
import os

src = r'k:\documents\My Clippings.txt'
dst = os.path.join(os.getcwd(), 'draft.txt')

lines = []
if os.path.exists(src):
  with open(src, encoding='utf-8-sig') as file:
    lines = file.readlines()
  with open(dst, 'a', encoding='utf-8-sig') as file:
    for line in lines:
      file.write(line)
  os.remove(src)
  print('clippings updated')
else:
  print('clippings NOT updated')
  quit()

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

with open(dst, 'a', encoding='utf-8-sig') as f:
  f.write('\n')
  for clip in clips:
    if 'Blogue do Caloni' in clip['src']:
      for line in clip['lines']:
        match = 'call matchadd("search", "' + line.replace('"', "\\\"") + '")\n'
        f.write(match)

with open(dst, 'a', encoding='utf-8-sig') as f:
  f.write('\n')
  for clip in clips:
    if 'Blogue do Caloni' in clip['src']:
      for line in clip['lines']:
        f.write(line + '\n')
