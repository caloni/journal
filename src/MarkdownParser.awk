# Transform markdown text to parsed text.
#
# The following information is available after parsing:
#
# - G_DATE_SLUG_TITLE post title indexed by date and slug
# - G_INDEX index of all posts indexed by slug
# - G_INDEX_METADATA info read in metadata file
# - G_MONTHS months with posts
# - G_POSTS_IMAGES available image in posts
# - G_POST_SLUG_BY_POSITION post slugs in appearance order
# - G_SETTINGS global settings
# - G_SLUGS_BY_TAGS_AND_DATES slugs indexed by post tags and date
# - G_TITLE_TO_SLUG slugs indexed by post title
# 
# G_INDEX contains most of post information indexed by slug such as:
# 
# - title from post header (the # level 1 from markdown)
# - slug from header fields or title if inexistent
# - date from header fields
# - tags from header fields
# - extlink, external link from header fields (the old blogging)
# - link, internal formed by month and slug
# - month based on date
# - letter that is the first letter of title
# - summary that are the first words from post
# - image, last one for the post
# - links from post text, internal or external
# - lines from post text
# - tag_nav tags inside navigation, with:
# - prev_in_tag previous post for same tag and
# - next_in_tag next post for same tag

# TODO get hardcoded personal info from metadata file
BEGIN {
  G_SETTINGS["generator"] = "https://github.com/caloni/journal"
  G_SETTINGS["public_repo_github_address"] = "https://github.com/caloni/journal"
  G_SETTINGS["post_header_fields"] = "date link slug tags"
}

# Receives current line, extract type information,
# remove some format (lists) and update G_NEW_POST
# with new line if available and G_CONTENT_STATE with
# current state.
function MarkdownParser_FormatContent(a_line,    l_type)
{
  do
  {
    if( index(a_line, "```") == 1 )
    {
      a_line = ""
      if( G_CONTENT_STATE["```"] )
      {
        G_CONTENT_STATE["```"] = 0
      }
      else
      {
        G_CONTENT_STATE["```"] = 1
      }
      return 0
    }
    else if( G_CONTENT_STATE["```"] )
    {
      l_type = "pre"
      break
    }

    if( a_line ~ /^    / )
    {
      sub(/^ /, "", a_line)
      if( ! G_CONTENT_STATE[" "] )
      {
        G_CONTENT_STATE[" "] = 1
      }
      l_type = "pre"
      break
    }
    else if ( G_CONTENT_STATE[" "] )
    {
        G_CONTENT_STATE[" "] = 0
    }

    if( a_line ~ /^ *- */ )
    {
      a_line = gensub(/ *- *(.*)/, "\\1", "g", a_line)
      if( ! G_CONTENT_STATE["-"] )
      {
        G_CONTENT_STATE["-"] = 1
      }
      l_type = "list"
      break
    }
    else if ( G_CONTENT_STATE["-"] )
    {
        G_CONTENT_STATE["-"] = 0
    }

    if( a_line ~ /^>/ )
    {
      sub(/^> ?/, "", a_line)
      l_type = "blockquote"
      break
    }

    if( a_line ~ /^#{2,6} / )
    {
      if( a_line ~ /^## / )
      {
        l_type = "h2"
      }
      else if( a_line ~ /^### / )
      {
        l_type = "h3"
      }
      else if( a_line ~ /^#### / )
      {
        l_type = "h4"
      }
      else if( a_line ~ /^##### / )
      {
        l_type = "h5"
      }
      else if( a_line ~ /^###### / )
      {
        l_type = "h6"
      }
      gsub(/^#+ /, "", a_line)
      break
    }

    if( match($0, /^!\[([^]]*)\]\( *([^" )]+) *"?([^"]*)?"?\)/, a) )
    {
      G_NEW_POST["image"] = a[2]
      G_POSTS_IMAGES[a[2]] = a[2]
      a_line = a[2]
      l_type = "img"
      break
    }

    gsub(/&/, "&amp;", a_line)
    gsub(/</, "\\&lt;", a_line)
    gsub(/>/, "\\&gt;", a_line)
    l_type = "p"
  }
  while( 0 )

  G_NEW_POST["totalLines"] += 1
  G_NEW_POST["lines"][G_NEW_POST["totalLines"]]["content"] = a_line
  G_NEW_POST["lines"][G_NEW_POST["totalLines"]]["type"] = l_type
}

# Flush current post, clean post state and info
function MarkdownParser_CopyNewPost(    l_slug, l_tags, l_key, l_key2)
{
  l_slug = G_NEW_POST["slug"]
  G_POST_SLUG_BY_POSITION[++G_SETTINGS["totalPosts"]] = l_slug
  if( "link" in G_NEW_POST )
  {
    G_NEW_POST["tags"] = G_NEW_POST["tags"] " blogging"
  }
  split(G_NEW_POST["tags"], l_tags)

  if( "date" in G_NEW_POST )
  {
    G_NEW_POST["month"] = substr(G_NEW_POST["date"], 1, 7)
    G_MONTHS[G_NEW_POST["month"]] = G_NEW_POST["month"]
    G_DATE_SLUG_TITLE[G_NEW_POST["date"]][l_slug] = G_NEW_POST["title"]
    for( l_key in l_tags )
    {
      G_SLUGS_BY_TAGS_AND_DATES[l_tags[l_key]][G_NEW_POST["date"]][l_slug] = l_slug
    }
    G_INDEX[l_slug]["month"] = G_NEW_POST["month"]
    G_INDEX[l_slug]["date"] = G_NEW_POST["date"]
  }

  G_INDEX[l_slug]["slug"] = l_slug
  G_INDEX[l_slug]["title"] = G_NEW_POST["title"]
  G_INDEX[l_slug]["link"] = G_NEW_POST["month"] ".html#" l_slug
  G_INDEX[l_slug]["letter"] = substr(G_NEW_POST["title"], 1, 1)
  G_INDEX[l_slug]["summary"] = G_NEW_POST["summary"]
  G_INDEX[l_slug]["tags"] = G_NEW_POST["tags"]
  G_INDEX[l_slug]["image"] = G_NEW_POST["image"]
  if( "link" in G_NEW_POST )
  {
    G_INDEX[l_slug]["extlink"] = G_NEW_POST["link"]
  }
  if( "links" in G_NEW_POST )
  {
    for( l_key in G_NEW_POST["links"] )
    {
      G_INDEX[l_slug]["links"][l_key] = G_NEW_POST["links"][l_key]
    }
  }
  if( "lines" in G_NEW_POST )
  {
    for( l_key in G_NEW_POST["lines"] )
    {
      for( l_key2 in G_NEW_POST["lines"][l_key] )
      {
        G_INDEX[l_slug]["lines"][l_key][l_key2] = G_NEW_POST["lines"][l_key][l_key2]
      }
    }
  }
  G_TITLE_TO_SLUG[G_NEW_POST["title"]] = l_slug
  # cleanup
  split("", l_tags)
  split("", G_CONTENT_STATE)
  split("", G_NEW_POST)
}

# Feed tags inside navigation
function MarkdownParser_PopulateTagsNavigation(    l_prevInTag, l_key, l_key2, l_key3)
{
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( l_key in G_SLUGS_BY_TAGS_AND_DATES )
  {
    l_prevInTag = ""
    PROCINFO["sorted_in"] = "@ind_num_desc"
    for( l_key2 in G_SLUGS_BY_TAGS_AND_DATES[l_key] )
    {
      for( l_key3 in G_SLUGS_BY_TAGS_AND_DATES[l_key][l_key2] )
      {
        if( l_prevInTag != "" )
        {
          G_INDEX[l_key3]["tag_nav"][l_key]["prev_in_tag"] = l_prevInTag
          G_INDEX[l_prevInTag]["tag_nav"][l_key]["next_in_tag"] = l_key3
        }
        l_prevInTag = l_key3
      }
    }
  }
}

# read previous metadata content
$1 == "metadata_current_date" { G_SETTINGS["date"] = $2 ; print "current_date " $2 ; next }
$1 == "metadata_build_version" { G_SETTINGS["build"] = $2 ; print "build_version " $2 ; next }
$1 == "metadata_chapter" { G_INDEX_METADATA[$2]["chapter"] = $3 ; next }
$1 == "metadata_tags" { next }

# if it is a md title
/^# / && !G_CONTENT_STATE["```"] {
  if( "title" in G_NEW_POST )
  {
    # flush previous post
    MarkdownParser_CopyNewPost()
  }
  G_NEW_POST["title"] = substr($0, 3)
  G_NEW_POST["slug"] = Util_TitleToSlug(G_NEW_POST["title"])
  next
}

# extract post link to other post and mark link as used
/[^!]\[[^]]+\]\([^)]+\)/ && !G_CONTENT_STATE["```"] {
  if( match($0, /[^!]\[([^]]+)\]\(([^)]+)\)/, l_array) )
  {
    if( !(l_array[2] ~ /^(https?)|(ftp)|(mailto):/) )
    {
      sub("\\(" l_array[2] "\\)", "", $0)
      G_NEW_POST["links"][l_array[1]] = l_array[2]
      G_INDEX_METADATA[l_array[2]]["used"] += 1
    }
    # cleanup
    split("", l_array)
  }
}

# parse external link or header fields
/^\[[^]]+\]:/ && !G_CONTENT_STATE["```"] {
  if( match($0, /^\[([^]]+)\]: *([^" ]+) *"?([^"]+)?"?/, l_array) )
  {
    # external link
    if( l_array[2] ~ /^(https?)|(ftp)|(mailto):/ )
    {
      # TODO remove html from the parsers (keep in the writers)
      l_array[2] = "<a href=\"" l_array[2] "\">" l_array[1] "</a>"
    }
    # valid header fields
    else if( index(G_SETTINGS["post_header_fields"], l_array[1]) )
    {
      G_NEW_POST[l_array[1]] = l_array[3]
    }
    # invalid link because metadata from all posts were already read
    else if( !(l_array[2] in G_INDEX_METADATA) )
    {
      print "warning: link", l_array[2], "not found for name", l_array[1], "and title", l_array[3]
      print $0
    }
    else
    {
      # mark link slug as used
      G_INDEX_METADATA[l_array[2]]["used"] += 1
    }
    G_NEW_POST["links"][l_array[1]] = l_array[2]
    # cleanup
    split("", l_array)
  }
  else
  {
    print "error: invalid link at", $0
  }
  next
}

# non empty line
/.+/ {
  l_totalLines = G_NEW_POST["totalLines"]
  MarkdownParser_FormatContent($0)
  if( G_NEW_POST["totalLines"] > l_totalLines )
  {
    if( length(G_NEW_POST["summary"]) < 200 )
    {
      if( index($0, "{{") == 0 && index($0, "```") == 0 )
      {
        # update summary if not image or preformatted
        G_NEW_POST["summary"] = G_NEW_POST["summary"] " " $0
      }
    }
  }
  # cleanup
  l_totalLines = ""
}

# Flush last post
# Detect links not used
# Feed tags inside navigation
END {
  if( "title" in G_NEW_POST )
  {
    # flush previous post
    MarkdownParser_CopyNewPost()
  }
  MarkdownParser_PopulateTagsNavigation()
}
