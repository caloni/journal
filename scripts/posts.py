import argparse

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

