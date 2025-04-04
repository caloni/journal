import os
import shutil
import subprocess

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')
print('basedir:', os.getcwd())

old_dir = os.getcwd()

if not os.path.exists(r'public/blog'):
  os.makedirs(r'public/blog')
os.chdir(r'public/blog')

for filename in os.listdir('.'):
  try:
    if os.path.isfile(filename) or os.path.islink(filename):
      os.unlink(filename)
    elif os.path.isdir(filename) and filename != '.git':
      shutil.rmtree(filename)
  except Exception as e:
    print('Failed to delete %s. Reason: %s' % (filename, e))

os.chdir(old_dir)

shutil.copytree(r'blog', r'public/blog', dirs_exist_ok=True)
shutil.copytree(r'img/blog', r'public/blog/img', dirs_exist_ok=True)

os.environ['LC_ALL'] = 'en_US.UTF-8'

process = subprocess.run(['gawk', '-f', r'code/Util.awk', '-f', r'code/MetadataWriter.awk', 'journal.md'], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('MetadataWriter.awk returned', process.returncode)

process = subprocess.run(['gawk', '-f', r'code/Util.awk', '-f', r'code/MarkdownParser.awk', '-f', r'code/BlogWriter.awk', r'public/metadata.txt', 'journal.md'], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('txt2blog.awk returned', process.returncode)

print('blog generated')
