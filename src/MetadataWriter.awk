# Capture metadata from pseudo-markdown text to further processing.
#
# The G_INDEX array will contain fields "date" and "link" indexed
# by slug and passed between scripts.
#
# The G_METADATA array will contain fields "chaptersBySlug" and
# "tags".

FNR == 1 {
  # if a file with posts has private in its path then it is a private note
  if( (index(FILENAME, "private")) > 0 )
  {
    G_METADATA["tags"]["private"] = "private"
  }
}

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

# Copy date and link to G_INDEX, chapter to G_METADATA.
# Deletes G_NEW_POST completely after use.
function MetadataWriter_CopyNewPost(    l_slug, l_chapter)
{
  l_slug = G_NEW_POST["slug"]
  l_chapter = MetadataWriter_GetChapterFromDateField(G_NEW_POST["date"])
  if( l_slug in G_INDEX )
  {
    print "warning: slug", l_slug, "duplicated in", G_INDEX[l_slug]["date"], "and", G_NEW_POST["date"]
  }
  G_INDEX[l_slug]["date"] = G_NEW_POST["date"]
  G_INDEX[l_slug]["link"] = link
  G_METADATA["chaptersBySlug"][l_slug] = l_chapter
  split("", G_NEW_POST)
}

function MetadataWriter_Flush(    l_slug, l_tag, l_tags)
{
  for( l_slug in G_METADATA["chaptersBySlug"] )
  {
    print "metadata_chapter", l_slug, G_METADATA["chaptersBySlug"][l_slug] > G_METADATA["output"]
  }
  for( l_tag in G_METADATA["tags"] )
  {
    l_tag = l_tags " " l_tag
  }
  print "metadata_tags", l_tags > G_METADATA["output"]
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
    MetadataWriter_CopyNewPost()
  }
  G_NEW_POST["title"] = substr($0, 3)
  G_NEW_POST["slug"] = Util_TitleToSlug(G_NEW_POST["title"])
  next
}

# if it is a custom field
/^\[[^]]+\]:/ {
  if( match($0, /^\[([^]]+)\]: *([^" ]+) *"?([^"]+)?"?/, l_array) )
  {
    if( l_array[1] == "date" || l_array[1] == "slug" )
    {
      G_NEW_POST[l_array[1]] = l_array[3]
    }
    else if( l_array[1] == "tags" )
    {
      split(l_array[3], tags)
      for( i in tags )
      {
        G_METADATA["tags"][tags[i]] = tags[i]
      }
    }
    # if a post has external link then it is about blogging
    else if( l_array[1] == "link" )
    {
      G_METADATA["tags"]["blogging"] = "blogging"
    }
    split("", l_array)
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
    MetadataWriter_CopyNewPost()
  }
  MetadataWriter_Flush()
  # cleanup
  split("", G_NEW_POST)
  split("", G_CONTENT_STATE)
  split("", G_METADATA)
  # not clean G_INDEX
}
