import posts
import subprocess
import sys
import pyperclip

if len(sys.argv) > 1:
  proc = subprocess.Popen("hugo --config themes/blog/config.toml new posts/{}/index.md".format(sys.argv[1]), stdout=subprocess.PIPE, shell=True)
  proc.wait()
  clip = pyperclip.paste().splitlines()
  def edit(post, fname):
    post['title'] = clip[0]
    post.content = '\n'.join(clip[2:])
  path = 'content/posts/' + sys.argv[1] + '/index.md'
  posts.transform([ edit ], [ path ])
  print('post written in', path)
else:
  print('How to use: new.py [slug]')

