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

  if( slug in Index ) {
    print "warning: slug", slug, "duplicated in", Index[slug]["date"], "and", date
  }
  Index[slug]["date"] = date
  Index[slug]["link"] = link
  print "metadata_slug", slug, link > Metadata["output"]

  delete NewPost
  NewPost["date"] = date
}


/^```/ {
  if( ContentState["```"] ) {
    ContentState["```"] = 0
  } else {
    ContentState["```"] = 1
  }
  next
}


/^# / && !ContentState["```"] {
  if( "title" in NewPost ) {
    FlushNewPost()
  }
  NewPost["title"] = substr($0, 3)
  NewPost["slug"] = ToSlug(NewPost["title"])
  next
}


/^\[[^]]+\]:/ && !ContentState["```"] {
  if( match($0, /^\[([^]]+)\]: *([^" ]+) *"?([^"]+)?"?/, a) ) {
    if( a[1] == "slug" || a[1] == "date" ) {
      NewPost[a[1]] = a[3]
    }
  }
  next
}

