import os
import shutil
import subprocess

old_dir = os.getcwd()

if not os.path.exists(r'public/book'):
  os.makedirs(r'public/book')
os.chdir(r'public/book')

for filename in os.listdir('.'):
  try:
    if os.path.isfile(filename) or os.path.islink(filename):
      os.unlink(filename)
    elif os.path.isdir(filename) and filename != '.git':
      shutil.rmtree(filename)
  except Exception as e:
    print('Failed to delete %s. Reason: %s' % (filename, e))

os.chdir(old_dir)

shutil.copytree(r'book', r'public/book', dirs_exist_ok=True)
shutil.copytree(r'img/journal', r'public/book/EPUB/img', dirs_exist_ok=True)
shutil.copytree(r'img/blog', r'public/book/EPUB/img', dirs_exist_ok=True)
shutil.copytree(r'img/journal', r'public/book/MOBI/img', dirs_exist_ok=True)
shutil.copytree(r'img/blog', r'public/book/MOBI/img', dirs_exist_ok=True)

os.environ['LC_ALL'] = 'en_US.UTF-8'

process = subprocess.run(['gawk', '-f', r'scripts/txt2epub.awk', 'journal.txt'], check=True)
process = subprocess.run(['gawk', '-f', r'scripts/txt2mobi.awk', 'journal.txt'], check=True)

os.chdir(r'public/book')
process = subprocess.run(['python', r'repack.py'], check=True)
os.chdir(old_dir)

print('do you want to convert and copy to kindle? (Y/N)')
publish_answer = input()
if 'yYsS1yessim'.find(publish_answer) != -1:
  os.chdir(r'public/book')
  process = subprocess.run(['kindlegen', 'caloni.epub', '-o', r'caloni.mobi'], check=False)
  if os.path.exists(r'k:/documents'):
    shutil.copy2(r'caloni.mobi', r'k:/documents/caloni.mobi', exist_ok=True)
    print('book copied successfully')
  os.chdir(old_dir)
