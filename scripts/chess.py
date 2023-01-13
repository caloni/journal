import posts
import subprocess
import sys
import pyperclip
import os
import re

if len(sys.argv) > 1:
  baseName = sys.argv[1]
  link = None

  proc = subprocess.Popen("hugo --config themes/blog/config.toml new chess/{}/index.md".format(baseName), stdout=subprocess.PIPE, shell=True)
  proc.wait()
  os.rename("content/chess/{}".format(baseName), "content/posts/{}".format(baseName))
  path = "content/posts/{}/index.md".format(baseName)

  clip = pyperclip.paste().splitlines()

  def summary(clip):
    link = None
    pgn = [ '```' ]
    for c in clip:
      if len(c) and c[0] == '[':
        if c.find('[Event ') == 0 or c.find('[Site ') == 0 or c.find('[Date ') == 0 or c.find('[White ') == 0 or c.find('[Black ') == 0:
          pgn.append(c)
        if c.find('[Link') == 0:
          link = re.search('\[Link "(.*)"\]', c).group(1)
        continue
      pgn.append(c)
    pgn.append('```')
    return pgn, link

  pgn, link = summary(clip)

  def edit(post, fname):
    post['link'] = link
    post.content = '\n'.join(pgn)

  posts.transform([ edit ], [ path ])
  print("post written in", path)

  #with open("static/chess/{}.txt".format(baseName), "w", encoding="utf8") as f:
  #  def findLink(clip):
  #    for c in clip:
  #      srch = re.search('^\[Link "(.*)"\]', c)
  #      if srch:
  #        return srch.groups(1)[0]
  #  clip = pyperclip.paste().splitlines()
  #  f.writelines('\n'.join(clip[0:]))
  #  link = findLink(clip)

  #if link:
  #  def edit(post, fname):
  #    post['link'] = link
  #  posts.transform([ edit ], [ 'content/' + file ])

else:
  print('How to use: chess.py [slug]')

