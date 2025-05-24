# Transform parsed text to epub.
# The following information is available (and cleaned in the end)
# after parsing:
#
# - G_CHAPTERS
# - G_CONVERT_LETTERS
# - G_INDEX
# - G_LETTERS
# - G_POST_SLUG_BY_POSITION
# - G_SETTINGS
# - G_TITLES_BY_TAGS

BEGIN {
  # Initialize global settings for blog configuration
  # TODO get hardcoded personal info from metadata file
  G_SETTINGS["title"] = "Blogue do Caloni: Programação, Depuração, Transpiração"
  G_SETTINGS["author"] = "Wanderley Caloni"
  G_SETTINGS["publisher"] = "Caloni"
  G_SETTINGS["playOrder"] = 2
  G_SETTINGS["lastYear"] = G_SETTINGS["firstYear"] ="2000"
  G_SETTINGS["currentId"] = 2
}

function BookWriter_SlugToId(a_slug,    l_id)
{
  l_id = a_slug
  gsub(/-/, "", l_id)
  gsub(/ /, "_", l_id)
  gsub(/#/, "sharp", l_id)
  l_id = "_" l_id
  if( a_slug == "_" ) {
    print "warning: empty l_id (_) to " l_id
  }
  return l_id
}

function BookWriter_SlugToEpubId(a_slug,    l_id)
{
  l_id = a_slug
  gsub(/-/, "", l_id)
  gsub(/ /, "_", l_id)
  l_id = "_" l_id
  if( a_slug == "_" ) {
    print "warning: empty l_id (_) to " l_id
  }
  return l_id
}

function BookWriter_RemoveLinksFromText(a_text)
{
  gsub(/\?[^"]+/, "", a_text)
  return a_text
}

function BookWriter_CharacterIsNumeric(a_object,    l_characters, l_ret)
{
    switch (typeof(a_object))
    {
    case "strnum":
    case "number":
        l_ret = 1
        break
    case "string":
        l_ret = (split(a_object, l_characters, " ") == 1) && (typeof(l_characters[1]) == "strnum") ? 1 : 0
        break
    default:
        l_ret = 0
    }
    split("", l_characters) # cleanup
    return l_ret
}

function BookWriter_CharacterToLetter(a_char,    l_convertChart)
{
  if( !("Á" in G_CONVERT_LETTERS) )
  {
    G_CONVERT_LETTERS["Á"] = "A"
    G_CONVERT_LETTERS["À"] = "A"
    G_CONVERT_LETTERS["Â"] = "A"
    G_CONVERT_LETTERS["Ã"] = "A"
    G_CONVERT_LETTERS["É"] = "E"
    G_CONVERT_LETTERS["Ê"] = "E"
    G_CONVERT_LETTERS["Ê"] = "E"
    G_CONVERT_LETTERS["Ô"] = "O"
    G_CONVERT_LETTERS["Õ"] = "O"
    G_CONVERT_LETTERS["Ó"] = "O"
    G_CONVERT_LETTERS["Ú"] = "U"
    G_CONVERT_LETTERS["Í"] = "I"
    G_CONVERT_LETTERS["Ï"] = "I"
    G_CONVERT_LETTERS["("] = "#"
    G_CONVERT_LETTERS[")"] = "#"
    G_CONVERT_LETTERS["'"] = "#"
    G_CONVERT_LETTERS["\""] = "#"
  }
  if( BookWriter_CharacterIsNumeric(a_char) )
  {
    return "#"
  }
  a_char = toupper(a_char)
  l_convertChart = G_CONVERT_LETTERS[a_char]
  if( l_convertChart != "" )
  {
    return l_convertChart
  }
  return a_char
}

function BookWriter_WritePost(a_slug,    l_fchapter, l_file, l_tags, l_postText, l_prefix, l_suffix, l_key, l_key2)
{
  l_fchapter = BookWriter_SlugToId(G_INDEX[a_slug]["chapter"])
  G_CHAPTERS[G_INDEX[a_slug]["chapter"]] = G_INDEX[a_slug]["chapter"]

  split(G_INDEX[a_slug]["tags"], l_tags)
  for( l_key in l_tags )
  {
    G_TITLES_BY_TAGS[l_tags[l_key]][G_INDEX[a_slug]["title"]] = G_INDEX[a_slug]["title"]
  }

  # first post in chapter
  if( ! (l_fchapter in Files) )
  {
    l_postText = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    l_postText = l_postText "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">\n"
    l_postText = l_postText "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>\n"
    l_postText = l_postText "<title>" Util_TextToHtml(G_INDEX[a_slug]["chapter"]) "</title>\n"
    l_postText = l_postText "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />\n"
    l_postText = l_postText "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />\n"
    l_postText = l_postText "</head>\n"
    l_postText = l_postText "<body>\n"
    l_postText = l_postText "<div class=\"body\">\n"
    l_postText = l_postText "<span epub:type=\"pagebreak\" id=\"" BookWriter_SlugToId(G_INDEX[a_slug]["chapter"]) "\" title=\"" Util_TextToHtml(G_INDEX[a_slug]["chapter"]) "\"/>\n"
    l_postText = l_postText "<h1 class=\"chapter-title\"><strong>" Util_TextToHtml(G_INDEX[a_slug]["chapter"]) "</strong></h1>\n"
    Files[l_fchapter] = l_fchapter
  }

  # for each post line
  for( l_key in G_INDEX[a_slug]["lines"] )
  {
    l_prefix = ""
    l_suffix = ""

    # line is non empty
    if( G_INDEX[a_slug]["lines"][l_key]["content"] != "" )
    {
      # is not preformatted nor quote block
      if( G_INDEX[a_slug]["lines"][l_key]["type"] != "pre" && G_INDEX[a_slug]["lines"][l_key]["type"] != "blockquote" )
      {
        # it is a header (h1, h2, ...)
        if ( substr(G_INDEX[a_slug]["lines"][l_key]["type"], 1, 1) == "h" && length(G_INDEX[a_slug]["lines"][l_key]["type"]) == 2 )
        {
          # replace ampersand only for headers for some reason
          gsub(/&/, "&amp;", G_INDEX[a_slug]["lines"][l_key]["content"])
        }
        # TODO links translation, need more clarification
        for( l_key2 in G_INDEX[a_slug]["links"] )
        {
          if( G_INDEX[a_slug]["links"][l_key2] in G_INDEX_METADATA )
          {
            G_INDEX[a_slug]["links"][l_key2] = "<a href=\"" BookWriter_SlugToEpubId(G_INDEX_METADATA[G_INDEX[a_slug]["links"][l_key2]]["chapter"]) ".xhtml#" BookWriter_SlugToEpubId(G_INDEX[a_slug]["links"][l_key2]) "\">" l_key2 "</a>"
          }
          search = "\\[" l_key2 "\\]"
          gsub(search, BookWriter_RemoveLinksFromText(G_INDEX[a_slug]["links"][l_key2]), G_INDEX[a_slug]["lines"][l_key]["content"])
        }
      }

      # preformatted content
      if( G_INDEX[a_slug]["lines"][l_key]["type"] == "pre" )
      {
        G_INDEX[a_slug]["lines"][l_key]["content"] = Util_TextToHtml(G_INDEX[a_slug]["lines"][l_key]["content"]) "\n"
        if( G_INDEX[a_slug]["lines"][l_key-1]["type"] != G_INDEX[a_slug]["lines"][l_key]["type"] )
        {
          G_INDEX[a_slug]["lines"][l_key]["content"] = "<" G_INDEX[a_slug]["lines"][l_key]["type"] ">\n" G_INDEX[a_slug]["lines"][l_key]["content"]
        }
        if( G_INDEX[a_slug]["lines"][l_key+1]["type"] != G_INDEX[a_slug]["lines"][l_key]["type"] )
        {
          G_INDEX[a_slug]["lines"][l_key]["content"] = G_INDEX[a_slug]["lines"][l_key]["content"] "</" G_INDEX[a_slug]["lines"][l_key]["type"] ">\n"
        }
      }
      # quote block
      else if( G_INDEX[a_slug]["lines"][l_key]["type"] == "blockquote" )
      {
        l_prefix = "<p>" Util_TextToHtml("> ")
        l_suffix = "</p>\n"
        G_INDEX[a_slug]["lines"][l_key]["content"] = Util_TextToHtml(G_INDEX[a_slug]["lines"][l_key]["content"])
      }
      # list
      else if( G_INDEX[a_slug]["lines"][l_key]["type"] == "list" )
      {
        l_prefix = l_prefix "<li>"
        l_suffix = l_suffix "</li>"
        if( G_INDEX[a_slug]["lines"][l_key-1]["type"] != "list" )
        {
          l_prefix = "<ul>" l_prefix
        }
        if( G_INDEX[a_slug]["lines"][l_key+1]["type"] != "list" )
        {
          l_suffix = l_suffix "</ul>"
        }
        l_suffix = l_suffix "\n"
        G_INDEX[a_slug]["lines"][l_key]["content"] = Util_TextToHtml(G_INDEX[a_slug]["lines"][l_key]["content"])
      }
      # headers h1, h2, ...
      else if ( substr(G_INDEX[a_slug]["lines"][l_key]["type"], 1, 1) == "h" && length(G_INDEX[a_slug]["lines"][l_key]["type"]) == 2 )
      {
        l_prefix = l_prefix "<h" substr(G_INDEX[a_slug]["lines"][l_key]["type"], 2, 1) ">"
        l_suffix = l_suffix "</h" substr(G_INDEX[a_slug]["lines"][l_key]["type"], 2, 1) ">\n"
      }
      # paragraph
      else if ( G_INDEX[a_slug]["lines"][l_key]["type"] == "p")
      {
        l_prefix = l_prefix "<p>"
        l_suffix = l_suffix "</p>\n"
      }
      # image
      else if ( G_INDEX[a_slug]["lines"][l_key]["type"] == "img")
      {
        l_prefix = l_prefix "<img src=\"img/"
        l_suffix = l_suffix "\"/>\n"
      }

      G_INDEX[a_slug]["lines"][l_key]["content"] = l_prefix G_INDEX[a_slug]["lines"][l_key]["content"] l_suffix
    }
  }

  # let's begin to mount post content
  # first the span with anchor
  l_postText = l_postText "<span epub:type=\"pagebreak\" id=\"" BookWriter_SlugToId(a_slug) "\" title=\"" Util_TextToHtml(G_INDEX[a_slug]["title"]) "\"/>\n"
  # next begin section
  l_postText = l_postText "<section title=\"" Util_TextToHtml(G_INDEX[a_slug]["title"]) "\" epub:type=\"bodymatter chapter\">\n"
  l_postText = l_postText "<h1 class=\"chapter-subtitle\"><strong>" Util_TextToHtml(G_INDEX[a_slug]["title"]) "</strong></h1>\n"
  l_postText = l_postText "<p class=\"note-title\">" G_INDEX[a_slug]["date"]
  # tags
  for( l_key in l_tags )
  {
    l_postText = l_postText " "
    if( "tag_nav" in G_INDEX[a_slug] && l_tags[l_key] in G_INDEX[a_slug]["tag_nav"] && "prev_in_tag" in G_INDEX[a_slug]["tag_nav"][l_tags[l_key]] )
    {
      l_postText = l_postText "<a href=\"" BookWriter_SlugToId(G_INDEX[G_INDEX[a_slug]["tag_nav"][l_tags[l_key]]["prev_in_tag"]]["chapter"]) ".xhtml#" BookWriter_SlugToId(G_INDEX[a_slug]["tag_nav"][l_tags[l_key]]["prev_in_tag"]) "\">&lt;</a>"
    }
    l_postText = l_postText "<a href=\"toc" BookWriter_SlugToId(l_tags[l_key]) ".xhtml\">" l_tags[l_key] "</a>"
    if( "tag_nav" in G_INDEX[a_slug] && l_tags[l_key] in G_INDEX[a_slug]["tag_nav"] && "next_in_tag" in G_INDEX[a_slug]["tag_nav"][l_tags[l_key]] )
    {
      l_postText = l_postText "<a href=\"" BookWriter_SlugToId(G_INDEX[G_INDEX[a_slug]["tag_nav"][l_tags[l_key]]["next_in_tag"]]["chapter"]) ".xhtml#" BookWriter_SlugToId(G_INDEX[a_slug]["tag_nav"][l_tags[l_key]]["next_in_tag"]) "\">&gt;</a>"
    }
  }
  l_postText = l_postText "</p>\n\n"
  # finally post lines
  for( l_key in G_INDEX[a_slug]["lines"] )
  {
    l_postText = l_postText G_INDEX[a_slug]["lines"][l_key]["content"]
  }
  l_postText = l_postText "</section>"

  # flush to file
  l_file = "public\\book\\EPUB\\" l_fchapter ".xhtml"
  print l_postText > l_file

  # cleanup
  split("", l_tags)
}

# Write all posts to months page ordered by journal numerical position
# Processes posts in chronological order and writes them to their respective month pages
function BookWriter_FlushPosts(    l_position, l_slug)
{
  BookWriter_PopulateChapters()
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( l_position in G_POST_SLUG_BY_POSITION )
  {
    l_slug = G_POST_SLUG_BY_POSITION[l_position]
    # posts without a date are treated as invisible drafts
    if( !("date" in G_INDEX[l_slug]) )
    {
      print "skipping", l_slug
      continue
    }
    BookWriter_WritePost(l_slug)
  }
}

function BookWriter_PopulateChapters(    l_slug)
{
  for( l_slug in G_INDEX )
  {
    if( !("month" in G_INDEX[l_slug]) )
    {
      continue
    }
    G_INDEX[l_slug]["chapter"] = G_INDEX[l_slug]["month"]
  }
}

function BookWriter_FlushPostsPages(    l_key, l_file)
{
  for( l_key in Files )
  {
    l_file = "public\\book\\EPUB\\" l_key ".xhtml"
    print "</div>" > l_file
    print "</body>" > l_file
    print "</html>" > l_file
  }
}

function BookWriter_FlushPackage(    l_key, l_file, l_totalImages)
{
  l_file = "public\\book\\EPUB\\package.opf"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > l_file
  print "<package xmlns=\"http://www.idpf.org/2007/opf\" version=\"3.0\" unique-identifier=\"p0000000000000\">" > l_file
  print "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\">" > l_file
  print "<dc:title id=\"title\">" G_SETTINGS["title"] "</dc:title>" > l_file
  print "<dc:creator>" G_SETTINGS["author"] "</dc:creator>" > l_file
  print "<dc:publisher>" G_SETTINGS["publisher"] "</dc:publisher>" > l_file
  print "<dc:rights>Copyright " G_SETTINGS["build"] "</dc:rights>" > l_file
  print "<dc:identifier id=\"p0000000000000\">0000000000000</dc:identifier>" > l_file
  print "<dc:source id=\"src-id\">urn:isbn:0000000000000</dc:source>" > l_file
  print "<dc:language>pt-BR</dc:language>" > l_file
  print "<meta property=\"dcterms:modified\">" G_SETTINGS["date"] "</meta>" > l_file
  print "</metadata>" > l_file
  print "<manifest>" > l_file
  print "<item id=\"cover\" href=\"cover.xhtml\" media-type=\"application/xhtml+xml\"/>" > l_file
  print "<item id=\"cover-image\" properties=\"cover-image\" href=\"img/cover.jpg\" media-type=\"image/jpeg\"/>" > l_file
  print "<item id=\"style\" href=\"css/stylesheet.css\" media-type=\"text/css\"/>" > l_file
  print "<item id=\"ncx\" properties=\"nav\" href=\"ncx.xhtml\" media-type=\"application/xhtml+xml\"/>" > l_file
  print "<item id=\"ncx1\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\"/>" > l_file
  print "<item id=\"page-template\" href=\"css/page-template.xpgt\" media-type=\"application/adobe-page-template+xml\"/>" > l_file
  print "<item id=\"titlepage\" href=\"titlepage.xhtml\" media-type=\"application/xhtml+xml\"/>" > l_file
  print "<item id=\"toc\" href=\"toc.xhtml\" media-type=\"application/xhtml+xml\"/>" > l_file
  for( l_key in G_TITLES_BY_TAGS )
  {
    print "<item id=\"toc_" l_key "\" href=\"toc_" l_key ".xhtml\" media-type=\"application/xhtml+xml\"/>" > l_file
  }
  print "<item id=\"index\" href=\"index.xhtml\" media-type=\"application/xhtml+xml\"/>" > l_file
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( l_key in G_CHAPTERS )
  {
    print "<item id=\"" BookWriter_SlugToId(l_key) "\" href=\"" BookWriter_SlugToId(l_key) ".xhtml\" media-type=\"application/xhtml+xml\"/>" > l_file
  }
  l_totalImages = 0
  for( l_key in G_POSTS_IMAGES )
  {
    if( index(l_key, "jpg") || index(l_key, "jpeg") )
    {
      print "<item id=\"img-id-" ++l_totalImages "\" href=\"img/" l_key "\" media-type=\"image/jpeg\"/>" > l_file
    }
    else if( index(l_key, "gif") )
    {
      print "<item id=\"img-id-" ++l_totalImages "\" href=\"img/" l_key "\" media-type=\"image/gif\"/>" > l_file
    }
    else if( index(l_key, "png") )
    {
      print "<item id=\"img-id-" ++l_totalImages "\" href=\"img/" l_key "\" media-type=\"image/png\"/>" > l_file
    }
    else if( index(l_key, "svg") )
    {
      print "<item id=\"img-id-" ++l_totalImages "\" href=\"img/" l_key "\" media-type=\"image/svg\"/>" > l_file
    }
  }
  print "</manifest>" > l_file
  print "<spine toc=\"ncx1\">" > l_file
  print "<itemref idref=\"cover\" linear=\"yes\"/>" > l_file
  print "<itemref idref=\"titlepage\" linear=\"yes\"/>" > l_file
  print "<itemref idref=\"toc\" linear=\"yes\"/>" > l_file
  for( l_key in G_TITLES_BY_TAGS )
  {
    print "<itemref linear=\"yes\" idref=\"toc" BookWriter_SlugToId(l_key) "\"/>" > l_file
  }
  for( l_key in G_CHAPTERS )
  {
    print "<itemref linear=\"yes\" idref=\"" BookWriter_SlugToId(l_key) "\"/>" > l_file
  }
  print "<itemref linear=\"yes\" idref=\"index\"/>" > l_file
  print "<itemref linear=\"yes\" idref=\"ncx\"/>" > l_file
  print "</spine>" > l_file
  print "</package>" > l_file
}

function BookWriter_FlushTocNcx(    l_file, l_key)
{
  l_file = "public\\book\\EPUB\\toc.ncx"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > l_file
  print "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"en-US\">" > l_file
  print "<head>" > l_file
  print "<meta name=\"dtb:uid\" content=\"0000000000000\"/>" > l_file
  print "<meta name=\"dtb:depth\" content=\"1\"/>" > l_file
  print "<meta name=\"dtb:totalPageCount\" content=\"0\"/>" > l_file
  print "<meta name=\"dtb:maxPageNumber\" content=\"0\"/>" > l_file
  print "</head>" > l_file
  print "<docTitle><text>Blogue do Caloni: Programação, Depuração, Transpiração</text></docTitle>" > l_file
  print "<docAuthor><text>Wanderley Caloni</text></docAuthor>" > l_file
  print "<navMap>" > l_file
  print "<navPoint id=\"cover\" playOrder=\"1\"><navLabel><text>Cover</text></navLabel><content src=\"cover.xhtml\"/></navPoint>" > l_file
  print "<navPoint id=\"toc\" playOrder=\"2\"><navLabel><text>Contents</text></navLabel><content src=\"toc.xhtml\"/></navPoint>" > l_file
  # why is this commented?
  #for( l_key in G_CHAPTERS )
  #{
  #  print "<navPoint playOrder=\"" ++G_SETTINGS["playOrder"] "\" id=\"" BookWriter_SlugToId(l_key) "\"><navLabel><text>" Util_TextToHtml(l_key) "</text></navLabel>\
  #    <content src=\"" BookWriter_SlugToId(l_key) ".xhtml\"/></navPoint>" > l_file
  #}
  print "</navMap>" > l_file
  print "</ncx>" > l_file
}

function BookWriter_FlushTocPage(    l_file, l_year, l_month, l_key)
{
  l_file = "public\\book\\EPUB\\toc.xhtml"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > l_file
  print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > l_file
  print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > l_file
  print "<title>Blogue do Caloni: Programação, Depuração, Transpiração</title>" > l_file
  print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > l_file
  print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > l_file
  print "</head>" > l_file
  print "<body>" > l_file
  print "<div class=\"body\">" > l_file
  print "<a id=\"piii\"></a>" > l_file
  print "<h1 class=\"toc-title\">Contents</h1>" > l_file
  print "<p id=\"indx-1\" class=\"toca\"><a href=\"index.xhtml\"><strong>Index</strong></a></p>" > l_file
  for( l_key in G_CHAPTERS )
  {
    l_year = substr(l_key, 1, 4)
    l_month = substr(l_key, 6, 2)
    if( l_year != G_SETTINGS["lastYear"] )
    {
      if( G_SETTINGS["lastYear"] != G_SETTINGS["firstYear"] )
      {
        print "</p>" > l_file
      }
      print "<p id=\"" BookWriter_SlugToId(l_key) "\" class=\"toc\"><strong>" l_year "</strong>" > l_file
      G_SETTINGS["lastYear"] = l_year
    }
    print "<a href=\"" BookWriter_SlugToId(l_key) ".xhtml\"> " Util_TextToHtml(l_month) " </a>" > l_file
  }
  print "</p>" > l_file
  print "<a id=\"piv\"></a>" > l_file
  print "</div>" > l_file
  print "</body>" > l_file
  print "</html>" > l_file
}

function BookWriter_FlushTagsPage(    l_key, l_file)
{
  for( l_key in G_TITLES_BY_TAGS )
  {
    l_file = "public\\book\\EPUB\\toc_" l_key ".xhtml"
    print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > l_file
    print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > l_file
    print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > l_file
    print "<title>" l_key "</title>" > l_file
    print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > l_file
    print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > l_file
    print "</head>" > l_file
    print "<body>" > l_file
    print "<div class=\"body\">" > l_file
    print "<h1 class=\"toc-title\">" l_key "</h1>" > l_file
    print "<ul>" > l_file
    for( tit in G_TITLES_BY_TAGS[l_key] )
    {
      print "<li><a href=\"" BookWriter_SlugToId(G_INDEX[G_TITLE_TO_SLUG[tit]]["chapter"]) ".xhtml#" BookWriter_SlugToId(G_TITLE_TO_SLUG[tit]) "\">" Util_TextToHtml(tit) "</a></li>" > l_file
    }
    print "</ul>" > l_file
    print "</div>" > l_file
    print "</body>" > l_file
    print "</html>" > l_file
  }
}

function BookWriter_FlushNcx(    l_file)
{
  l_file = "public\\book\\EPUB\\ncx.xhtml"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > l_file
  print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > l_file
  print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > l_file
  print "<title>Blogue do Caloni: Programação, Depuração, Transpiração</title>" > l_file
  print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > l_file
  print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > l_file
  print "</head>" > l_file
  print "<body>" > l_file
  print "<nav epub:type=\"toc\">" > l_file
  print "<h2>Contents</h2>" > l_file
  print "<ol epub:type=\"list\">" > l_file
  #for( c in G_CHAPTERS )
  #{
  #  print "<li><a href=\"" BookWriter_SlugToId(c) ".xhtml\">" Util_TextToHtml(c) "</a></li>" > l_file
  #}
  print "<li><a href=\"index.xhtml\">Index</a></li>" > l_file
  print "</ol>" > l_file
  print "</nav>" > l_file
  print "</body>" > l_file
  print "</html>" > l_file
}

function BookWriter_FlushIndexPage(    l_key, l_letter, l_file)
{
  l_file = "public\\book\\EPUB\\index.xhtml"
  print "<!DOCTYPE html>" > l_file
  print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\" xml:lang=\"en-US\" lang=\"en-US\">" > l_file
  print "<head>" > l_file
  print "<meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > l_file
  print "<title>Blogue do Caloni: Programação, Depuração, Transpiração</title>" > l_file
  print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > l_file
  print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > l_file
  print "</head>" > l_file
  print "<body>" > l_file
  print "<h1 class=\"index-title\"><span epub:type=\"pagebreak\" id=\"idx\" title=\"Index\"/><a href=\"toc.xhtml#indx-1\"><strong>Index</strong></a></h1>" > l_file
  print "<section epub:type=\"index-group\" id=\"letters\">" > l_file
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( l_key in G_INDEX )
  {
    if( !("date" in G_INDEX[l_key]) )
    {
      continue
    }
    l_letter = BookWriter_CharacterToLetter(G_INDEX[l_key]["letter"])
    t = G_INDEX[l_key]["title"]
    if( G_LETTERS[l_letter] == "" )
    {
      G_LETTERS[l_letter] = "<h3 id=\"" BookWriter_SlugToId(l_letter) "\" class=\"groupletter\">" Util_TextToHtml(l_letter) "</h3>\n"\
        "<ul class=\"indexlevel1\">"
    }
    G_LETTERS[l_letter] = G_LETTERS[l_letter] "<li epub:type=\"index-entry\" class=\"indexhead1\" id=\"mh" G_SETTINGS["currentId"]++ "\">"\
      "<a href=\"" BookWriter_SlugToId(G_INDEX[l_key]["chapter"]) ".xhtml#" BookWriter_SlugToId(l_key) "\">" Util_TextToHtml(t) "</a></li>\n"
  }
  for( l_key in G_LETTERS )
  {
    print "<a href=\"#" BookWriter_SlugToId(l_key) "\">" l_key "</a>" > l_file
  }
  print "<h3 id=\"toc_tags\" class=\"groupletter\">Tags</h3>\n<ul class=\"indexlevel1\">" > l_file
  for( l_key in G_TITLES_BY_TAGS )
  {
    tocxhtml = "public\\book\\EPUB\\toc_" l_key ".xhtml"
    print "<li epub:type=\"index-entry\" class=\"indexhead1\" id=\"mh" G_SETTINGS["currentId"]++ "\">"\
      "<a href=\"toc" BookWriter_SlugToId(l_key) ".xhtml\">" Util_TextToHtml(l_key) "</a></li>\n" > l_file
  }
  print "</ul>" > l_file
  for( l_key in G_LETTERS )
  {
    print G_LETTERS[l_key] > l_file
    print "</ul>" > l_file
  }
  print "</section>" > l_file
  print "</body>" > l_file
  print "</html>" > l_file
}

# Main execution block
# Processes all posts and generates all necessary files
END {
  # Process and write all posts
  BookWriter_FlushPosts()

  # Generate all required files
  BookWriter_FlushPostsPages()
  BookWriter_FlushPackage()
  BookWriter_FlushTocNcx()
  BookWriter_FlushTocPage()
  BookWriter_FlushTagsPage()
  BookWriter_FlushNcx()
  BookWriter_FlushIndexPage()

  # Cleanup global arrays
  split("", G_CHAPTERS)
  split("", G_CONVERT_LETTERS)
  split("", G_INDEX)
  split("", G_LETTERS)
  split("", G_POST_SLUG_BY_POSITION)
  split("", G_SETTINGS)
  split("", G_TITLES_BY_TAGS)
}
