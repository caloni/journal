# Transform parsed text to epub.

BEGIN {
  # Initialize global settings for blog configuration
  # TODO get hardcoded personal info from metadata file
  G_SETTINGS["title"] = "Blogue do Caloni: Programação, Depuração, Transpiração"
  G_SETTINGS["author"] = "Wanderley Caloni"
  G_SETTINGS["publisher"] = "Caloni"
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

function BookWriter_CharacterIsNumeric(a_object,    l_characters)
{
    switch (typeof(a_object)) {
    case "strnum":
    case "number":
        return 1
    case "string":
        return (split(a_object, l_characters, " ") == 1) && (typeof(l_characters[1]) == "strnum")
    default:
        return 0
    }
}

function BookWriter_CharacterToLetter(a_char)
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
  c = G_CONVERT_LETTERS[a_char]
  if( c != "" )
  {
    return c
  }
  return a_char
}

function BookWriter_WritePost(a_slug,    l_fchapter, l_file, l_tags, l_postText, l_prefix, l_suffix, l_key)
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
        for( j in G_INDEX[a_slug]["links"] )
        {
          if( G_INDEX[a_slug]["links"][j] in G_INDEX_METADATA )
          {
            G_INDEX[a_slug]["links"][j] = "<a href=\"" BookWriter_SlugToEpubId(G_INDEX_METADATA[G_INDEX[a_slug]["links"][j]]["chapter"]) ".xhtml#" BookWriter_SlugToEpubId(G_INDEX[a_slug]["links"][j]) "\">" j "</a>"
          }
          search = "\\[" j "\\]"
          gsub(search, BookWriter_RemoveLinksFromText(G_INDEX[a_slug]["links"][j]), G_INDEX[a_slug]["lines"][l_key]["content"])
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

function BookWriter_PopulateChapters(    slug)
{
  for( slug in G_INDEX )
  {
    if( !("month" in G_INDEX[slug]) )
    {
      continue
    }
    G_INDEX[slug]["chapter"] = G_INDEX[slug]["month"]
  }
}

function BookWriter_FlushPostsPages()
{
  for( f in Files )
  {
    file = "public\\book\\EPUB\\" f ".xhtml"
    print "</div>" > file
    print "</body>" > file
    print "</html>" > file
  }
}

function BookWriter_FlushPackage(    l_key)
{
  package = "public\\book\\EPUB\\package.opf"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > package
  print "<package xmlns=\"http://www.idpf.org/2007/opf\" version=\"3.0\" unique-identifier=\"p0000000000000\">" > package
  print "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\">" > package
  print "<dc:title id=\"title\">" G_SETTINGS["title"] "</dc:title>" > package
  print "<dc:creator>" G_SETTINGS["author"] "</dc:creator>" > package
  print "<dc:publisher>" G_SETTINGS["publisher"] "</dc:publisher>" > package
  print "<dc:rights>Copyright " G_SETTINGS["build"] "</dc:rights>" > package
  print "<dc:identifier id=\"p0000000000000\">0000000000000</dc:identifier>" > package
  print "<dc:source id=\"src-id\">urn:isbn:0000000000000</dc:source>" > package
  print "<dc:language>pt-BR</dc:language>" > package
  print "<meta property=\"dcterms:modified\">" G_SETTINGS["date"] "</meta>" > package
  print "</metadata>" > package
  print "<manifest>" > package
  print "<item id=\"cover\" href=\"cover.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  print "<item id=\"cover-image\" properties=\"cover-image\" href=\"img/cover.jpg\" media-type=\"image/jpeg\"/>" > package
  print "<item id=\"style\" href=\"css/stylesheet.css\" media-type=\"text/css\"/>" > package
  print "<item id=\"ncx\" properties=\"nav\" href=\"ncx.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  print "<item id=\"ncx1\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\"/>" > package
  print "<item id=\"page-template\" href=\"css/page-template.xpgt\" media-type=\"application/adobe-page-template+xml\"/>" > package
  print "<item id=\"titlepage\" href=\"titlepage.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  print "<item id=\"toc\" href=\"toc.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  for( l_key in G_TITLES_BY_TAGS )
  {
    print "<item id=\"toc_" l_key "\" href=\"toc_" l_key ".xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  }
  print "<item id=\"index\" href=\"index.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( c in G_CHAPTERS )
  {
    print "<item id=\"" BookWriter_SlugToId(c) "\" href=\"" BookWriter_SlugToId(c) ".xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  }
  totalImages = 0
  for( image in G_POSTS_IMAGES )
  {
    if( index(image, "jpg") || index(image, "jpeg") )
    {
      print "<item id=\"img-id-" ++totalImages "\" href=\"img/" image "\" media-type=\"image/jpeg\"/>" > package
    }
    else if( index(image, "gif") )
    {
      print "<item id=\"img-id-" ++totalImages "\" href=\"img/" image "\" media-type=\"image/gif\"/>" > package
    }
    else if( index(image, "png") )
    {
      print "<item id=\"img-id-" ++totalImages "\" href=\"img/" image "\" media-type=\"image/png\"/>" > package
    }
    else if( index(image, "svg") )
    {
      print "<item id=\"img-id-" ++totalImages "\" href=\"img/" image "\" media-type=\"image/svg\"/>" > package
    }
  }
  print "</manifest>" > package
  print "<spine toc=\"ncx1\">" > package
  print "<itemref idref=\"cover\" linear=\"yes\"/>" > package
  print "<itemref idref=\"titlepage\" linear=\"yes\"/>" > package
  print "<itemref idref=\"toc\" linear=\"yes\"/>" > package
  for( l_key in G_TITLES_BY_TAGS )
  {
    print "<itemref linear=\"yes\" idref=\"toc" BookWriter_SlugToId(l_key) "\"/>" > package
  }
  for( c in G_CHAPTERS )
  {
    print "<itemref linear=\"yes\" idref=\"" BookWriter_SlugToId(c) "\"/>" > package
  }
  print "<itemref linear=\"yes\" idref=\"index\"/>" > package
  print "<itemref linear=\"yes\" idref=\"ncx\"/>" > package
  print "</spine>" > package
  print "</package>" > package
}

function BookWriter_FlushTocNcx()
{
  tocncx = "public\\book\\EPUB\\toc.ncx"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > tocncx
  print "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"en-US\">" > tocncx
  print "<head>" > tocncx
  print "<meta name=\"dtb:uid\" content=\"0000000000000\"/>" > tocncx
  print "<meta name=\"dtb:depth\" content=\"1\"/>" > tocncx
  print "<meta name=\"dtb:totalPageCount\" content=\"0\"/>" > tocncx
  print "<meta name=\"dtb:maxPageNumber\" content=\"0\"/>" > tocncx
  print "</head>" > tocncx
  print "<docTitle><text>Blogue do Caloni: Programação, Depuração, Transpiração</text></docTitle>" > tocncx
  print "<docAuthor><text>Wanderley Caloni</text></docAuthor>" > tocncx
  print "<navMap>" > tocncx
  print "<navPoint id=\"cover\" playOrder=\"1\"><navLabel><text>Cover</text></navLabel><content src=\"cover.xhtml\"/></navPoint>" > tocncx
  print "<navPoint id=\"toc\" playOrder=\"2\"><navLabel><text>Contents</text></navLabel><content src=\"toc.xhtml\"/></navPoint>" > tocncx
  playOrder = 2
  #for( c in G_CHAPTERS )
  #{
  #  print "<navPoint playOrder=\"" ++playOrder "\" id=\"" BookWriter_SlugToId(c) "\"><navLabel><text>" Util_TextToHtml(c) "</text></navLabel>\
  #    <content src=\"" BookWriter_SlugToId(c) ".xhtml\"/></navPoint>" > tocncx
  #}
  print "</navMap>" > tocncx
  print "</ncx>" > tocncx
}

function BookWriter_FlushTocPage()
{
  tocxhtml = "public\\book\\EPUB\\toc.xhtml"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > tocxhtml
  print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > tocxhtml
  print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > tocxhtml
  print "<title>Blogue do Caloni: Programação, Depuração, Transpiração</title>" > tocxhtml
  print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > tocxhtml
  print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > tocxhtml
  print "</head>" > tocxhtml
  print "<body>" > tocxhtml
  print "<div class=\"body\">" > tocxhtml
  print "<a id=\"piii\"></a>" > tocxhtml
  print "<h1 class=\"toc-title\">Contents</h1>" > tocxhtml
  print "<p id=\"indx-1\" class=\"toca\"><a href=\"index.xhtml\"><strong>Index</strong></a></p>" > tocxhtml
  lastyear = "2000"
  for( c in G_CHAPTERS )
  {
    year = substr(c, 1, 4)
    mon = substr(c, 6, 2)
    if( year != lastyear )
    {
      if( lastyear != "2000" )
      {
        print "</p>" > tocxhtml
      }
      print "<p id=\"" BookWriter_SlugToId(c) "\" class=\"toc\"><strong>" year "</strong>" > tocxhtml
      lastyear = year
    }
    print "<a href=\"" BookWriter_SlugToId(c) ".xhtml\"> " \
      Util_TextToHtml(mon) " </a>" > tocxhtml
  }
  print "</p>" > tocxhtml
  print "<a id=\"piv\"></a>" > tocxhtml
  print "</div>" > tocxhtml
  print "</body>" > tocxhtml
  print "</html>" > tocxhtml
}

function BookWriter_FlushTagsPage()
{
  for( i in G_TITLES_BY_TAGS ) {
    tocxhtml = "public\\book\\EPUB\\toc_" i ".xhtml"
    print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > tocxhtml
    print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > tocxhtml
    print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > tocxhtml
    print "<title>" i "</title>" > tocxhtml
    print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > tocxhtml
    print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > tocxhtml
    print "</head>" > tocxhtml
    print "<body>" > tocxhtml
    print "<div class=\"body\">" > tocxhtml
    print "<h1 class=\"toc-title\">" i "</h1>" > tocxhtml
    print "<ul>" > tocxhtml
    for( tit in G_TITLES_BY_TAGS[i] ) {
      print "<li><a href=\"" BookWriter_SlugToId(G_INDEX[G_TITLE_TO_SLUG[tit]]["chapter"]) ".xhtml#" BookWriter_SlugToId(G_TITLE_TO_SLUG[tit]) "\">" Util_TextToHtml(tit) "</a></li>" > tocxhtml
    }
    print "</ul>" > tocxhtml
    print "</div>" > tocxhtml
    print "</body>" > tocxhtml
    print "</html>" > tocxhtml
  }
}

function BookWriter_FlushNcx()
{
  ncxhtml = "public\\book\\EPUB\\ncx.xhtml"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > ncxhtml
  print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > ncxhtml
  print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > ncxhtml
  print "<title>Blogue do Caloni: Programação, Depuração, Transpiração</title>" > ncxhtml
  print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > ncxhtml
  print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > ncxhtml
  print "</head>" > ncxhtml
  print "<body>" > ncxhtml
  print "<nav epub:type=\"toc\">" > ncxhtml
  print "<h2>Contents</h2>" > ncxhtml
  print "<ol epub:type=\"list\">" > ncxhtml
  #for( c in G_CHAPTERS ) {
  #  print "<li><a href=\"" BookWriter_SlugToId(c) ".xhtml\">" Util_TextToHtml(c) "</a></li>" > ncxhtml
  #}
  print "<li><a href=\"index.xhtml\">Index</a></li>" > ncxhtml
  print "</ol>" > ncxhtml
  print "</nav>" > ncxhtml
  print "</body>" > ncxhtml
  print "</html>" > ncxhtml
}

function BookWriter_FlushIndexPage()
{
  indexx = "public\\book\\EPUB\\index.xhtml"
  print "<!DOCTYPE html>" > indexx
  print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\" xml:lang=\"en-US\" lang=\"en-US\">" > indexx
  print "<head>" > indexx
  print "<meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > indexx
  print "<title>Blogue do Caloni: Programação, Depuração, Transpiração</title>" > indexx
  print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > indexx
  print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > indexx
  print "</head>" > indexx
  print "<body>" > indexx
  print "<h1 class=\"index-title\"><span epub:type=\"pagebreak\" id=\"idx\" title=\"Index\"/><a href=\"toc.xhtml#indx-1\"><strong>Index</strong></a></h1>" > indexx
  print "<section epub:type=\"index-group\" id=\"letters\">" > indexx
  PROCINFO["sorted_in"] = "@ind_str_asc"
  currid = 2
  for( i in G_INDEX ) {
    if( !("date" in G_INDEX[i]) ) {
      continue
    }
    l = BookWriter_CharacterToLetter(G_INDEX[i]["letter"])
    t = G_INDEX[i]["title"]
    if( G_LETTERS[l] == "" ) {
      G_LETTERS[l] = "<h3 id=\"" BookWriter_SlugToId(l) "\" class=\"groupletter\">" Util_TextToHtml(l) "</h3>\n"\
        "<ul class=\"indexlevel1\">"
    }
    G_LETTERS[l] = G_LETTERS[l] "<li epub:type=\"index-entry\" class=\"indexhead1\" id=\"mh" currid++ "\">"\
      "<a href=\"" BookWriter_SlugToId(G_INDEX[i]["chapter"]) ".xhtml#" BookWriter_SlugToId(i) "\">" Util_TextToHtml(t) "</a></li>\n"
  }
  for( l in G_LETTERS ) {
    print "<a href=\"#" BookWriter_SlugToId(l) "\">" l "</a>" > indexx
  }
  print "<h3 id=\"toc_tags\" class=\"groupletter\">Tags</h3>\n"\
    "<ul class=\"indexlevel1\">" > indexx
  for( i in G_TITLES_BY_TAGS ) {
    tocxhtml = "public\\book\\EPUB\\toc_" i ".xhtml"
    print "<li epub:type=\"index-entry\" class=\"indexhead1\" id=\"mh" currid++ "\">"\
      "<a href=\"toc" BookWriter_SlugToId(i) ".xhtml\">" Util_TextToHtml(i) "</a></li>\n" > indexx
  }
  print "</ul>" > indexx
  for( l in G_LETTERS ) {
    print G_LETTERS[l] > indexx
    print "</ul>" > indexx
  }
  print "</section>" > indexx
  print "</body>" > indexx
  print "</html>" > indexx
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
}

