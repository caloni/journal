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

function writetophtml(file, backLink)
{
  print "<!DOCTYPE html>" > file
  print "<html lang=\"en-us\" dir=\"ltr\" itemscope itemtype=\"http://schema.org/Article\">" > file
  print "<head>" > file
  print "<meta charset=\"utf-8\" />" > file
  print "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/>" > file
  print "<title>Blogue do Caloni</title>" > file
  print "<meta name=\"author\" content=\"\" />" > file
  print "<meta name=\"generator\" content=\"Hugo 0.110.0\">" > file
  print "<meta property=\"og:title\" content=\"Blogue do Caloni\"/>" > file
  print "<meta property=\"og:type\" content=\"website\"/>" > file
  print "<meta property=\"og:url\" content=\"http://www.caloni.com.br/\"/>" > file
  print "<meta property=\"og:image\" content=\"/img/author.jpg\"/>" > file
  print "<meta property=\"og:description\" content=\"\"/>" > file
  print "<link href=\"/index.xml\" rel=\"feed\" type=\"application/rss+xml\" title=\"Blogue do Caloni\"/>" > file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/custom.css\"/>" > file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/jquery-ui.css\"/>" > file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/board-min.css\"/>" > file
  print "<script src=\"/js/jquery-1.10.2.js\"></script>" > file
  print "<script src=\"/js/jquery-ui.js\"></script>" > file
  print "<script src=\"/js/pgnyui.js\"></script>" > file
  print "<script src=\"/js/pgnviewer.js\"></script>" > file
  print "<script>" > file
  print "var quick_search_posts = [ \"posts.html\", \"months.html\" ];" > file
  print "</script>" > file
  print "<script src=\"/js/quick_search.js\"></script>" > file
  print "<script src=\"/js/list.js\"></script>" > file
  print "<link rel=\"icon\" href=\"/img/favicon.ico\"/>" > file
  print "</head>" > file
  print "<body style=\"min-height:100vh;display:flex;flex-direction:column\">" > file
  print "<nav class=\"navbar has-shadow is-white\"" > file
  print "role=\"navigation\" aria-label=\"main navigation\">" > file
  print "<div class=\"container\">" > file
  print "<div class=\"navbar-brand\">" > file
  print "<pre><span style=\"font-size: 3px; margin: 0; display: block;\">" > file
  print "" > file
  print "&amp;*/. .*%@@@@@@@@@@@@&amp;/    , &amp;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" > file
  print "@@@@@@@@@#,*@@@@%,*&amp;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" > file
  print "@@@@@@@@@@@@./@.(@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" > file
  print "@@@@**#./((,*, *./((*,#,&amp;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" > file
  print "@@@@@.#@@%&amp;@@* (@@%&amp;@@/#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" > file
  print "@@@@@,#@@&amp;&amp;@@, (@@&amp;&amp;@@*#@@@@@@@@@@@@@#/,,.         ..,/#&amp;@@@@@@@@@@@@@@@@@@@@@@@" > file
  print "@@@@@&amp;, .//      .//  .%@@@@@@&amp;#.  .,****.,*************,.  .(&amp;@@@@@@@@@@@@@@@@@" > file
  print "@@@*                     ,@/  .**,,,*********,   ,***,   ****    *@@@@@@@@@@@@@@" > file
  print "@#                         ,***      .******       ,***,*****,      *&amp;@@@@@@@@@@" > file
  print "&amp;                           ,**      .******.     .******************  (@@@@@@@@" > file
  print "                             ****..,*************************,    ,***   (@@@@@@" > file
  print "@@@@#,,,,,,,,,,,,,,,,,,,,*********,   ,**************,  .***,      .**. .  %@@@@" > file
  print "@@@@%                   .***,.,****,.,***,     ,****      ***.     ,******, /@@@" > file
  print "@@@@@&amp;.                ,**       *******,       *****,  .******************, *@@" > file
  print "@@@@@@@&amp;,            ,****      .********,.   .*************,  ,*****,    ,** /@" > file
  print "@@@@@@@@@@@@&amp;/ .****,    ************.   ****************************      **. #" > file
  print "@@@@@@@@@@@@@, *****,    ,***********    .******,   ,****.   .*****,***,,***** ," > file
  print "@@@@@@@@@@@@&amp;..**************,  ,***********************       ,*,    ********.." > file
  print "@@@@@@@@@@@@&amp; ,**.    ,******.  .*******.    .**********,     .**********. .**, " > file
  print "@@@@@@@@@@@@% ,*       ,***************.      .*****************************,   " > file
  print "@@@@@@@@@@@@@&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;@,(&amp;&amp;&amp;&amp;@,(&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;#*@&amp;&amp;&amp;@*#&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;&amp;" > file
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@(.#@@@@(.#@@@@@@@@@@@@@@@@@@@@@#**@@@@(.#@@@@@@@@@@@" > file
  print "" > file
  print "</span></pre>" > file
  print "&nbsp;" > file
  print "<a class=\"navbar-item\" title=\"Who is this?\" href=\"" backLink "\">" > file
  print "<div class=\"is-4\">Blogue do Caloni</div>" > file
  print "</a>" > file
  print "</div>" > file
  print "</div>" > file
  print "</nav>" > file
  print "<div class=\"container\">" > file
  print "<div class=\"column\">" > file
  print "<div style=\"min-height:56vh\">" > file
  print "<div style=\"padding-bottom: 1em;\"></div>" > file
}

function writebottomhtml(file)
{
  print "</div>" > file
  print "</div>" > file
  print "</section>" > file
  print "<footer class=\"footer\">" > file
  print "<div class=\"container\">" > file
  print "<span style=\"float: right;\"><i style=\"font-size: small;\">Blogue do Caloni" > file
  print "<a href=\"https://github.com/Caloni/blog\">2023-08-02 18:52:07 -0300</a>" > file
  print "</i></span>" > file
  print "</div>" > file
  print "</footer>" > file
  print "</body>" > file
  print "</html>" > file
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
  slugs[slug]["content"] = content
  slugs[slug]["summary"] = summary
  titleToSlug[title] = slug
  titleToChapter[title] = chapter
  if( draft ) {
    draftToSlug[title] = slug
  }
  file = "public\\blog_awk\\" chapter ".html"
  if( ! (chapter in files) ) {
    writetophtml(file, "index.html")
    print "<h1 class=\"chapter-title\"><strong>" tohtml(chapter) "</strong></h1>" > file
    files[chapter] = chapter
  }
  print "<span id=\"" toid(slug) "\" title=\"" tohtml(title) "\"/>" > file
  print "<section title=\"" tohtml(title) "\">" > file
  print "<h1 class=\"chapter-subtitle\"><strong><a href=\"" chapter ".html#" toid(slug) "\">" tohtml(title) "</a></strong></h1>" > file
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
  if( content ) {
    content = content "\n<p>" $0 "</p>"
  } else {
    summary = $0
    content = "\n<p>" $0 "</p>"
  }
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
    writebottomhtml(file)
  }

  PROCINFO["sorted_in"] = "@ind_num_asc"
  monthshtml = "public\\blog_awk\\months.html"
  writetophtml(monthshtml, "index.html")
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
  writebottomhtml(monthshtml)

  postshtml = "public\\blog_awk\\posts.html"
  writetophtml(postshtml, "index.html")
  PROCINFO["sorted_in"] = "@ind_str_asc"
  print "<input type=\"text\" name=\"filter\" value=\"\" id=\"filter\" placeholder=\"enter to select\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > postshtml
  print "<button id=\"filterbutton\" style=\"font-size: 1rem;\" onclick=\"ApplyFilter($('#filter').val());\">select</button>" > postshtml
  print "<button id=\"removebutton\" style=\"font-size: 1rem;\" onclick=\"ApplyNotFilter($('#filter').val());\">remove</button>" > postshtml
  print "<button id=\"randombutton\" style=\"font-size: 1rem;\" onclick=\"window.location = randomPost;\">random</button>" > postshtml
  print "<div><big><b><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></b></big></div>" > postshtml
  print "<table class=\"sortable\" style=\"width: 100%;\">" > postshtml
  for( e in entries ) {
    split(e, letterAndTitle, SUBSEP)
    title = letterAndTitle[2]
    print "<tr><td><b><a href=\"" titleToChapter[title] ".html#" toid(titleToSlug[title]) "\">" tohtml(title) "</a></b>" > postshtml
    print "<small><i>" slugs[titleToSlug[title]]["date"] " [term1] [term2] " slugs[titleToSlug[title]]["summary"] "[...]</small></i>" > postshtml
    print "</td></tr>" > postshtml
  }
  print "</table>" > postshtml
  writebottomhtml(postshtml)

  indexhtml = "public\\blog_awk\\index.html"
  writetophtml(indexhtml, "2007-06.html#_about")
  print "<input type=\"text\" name=\"quick_search_name\" value=\"\" id=\"quick_search\" placeholder=\"digite uma url\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > indexhtml
  print "<big><b>? <a id=\"quick_search_search_engine\" href=\"https://duckduckgo.com/?q=site%3Acaloni.com.br\">patopatovai</a></big><small><i>: usar um motor para buscar.</small></i></br>" > indexhtml
  print "<big><b>> <a href=\"posts.html\">posts</a></b></big><small><i>: todos os postes do blogue.</small></i></br>" > indexhtml
  print "<div><big><b><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></b></big></div>" > indexhtml
  print "<table class=\"sortable\" style=\"width: 100%;\">" > indexhtml
  print "</table>" > indexhtml
  writebottomhtml(indexhtml)
}

