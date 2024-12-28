import argparse
import sys

parser = argparse.ArgumentParser(description='Optional app description')
parser.add_argument('--command', type=str, help='Command. Type "--command help" to list commands.')
parser.add_argument('--file', type=str, help='Input (and output) file (default blog.txt).', default='blog.txt')
args = parser.parse_args()

if args.command == 'help':
  print("No command available. Just read and write blog.txt.")
  sys.exit(0);

posts = []
for line in open(args.file, encoding='utf8').readlines():
  if line[0] == '=':
    posts.append("")
  posts[-1] += line

with open(args.file, 'w', encoding='utf8') as file:
  bytesWritten = 0
  for post in posts:
    bytesWritten += file.write(post)
  print('written', bytesWritten, 'bytes to', args.file)

