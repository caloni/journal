# Capture metadata from pseudo-markdown text to further processing.

# The metadata.txt file will be used in further scripts to gather
# location of posts and other information about texts, such as
# all tags or current date.
#
# TODO Keep a source controlled metadata.txt with specific details
# for the publishing system, such as author name, blog title and so
# on, removing these details from the scripts.
BEGIN {
  G_METADATA["output"] = "public\\metadata.txt"
}

# Check if the first word matches the date pattern YYYY-MM-DD.
# Extract and print 'YYYY-MM' or print the first word as is.
#
# TODO The first word is not being correctly formatted together
# with months in Writers, need to check this.
function MetadataWriter_GetChapterFromDateField(a_date,    l_words, l_first)
{
  split(a_date, l_words, " ")
  l_first = l_words[1]
  if (l_first ~ /^[0-9]{4}-[0-9]{2}-[0-9]{2}$/)
  {
    return substr(l_first, 1, 7)
  }
  else
  {
    return l_first
  }
}

# Deletes G_NEW_POST completely after use.
function MetadataWriter_FlushNewPostToMetadataOutput(    l_chapter)
{
  l_chapter = MetadataWriter_GetChapterFromDateField(G_NEW_POST["date"])
  if( G_NEW_POST["slug"] in G_INDEX )
  {
    print "warning: slug", G_NEW_POST["slug"], "duplicated in", G_INDEX[slug]["date"], "and", G_NEW_POST["date"]
  }
  G_INDEX[G_NEW_POST["slug"]]["date"] = G_NEW_POST["date"]
  G_INDEX[G_NEW_POST["slug"]]["link"] = link
  print "metadata_chapter", G_NEW_POST["slug"], l_chapter, G_NEW_POST["explicit_slug"] > G_METADATA["output"]
  delete G_NEW_POST
}

# Flushes all captured tags from the posts.
function MetadataWriter_FlushTags(    l_tags)
{
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( i in G_ALL_TAGS )
  {
    l_tags = l_tags " " G_ALL_TAGS[i]
  }
  print "tags:" l_tags
  l_tags = "metadata_tags" l_tags
  print l_tags > G_METADATA["output"]
}

# toggle preformatted content
/^```/ {
  G_CONTENT_STATE["```"] = !G_CONTENT_STATE["```"]
  next
}

# skip preformatted content
G_CONTENT_STATE["```"] {
  next
}

# if it is a md title
/^# / {
  if( "title" in G_NEW_POST )
  {
    # flush previous post
    MetadataWriter_FlushNewPostToMetadataOutput()
  }
  G_NEW_POST["title"] = substr($0, 3)
  G_NEW_POST["slug"] = Util_ToSlug(G_NEW_POST["title"])
  next
}

# if it is a custom field
/^\[[^]]+\]:/ {
  if( match($0, /^\[([^]]+)\]: *([^" ]+) *"?([^"]+)?"?/, l_array) )
  {
    if( l_array[1] == "slug" || l_array[1] == "date" )
    {
      G_NEW_POST[l_array[1]] = l_array[3]
    }
    else if( l_array[1] == "slug" )
    {
      G_NEW_POST["explicit_slug"] = "explicit_slug"
    }
    else if( l_array[1] == "tags" )
    {
      split(l_array[3], tags)
      for( i in tags )
      {
        G_ALL_TAGS[tags[i]] = tags[i]
      }
    }
    delete l_array
  }
  next
}

# Flushes last post from the file.
# Flushes all captured tags from the posts.
# Delete global variables to not mixed up with further scripts.
# Maintain global variables that should be used in further scripts.
END {
  if( "title" in G_NEW_POST )
  {
    MetadataWriter_FlushNewPostToMetadataOutput()
  }
  MetadataWriter_FlushTags()
  delete G_NEW_POST
  delete G_CONTENT_STATE
  delete G_METADATA
  delete G_ALL_TAGS
  # not delete G_INDEX
}
