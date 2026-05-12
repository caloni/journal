#!/usr/bin/env python3
"""
Wrapper to call what is needed to publish blog and book.
"""

import os
import shutil
import subprocess

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')

# Publish scripts (git operations, etc.)
publish_scripts = [ 'publish_blog.py', 'publish_book.py' ]
for script in publish_scripts:
  print('running', script)
  process = subprocess.run(['python', os.path.join(dname, script)], check=True)
  if process.stdout:
    print(process.stdout)
  if process.returncode:
    print(script, 'returned', process.returncode)
