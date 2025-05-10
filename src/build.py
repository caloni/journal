import os
import shutil
import subprocess

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')

scripts = [ 'test.py', 'journal2blog.py', 'journal2book.py' ]
for script in scripts:
  print('running', script)
  process = subprocess.run(['python', os.path.join(dname, script)], check=True)
  if process.stdout:
    print(process.stdout)
  if process.returncode:
    print(script, 'returned', process.returncode)

