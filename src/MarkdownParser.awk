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
      if( G_CONTENT_STATE["```"] ) {
        G_CONTENT_STATE["```"] = 0
      } else {
        G_CONTENT_STATE["```"] = 1
      }
      return 0
    } else if( G_CONTENT_STATE["```"] ) {
      type = "pre"
      break
    }

    if( line ~ /^    / ) {
      sub(/^ /, "", line)
      if( ! G_CONTENT_STATE[" "] ) {
        G_CONTENT_STATE[" "] = 1
      }
      type = "pre"
      break
    } else if ( G_CONTENT_STATE[" "] ) {
        G_CONTENT_STATE[" "] = 0
    }

    if( line ~ /^ *- */ ) {
      line = gensub(/ *- *(.*)/, "\\1", "g", line)
      if( ! G_CONTENT_STATE["-"] ) {
        G_CONTENT_STATE["-"] = 1
      }
      type = "list"
      break
    } else if ( G_CONTENT_STATE["-"] ) {
        G_CONTENT_STATE["-"] = 0
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
      G_NEW_POST["image"] = a[2]
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

  G_NEW_POST["totalLines"] += 1
  G_NEW_POST["lines"][G_NEW_POST["totalLines"]]["content"] = line
  G_NEW_POST["lines"][G_NEW_POST["totalLines"]]["type"] = type
}

function FlushContentState(slug,    lastLine)
{
  lastLine = length(G_NEW_POST["lines"])
  delete G_CONTENT_STATE
  delete G_NEW_POST
}

function CopyNewPost(    slug, tags, i, j)
{
  slug = G_NEW_POST["slug"]
  G_POST_SLUG_BY_POSITION[++G_POSTS] = slug
  if( "link" in G_NEW_POST ) {
    G_NEW_POST["tags"] = G_NEW_POST["tags"] " blogging"
  }
  split(G_NEW_POST["tags"], tags)

  if( "date" in G_NEW_POST ) {
    G_NEW_POST["month"] = substr(G_NEW_POST["date"], 1, 7)
    G_MONTHS[G_NEW_POST["month"]] = G_NEW_POST["month"]
    G_DATE_SLUG_TITLE[G_NEW_POST["date"]][slug] = G_NEW_POST["title"]
    for( i in tags ) {
      G_SLUGS_BY_TAGS_AND_DATES[tags[i]][G_NEW_POST["date"]][slug] = slug
    }

    G_INDEX[slug]["month"] = G_NEW_POST["month"]
    G_INDEX[slug]["date"] = G_NEW_POST["date"]
  }

  G_INDEX[slug]["slug"] = slug
  G_INDEX[slug]["title"] = G_NEW_POST["title"]
  G_INDEX[slug]["link"] = G_NEW_POST["month"] ".html#" slug
  G_INDEX[slug]["letter"] = substr(G_NEW_POST["title"], 1, 1)
  G_INDEX[slug]["summary"] = G_NEW_POST["summary"]
  G_INDEX[slug]["tags"] = G_NEW_POST["tags"]
  G_INDEX[slug]["image"] = G_NEW_POST["image"]
  if( "link" in G_NEW_POST ) {
    G_INDEX[slug]["extlink"] = G_NEW_POST["link"]
  }
  if( "links" in G_NEW_POST ) {
    for( i in G_NEW_POST["links"] ) {
      G_INDEX[slug]["links"][i] = G_NEW_POST["links"][i]
    }
  }
  if( "lines" in G_NEW_POST ) {
    for( i in G_NEW_POST["lines"] ) {
      for( j in G_NEW_POST["lines"][i] ) {
        G_INDEX[slug]["lines"][i][j] = G_NEW_POST["lines"][i][j]
      }
    }
  }
  G_TITLE_TO_SLUG[G_NEW_POST["title"]] = slug
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

/^# / && !G_CONTENT_STATE["```"] {
  if( "title" in G_NEW_POST ) {
    CopyNewPost()
  }
  G_NEW_POST["title"] = substr($0, 3)
  G_NEW_POST["slug"] = ToSlug(G_NEW_POST["title"])
  next
}

/[^!]\[[^]]+\]\([^)]+\)/ && !G_CONTENT_STATE["```"] {
  if( match($0, /[^!]\[([^]]+)\]\(([^)]+)\)/, a) ) {
    if( !(a[2] ~ /^(https?)|(ftp)|(mailto):/) ) {
      sub("\\(" a[2] "\\)", "", $0)
      G_NEW_POST["links"][a[1]] = a[2]
      G_INDEX_METADATA[a[2]]["used"] += 1
    }
  }
}

/^\[[^]]+\]:/ && !G_CONTENT_STATE["```"] {
  if( match($0, /^\[([^]]+)\]: *([^" ]+) *"?([^"]+)?"?/, a) ) {
    if( a[2] ~ /^(https?)|(ftp)|(mailto):/ ) {
      a[2] = "<a href=\"" a[2] "\">" a[1] "</a>"
    }
    else if( index(G_SETTINGS["post_header_fields"], a[1]) ) {
      G_NEW_POST[a[1]] = a[3]
    } else if( !(a[2] in G_INDEX_METADATA) ) {
      print "warning: link", a[2], "not found for name", a[1], "and title", a[3]
      print $0
    } else {
      G_INDEX_METADATA[a[2]]["used"] += 1
    }
    G_NEW_POST["links"][a[1]] = a[2]
    delete a
  } else {
    print "error: invalid link at", $0
  }
  next
}

/.+/ {
  G_NEW_POST_TOTAL_LINES = G_NEW_POST["totalLines"]
  FormatContent($0)
  if( G_NEW_POST["totalLines"] > G_NEW_POST_TOTAL_LINES ) {
    if( length(G_NEW_POST["summary"]) < 200 ) {
      if( index($0, "{{") == 0 && index($0, "```") == 0 ) {
        G_NEW_POST["summary"] = G_NEW_POST["summary"] " " $0
      }
    }
  }
}

END {
  if( "title" in G_NEW_POST ) {
    CopyNewPost()
  }
  for( i in G_INDEX_METADATA ) {
    if( G_INDEX_METADATA[i]["explicit_slug"] && !("used" in G_INDEX_METADATA[i]) ) {
      print "warning: link", i, "not being used"
    }
  }
  PopulateTagsNavigation()
}
