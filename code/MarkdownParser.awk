# Parse pseudo-markdown text to parsed text.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2025-01-04

#include util.awk

BEGIN {
  Settings["generator"] = "parser.awk 0.1.0"
  Settings["post_header_fields"] = "date link slug tags update"
}

function FormatContent(line, lastLine,    prefix, suffix, paragraph, newLine, headerLevel, endName, name, link)
{
  prefix = ""
  suffix = "\n"
  paragraph = 1
  newLine = 0
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
        prefix = prefix "<ul>"
        ContentState["-"] = 1
      }
      prefix = prefix "<li>"
      suffix = "</li>" suffix
      paragraph = 0
    } else if ( ContentState["-"] ) {
        prefix = "</ul>\n"
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

      prefix = prefix "<h" headerLevel ">"
      suffix = "</h" headerLevel ">" suffix
      paragraph = 0
    }

    if( match($0, /^!\[([^]]*)\]\( *([^" )]+) *"?([^"]*)?"?\)/, a) ) {
      NewPost["image"] = a[2]
      PostsImages[a[2]] = a[2]
      line = "<img src=\"img/" a[2] "\"/>"
      ContentType = "img"
      break
    }

    gsub(/&/, "&amp;", line)
    gsub(/</, "\\&lt;", line)
    gsub(/>/, "\\&gt;", line)
    line = gensub(/\[([^]]+)\]\(([^)]+)\)/, "<a href=\"\\2\">\\1</a>", "g", line)

    if( paragraph ) {
      line = "<p>" line "</p>"
      ContentType = "p"
    } else {
      ContentType = ""
    }

  } while( 0 )

  newLine = lastLine + 1
  NewPost["lines"][newLine]["content"] = prefix line suffix
  NewPost["lines"][newLine]["type"] = ContentType
  return newLine
}

function FlushContentState(slug,    lastLine)
{
  lastLine = length(NewPost["lines"])
  if ( ContentState["-"] ) {
    Index[slug]["lines"][lastLine]["content"] = Index[slug]["lines"][lastLine]["content"] "</ul>\n"
    ContentState["-"] = 0
  }
  delete ContentState
  delete NewPost
}

function CopyNewPost(    slug, tags, post, i, j)
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
$1 == "metadata_slug" { IndexMetadata[$2]["link"] = $3 ; next }

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
    else if( a[2] in IndexMetadata ) {
      a[2] = IndexMetadata[a[2]]["link"]
      a[2] = "<a href=\"" a[2] "\">" a[1] "</a>"
    }
    else if( index(Settings["post_header_fields"], a[1]) ) {
      NewPost[a[1]] = a[3]
    } else {
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
  i = FormatContent($0, NewPost["totalLines"])
  if( i ) {
    NewPost["totalLines"] = i
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
