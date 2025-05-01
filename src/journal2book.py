import datetime
import os
import shutil
import subprocess

private = True
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')
print('basedir:', os.getcwd())

now = datetime.datetime.now()
current_date = now.astimezone().strftime('%Y-%m-%dT%H:%M:%S%z')
build_version = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode('utf-8').strip()
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
shutil.copytree(r'img/book', r'public/book/EPUB/img', dirs_exist_ok=True)
shutil.copytree(r'img/blog', r'public/book/EPUB/img', dirs_exist_ok=True)

os.environ['LC_ALL'] = 'en_US.UTF-8'

command = ['gawk', '-f', os.path.join(dname, 'Util.awk'), '-f', os.path.join(dname, 'MetadataWriter.awk'), 'journal.md']
if private:
  command.append('private/journal.md')
process = subprocess.run(command, check=True)
with open(r'public/metadata.txt', 'ab') as f:
    f.write(('metadata_current_date ' + current_date + '\n').encode('utf-8'))
    f.write(('metadata_build_version ' + build_version + '\n').encode('utf-8'))
command = ['gawk', '-f', os.path.join(dname, 'Util.awk'), '-f', os.path.join(dname, 'MarkdownParser.awk'), '-f', os.path.join(dname, 'BookWriter.awk'), r'public/metadata.txt', 'journal.md']
if private:
  command.append('private/journal.md')
process = subprocess.run(command, check=True)

os.chdir(r'public/book')
process = subprocess.run(['python', r'repack.py'], check=True)
os.chdir(old_dir)
