import os
import subprocess

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')

process = subprocess.run(['python', r'code/Journal2Blog.py'], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('blogbuild.py returned', process.returncode)

process = subprocess.run(['git', 'add', '--all'], check=False)
process = subprocess.run(['git', 'commit', '-m', 'publishing some shit.'], check=False)
process = subprocess.run(['git', 'push'], check=False)

old_dir = os.getcwd()
os.chdir(r'public/blog')
process = subprocess.run(['git', 'add', '--all'], check=False)
process = subprocess.run(['git', 'commit', '-m', 'here comes great news.'], check=False)
process = subprocess.run(['git', 'push'], check=False)
os.chdir(old_dir)
