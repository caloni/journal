import frontmatter
from glob import glob


path = "content/posts/**/*.md"
files = glob(path)
verbose = False


def transform(operations, files):
  for fname in files:
    if verbose:
      print(fname)
    with open(fname, 'r', encoding='utf8') as f:
      post = frontmatter.load(f)
      for op in operations:
        op(post, fname)
      post_str = frontmatter.dumps(post)
      f.close()
      with open(fname, 'w', newline='\n', encoding='utf8') as f:
        f.write(post_str)


def format(none):
  def nothing(post, fname):
    pass
  transform([ nothing ], files)


def add_item(post, collection, item):
  if post.get(collection):
    if item not in post[collection]:
      post[collection].append(item)
      return True
  else:
    post[collection] = [ item ]
    return True
  return False


def remove_item(post, collection, item):
  if post.get(collection) and item in post[collection]:
    post[collection].remove(item)
    return True
  return False


def move_category_to_tag(category):
  def func(post, fname):
    if remove_item(post, 'categories', category):
      add_item(post, 'tags', category)
  transform([ func ], files)


def move_tag_to_category(tag):
  def func(post, fname):
    if remove_item(post, 'tags', tag):
      add_item(post, 'categories', tag)
  transform([ func ], files)


def add_tag(tag):
  def func(post, fname):
    add_item(post, 'tags', tag)
  transform([ func ], files)


def add_category(category):
  def func(post, fname):
    add_item(post, 'categories', category)
  transform([ func ], files)


def remove_tag(tag):
  def func(post, fname):
    remove_item(post, 'tags', tag)
  transform([ func ], files)


def remove_category(category):
  def func(post, fname):
    remove_item(post, 'categories', category)
  transform([ func ], files)


def select(filter):
  global files, path

  if filter == '':
    for f in files:
      print(f)
    print(f'{len(files)} items')
  elif filter == '*':
    files = glob(path)
  else:
    newFiles = []
    def func(post, fname):
      terms = [ 'tags', 'categories' ]
      for t in terms:
        if post.get(t):
          if filter in post[t]:
            newFiles.append(fname)
            break
      return True
    transform([ func ], files)
    files = newFiles


def main():
  funcs = {
    'format': format, 
    'select': select, 
    'add_tag': add_tag,
    'add_category': add_category,
    'remove_tag': remove_tag,
    'remove_category': remove_category,
    'move_category_to_tag': move_category_to_tag,
    'move_tag_to_category': move_tag_to_category
  }

  while True:
    print("|".join(funcs))
    cmd = input()
    arg = input()
    funcs[cmd](arg)


if __name__ == "__main__":
  main()

