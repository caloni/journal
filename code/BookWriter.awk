# Transform parsed text to epub.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2025-01-30
# Depends on: MarkdownParser, Util.

BEGIN {
  Book["copyright"] = "BlogWriter 0.2.0 " Settings["generator"]
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
    for( i in Index[slug]["lines"] ) {
      prefix = ""
      suffix = ""
      if( Index[slug]["lines"][i]["content"] != "" ) {
        if( Index[slug]["lines"][i]["type"] != "pre" && Index[slug]["lines"][i]["type"] != "blockquote" ) {
          if( "links" in Index[slug] ) {
            for( j in Index[slug]["links"] ) {
              if( Index[slug]["links"][j] in IndexMetadata ) {
                Index[slug]["links"][j] = "<a href=\"" ToEpubId(IndexMetadata[Index[slug]["links"][j]]["chapter"]) ".xhtml#" ToEpubId(Index[slug]["links"][j]) "\">" j "</a>"
              }
              search = "\\[" j "\\]"
              gsub(search, ToEpubLink(Index[slug]["links"][j]), Index[slug]["lines"][i]["content"])
            }
          }
        }

        if( Index[slug]["lines"][i]["type"] == "pre" ) {
          Index[slug]["lines"][i]["content"] = ToHtml(Index[slug]["lines"][i]["content"]) "\n"
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
            Index[slug]["lines"][i]["content"] = ToHtml(Index[slug]["lines"][i]["content"])
          }
          else if( Index[slug]["lines"][i]["type"] == "list" ) {
            prefix = prefix "<li>"
            suffix = suffix "</li>"
            if( Index[slug]["lines"][i-1]["type"] != "list" ) {
              prefix = "<ul>" prefix
            }
            if( Index[slug]["lines"][i+1]["type"] != "list" ) {
              suffix = suffix "</ul>"
            }
            suffix = suffix "\n"
            Index[slug]["lines"][i]["content"] = ToHtml(Index[slug]["lines"][i]["content"])
          } else if ( substr(Index[slug]["lines"][i]["type"], 1, 1) == "h" && length(Index[slug]["lines"][i]["type"]) == 2 ) {
            prefix = prefix "<h" substr(Index[slug]["lines"][i]["type"], 2, 1) ">"
            suffix = suffix "</h" substr(Index[slug]["lines"][i]["type"], 2, 1) ">\n"
          } else if ( Index[slug]["lines"][i]["type"] == "p") {
            prefix = prefix "<p>"
            suffix = suffix "</p>\n"
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
  print "<dc:rights>Copyright " Book["copyright"] "</dc:rights>" > package
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

