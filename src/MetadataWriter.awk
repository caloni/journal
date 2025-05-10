# Capture metadata from pseudo-markdown text to further processing.

BEGIN {
  G_METADATA["output"] = "public\\metadata.txt"
}

function MetadataWriter_GetChapterFromDate(date,    words, n, first)
{
  n = split(date, words, " ")
  first = words[1]

  # Check if the first word matches the date pattern YYYY-MM-DD
  if (first ~ /^[0-9]{4}-[0-9]{2}-[0-9]{2}$/)
    return substr(first, 1, 7)  # Extract and print 'YYYY-MM'
  else
    return first  # Print the first word as is
  # todo the first word is not being correctly formatted together
  # with months, need to check this
}


function MetadataWriter_FlushNewPost(    chapter)
{
  chapter = MetadataWriter_GetChapterFromDate(G_NEW_POST["date"])

  if( G_NEW_POST["slug"] in G_INDEX ) {
    print "warning: slug", G_NEW_POST["slug"], "duplicated in", G_INDEX[slug]["date"], "and", G_NEW_POST["date"]
  }
  G_INDEX[G_NEW_POST["slug"]]["date"] = G_NEW_POST["date"]
  G_INDEX[G_NEW_POST["slug"]]["link"] = link
  print "metadata_chapter", G_NEW_POST["slug"], chapter, G_NEW_POST["explicit_slug"] > G_METADATA["output"]

  delete G_NEW_POST
}

function MetadataWriter_FlushTags(    tags)
{
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( i in G_ALL_TAGS ) {
    tags = tags " " G_ALL_TAGS[i]
  }
  print "tags:" tags
  tags = "metadata_tags" tags
  print tags > G_METADATA["output"]
}

/^```/ {
  if( G_CONTENT_STATE["```"] ) {
    G_CONTENT_STATE["```"] = 0
  } else {
    G_CONTENT_STATE["```"] = 1
  }
  next
}

/^# / && !G_CONTENT_STATE["```"] {
  if( "title" in G_NEW_POST ) {
    MetadataWriter_FlushNewPost()
  }
  G_NEW_POST["title"] = substr($0, 3)
  G_NEW_POST["slug"] = Util_ToSlug(G_NEW_POST["title"])
  next
}

/^\[[^]]+\]:/ && !G_CONTENT_STATE["```"] {
  if( match($0, /^\[([^]]+)\]: *([^" ]+) *"?([^"]+)?"?/, a) ) {
    if( a[1] == "slug" || a[1] == "date" ) {
      G_NEW_POST[a[1]] = a[3]
    }
    else if( a[1] == "slug" ) {
      G_NEW_POST["explicit_slug"] = "explicit_slug"
    }
    else if( a[1] == "tags" ) {
      split(a[3], tags)
      for( i in tags ) {
        G_ALL_TAGS[tags[i]] = tags[i]
      }
    }
  }
  next
}

END {
  if( "title" in G_NEW_POST ) {
    MetadataWriter_FlushNewPost()
  }
  MetadataWriter_FlushTags()
}
