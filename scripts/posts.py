import argparse
import sys

parser = argparse.ArgumentParser(description='Optional app description')
parser.add_argument('--command', type=str, help='Command. Type "--command help" to list commands.')
args = parser.parse_args()

if args.command == 'help':
  print("No command available. Just read and write blog.txt.")
  sys.exit(0);

posts = []
for line in open('blog.txt', encoding='utf8').readlines():
  if line[0] == '=':
    posts.append("")
  posts[-1] += line

with open('blog.txt', 'w', encoding='utf8') as file:
  bytesWritten = 0
  for post in posts:
    bytesWritten += file.write(post)
  print('written', bytesWritten, 'bytes')

