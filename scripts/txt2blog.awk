function toid(str)
{
  return str
}


function toslug(str)
{
  gsub(/[ÁÀÂÃáàâã]/, "a", str)
  gsub(/[ÉÊÊéêê]/, "e", str)
  gsub(/[ÔÕÓôõó]/, "o", str)
  gsub(/[Úú]/, "u", str)
  gsub(/[ÍÏíï]/, "i", str)
  gsub(/[#()'",;:-]/, "", str)
  gsub(/ /, "-", str)
  return tolower(str)
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


function writetophtml(file, title, backLink, filter, quickSearch)
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
  print "<script src=\"/js/copy_clipboard.js\"></script>" > file
  print "<script>" > file
  print "var quick_search_posts = [ " > file
  for( qs in quickSearch ) {
    print "\"" quickSearch[qs] "\"," > file
  }
  print " ]; " > file
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
  print "<a class=\"navbar-item\" href=\"" backLink "\">" > file
  print "<div class=\"is-4\">" title "</div>" > file
  print "</a>" > file
  print "</div>" > file
  print "</div>" > file
  print "</nav>" > file
  print "<div class=\"container\">" > file
  print "<div class=\"column\">" > file
  print "<div style=\"min-height:56vh\">" > file
  print "<div style=\"padding-bottom: 1em;\"></div>" > file
  if( filter ) {
    print "<input type=\"text\" name=\"filter\" value=\"\" id=\"filter\" placeholder=\"enter to select\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > file
    print "<button id=\"filterbutton\" style=\"font-size: 1rem;\" onclick=\"ApplyFilter($('#filter').val());\">select</button>" > file
    print "<button id=\"removebutton\" style=\"font-size: 1rem;\" onclick=\"ApplyNotFilter($('#filter').val());\">remove</button>" > file
    print "<button id=\"randombutton\" style=\"font-size: 1rem;\" onclick=\"window.location = randomPost;\">random</button>" > file
    print "<div><big><b><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></b></big></div>" > file
    print "<table class=\"sortable\" style=\"width: 100%;\">" > file
  }
}


function writebottomhtml(file, filter, nextLink, prevLink, version)
{
  if( filter ) {
    print "</table>" > file
  }
  print "<span style=\"float: left;\">" > file
  if( nextLink && nextLink != "" ) {
    print " <a href=\"" nextLink "\"><<</a>" > file
  }
  if( prevLink && prevLink != "" ) {
    print " <a href=\"" prevLink "\">>></a>" > file
  }
  bottomLine = "Blogue do Caloni"
  if( version ) {
    bottomLine = bottomLine " v. " version
  }
  print "</span>" > file
  print "</div>" > file
  print "</div>" > file
  print "</section>" > file
  print "<footer class=\"footer\">" > file
  print "<div class=\"container\">" > file
  print "<span style=\"float: right;\"><i style=\"font-size: small;\">" bottomLine "</i></span>" > file
  print "</div>" > file
  print "</footer>" > file
  print "</body>" > file
  print "</html>" > file
}


function formatContent(content)
{
  prefix = "\n"
  suffix = ""
  paragraph = 1

  do {
    if( index(content, "```") == 1 ) {
      content = ""
      if( contentState["```"] ) {
        prefix = "</pre>"
        contentState["```"] = 0
      } else {
        contentState["```"] = 1
        prefix = prefix "<pre>"
      }
      break
    } else if( contentState["```"] ) {
      break
    }

    if( content ~ /^ *- */ ) {
      content = gensub(/ *- *(.*)/, "\\1", "g", content)
      if( ! contentState["-"] ) {
        prefix = prefix "<ul>"
        contentState["-"] = 1
      }
      prefix = prefix "<li>"
      suffix = "</li>" suffix
      paragraph = 0
    } else if ( contentState["-"] ) {
        prefix = "</ul>\n"
        contentState["-"] = 0
    }

    if( content ~ /^ +/ ) {
      sub(/^ /, "", content)
      if( ! contentState[" "] ) {
        prefix = prefix "<pre>"
        contentState[" "] = 1
      }
      break
    } else if ( contentState[" "] ) {
        prefix = "</pre>\n"
        contentState[" "] = 0
    }

    if( content ~ /^#+ / ) {

      if( content ~ /^# / ) {
        headerLevel = 2
      } else if( content ~ /^## / ) {
        headerLevel = 3
      } else if( content ~ /^### / ) {
        headerLevel = 4
      } else {
        headerLevel = 5
      }
      gsub(/^#+ /, "", content)

      prefix = prefix "<h" headerLevel ">"
      suffix = "</h" headerLevel ">" suffix
      paragraph = 0
    }

    if( content ~ /^\[[^]]+\]:/ ) {
      endName = index(content, ":")
      name = substr(content, 2, endName - 3)
      link = substr(content, endName + 2)
      if( link ~ /{{< ref "/ ) {
        link = gensub(/{{< ref "(.*)" >}}/, "posts.html?q=\\1", "g", link)
      }
      else if( link ~ /{{< ref / ) {
        link = gensub(/{{< ref (.*) >}}/, "posts.html?q=\\1", "g", link)
      }
      else if( link ~ /{{< relref "/ ) {
        link = gensub(/{{< relref "(.*)" >}}/, "posts.html?q=\\1", "g", link)
      }
      link = "<a href=\"" link "\">" name "</a>"
      links[name] = link
      content = ""
      break
    }

    if( index(content, "{{< image src=") == 1 ) {
      image = gensub(/{{< image src="(.*)" >}}/, slug "-\\1", "g", content)
      content = gensub(/{{< image src="(.*)" >}}/, "<img src=\"img/" slug "-\\1\"/>", "g", content)
      break
    }

    gsub(/&/, "&amp;", content)
    gsub(/</, "\\&lt;", content)
    gsub(/>/, "\\&gt;", content)
    content = gensub(/\[([^]]+)\]\(([^)]+)\)/, "<a href=\"\\2\">\\1</a>", "g", content)

    if( paragraph ) {
      content = "<p>" content "</p>"
    }

  } while( 0 )

  return prefix content suffix
}

function writepost(    stags)
{
  ++postCount
  if( slug == "" ) {
    slug = toslug(title)
  }
  entries[date][slug] = title

  if( draft ) {
    draftToSlug[title] = slug
    chapter = "drafts"
    quickSearch["drafts"] = "drafts.html"
  }

  repostTags = ""
  split(g_tags, stags)
  for( t in stags ) {
    g_titlesByTags[stags[t]][title] = title
    g_titlesByTagsAndDates[stags[t]][date][title] = title
    repostTags = repostTags " [" stags[t] "]"
  }
  slugs[slug]["slug"] = slug
  slugs[slug]["title"] = title
  slugs[slug]["date"] = date
  slugs[slug]["content"] = content
  slugs[slug]["summary"] = summary
  slugs[slug]["tags"] = g_tags
  slugs[slug]["image"] = image
  titleToSlug[title] = slug
  titleToChapter[title] = chapter

  if ( repost != "" ) {
    file = "public\\blog\\repost.html"
    if( ! ("repost" in files) ) {
      writetophtml(file, "caloni::repost", "index.html", 1)
      files["repost"] = "repost"
    }
    post = "<tr><td><b><a href=\"" chapter ".html#" toid(slug) "\">" tohtml(title) "</a></b>\n"
    post = post "<small><i>" repost " [" date "] " repostTags " " summary "</small></i>\n"
    post = post "</td></tr>\n"
    g_postsByMonth["repost"][repost] = g_postsByMonth["repost"][repost] "\n" post
  }

  file = "public\\blog\\" chapter ".html"
  if( ! (chapter in files) ) {
    writetophtml(file, "caloni::" chapter, "months.html", 0)
    files[chapter] = chapter
  }
  ssstags = ""
  split(g_tags, sstags)
  for( st in sstags ) {
    ssstags = ssstags " <a href=\"" sstags[st] ".html\">" sstags[st] "</a>"
  }
  for( name in links ) {
    search = "\\[" name "\\]"
    gsub(search, links[name], content)
  }
  content = gensub(/\[([^\]]+)\]/, "<a href=\"posts.html?q=\\1\">\\1</a>", "g", content)

  post = "<span id=\"" toid(slug) "\" title=\"" tohtml(title) "\"/></span>\n"
  post = post "<section id=\"section-" toid(slug) "\">\n"
  if( postlink != "" ) {
    post = post "<p class=\"title\"><a href=\"" chapter ".html#" toid(slug) "\">#</a> " tohtml(title) " <a class=\"external\" href=\"" postlink "\">[link]</a></p>\n"
  } else {
    post = post "<p class=\"title\"><a href=\"" chapter ".html#" toid(slug) "\">#</a> " tohtml(title) "</p>\n"
  }
  post = post "<p class=\"note-title\"><small>" date " " ssstags " </small><a href=\"" chapter ".html\">^</a> <button onclick=\"copy_clipboard('section#section-" toid(slug) "')\">ctrl_c</button></p>\n"
  post = post content "\n"
  post = post "</section><hr/>\n"
  g_postsByMonth[chapter][date] = g_postsByMonth[chapter][date] "\n" post
  postLink = "<li><small><a href=\"" chapter ".html#" slug "\">" tohtml(title) "</a></small></li>"
  g_postLinksByMonth[chapter][date] = g_postLinksByMonth[chapter][date] "\n" postLink

  quickSearch[slug] = chapter ".html#" toid(slug)
}


/^= / {
  if( content ) {
    writepost()
    content = ""
    slug = ""
    postlink = ""
    g_tags = ""
    draft = 0
    repost = ""
    summary = ""
    image = ""
    delete links
  }
  title = substr($0, 3)
}


/^:/ {
  if( $1 == ":slug:" ) {
    slug = $2
  }
  else if( $1 == ":link:" ) {
    postlink = $2
  }
  else if( $1 == ":date:" ) {
    date = $2
    chapter = substr(date, 1, 7)
    chapters[chapter] = chapter
  }
  else if( $1 == ":tags:" ) {
    i = 2
    while( i <= NF ) {
      tag = $i
      if( tag != "null" ) {
        g_tags = g_tags " " tag
      }
      ++i
    }
  }
  else if( $1 == ":draft:" ) {
    draft = 1
  }
  else if( $1 == ":repost:" ) {
    repost = $2
  }
}


/^[^=:]/ {
  newContent = formatContent($0)
  if( content ) {
    content = content newContent
  } else {
    content = newContent
  }
  if( length(summary) < 200 ) {
    if( index($0, "{{") == 0 && index($0, "```") == 0 ) {
      summary = summary " " $0
    }
  }
}


BEGIN {
  "date" | getline currentDate
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

  PROCINFO["sorted_in"] = "@ind_num_asc"
  nChapter = "index"
  for( chapter in chapters ) {
    nextChapter[chapter] = nChapter
    nChapter = chapter
  }
  PROCINFO["sorted_in"] = "@ind_num_desc"
  monthshtml = "public\\blog\\months.html"
  writetophtml(monthshtml, "caloni::months", "index.html", 0)
  lastyear = "2001"
  pChapter = "index"
  for( chapter in chapters ) {
    prevChapter[chapter] = pChapter
    pChapter = chapter
    year = substr(chapter, 1, 4)
    mon = substr(chapter, 6, 2)
    if( year != lastyear ) {
      if( lastyear != "2001" ) {
        print "</p>" > monthshtml
      }
      print "<p id=\"" toid(chapter) "\" class=\"toc\"><strong>" year "</strong>" > monthshtml
      lastyear = year
    }
    print "<a href=\"" chapter ".html\"> " tohtml(mon) " </a>" > monthshtml
    quickSearch[chapter] = chapter ".html"
    if( ! lastmonth ) {
      lastmonth = chapter
    }
  }
  print "</p>" > monthshtml
  writebottomhtml(monthshtml, 0)
  quickSearch["months"] = "months.html"

  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( f in files ) {
    postsContent = ""
    file = "public\\blog\\" f ".html"
    if( f == "repost" ) {
      PROCINFO["sorted_in"] = "@ind_num_desc"
    } else {
      PROCINFO["sorted_in"] = "@ind_num_asc"
    }
    for( date in g_postsByMonth[f] ) {
      postsContent = postsContent "\n" g_postsByMonth[f][date]
    }
    if( f != "repost" ) {
      print "<ul style=\"list-style: none;\">" > file
      for( date in g_postLinksByMonth[f] ) {
        print g_postLinksByMonth[f][date] > file
      }
      print "</ul>" > file
    }
    print postsContent > file
    if( f != "repost" ) {
      writebottomhtml(file, 0, nextChapter[f] ".html", prevChapter[f] ".html")
    } else {
      writebottomhtml(file, 1)
    }
  }
  quickSearch["repost"] = "repost.html"

  PROCINFO["sorted_in"] = "@ind_num_desc"
  for( t in g_titlesByTagsAndDates ) {
    quickSearch[t] = t ".html"
    file = "public\\blog\\" t ".html"
    writetophtml(file, "caloni::" t, "index.html", 1)
    for( d in g_titlesByTagsAndDates[t] ) {
      for( title in g_titlesByTagsAndDates[t][d] ) {
        slug = titleToSlug[title]
        slugTerms = slugs[slug]["tags"] 
        split(slugTerms, sslugTerms)
        ssslugTerms = ""
        for( st in sslugTerms ) {
          ssslugTerms = ssslugTerms " [" sslugTerms[st] "]"
        }
        print "<tr><td>" > file
        if( slugs[slug]["image"] ) {
          print "<img src=\"img/" slugs[slug]["image"] "\"/>" > file
        }
        print "<b><a href=\"" titleToChapter[title] ".html#" toid(slug) "\">" tohtml(title) "</a></b>" > file
        print "<small><i>" slugs[slug]["date"] ssslugTerms " " slugs[slug]["summary"] "</small></i>" > file
        print "</td></tr>" > file
      }
    }
    writebottomhtml(file, 1)
  }

  tagshtml = "public\\blog\\tags.html"
  writetophtml(tagshtml, "caloni::tags", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( t in g_titlesByTagsAndDates ) {
    titles = ""
    totalTitles = 0
    PROCINFO["sorted_in"] = "@ind_num_desc"
    for( d in g_titlesByTagsAndDates[t] ) {
      for( title in g_titlesByTagsAndDates[t][d] ) {
        if( titles == "" ) {
          titles = title
        } else {
          titles = titles " - " title
        }
        totalTitles = totalTitles + 1
        if( totalTitles > 15 ) {
          break
        }
      }
      if( totalTitles > 15 ) {
        break
      }
    }
    print "<tr><td><b><a href=\"" t ".html" "\">" tohtml(t) "</a></b>" > tagshtml
    print "<small><i>" titles "</small></i>" > tagshtml
    print "</td></tr>" > tagshtml
  }
  writebottomhtml(tagshtml, 1)
  quickSearch["tags"] = "tags.html"

  postshtml = "public\\blog\\posts.html"
  writetophtml(postshtml, "caloni::posts", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_desc"
  for( date in entries ) {
    for( slug in entries[date] ) {
      title = entries[date][slug]
      split(slugs[slug]["tags"], tags)
      stags = ""
      for( t in tags ) {
        stags = stags " [" tags[t] "]"
      }
      print "<tr><td>" > postshtml
      if( slugs[slug]["image"] ) {
        print "<img src=\"img/" slugs[slug]["image"] "\"/>" > postshtml
      }
      print "<b><a href=\"" titleToChapter[title] ".html#" toid(slug) "\">" tohtml(title) "</a></b>" > postshtml
      print "<small><i>" slugs[slug]["date"] stags " " slugs[slug]["summary"] " " slug "</small></i>" > postshtml
      print "</td></tr>" > postshtml
    }
  }
  writebottomhtml(postshtml, 1)
  quickSearch["posts"] = "posts.html"

  indexhtml = "public\\blog\\index.html"
  writetophtml(indexhtml, "Blogue do Caloni", "2007-06.html#_about", 0, quickSearch)
  print "<input type=\"text\" name=\"quick_search_name\" value=\"\" id=\"quick_search\" placeholder=\"digite uma url\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > indexhtml
  print "<big>>> <a href=\"coding.html\">coding</a></big><small><i>: programação, depuração, transpiração.</small></i></br>" > indexhtml
  print "<big>>> <a href=\"movies.html\">movies</a></big><small><i>: o finado Cine Tênis Verde veio parar aqui.</small></i></br>" > indexhtml
  print "<big>? <a id=\"quick_search_search_engine\" href=\"https://duckduckgo.com/?q=site%3Acaloni.com.br\">patopatovai</a></big><small><i>: usar um motor para buscar.</small></i></br>" > indexhtml
  print "<big>> <a href=\"tags.html\">tags</a></big><small><i>: todos os rótulos dos postes.</small></i></br>" > indexhtml
  print "<big>> <a href=\"" lastmonth ".html\">news</a></big><small><i>: postes publicados no último mês.</small></i></br>" > indexhtml
  print "<big>> <a href=\"months.html\">months</a></big><small><i>: lista dos meses com postes.</small></i></br>" > indexhtml
  print "<big>> <a href=\"posts.html\">posts</a></big><small><i>: lista com todos os postes do blogue.</small></i></br>" > indexhtml
  print "<big>> <a href=\"repost.html\">reposts</a></big><small><i>: vale a pena postar de novo.</small></i></br>" > indexhtml
  print "<big>> <a href=\"drafts.html\">drafts</a></big><small><i>: postes em progresso.</small></i></br>" > indexhtml
  print "<div><big><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></big></div>" > indexhtml
  print "<table class=\"sortable\" style=\"width: 100%;\">" > indexhtml
  print "</table>" > indexhtml
  writebottomhtml(indexhtml, 0, "", "", currentDate)

  notfoundhtml = "public\\blog\\404.html"
  writetophtml(notfoundhtml, "caloni::404 page not found", "posts.html", 0)
  print "<div class=\"container\">" > notfoundhtml
  print "  <p class=\"title\">Opa, essa página não foi encontrada.</p>" > notfoundhtml
  print "    <div class=\"content\">" > notfoundhtml
  print "      <p>Não quer fazer uma <a href=\"/posts.html\">busca</a>? Às vezes eu mexo e remexo as coisas por aqui.</p>" > notfoundhtml
  print "    </div>" > notfoundhtml
  print "</div>" > notfoundhtml
  writebottomhtml(notfoundhtml, 0)
}

