# Transform pseudo-markdown text to epub book.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2024-11-14

#include util.awk

BEGIN {
}

function WriteToHtml()
{
  ++postCount
  if( slug == "" ) {
    slug = ToSlug(title)
  }
  entries[substr(title, 1, 1),title] = title
  sterms = ""
  split(tags, stags)
  for( t in stags ) {
    terms[stags[t]][title] = title
    sterms = sterms " <a href=\"toc" ToId(stags[t]) ".xhtml\">" stags[t] "</a>"
  }
  slugs[slug]["slug"] = slug
  slugs[slug]["title"] = title
  slugs[slug]["date"] = date
  titleToSlug[title] = slug
  titleToChapter[title] = chapter
  if( draft ) {
    draftToSlug[title] = slug
    drafts = "public\\book\\drafts.txt"
    print "= " title > drafts
    print draftContent > drafts
    print "\n\n" > drafts
  }
  fchapter = ToId(chapter)
  file = "public\\book\\EPUB\\" fchapter ".xhtml"
  if( ! (fchapter in files) ) {
    print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > file
    print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > file
    print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > file
    print "<title>" ToHtml(chapter) "</title>" > file
    print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > file
    print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > file
    print "</head>" > file
    print "<body>" > file
    print "<div class=\"body\">" > file
    print "<span epub:type=\"pagebreak\" id=\"" ToId(chapter) "\" title=\"" ToHtml(chapter) "\"/>" > file
    print "<h1 class=\"chapter-title\"><strong>" ToHtml(chapter) "</strong></h1>" > file
    files[fchapter] = fchapter
  }
  print "<span epub:type=\"pagebreak\" id=\"" ToId(slug) "\" title=\"" ToHtml(title) "\"/>" > file
  print "<section title=\"" ToHtml(title) "\" epub:type=\"bodymatter chapter\">" > file
  print "<h1 class=\"chapter-subtitle\"><strong>" ToHtml(title) "</strong></h1>" > file
  print "<p class=\"note-title\">" date sterms "</p>" > file
  print content > file
  print "</section>" > file
}


function FormatContent(content)
{
  prefix = "\n"
  suffix = ""
  paragraph = 1

  do {
    #todo: test
    #if( index(content, "```") == 1 ) {
    #  content = ""
    #  if( contentState["```"] ) {
    #    prefix = "</pre>"
    #    contentState["```"] = 0
    #  } else {
    #    contentState["```"] = 1
    #    prefix = prefix "<pre>"
    #  }
    #  break
    #} else if( contentState["```"] ) {
    #  break
    #}

    #todo: test
    #if( content ~ /^ *- */ ) {
    #  content = gensub(/ *- *(.*)/, "\\1", "g", content)
    #  if( ! contentState["-"] ) {
    #    prefix = prefix "<ul>"
    #    contentState["-"] = 1
    #  }
    #  prefix = prefix "<li>"
    #  suffix = "</li>" suffix
    #  paragraph = 0
    #} else if ( contentState["-"] ) {
    #    prefix = "</ul>\n"
    #    contentState["-"] = 0
    #}

    #todo: test
    #if( content ~ /^ +/ ) {
    #  sub(/^ /, "", content)
    #  if( ! contentState[" "] ) {
    #    prefix = prefix "<pre>"
    #    contentState[" "] = 1
    #  }
    #  break
    #} else if ( contentState[" "] ) {
    #    prefix = "</pre>\n"
    #    contentState[" "] = 0
    #}

    #todo: test
    #if( content ~ /^#+ / ) {

    #  if( content ~ /^# / ) {
    #    headerLevel = 2
    #  } else if( content ~ /^## / ) {
    #    headerLevel = 3
    #  } else if( content ~ /^### / ) {
    #    headerLevel = 4
    #  } else {
    #    headerLevel = 5
    #  }
    #  gsub(/^#+ /, "", content)

    #  prefix = prefix "<h" headerLevel ">"
    #  suffix = "</h" headerLevel ">" suffix
    #  paragraph = 0
    #}

    if( content ~ /^\[[^]]+\]:/ ) {
      endName = index(content, ":")
      name = substr(content, 2, endName - 3)
      link = substr(content, endName + 2)

      if( link ~ /[a-z]:\/\// ) {
        link = "<a href=\"" link "\">" name "</a>"
      }
      else {
        link = gensub(/(.*)/, "index.xhtml", "g", link)
        link = "<a href=\"" link "\">" name "</a>"
      }

      links[name] = link
      content = ""
      break
    }

    if( match($0, /^!\[([^]]*)\]\( *([^" )]+) *"?([^"]*)?"?\)/, a) ) {
      newImage = a[2]
      g_postImages[newImage] = newImage
      content = "<img src=\"img/" a[2] "\"/>"
      break
    }

    gsub(/&/, "&amp;", content)
    gsub(/</, "\\&lt;", content)
    gsub(/>/, "\\&gt;", content)
    content = gensub(/\[([^&]]+)\]\(([^)]+)\)/, "<a href=\"\\2\">\\1</a>", "g", content)

    if( paragraph ) {
      content = "<p>" content "</p>"
    }

  } while( 0 )

  return prefix content suffix
}

$1 == "metadata_slug" { Index[$2]["link"] = $3 ; next }

/^# / {
  if( content ) {
    WriteToHtml()
    content = ""
    draftContent = ""
    slug = ""
    tags = ""
    draft = 0
  }
  title = substr($0, 3)
  next
}

/^\[[^]]+\]:/ {
  if( match($0, /^\[([^]]+)\]: *([^" ]+) *"?([^"]+)?"?/, a) ) {
    if( a[1] == "date" ) {
      date = a[3]
      chapter = substr(date, 1, 7)
      chapters[chapter] = chapter
    } else if( a[1] == "tags" ) {
      tags = a[3]
    } else if( a[1] == "draft" ) {
      draft = 1
    }
  }
}

/^:/ {
  #print "header " $0
  if( $1 == ":slug:" ) {
    slug = $2
  }
  else if( $1 == ":date:" ) {
    date = $2
    chapter = substr(date, 1, 7)
    chapters[chapter] = chapter
  }
  else if( $1 == ":tags:" ) {
    tagidx = 2
    while( tagidx <= NF ) {
      tag = $tagidx
      if( tag != "null" ) {
        tags = tags " " tag
      }
      ++tagidx
    }
  }
  else if( $1 == ":draft:" ) {
    draft = 1
  }
}

/^[^:]/ {
  draftContent = draftContent "\n\n" $0
  newContent = FormatContent($0)
  if( content ) {
    content = content newContent
  } else {
    summary = $0
    content = newContent
  }
}

function FlushPostsPages()
{
  for( f in files ) {
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
  print "<dc:title id=\"title\">Blogue do Caloni: Programação, Depuração, Transpiração</dc:title>" > package
  print "<dc:creator>Wanderley Caloni</dc:creator>" > package
  print "<dc:publisher>Caloni</dc:publisher>" > package
  print "<dc:rights>Copyright 404 Not Found</dc:rights>" > package
  print "<dc:identifier id=\"p0000000000000\">0000000000000</dc:identifier>" > package
  print "<dc:source id=\"src-id\">urn:isbn:0000000000000</dc:source>" > package
  print "<dc:language>pt-BR</dc:language>" > package
  "date" | getline currentDate
  print "<meta property=\"dcterms:modified\">" currentDate "</meta>" > package
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
  for( term in terms ) {
    print "<item id=\"toc_" term "\" href=\"toc_" term ".xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  }
  print "<item id=\"index\" href=\"index.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( chapter in chapters ) {
    print "<item id=\"" ToId(chapter) "\" href=\"" ToId(chapter) ".xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  }
  totalImages = 0
  for( image in g_postImages ) {
    if( index(image, "jpg") || index(image, "jpeg") ) {
      print "<item id=\"img-id-" ++totalImages "\" href=\"" image "\" media-type=\"image/jpeg\"/>" > package
    } else if( index(image, "gif") ) {
      print "<item id=\"img-id-" ++totalImages "\" href=\"" image "\" media-type=\"image/gif\"/>" > package
    } else if( index(image, "png") ) {
      print "<item id=\"img-id-" ++totalImages "\" href=\"" image "\" media-type=\"image/png\"/>" > package
    }
  }
  print "</manifest>" > package
  print "<spine toc=\"ncx1\">" > package
  print "<itemref idref=\"cover\" linear=\"yes\"/>" > package
  print "<itemref idref=\"titlepage\" linear=\"yes\"/>" > package
  print "<itemref idref=\"toc\" linear=\"yes\"/>" > package
  for( term in terms ) {
    print "<itemref linear=\"yes\" idref=\"toc" ToId(term) "\"/>" > package
  }
  for( chapter in chapters ) {
    print "<itemref linear=\"yes\" idref=\"" ToId(chapter) "\"/>" > package
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
  #for( chapter in chapters ) {
  #  print "<navPoint playOrder=\"" ++playOrder "\" id=\"" ToId(chapter) "\"><navLabel><text>" ToHtml(chapter) "</text></navLabel>\
  #    <content src=\"" ToId(chapter) ".xhtml\"/></navPoint>" > tocncx
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
  for( chapter in chapters ) {
    year = substr(chapter, 1, 4)
    mon = substr(chapter, 6, 2)
    if( year != lastyear ) {
      if( lastyear != "2001" ) {
        print "</p>" > tocxhtml
      }
      print "<p id=\"" ToId(chapter) "\" class=\"toc\"><strong>" year "</strong>" > tocxhtml
      lastyear = year
    }
    print "<a href=\"" ToId(chapter) ".xhtml\"> " \
      ToHtml(mon) " </a>" > tocxhtml
  }
  print "</p>" > tocxhtml
  print "<p id=\"indx-2\" class=\"tocb\"><strong>Drafts</strong></p>" > tocxhtml
  for( title in draftToSlug ) {
    slug = draftToSlug[title]
    print "<p id=\"" ToId(slug) "\" class=\"toc\"><a href=\"" ToId(titleToChapter[title]) ".xhtml#" ToId(slug) "\">" title "</a></p>" > tocxhtml
  }
  print "<a id=\"piv\"></a>" > tocxhtml
  print "</div>" > tocxhtml
  print "</body>" > tocxhtml
  print "</html>" > tocxhtml
}

function FlushTagsPage()
{
  for( term in terms ) {
    tocxhtml = "public\\book\\EPUB\\toc_" term ".xhtml"
    print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > tocxhtml
    print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > tocxhtml
    print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > tocxhtml
    print "<title>" term "</title>" > tocxhtml
    print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > tocxhtml
    print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > tocxhtml
    print "</head>" > tocxhtml
    print "<body>" > tocxhtml
    print "<div class=\"body\">" > tocxhtml
    print "<h1 class=\"toc-title\">" term "</h1>" > tocxhtml
    print "<ul>" > tocxhtml
    for( tit in terms[term] ) {
      print "<li><a href=\"" ToId(titleToChapter[tit]) ".xhtml#" ToId(titleToSlug[tit]) "\">" ToHtml(tit) "</a></li>" > tocxhtml
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
  #for( chapter in chapters ) {
  #  print "<li><a href=\"" ToId(chapter) ".xhtml\">" ToHtml(chapter) "</a></li>" > ncxhtml
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
  for( e in entries ) {
    split(e, letterAndTitle, SUBSEP)
    letter = ToLetter(letterAndTitle[1])
    title = letterAndTitle[2]
    if( letters[letter] == "" ) {
      letters[letter] = "<h3 id=\"" ToId(letter) "\" class=\"groupletter\">" ToHtml(letter) "</h3>\n"\
        "<ul class=\"indexlevel1\">"
    }
    letters[letter] = letters[letter] "<li epub:type=\"index-entry\" class=\"indexhead1\" id=\"mh" currid++ "\">"\
      "<a href=\"" ToId(titleToChapter[title]) ".xhtml#" ToId(titleToSlug[title]) "\">" ToHtml(title) "</a></li>\n"
  }
  for( letter in letters ) {
    print "<a href=\"#" ToId(letter) "\">" letter "</a>" > indexx
  }
  print "<h3 id=\"toc" ToId(term) "\" class=\"groupletter\">Terms</h3>\n"\
    "<ul class=\"indexlevel1\">" > indexx
  for( term in terms ) {
    tocxhtml = "public\\book\\EPUB\\toc_" term ".xhtml"
    print "<li epub:type=\"index-entry\" class=\"indexhead1\" id=\"mh" currid++ "\">"\
      "<a href=\"toc" ToId(term) ".xhtml\">" ToHtml(term) "</a></li>\n" > indexx
  }
  print "</ul>" > indexx
  for( letter in letters ) {
    print letters[letter] > indexx
    print "</ul>" > indexx
  }
  print "</section>" > indexx
  print "</body>" > indexx
  print "</html>" > indexx
}

END {
  if( content ) {
    WriteToHtml()
    content = ""
    draftContent = ""
  }

  FlushPostsPages()
  FlushPackage()
  FlushTocNcx()
  FlushTocPage()
  FlushTagsPage()
  FlushNcx()
  FlushIndexPage()
}

