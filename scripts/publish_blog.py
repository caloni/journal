#!/usr/bin/env python3
"""
Wrapper to call what is needed to publish blog (and journal).
Changes are uploaded to remote server as final and this
repo.
"""

import os
import subprocess
import shutil
from datetime import datetime
import tempfile

def ignore_git(dir, files):
    return ['.git'] if '.git' in files else []

private = False # it is not a submodule anymore
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')

# Generate blog and journal
print('Generating blog and journal...')
scripts = [ 'journal2blog.py', 'journal2journal.py' ]
for script in scripts:
  print('running', script)
  process = subprocess.run(['python', os.path.join(dname, script)], check=True)
  if process.stdout:
    print(process.stdout)
  if process.returncode:
    print(script, 'returned', process.returncode)

if private:
  old_dir = os.getcwd()
  os.chdir(r'private')
  process = subprocess.run(['git', 'add', '--all'], check=False)
  process = subprocess.run(['git', 'commit', '-m', 'Add journal changes'], check=False)
  process = subprocess.run(['git', 'push'], check=False)
  os.chdir(old_dir)

process = subprocess.run(['git', 'add', '--all'], check=False)
process = subprocess.run(['git', 'commit', '-m', 'Add journal changes'], check=False)
process = subprocess.run(['git', 'push'], check=False)

old_dir = os.getcwd()
os.chdir(r'publish/output/caloni.github.io')
process = subprocess.run(['git', 'add', '--all'], check=False)
process = subprocess.run(['git', 'commit', '-m', 'Publish changes'], check=False)
process = subprocess.run(['git', 'push'], check=False)
os.chdir(old_dir)
