# Transform markdown text to parsed text.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2025-01-30
# Depends on: Util.

BEGIN {
  G_SETTINGS["generator"] = "MarkdownParser 0.2.0"
  G_SETTINGS["post_header_fields"] = "date link slug tags"
}

function FormatContent(line,    type)
{
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
      type = "pre"
      break
    }

    if( line ~ /^    / ) {
      sub(/^ /, "", line)
      if( ! ContentState[" "] ) {
        ContentState[" "] = 1
      }
      type = "pre"
      break
    } else if ( ContentState[" "] ) {
        ContentState[" "] = 0
    }

    if( line ~ /^ *- */ ) {
      line = gensub(/ *- *(.*)/, "\\1", "g", line)
      if( ! ContentState["-"] ) {
        ContentState["-"] = 1
      }
      type = "list"
      break
    } else if ( ContentState["-"] ) {
        ContentState["-"] = 0
    }

    if( line ~ /^>/ ) {
      sub(/^> ?/, "", line)
      type = "blockquote"
      break
    }

    if( line ~ /^#{2,6} / ) {

      if( line ~ /^## / ) {
        type = "h2"
      } else if( line ~ /^### / ) {
        type = "h3"
      } else if( line ~ /^#### / ) {
        type = "h4"
      } else if( line ~ /^##### / ) {
        type = "h5"
      } else if( line ~ /^###### / ) {
        type = "h6"
      }
      gsub(/^#+ /, "", line)
      break
    }

    if( match($0, /^!\[([^]]*)\]\( *([^" )]+) *"?([^"]*)?"?\)/, a) ) {
      NewPost["image"] = a[2]
      G_POSTS_IMAGES[a[2]] = a[2]
      line = a[2]
      type = "img"
      break
    }

    gsub(/&/, "&amp;", line)
    gsub(/</, "\\&lt;", line)
    gsub(/>/, "\\&gt;", line)
    type = "p"

  } while( 0 )

  NewPost["totalLines"] += 1
  NewPost["lines"][NewPost["totalLines"]]["content"] = line
  NewPost["lines"][NewPost["totalLines"]]["type"] = type
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
  G_POST_SLUG_BY_POSITION[++G_POSTS] = slug
  if( "link" in NewPost ) {
    NewPost["tags"] = NewPost["tags"] " blogging"
  }
  split(NewPost["tags"], tags)

  if( "date" in NewPost ) {
    NewPost["month"] = substr(NewPost["date"], 1, 7)
    G_MONTHS[NewPost["month"]] = NewPost["month"]
    G_DATE_SLUG_TITLE[NewPost["date"]][slug] = NewPost["title"]
    for( i in tags ) {
      G_SLUGS_BY_TAGS_AND_DATES[tags[i]][NewPost["date"]][slug] = slug
    }

    G_INDEX[slug]["month"] = NewPost["month"]
    G_INDEX[slug]["date"] = NewPost["date"]
  }

  G_INDEX[slug]["slug"] = slug
  G_INDEX[slug]["title"] = NewPost["title"]
  G_INDEX[slug]["link"] = NewPost["month"] ".html#" slug
  G_INDEX[slug]["letter"] = substr(NewPost["title"], 1, 1)
  G_INDEX[slug]["summary"] = NewPost["summary"]
  G_INDEX[slug]["tags"] = NewPost["tags"]
  G_INDEX[slug]["image"] = NewPost["image"]
  if( "link" in NewPost ) {
    G_INDEX[slug]["extlink"] = NewPost["link"]
  }
  if( "links" in NewPost ) {
    for( i in NewPost["links"] ) {
      G_INDEX[slug]["links"][i] = NewPost["links"][i]
    }
  }
  if( "lines" in NewPost ) {
    for( i in NewPost["lines"] ) {
      for( j in NewPost["lines"][i] ) {
        G_INDEX[slug]["lines"][i][j] = NewPost["lines"][i][j]
      }
    }
  }
  G_TITLE_TO_SLUG[NewPost["title"]] = slug
  FlushContentState(slug)
}

function PopulateTagsNavigation(    prevInTag, i, j, k, f)
{
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( i in G_SLUGS_BY_TAGS_AND_DATES ) {
    prevInTag = ""
    PROCINFO["sorted_in"] = "@ind_num_desc"
    for( j in G_SLUGS_BY_TAGS_AND_DATES[i] ) {
      for( k in G_SLUGS_BY_TAGS_AND_DATES[i][j] ) {
        if( prevInTag != "" ) {
          G_INDEX[k]["tag_nav"][i]["prev_in_tag"] = prevInTag
          G_INDEX[prevInTag]["tag_nav"][i]["next_in_tag"] = k
        }
        prevInTag = k
      }
    }
  }
}

$1 == "metadata_current_date" { G_SETTINGS["date"] = $2 ; next }
$1 == "metadata_chapter" { G_INDEX_METADATA[$2]["chapter"] = $3 ; G_INDEX_METADATA[$2]["explicit_slug"] = $4 ; next }
$1 == "metadata_tags" { next }

/^# / && !ContentState["```"] {
  if( "title" in NewPost ) {
    CopyNewPost()
  }
  NewPost["title"] = substr($0, 3)
  NewPost["slug"] = ToSlug(NewPost["title"])
  next
}

/[^!]\[[^]]+\]\([^)]+\)/ && !ContentState["```"] {
  if( match($0, /[^!]\[([^]]+)\]\(([^)]+)\)/, a) ) {
    if( !(a[2] ~ /^(https?)|(ftp)|(mailto):/) ) {
      sub("\\(" a[2] "\\)", "", $0)
      NewPost["links"][a[1]] = a[2]
      G_INDEX_METADATA[a[2]]["used"] += 1
    }
  }
}

/^\[[^]]+\]:/ && !ContentState["```"] {
  if( match($0, /^\[([^]]+)\]: *([^" ]+) *"?([^"]+)?"?/, a) ) {
    if( a[2] ~ /^(https?)|(ftp)|(mailto):/ ) {
      a[2] = "<a href=\"" a[2] "\">" a[1] "</a>"
    }
    else if( index(G_SETTINGS["post_header_fields"], a[1]) ) {
      NewPost[a[1]] = a[3]
    } else if( !(a[2] in G_INDEX_METADATA) ) {
      print "warning: link", a[2], "not found for name", a[1], "and title", a[3]
      print $0
    } else {
      G_INDEX_METADATA[a[2]]["used"] += 1
    }
    NewPost["links"][a[1]] = a[2]
    delete a
  } else {
    print "error: invalid link at", $0
  }
  next
}

/.+/ {
  G_NEW_POST_TOTAL_LINES = NewPost["totalLines"]
  FormatContent($0)
  if( NewPost["totalLines"] > G_NEW_POST_TOTAL_LINES ) {
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
  for( i in G_INDEX_METADATA ) {
    if( G_INDEX_METADATA[i]["explicit_slug"] && !("used" in G_INDEX_METADATA[i]) ) {
      print "warning: link", i, "not being used"
    }
  }
  PopulateTagsNavigation()
}
