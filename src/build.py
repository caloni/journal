import os
import shutil
import subprocess

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')

process = subprocess.run(['python', os.path.join(dname, 'test.py')], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('test.py returned', process.returncode)

process = subprocess.run(['python', os.path.join(dname, 'journal2blog.py')], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('journal2blog.py returned', process.returncode)

process = subprocess.run(['python', os.path.join(dname, 'journal2book.py')], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('journal2book.py returned', process.returncode)

