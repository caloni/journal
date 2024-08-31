# Capture metadata from pseudo-markdown text to further processing.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2024-08-31

#include util.awk

BEGIN {
  Metadata["output"] = "public\\metadata.txt"
}


function FlushNewPost(    slug, date, chapter, link)
{
  slug = NewPost["slug"]
  date = NewPost["date"]

  if( "draft" in NewPost ) {
    chapter = "drafts"
  } else {
    chapter = substr(date, 1, 7)
  }

  link = chapter ".html#" slug

  print "metadata_slug", slug, link > Metadata["output"]

  delete NewPost
  NewPost["date"] = date
}


/^= / {
  if( "title" in NewPost ) {
    FlushNewPost()
  }
  NewPost["title"] = substr($0, 3)
  NewPost["slug"] = ToSlug(NewPost["title"])
}

$1 == ":date:" { NewPost["date"] = $2 }
$1 == ":slug:" { NewPost["slug"] = $2 }
