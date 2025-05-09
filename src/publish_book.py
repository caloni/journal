import os
import shutil
import subprocess

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')

process = subprocess.run(['python', os.path.join(dname, 'journal2book.py')], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('blogbuild.py returned', process.returncode)

old_dir = os.getcwd()
os.chdir(r'public/book')
process = subprocess.run(['kindlegen', 'caloni.epub', '-o', r'caloni.mobi'], check=False)
if os.path.exists(r'k:/documents'):
  shutil.copy(r'caloni.mobi', r'k:/documents/caloni.mobi')
  print('book published successfully')
os.chdir(old_dir)
