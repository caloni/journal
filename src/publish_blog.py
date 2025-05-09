import os
import subprocess
import shutil
from datetime import datetime
import tempfile

def ignore_git(dir, files):
    return ['.git'] if '.git' in files else []

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
    
    # Create a temporary directory for the filtered content
    with tempfile.TemporaryDirectory() as temp_dir:
        # Copy files to temp directory, ignoring .git
        shutil.copytree(os.getcwd(), temp_dir, ignore=ignore_git, dirs_exist_ok=True)
        # Create zip from temp directory
        shutil.make_archive(
            os.path.join(backup_dir, 'journal'),
            'zip',
            temp_dir)
    print(f'backup created at {backup_path}')

process = subprocess.run(['python', os.path.join(dname, 'journal2blog.py')], check=True)
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
