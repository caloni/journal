import posts
import subprocess
import sys
import pyperclip
import os
import re

if len(sys.argv) > 1:
  baseName = sys.argv[1]
  file = "blog/{}.md".format(baseName)
  link = None

  proc = subprocess.Popen("hugo new chess/{}".format(file), stdout=subprocess.PIPE, shell=True)
  proc.wait()
  os.rename("content/chess/blog/{}.md".format(baseName), "content/blog/{}.md".format(baseName))

  with open("static/chess/{}.txt".format(baseName), "w", encoding="utf8") as f:
    def findLink(clip):
      for c in clip:
        srch = re.search('^\[Link "(.*)"\]', c)
        if srch:
          return srch.groups(1)[0]
    clip = pyperclip.paste().splitlines()
    f.writelines('\n'.join(clip[0:]))
    link = findLink(clip)

  if link:
    def edit(post, fname):
      post['link'] = link
    posts.transform([ edit ], [ 'content/' + file ])

else:
  print('How to use: chess.py [slug]')

