BEGIN {
}

/^= / {
  if( content ) {
    file = "public\\epub_awk\\EPUB\\" slug ".xhtml"
    print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > file
    print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">" > file
    print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > file
    print "<title>" title "</title>" > file
    print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > file
    print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > file
    print "</head>" > file
    print "<body>" > file
    print "<div class=\"body\"><span epub:type=\"pagebreak\" id=\"p1\" title=\"1\"/>" > file
    print "<section title=\"" title "\" epub:type=\"bodymatter chapter\">" > file
    print "<h1 class=\"chapter-title\"><a id=\"pinpt2\"/><a href=\"toc.xhtml#chap-1\"><strong>" title "</strong></a></h1>" > file
    print content > file
    print "</section>" > file
    print "</div>" > file
    print "</body>" > file
    print "</html>" > file
    content = ""
  }
  title = substr($0, 3);
  #print "title: " title
  ++postCount
}

/^:/ {
  #print "header " $0
  if( $1 == ":slug:" ) slug = $2
}

/^[^=:]/ {
  #print "content " $0
  content = content "\n<p>" $0 "</p>"
}

END {
}

