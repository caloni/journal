import shutil
import os

src = r'k:\documents\My Clippings.txt'
dst = r'C:\Users\caloni\blog\static\txt\clippings.txt'

if os.path.exists(src):
  shutil.move(src, dst)
  print('clippings updated')
else:
  print('clippings NOT updated')

# todo: parse clippings to clip lines
# todo: transform clip lines to vim so file and save it
