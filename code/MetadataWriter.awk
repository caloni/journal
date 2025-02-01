# Capture metadata from pseudo-markdown text to further processing.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2024-08-31

#include util.awk

BEGIN {
  Metadata["output"] = "public\\metadata.txt"
}


function FlushNewPost(    chapter)
{
  chapter = substr(NewPost["date"], 1, 7)

  if( NewPost["slug"] in Index ) {
    print "warning: slug", NewPost["slug"], "duplicated in", Index[slug]["date"], "and", NewPost["date"]
  }
  Index[NewPost["slug"]]["date"] = NewPost["date"]
  Index[NewPost["slug"]]["link"] = link
  print "metadata_chapter", NewPost["slug"], chapter, NewPost["explicit_slug"] > Metadata["output"]

  delete NewPost
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
    if( a[1] == "slug" ) {
      NewPost["explicit_slug"] = "explicit_slug"
    }
  }
  next
}

END {
  if( "title" in NewPost ) {
    FlushNewPost()
  }
}
