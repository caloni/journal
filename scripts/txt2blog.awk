function toid(str)
{
  gsub(/-/, "", str)
  gsub(/#/, "sharp", str)
  str = "_" str
  return str
}

function tohtml(str)
{
  gsub(/&/, "&amp;", str)
  gsub(/</, "\\&lt;", str)
  gsub(/>/, "\\&gt;", str)
  return str
}

function isnumeric(x, f)
{
    switch (typeof(x)) {
    case "strnum":
    case "number":
        return 1
    case "string":
        return (split(x, f, " ") == 1) && (typeof(f[1]) == "strnum")
    default:
        return 0
    }
}

function toletter(str)
{
  if( isnumeric(str) ) return "#"
  str = toupper(str)
  conv = convertLetters[str]
  if( conv != "" ) return conv
  return str
}

function writepost()
{
  ++postCount
  entries[substr(title, 1, 1),title] = title
  split(categories, scategories)
  sterms = ""
  for( c in scategories ) {
    terms[scategories[c]][title] = title
    sterms = sterms " " scategories[c]
  }
  split(tags, stags)
  for( t in stags ) {
    terms[stags[t]][title] = title
    sterms = sterms " " stags[t]
  }
  slugs[slug]["slug"] = slug
  slugs[slug]["title"] = title
  slugs[slug]["date"] = date
  titleToSlug[title] = slug
  titleToChapter[title] = chapter
  if( draft ) {
    draftToSlug[title] = slug
  }
  file = "public\\blog_awk\\" chapter ".html"
  if( ! (chapter in files) ) {
    print "<html>" > file
    print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > file
    print "<title>" tohtml(chapter) "</title>" > file
    print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > file
    print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > file
    print "</head>" > file
    print "<body>" > file
    print "<div class=\"body\">" > file
    print "<span id=\"" toid(chapter) "\" title=\"" tohtml(chapter) "\"/>" > file
    print "<h1 class=\"chapter-title\"><strong>" tohtml(chapter) "</strong></h1>" > file
    files[chapter] = chapter
  }
  print "<span id=\"" toid(slug) "\" title=\"" tohtml(title) "\"/>" > file
  print "<section title=\"" tohtml(title) "\">" > file
  print "<h1 class=\"chapter-subtitle\"><strong>" tohtml(title) "</strong></h1>" > file
  print "<p class=\"note-title\">" date sterms "</p>" > file
  print content > file
  print "</section>" > file
}

/^= / {
  if( content ) {
    writepost()
    content = ""
    slug = ""
    tags = ""
    categories = ""
    draft = 0
  }
  title = substr($0, 3)
}

/^:/ {
  #print "header " $0
  if( $1 == ":slug:" ) {
    slug = $2
  }
  else if( $1 == ":date:" ) {
    date = substr($2, 2, 10)
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
  else if( $1 == ":categories:" ) {
    catidx = 2
    while( catidx <= NF ) {
      cat = $catidx
      if( cat != "null" ) {
        categories = categories " " cat
      }
      ++catidx
    }
  }
  else if( $1 == ":draft:" ) {
    draft = 1
  }
}

/^[^=:]/ {
  gsub(/&/, "&amp;")
  gsub(/</, "\\&lt;")
  gsub(/>/, "\\&gt;")
  content = content "\n<p>" $0 "</p>"
}

BEGIN {
  convertLetters["Á"] = "A"
  convertLetters["À"] = "A"
  convertLetters["Â"] = "A"
  convertLetters["Ã"] = "A"
  convertLetters["É"] = "E"
  convertLetters["Ê"] = "E"
  convertLetters["Ê"] = "E"
  convertLetters["Ô"] = "O"
  convertLetters["Õ"] = "O"
  convertLetters["Ó"] = "O"
  convertLetters["Ú"] = "U"
  convertLetters["Í"] = "I"
  convertLetters["Ï"] = "I"
  convertLetters["("] = "#"
  convertLetters[")"] = "#"
  convertLetters["'"] = "#"
  convertLetters["\""] = "#"
}

END {
  if( content ) {
    writepost()
    content = ""
  }

  for( f in files ) {
    file = "public\\blog_awk\\" f ".html"
    print "</div>" > file
    print "</body>" > file
    print "</html>" > file
  }

  PROCINFO["sorted_in"] = "@ind_num_asc"
  monthshtml = "public\\blog_awk\\months.html"
  print "<html>" > monthshtml
  print "<head><meta http-equiv=\"default-style\" content=\"text/html; charset=utf-8\"/>" > monthshtml
  print "<title>Blogue do Caloni: Programação, Depuração, Transpiração</title>" > monthshtml
  print "<link rel=\"stylesheet\" href=\"css/stylesheet.css\" type=\"text/css\" />" > monthshtml
  print "<link rel=\"stylesheet\" href=\"css/page-template.xpgt\" type=\"application/adobe-page-template+xml\" />" > monthshtml
  print "</head>" > monthshtml
  print "<body>" > monthshtml
  print "<div class=\"body\">" > monthshtml
  print "<a id=\"piii\"></a>" > monthshtml
  lastyear = "2001"
  for( chapter in chapters ) {
    year = substr(chapter, 1, 4)
    mon = substr(chapter, 6, 2)
    if( year != lastyear ) {
      if( lastyear != "2001" ) {
        print "</p>" > monthshtml
      }
      print "<p id=\"" toid(chapter) "\" class=\"toc\"><strong>" year "</strong>" > monthshtml
      lastyear = year
    }
    print "<a href=\"" chapter ".html\"> " \
      tohtml(mon) " </a>" > monthshtml
  }
  print "</p>" > monthshtml
  print "<a id=\"piv\"></a>" > monthshtml
  print "</div>" > monthshtml
  print "</body>" > monthshtml
  print "</html>" > monthshtml
}

