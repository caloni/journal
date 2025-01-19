# Transform parsed text to epub.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2025-01-04

#include util.awk

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

    #todo fix when blockquote is added into BlogWriter.awk
    # and this situation occurs:
    # `<blockquote></ul>`
    # only when this is fixed we can think about formatting
    # epubs with blockquote equivalent
    #if( line ~ /^>/ ) {
    #  sub(/^> ?/, "", line)
    #  ContentType = "blockquote"
    #  suffix = ""
    #  break
    #}

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
    if( a[1] == "date" ) {
      NewPost["date"] = a[3]
    } else if( a[1] == "tags" ) {
      NewPost["tags"] = a[3]
    } else if( a[1] == "slug" ) {
      NewPost["slug"] = a[3]
    }
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

BEGIN {
  Book["title"] = "Blogue do Caloni: Programação, Depuração, Transpiração"
  Book["author"] = "Wanderley Caloni"
  Book["publisher"] = "Caloni"
}

function FlushPost(slug,    chapter, fchapter, tags, post, prefix, suffix)
{
  post = ""
  chapter = Index[slug]["month"]
  fchapter = ToId(chapter)
  Chapters[chapter] = chapter
  TitleToChapter[Index[slug]["title"]] = chapter

  split(Index[slug]["tags"], tags)
  for( i in tags ) {
    TitlesByTags[tags[i]][Index[slug]["title"]] = Index[slug]["title"]
  }

  if( ! (fchapter in Files) ) {
    post = post "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    post = post "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">\n"
    post = post "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>\n"
    post = post "<title>" ToHtml(chapter) "</title>\n"
    post = post "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />\n"
    post = post "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />\n"
    post = post "</head>\n"
    post = post "<body>\n"
    post = post "<div class=\"body\">\n"
    post = post "<span epub:type=\"pagebreak\" id=\"" ToId(chapter) "\" title=\"" ToHtml(chapter) "\"/>\n"
    post = post "<h1 class=\"chapter-title\"><strong>" ToHtml(chapter) "</strong></h1>\n"
    Files[fchapter] = fchapter
  }

  if( length(Index[slug]["lines"]) ) {
    prefix = ""
    suffix = ""
    for( i in Index[slug]["lines"] ) {
      if( Index[slug]["lines"][i]["content"] != "" ) {
        if( Index[slug]["lines"][i]["type"] != "pre" && Index[slug]["lines"][i]["type"] != "blockquote" ) {
          if( "links" in Index[slug] ) {
            for( j in Index[slug]["links"] ) {
              search = "\\[" j "\\]"
              gsub(search, Index[slug]["links"][j], Index[slug]["lines"][i]["content"])
            }
          }
        }

        if( Index[slug]["lines"][i]["type"] == "pre" ) {
          Index[slug]["lines"][i]["content"] = ToHtml(Index[slug]["lines"][i]["content"])
          if( Index[slug]["lines"][i-1]["type"] != Index[slug]["lines"][i]["type"] ) {
            Index[slug]["lines"][i]["content"] = "<" Index[slug]["lines"][i]["type"] ">\n" Index[slug]["lines"][i]["content"]
          }
          if( Index[slug]["lines"][i+1]["type"] != Index[slug]["lines"][i]["type"] ) {
            Index[slug]["lines"][i]["content"] = Index[slug]["lines"][i]["content"] "</" Index[slug]["lines"][i]["type"] ">\n"
          }
        } else {
          if( Index[slug]["lines"][i]["type"] == "blockquote" ) {
            prefix = "<p>" ToHtml("> ")
            suffix = "</p>\n"
          }
          Index[slug]["lines"][i]["content"] = prefix Index[slug]["lines"][i]["content"] suffix
        }
      }
    }
  }

  post = post "<span epub:type=\"pagebreak\" id=\"" ToId(slug) "\" title=\"" ToHtml(Index[slug]["title"]) "\"/>\n"
  post = post "<section title=\"" ToHtml(Index[slug]["title"]) "\" epub:type=\"bodymatter chapter\">\n"
  post = post "<h1 class=\"chapter-subtitle\"><strong>" ToHtml(Index[slug]["title"]) "</strong></h1>\n"
  post = post "<p class=\"note-title\">" Index[slug]["date"]
  for( i in tags ) {
    post = post " <a href=\"toc" ToId(tags[i]) ".xhtml\">" tags[i] "</a>"
  }
  post = post "</p>\n\n"
  if( length(Index[slug]["lines"]) ) {
    for( i in Index[slug]["lines"] ) {
      post = post Index[slug]["lines"][i]["content"]
      #if( Index[slug]["lines"][i]["type"] != "pre" ) {
      #  post = post "\n"
      #}
    }
  }
  post = post "</section>"

  file = "public\\book\\EPUB\\" fchapter ".xhtml"
  print post > file
}

function FlushPosts(    position, slug)
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( position in PostSlugByPosition ) {
    slug = PostSlugByPosition[position]
    if( !("date" in Index[slug]) ) {
      print "skipping", slug
      continue
    }
    FlushPost(slug)
  }
}

function FlushPostsPages()
{
  for( f in Files ) {
    file = "public\\book\\EPUB\\" f ".xhtml"
    print "</div>" > file
    print "</body>" > file
    print "</html>" > file
  }
}

function FlushPackage()
{
  package = "public\\book\\EPUB\\package.opf"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > package
  print "<package xmlns=\"http://www.idpf.org/2007/opf\" version=\"3.0\" unique-identifier=\"p0000000000000\">" > package
  print "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\">" > package
  print "<dc:title id=\"title\">" Book["title"] "</dc:title>" > package
  print "<dc:creator>" Book["author"] "</dc:creator>" > package
  print "<dc:publisher>" Book["publisher"] "</dc:publisher>" > package
  print "<dc:rights>Copyright 404 Not Found</dc:rights>" > package
  print "<dc:identifier id=\"p0000000000000\">0000000000000</dc:identifier>" > package
  print "<dc:source id=\"src-id\">urn:isbn:0000000000000</dc:source>" > package
  print "<dc:language>pt-BR</dc:language>" > package
  print "<meta property=\"dcterms:modified\">" Settings["date"] "</meta>" > package
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
  for( i in TitlesByTags ) {
    print "<item id=\"toc_" i "\" href=\"toc_" i ".xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  }
  print "<item id=\"index\" href=\"index.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( c in Chapters ) {
    print "<item id=\"" ToId(c) "\" href=\"" ToId(c) ".xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  }
  totalImages = 0
  for( image in PostsImages ) {
    if( index(image, "jpg") || index(image, "jpeg") ) {
      print "<item id=\"img-id-" ++totalImages "\" href=\"img/" image "\" media-type=\"image/jpeg\"/>" > package
    } else if( index(image, "gif") ) {
      print "<item id=\"img-id-" ++totalImages "\" href=\"img/" image "\" media-type=\"image/gif\"/>" > package
    } else if( index(image, "png") ) {
      print "<item id=\"img-id-" ++totalImages "\" href=\"img/" image "\" media-type=\"image/png\"/>" > package
    } else if( index(image, "svg") ) {
      print "<item id=\"img-id-" ++totalImages "\" href=\"img/" image "\" media-type=\"image/svg\"/>" > package
    }
  }
  print "</manifest>" > package
  print "<spine toc=\"ncx1\">" > package
  print "<itemref idref=\"cover\" linear=\"yes\"/>" > package
  print "<itemref idref=\"titlepage\" linear=\"yes\"/>" > package
  print "<itemref idref=\"toc\" linear=\"yes\"/>" > package
  for( i in TitlesByTags ) {
    print "<itemref linear=\"yes\" idref=\"toc" ToId(i) "\"/>" > package
  }
  for( c in Chapters ) {
    print "<itemref linear=\"yes\" idref=\"" ToId(c) "\"/>" > package
  }
  print "<itemref linear=\"yes\" idref=\"index\"/>" > package
  print "<itemref linear=\"yes\" idref=\"ncx\"/>" > package
  print "</spine>" > package
  print "</package>" > package
}

function FlushTocNcx()
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
  #for( c in Chapters ) {
  #  print "<navPoint playOrder=\"" ++playOrder "\" id=\"" ToId(c) "\"><navLabel><text>" ToHtml(c) "</text></navLabel>\
  #    <content src=\"" ToId(c) ".xhtml\"/></navPoint>" > tocncx
  #}
  print "</navMap>" > tocncx
  print "</ncx>" > tocncx
}

function FlushTocPage()
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
  lastyear = "2001"
  for( c in Chapters ) {
    year = substr(c, 1, 4)
    mon = substr(c, 6, 2)
    if( year != lastyear ) {
      if( lastyear != "2001" ) {
        print "</p>" > tocxhtml
      }
      print "<p id=\"" ToId(c) "\" class=\"toc\"><strong>" year "</strong>" > tocxhtml
      lastyear = year
    }
    print "<a href=\"" ToId(c) ".xhtml\"> " \
      ToHtml(mon) " </a>" > tocxhtml
  }
  print "</p>" > tocxhtml
  print "<a id=\"piv\"></a>" > tocxhtml
  print "</div>" > tocxhtml
  print "</body>" > tocxhtml
  print "</html>" > tocxhtml
}

function FlushTagsPage()
{
  for( i in TitlesByTags ) {
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
    for( tit in TitlesByTags[i] ) {
      print "<li><a href=\"" ToId(TitleToChapter[tit]) ".xhtml#" ToId(TitleToSlug[tit]) "\">" ToHtml(tit) "</a></li>" > tocxhtml
    }
    print "</ul>" > tocxhtml
    print "</div>" > tocxhtml
    print "</body>" > tocxhtml
    print "</html>" > tocxhtml
  }
}

function FlushNcx()
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
  #for( c in Chapters ) {
  #  print "<li><a href=\"" ToId(c) ".xhtml\">" ToHtml(c) "</a></li>" > ncxhtml
  #}
  print "<li><a href=\"index.xhtml\">Index</a></li>" > ncxhtml
  print "</ol>" > ncxhtml
  print "</nav>" > ncxhtml
  print "</body>" > ncxhtml
  print "</html>" > ncxhtml
}

function FlushIndexPage()
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
  for( i in Index ) {
    if( !("date" in Index[i]) ) {
      continue
    }
    l = ToLetter(Index[i]["letter"])
    t = Index[i]["title"]
    if( Letters[l] == "" ) {
      Letters[l] = "<h3 id=\"" ToId(l) "\" class=\"groupletter\">" ToHtml(l) "</h3>\n"\
        "<ul class=\"indexlevel1\">"
    }
    Letters[l] = Letters[l] "<li epub:type=\"index-entry\" class=\"indexhead1\" id=\"mh" currid++ "\">"\
      "<a href=\"" ToId(TitleToChapter[t]) ".xhtml#" ToId(TitleToSlug[t]) "\">" ToHtml(t) "</a></li>\n"
  }
  for( l in Letters ) {
    print "<a href=\"#" ToId(l) "\">" l "</a>" > indexx
  }
  print "<h3 id=\"toc_tags\" class=\"groupletter\">Tags</h3>\n"\
    "<ul class=\"indexlevel1\">" > indexx
  for( i in TitlesByTags ) {
    tocxhtml = "public\\book\\EPUB\\toc_" i ".xhtml"
    print "<li epub:type=\"index-entry\" class=\"indexhead1\" id=\"mh" currid++ "\">"\
      "<a href=\"toc" ToId(i) ".xhtml\">" ToHtml(i) "</a></li>\n" > indexx
  }
  print "</ul>" > indexx
  for( l in Letters ) {
    print Letters[l] > indexx
    print "</ul>" > indexx
  }
  print "</section>" > indexx
  print "</body>" > indexx
  print "</html>" > indexx
}

END {
  FlushPosts()
  FlushPostsPages()
  FlushPackage()
  FlushTocNcx()
  FlushTocPage()
  FlushTagsPage()
  FlushNcx()
  FlushIndexPage()
}

