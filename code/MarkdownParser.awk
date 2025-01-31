# Transform markdown text to parsed text.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2025-01-30
# Depends on: Util.

BEGIN {
  Settings["generator"] = "MarkdownParser 0.2.0"
  Settings["post_header_fields"] = "date link slug tags update"
}

function FormatContent(line,    prefix, suffix, paragraph, headerLevel, endName, name, link)
{
  prefix = ""
  suffix = ""
  paragraph = 1
  headerLevel = 0
  endName = 0
  name = ""
  link = ""

  do {
    if( index(line, "```") == 1 ) {
      line = ""
      if( ContentState["```"] ) {
        ContentState["```"] = 0
      } else {
        ContentState["```"] = 1
      }
      return 0
    } else if( ContentState["```"] ) {
      ContentType = "pre"
      break
    }

    if( line ~ /^    / ) {
      sub(/^ /, "", line)
      if( ! ContentState[" "] ) {
        ContentState[" "] = 1
      }
      ContentType = "pre"
      break
    } else if ( ContentState[" "] ) {
        ContentState[" "] = 0
    }

    if( line ~ /^ *- */ ) {
      line = gensub(/ *- *(.*)/, "\\1", "g", line)
      if( ! ContentState["-"] ) {
        ContentState["-"] = 1
      }
      suffix = ""
      paragraph = 0
      ContentType = "list"
      break
    } else if ( ContentState["-"] ) {
        ContentState["-"] = 0
    }

    if( line ~ /^>/ ) {
      sub(/^> ?/, "", line)
      ContentType = "blockquote"
      suffix = ""
      break
    }

    if( line ~ /^#{2,6} / ) {

      if( line ~ /^## / ) {
        headerLevel = 2
        ContentType = "h2"
      } else if( line ~ /^### / ) {
        headerLevel = 3
        ContentType = "h3"
      } else if( line ~ /^#### / ) {
        headerLevel = 4
        ContentType = "h4"
      } else if( line ~ /^##### / ) {
        headerLevel = 5
        ContentType = "h5"
      } else if( line ~ /^###### / ) {
        headerLevel = 6
        ContentType = "h6"
      }
      gsub(/^#+ /, "", line)

      paragraph = 0
      suffix = ""
      #todo remove this subs
      gsub(/&/, "&amp;", line)
      gsub(/</, "\\&lt;", line)
      gsub(/>/, "\\&gt;", line)
      break
    }

    if( match($0, /^!\[([^]]*)\]\( *([^" )]+) *"?([^"]*)?"?\)/, a) ) {
      NewPost["image"] = a[2]
      PostsImages[a[2]] = a[2]
      line = a[2]
      ContentType = "img"
      break
    }

    gsub(/&/, "&amp;", line)
    gsub(/</, "\\&lt;", line)
    gsub(/>/, "\\&gt;", line)
    line = gensub(/\[([^]]+)\]\(([^)]+)\)/, "<a href=\"\\2\">\\1</a>", "g", line)

    if( paragraph ) {
      ContentType = "p"
    } else {
      ContentType = ""
    }

  } while( 0 )

  NewPost["totalLines"] += 1
  NewPost["lines"][NewPost["totalLines"]]["content"] = prefix line suffix
  NewPost["lines"][NewPost["totalLines"]]["type"] = ContentType
}

function FlushContentState(slug,    lastLine)
{
  lastLine = length(NewPost["lines"])
  delete ContentState
  delete NewPost
}

function CopyNewPost(    slug, tags, i, j)
{
  slug = NewPost["slug"]
  PostSlugByPosition[++Posts] = slug
  if( "link" in NewPost ) {
    NewPost["tags"] = NewPost["tags"] " blogging"
  }
  split(NewPost["tags"], tags)

  if( "date" in NewPost ) {
    NewPost["month"] = substr(NewPost["date"], 1, 7)
    Months[NewPost["month"]] = NewPost["month"]
    DateSlugTitle[NewPost["date"]][slug] = NewPost["title"]
    for( i in tags ) {
      SlugsByTagsAndDates[tags[i]][NewPost["date"]][slug] = slug
    }

    Index[slug]["month"] = NewPost["month"]
    Index[slug]["date"] = NewPost["date"]
  }

  Index[slug]["slug"] = slug
  Index[slug]["title"] = NewPost["title"]
  Index[slug]["link"] = NewPost["month"] ".html#" slug
  Index[slug]["letter"] = substr(NewPost["title"], 1, 1)
  Index[slug]["summary"] = NewPost["summary"]
  Index[slug]["tags"] = NewPost["tags"]
  Index[slug]["image"] = NewPost["image"]
  if( "update" in NewPost ) {
    Index[slug]["update"] = NewPost["update"]
  }
  if( "link" in NewPost ) {
    Index[slug]["extlink"] = NewPost["link"]
  }
  if( "links" in NewPost ) {
    for( i in NewPost["links"] ) {
      Index[slug]["links"][i] = NewPost["links"][i]
    }
  }
  if( "lines" in NewPost ) {
    for( i in NewPost["lines"] ) {
      for( j in NewPost["lines"][i] ) {
        Index[slug]["lines"][i][j] = NewPost["lines"][i][j]
      }
    }
  }
  TitleToSlug[NewPost["title"]] = slug
  FlushContentState(slug)
}

$1 == "metadata_current_date" { Settings["date"] = $2 ; next }
$1 == "metadata_chapter" { IndexMetadata[$2]["chapter"] = $3 ; next }

/^# / && !ContentState["```"] {
  if( "title" in NewPost ) {
    CopyNewPost()
  }
  NewPost["title"] = substr($0, 3)
  NewPost["slug"] = ToSlug(NewPost["title"])
  next
}

/^\[[^]]+\]:/ && !ContentState["```"] {
  if( match($0, /^\[([^]]+)\]: *([^" ]+) *"?([^"]+)?"?/, a) ) {
    if( a[2] ~ /^(https?)|(ftp)|(mailto):/ ) {
      a[2] = "<a href=\"" a[2] "\">" a[1] "</a>"
    }
    else if( index(Settings["post_header_fields"], a[1]) ) {
      NewPost[a[1]] = a[3]
    } else if( !(a[2] in IndexMetadata) ) {
      print "warning: link", a[2], "not found for name", a[1], "and title", a[3]
      print $0
      a[2] = gensub(/(.*)/, "posts.html?q=\\1", "g", a[2])
      a[2] = "<a href=\"" a[2] "\">" a[1] "</a>"
    }
    NewPost["links"][a[1]] = a[2]
    delete a
  } else {
    print "error: invalid link at", $0
  }
  next
}

/.+/ {
  NewPostTotalLines = NewPost["totalLines"]
  FormatContent($0)
  if( NewPost["totalLines"] > NewPostTotalLines ) {
    if( length(NewPost["summary"]) < 200 ) {
      if( index($0, "{{") == 0 && index($0, "```") == 0 ) {
        NewPost["summary"] = NewPost["summary"] " " $0
      }
    }
  }
}

END {
  if( "title" in NewPost ) {
    CopyNewPost()
  }
}
