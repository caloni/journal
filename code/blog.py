import os
import subprocess

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)
os.chdir('..')
print(os.getcwd())

print('do you want to publish the PUBLIC blog to the world wide web? (Y/N)')
publish_answer = input()

process = subprocess.run(['python', r'code/blogbuild.py'], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('blogbuild.py returned', process.returncode)

process = subprocess.run(['git', 'add', '--all'], check=False)
process = subprocess.run(['git', 'commit', '-m', 'publishing some shit.'], check=False)
process = subprocess.run(['git', 'push'], check=False)

if 'yYsS1yessim'.find(publish_answer) != -1:
  old_dir = os.getcwd()
  os.chdir(r'public/blog')
  process = subprocess.run(['git', 'add', '--all'], check=False)
  process = subprocess.run(['git', 'commit', '-m', 'here comes great news.'], check=False)
  process = subprocess.run(['git', 'push'], check=False)
  os.chdir(old_dir)

