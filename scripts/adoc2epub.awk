function toid(str)
{
  gsub(/-/, "", str);
  str = "_" str;
  return str;
}

function tohtml(str)
{
  gsub(/&/, "&amp;", str);
  gsub(/</, "\\&lt;", str);
  gsub(/>/, "\\&gt;", str);
  return str;
}

function writepost()
{
  file = "public\\epub_awk\\EPUB\\" date ".xhtml"
  if( ! (date in files) ) {
    print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > file
    print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > file
    print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > file
    print "<title>\"" tohtml(date) "\"</title>" > file
    print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > file
    print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > file
    print "</head>" > file
    print "<body>" > file
    print "<div class=\"body\">" > file
    print "<span epub:type=\"pagebreak\" id=\"" toid(date) "\" title=\"" tohtml(date) "\"/>" > file
    files[date] = date;
  }
  print "<span epub:type=\"pagebreak\" id=\"" toid(slug) "\" title=\"" tohtml(title) "\"/>" > file
  print "<section title=\"" tohtml(title) "\" epub:type=\"bodymatter chapter\">" > file
  print "<h1 class=\"chapter-title\"><strong>\"" tohtml(title) "\"</strong></h1>" > file
  print content > file
  print "</section>" > file
}

/^= / {
  if( content ) {
    writepost();
    content = "";
  }
  title = substr($0, 3);
  entries[substr(title, 1, 1),title] = title;
  ++postCount;
}

/^:/ {
  #print "header " $0
  if( $1 == ":slug:" ) {
    slug = $2
    slugs[slug]["slug"] = slug;
    slugs[slug]["title"] = title;
    titleToSlug[title] = slug;
  }
  else if( $1 == ":date:" ) {
    date = substr($2, 2, 7)
    date = substr(date, 1, 4) substr(date, 6, 2)
    dates[date] = date;
    slugs[slug]["date"] = date;
    titleToDate[title] = date;
  }
}

/^[^=:]/ {
  gsub(/&/, "&amp;");
  gsub(/</, "\\&lt;");
  gsub(/>/, "\\&gt;");
  content = content "\n<p>" $0 "</p>";
}

END {
  if( content ) {
    writepost();
    content = "";
  }

  for( f in files ) {
    file = "public\\epub_awk\\EPUB\\" f ".xhtml"
    print "</div>" > file
    print "</body>" > file
    print "</html>" > file
  }

  package = "public\\epub_awk\\EPUB\\package.opf"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > package
  print "<package xmlns=\"http://www.idpf.org/2007/opf\" version=\"3.0\" unique-identifier=\"p9781573878296\">" > package
  print "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\">" > package
  print "<dc:title id=\"title\">Blogue do Caloni: Programação, Depuração, Transpiração</dc:title>" > package
  print "<dc:creator>Fred Leise</dc:creator>" > package
  print "<dc:creator>Kate Mertes</dc:creator>" > package
  print "<dc:creator>Nan Badgett</dc:creator>" > package
  print "<dc:publisher>Information Today, Inc.</dc:publisher>" > package
  print "<dc:rights>Copyright &#x00A9; 2008 by American Society of Indexers, Inc.</dc:rights>" > package
  print "<dc:rights>All rights reserved. No part of this book may be reproduced in any form by any electronic or mechanical means, \
    including information storage and retrieval systems, without the express written permission from the Society except by a reviewer \
    who may quote brief passages for a review.</dc:rights>" > package
  print "<dc:identifier id=\"p9781573878296\">9781573878296</dc:identifier>" > package
  print "<dc:source id=\"src-id\">urn:isbn:9781573878296</dc:source>" > package
  print "<dc:language>en-US</dc:language>" > package
  print "<meta property=\"dcterms:modified\">2014-09-06T11:00:00Z</meta>" > package
  print "</metadata>" > package
  print "<manifest>" > package
  print "<item id=\"cover\" href=\"cover.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  print "<item id=\"cover-image\" properties=\"cover-image\" href=\"images/cover.jpg\" media-type=\"image/jpeg\"/>" > package
  print "<item id=\"style\" href=\"css/stylesheet.css\" media-type=\"text/css\"/>" > package
  print "<item id=\"ncx\" properties=\"nav\" href=\"ncx.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  print "<item id=\"ncx1\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\"/>" > package
  print "<item id=\"page-template\" href=\"css/page-template.xpgt\" media-type=\"application/adobe-page-template+xml\"/>" > package
  print "<item id=\"copyright\" href=\"copyright.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  print "<item id=\"titlepage\" href=\"titlepage.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  print "<item id=\"toc\" href=\"toc.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  print "<item id=\"index\" href=\"index.xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( date in dates ) {
    print "<item id=\"" toid(date) "\" href=\"" date ".xhtml\" media-type=\"application/xhtml+xml\"/>" > package
  }
  print "</manifest>" > package
  print "<spine toc=\"ncx1\">" > package
  print "<itemref idref=\"cover\" linear=\"yes\"/>" > package
  print "<itemref idref=\"titlepage\" linear=\"yes\"/>" > package
  print "<itemref idref=\"copyright\" linear=\"yes\"/>" > package
  print "<itemref idref=\"toc\" linear=\"yes\"/>" > package
  for( date in dates ) {
    print "<itemref linear=\"yes\" idref=\"" toid(date) "\"/>" > package
  }
  print "<itemref linear=\"yes\" idref=\"index\"/>" > package
  print "<itemref linear=\"yes\" idref=\"ncx\"/>" > package
  print "</spine>" > package
  print "</package>" > package

  tocncx = "public\\epub_awk\\EPUB\\toc.ncx"
  print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > tocncx
  print "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"en-US\">" > tocncx
  print "<head>" > tocncx
  print "<meta name=\"dtb:uid\" content=\"9781573878296\"/>" > tocncx
  print "<meta name=\"dtb:depth\" content=\"1\"/>" > tocncx
  print "<meta name=\"dtb:totalPageCount\" content=\"0\"/>" > tocncx
  print "<meta name=\"dtb:maxPageNumber\" content=\"0\"/>" > tocncx
  print "</head>" > tocncx
  print "<docTitle><text>Blogue do Caloni: Programação, Depuração, Transpiração</text></docTitle>" > tocncx
  print "<docAuthor><text>Fred Leise, Kate Mertes, and Nan Badgett</text></docAuthor>" > tocncx
  print "<navMap>" > tocncx
  print "<navPoint id=\"cover\" playOrder=\"1\"><navLabel><text>Cover</text></navLabel><content src=\"cover.xhtml\"/></navPoint>" > tocncx
  print "<navPoint id=\"copyright\" playOrder=\"2\"><navLabel><text>Copyright</text></navLabel>\
    <content src=\"copyright.xhtml\"/></navPoint>" > tocncx
  print "<navPoint id=\"toc\" playOrder=\"3\"><navLabel><text>Contents</text></navLabel><content src=\"toc.xhtml\"/></navPoint>" > tocncx
  playOrder = 3
  for( date in dates ) {
    print "<navPoint playOrder=\"" ++playOrder "\" id=\"" toid(date) "\"><navLabel><text>" tohtml(date) "</text></navLabel>\
      <content src=\"" date ".xhtml\"/></navPoint>" > tocncx
  }
  print "</navMap>" > tocncx
  print "</ncx>" > tocncx

  tocxhtml = "public\\epub_awk\\EPUB\\toc.xhtml"
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
  print "<p class=\"toca\"><a href=\"copyright.xhtml\"><strong>Copyright</strong></a></p>" > tocxhtml
  print "<p id=\"indx-1\" class=\"toca\"><a href=\"index.xhtml\"><strong>Index</strong></a></p>" > tocxhtml
  for( date in dates ) {
    print "<p id=\"" toid(date) "\" class=\"toc\"><a href=\"" date ".xhtml\"><strong>" \
      tohtml(date) "</strong></a></p>" > tocxhtml
  }
  print "<a id=\"piv\"></a>" > tocxhtml
  print "</div>" > tocxhtml
  print "</body>" > tocxhtml
  print "</html>" > tocxhtml

  ncxhtml = "public\\epub_awk\\EPUB\\ncx.xhtml"
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
  print "<li><a href=\"copyright.xhtml\">Copyright</a></li>" > ncxhtml
  for( date in dates ) {
    print "<li><a href=\"" date ".xhtml\">" tohtml(date) "</a></li>" > ncxhtml
  }
  print "<li><a href=\"index.xhtml\">Index</a></li>" > ncxhtml
  print "</ol>" > ncxhtml
  print "</nav>" > ncxhtml
  print "" > ncxhtml
  print "<nav epub:type=\"landmarks\">" > ncxhtml
  print "<h2>Landmarks</h2>" > ncxhtml
  print "<ol epub:type=\"list\">" > ncxhtml
  print "<li><a epub:type=\"cover\" href=\"cover.xhtml\">Cover</a></li>" > ncxhtml
  print "<li><a epub:type=\"titlepage\" href=\"titlepage.xhtml\">Title Page</a></li>" > ncxhtml
  print "<li><a epub:type=\"frontmatter\" href=\"copyright.xhtml\">Copyright</a></li>" > ncxhtml
  print "<li><a epub:type=\"toc\" href=\"toc.xhtml\">Table of Contents</a></li>" > ncxhtml
  for( date in dates ) {
    print "<li><a epub:type=\"bodymatter\" href=\"" date ".xhtml\">" \
      tohtml(date) "</a></li>" > ncxhtml
  }
  print "<li><a epub:type=\"backmatter\" href=\"index.xhtml\">Index</a></li>" > ncxhtml
  print "</ol>" > ncxhtml
  print "</nav>" > ncxhtml
  print "" > ncxhtml
  print "<nav epub:type=\"page-list\" hidden=\"\">" > ncxhtml
  print "<h2>Page Navigation</h2>" > ncxhtml
  print "<ol epub:type=\"list\">" > ncxhtml
  print "<li class=\"section-label-rw\" title=\"Inside cover\"><a href=\"cover.xhtml#pia\">Inside Cover</a></li>" > ncxhtml
  print "<li class=\"section-label-rw\" title=\"Title Page\"><a href=\"titlepage.xhtml#pi\">i</a></li>" > ncxhtml
  print "<li class=\"section-label-rw\" title=\"Copyright\"><a href=\"copyright.xhtml#pii\">ii</a></li>" > ncxhtml
  print "<li class=\"section-label-rw\" title=\"Table of Contents\"><a href=\"toc.xhtml#piii\">iii</a></li>" > ncxhtml
  print "<li><a href=\"toc.xhtml#piv\">iv</a></li>" > ncxhtml
  print "<li class=\"section-label-rw\" title=\"Index\"><a epub:type=\"index\" href=\"index.xhtml#p139\">139</a></li>" > ncxhtml
  print "</ol>" > ncxhtml
  print "</nav>" > ncxhtml
  print "</body>" > ncxhtml
  print "</html>" > ncxhtml

  indexx = "public\\epub_awk\\EPUB\\index.xhtml"
  print "<!DOCTYPE html>" > indexx
  print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\" xml:lang=\"en-US\" lang=\"en-US\">" > indexx
  print "<head>" > indexx
  print "<meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > indexx
  print "<title>Blogue do Caloni: Programação, Depuração, Transpiração</title>" > indexx
  print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > indexx
  print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > indexx
  print "</head>" > indexx
  print "<body>" > indexx
  print "<h1 class=\"index-title\"><span epub:type=\"pagebreak\" id=\"p139\" title=\"139\"/><a href=\"toc.xhtml#indx-1\"><strong>Index</strong></a></h1>" > indexx
  print "<section epub:type=\"index-group\" id=\"letters\">" > indexx
  PROCINFO["sorted_in"] = "@ind_str_asc"
  currid = 2;
  for( e in entries ) {
    split(e, letterAndTitle, SUBSEP)
    letter = letterAndTitle[1]
    title = letterAndTitle[2]
    if( letters[letter] == "" ) {
      letters[letter] = "<h3 id=\"_" letter "\" class=\"groupletter\">" tohtml(letter) "</h3>\n"\
        "<ul class=\"indexlevel1\">";
    }
    letters[letter] = letters[letter] "<li epub:type=\"index-entry\" class=\"indexhead1\" id=\"mh" currid++ "\">"\
      "<a href=\"" titleToDate[title] ".xhtml#" toid(titleToSlug[title]) "\">" tohtml(title) "</a></li>\n";
  }
  for( letter in letters ) {
    print "<a href=\"#_" letter "\">" letter "</a>" > indexx
  }
  for( letter in letters ) {
    print letters[letter] > indexx
    print "</ul>" > indexx
  }
  print "</section>" > indexx
  print "</body>" > indexx
  print "</html>" > indexx
}

