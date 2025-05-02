import os
import subprocess
import shutil
from datetime import datetime

backup = True
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')

if( backup ):
  backup_dir = os.path.join('..', 'backup')
  if not os.path.exists(backup_dir):
    os.makedirs(backup_dir)
  backup_path = os.path.join(backup_dir, 'journal.zip')
  print(f'creating backup at {backup_path}')
  shutil.make_archive(
    os.path.join(backup_dir, 'journal'),
    'zip',
    os.getcwd())
  print(f'backup created')

process = subprocess.run(['python', os.path.join(dname, 'Journal2Blog.py')], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('blogbuild.py returned', process.returncode)

process = subprocess.run(['git', 'add', '--all'], check=False)
process = subprocess.run(['git', 'commit', '-m', 'Add journal changes'], check=False)
process = subprocess.run(['git', 'push'], check=False)

old_dir = os.getcwd()
os.chdir(r'public/blog')
process = subprocess.run(['git', 'add', '--all'], check=False)
process = subprocess.run(['git', 'commit', '-m', 'Publish lastest posts'], check=False)
process = subprocess.run(['git', 'push'], check=False)
os.chdir(old_dir)
