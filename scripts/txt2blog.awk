# Transform pseudo-markdown text to html blog posts.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2024-08-29 v. 0.0.1


function ToId(str)
{
  return str
}


function ToSlug(str)
{
  gsub(/[ÁÀÂÃáàâã]/, "a", str)
  gsub(/[ÉÊÊéêê]/, "e", str)
  gsub(/[ÔÕÓôõó]/, "o", str)
  gsub(/[Úú]/, "u", str)
  gsub(/[ÍÏíï]/, "i", str)
  gsub(/[#()'",;:-]/, "", str)
  gsub(/&/, "and", str)
  gsub(/!/, "", str)
  gsub(/\?/, "", str)
  gsub(/ /, "-", str)
  return tolower(str)
}


function ToHtml(str)
{
  gsub(/&/, "&amp;", str)
  gsub(/</, "\\&lt;", str)
  gsub(/>/, "\\&gt;", str)
  return str
}


function WriteToHtml(file, title, backLink, filter, quickSearch)
{
  print "<!DOCTYPE html>" > file
  print "<html lang=\"en-us\" dir=\"ltr\" itemscope itemtype=\"http://schema.org/Article\">" > file
  print "<head>" > file
  print "<meta charset=\"utf-8\" />" > file
  print "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/>" > file
  print "<title>" Blog["title"] "</title>" > file
  print "<meta name=\"author\" content=\"\" />" > file
  print "<meta name=\"generator\" content=\"Hugo 0.110.0\">" > file
  print "<meta property=\"og:title\" content=\"" Blog["title"] "\"/>" > file
  print "<meta property=\"og:type\" content=\"website\"/>" > file
  print "<meta property=\"og:url\" content=\"http://www.caloni.com.br/\"/>" > file
  print "<meta property=\"og:image\" content=\"/img/author.jpg\"/>" > file
  print "<meta property=\"og:description\" content=\"\"/>" > file
  print "<link href=\"/index.xml\" rel=\"feed\" type=\"application/rss+xml\" title=\"" Blog["title"] "\"/>" > file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/custom.css\"/>" > file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/jquery-ui.css\"/>" > file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/board-min.css\"/>" > file
  print "<script src=\"/js/jquery-1.12.4.js\"></script>" > file
  print "<script src=\"/js/jquery-ui.js\"></script>" > file
  print "<script src=\"/js/pgnyui.js\"></script>" > file
  print "<script src=\"/js/pgnviewer.js\"></script>" > file
  print "<script src=\"/js/copy_clipboard.js\"></script>" > file
  print "<script>" > file
  print "var quick_search_posts = [ " > file
  for( i in quickSearch ) {
    print "\"" quickSearch[i] "\"," > file
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
  print "&nbsp;" > file
  print "<a class=\"navbar-item\" href=\"" backLink "\">" > file
  print "<div class=\"is-4\"><b>" title "</b></div>" > file
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


function WriteBottomHtml(file, filter, nextLink, prevLink, version)
{
  if( filter ) {
    print "</table>" > file
  }
  print "<span style=\"float: left;\">" > file
  if( nextLink && nextLink != "" ) {
    link = nextLink
    sub(/\.html/, "", nextLink)
    label = nextLink
    print " <a href=\"" link "\">[" label "]</a>" > file
  }
  if( prevLink && prevLink != "" ) {
    link = prevLink
    sub(/\.html/, "", prevLink)
    label = prevLink
    print " <a href=\"" link "\">[" label "]</a>" > file
  }
  print "</span>" > file
  print "</div>" > file
  print "</div>" > file
  print "</section>" > file
  print "<footer class=\"footer\">" > file
  print "<div class=\"container\">" > file
  if( version ) {
    print "<p><small><i>" version "</i></small></p>" > file
  }
  print "</div>" > file
  print "</footer>" > file
  print "</body>" > file
  print "</html>" > file
}


function FormatContent(line, lastLine)
{
  prefix = ""
  suffix = "\n"
  paragraph = 1
  newLine = 0
  type = ""

  do {
    if( index(line, "```") == 1 ) {
      line = ""
      if( contentState["```"] ) {
        contentState["```"] = 0
      } else {
        contentState["```"] = 1
      }
      return 0
    } else if( contentState["```"] ) {
      type = "pre"
      break
    }

    if( line ~ /^    / ) {
      sub(/^ /, "", line)
      if( ! contentState[" "] ) {
        contentState[" "] = 1
      }
      type = "pre"
      break
    } else if ( contentState[" "] ) {
        contentState[" "] = 0
    }

    if( line ~ /^ *- */ ) {
      line = gensub(/ *- *(.*)/, "\\1", "g", line)
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

    if( line ~ /^>/ ) {
      sub(/^> ?/, "", line)
      type = "blockquote"
      prefix = "<blockquote>"
      suffix = "</blockquote>"
      break
    }

    if( line ~ /^#+ / ) {

      if( line ~ /^## / ) {
        headerLevel = 2
        type = "h2"
      } else if( line ~ /^### / ) {
        headerLevel = 3
        type = "h3"
      } else if( line ~ /^#### / ) {
        headerLevel = 4
        type = "h4"
      } else if( line ~ /^##### / ) {
        headerLevel = 5
        type = "h5"
      } else if( line ~ /^###### / ) {
        headerLevel = 6
        type = "h6"
      }
      gsub(/^#+ /, "", line)

      prefix = prefix "<h" headerLevel ">"
      suffix = "</h" headerLevel ">" suffix
      paragraph = 0
    }

    if( line ~ /^\[[^]]+\]:/ ) {
      endName = index(line, ":")
      name = substr(line, 2, endName - 3)
      link = substr(line, endName + 2)

      if( link ~ /[a-z]:\/\// ) {
        link = "<a href=\"" link "\">" name "</a>"
      }
      else {
        link = gensub(/(.*)/, "posts.html?q=\\1", "g", link)
        link = "<a href=\"" link "\">" name "</a>"
      }

      NewPost["links"][name] = link
      line = ""
      type = "link"
      break
    }

    if( index(line, "image::") == 1 ) {
      NewPost["image"] = gensub(/image::(.*)\[.*\]/, NewPost["slug"] "-\\1", "g", line)
      line = gensub(/image::(.*)\[.*\]/, "<img src=\"img/" NewPost["slug"] "-\\1\"/>", "g", line)
      type = "img"
      break
    }

    gsub(/&/, "&amp;", line)
    gsub(/</, "\\&lt;", line)
    gsub(/>/, "\\&gt;", line)
    line = gensub(/\[([^]]+)\]\(([^)]+)\)/, "<a href=\"\\2\">\\1</a>", "g", line)

    if( paragraph ) {
      line = "<p>" line "</p>"
      type = "p"
    } else {
      type = ""
    }

  } while( 0 )

  newLine = lastLine + 1
  NewPost["lines"][newLine]["content"] = prefix line suffix
  NewPost["lines"][newLine]["type"] = type
  return newLine
}


function FlushNewPost(    date, chapter, tags, post)
{
  date = NewPost["date"]
  chapter = substr(date, 1, 7)
  split(NewPost["tags"], tags)
  post = ""

  Chapters[chapter] = chapter
  ++postCount
  entries[date][NewPost["slug"]] = NewPost["title"]

  if( "draft" in NewPost ) {
    draftToSlug[NewPost["title"]] = NewPost["slug"]
    chapter = "drafts"
    QuickSearch["drafts"] = "drafts.html"
  }

  Index[NewPost["slug"]]["slug"] = NewPost["slug"]
  Index[NewPost["slug"]]["title"] = NewPost["title"]
  Index[NewPost["slug"]]["date"] = date
  Index[NewPost["slug"]]["summary"] = NewPost["summary"]
  Index[NewPost["slug"]]["tags"] = NewPost["tags"]
  Index[NewPost["slug"]]["image"] = NewPost["image"]
  TitleToSlug[NewPost["title"]] = NewPost["slug"]
  TitleToChapter[NewPost["title"]] = chapter
  for( i in tags ) {
    TitlesByTagsAndDates[tags[i]][date][NewPost["title"]] = NewPost["title"]
  }

  if ( "repost" in NewPost ) {
    QuickSearch["repost"] = "repost.html"
    file = "public\\blog\\repost.html"
    if( ! ("repost" in files) ) {
      WriteToHtml(file, "caloni::repost", "index.html", 1)
      files["repost"] = "repost"
    }
    post = "<tr><td><b><a href=\"" chapter ".html#" ToId(NewPost["slug"]) "\">" ToHtml(NewPost["title"]) "</a></b>\n"
    post = post "<small><i>" NewPost["repost"] " [" date "] "
    for( i in tags ) {
      post = post " [" tags[i] "]"
    }
    post = post " " NewPost["summary"] "</small></i>\n"
    post = post "</td></tr>\n"
    PostsByMonth["repost"][NewPost["repost"]] = PostsByMonth["repost"][NewPost["repost"]] "\n" post
  }

  file = "public\\blog\\" chapter ".html"
  if( ! (chapter in files) ) {
    WriteToHtml(file, "caloni::" chapter, "months.html", 0)
    files[chapter] = chapter
  }
  for( i in NewPost["lines"] ) {
    if( NewPost["lines"][i]["content"] != "" ) {
      if( NewPost["lines"][i]["type"] != "pre" && NewPost["lines"][i]["type"] != "blockquote" ) {
        if( "links" in NewPost ) {
          for( name in NewPost["links"] ) {
            search = "\\[" name "\\]"
            gsub(search, NewPost["links"][name], NewPost["lines"][i]["content"])
          }
        }
      }

      if( NewPost["lines"][i]["type"] == "pre" ) {
        NewPost["lines"][i]["content"] = ToHtml(NewPost["lines"][i]["content"])
        if( NewPost["lines"][i-1]["type"] != NewPost["lines"][i]["type"] ) {
          NewPost["lines"][i]["content"] = "<" NewPost["lines"][i]["type"] ">\n" NewPost["lines"][i]["content"]
        }
        if( NewPost["lines"][i+1]["type"] != NewPost["lines"][i]["type"] ) {
          NewPost["lines"][i]["content"] = NewPost["lines"][i]["content"] "</" NewPost["lines"][i]["type"] ">\n"
        }
      } else {
        NewPost["lines"][i]["content"] = gensub(/\[([^\]]+)\]/, "<a href=\"posts.html?q=\\1\">\\1</a>", "g", NewPost["lines"][i]["content"])
      }
    }
  }

  post = "<span id=\"" ToId(NewPost["slug"]) "\" title=\"" ToHtml(NewPost["title"]) "\"/></span>\n"
  post = post "<section id=\"section-" ToId(NewPost["slug"]) "\">\n"
  if( "link" in NewPost ) {
    post = post "<p class=\"title\"><a href=\"" chapter ".html#" ToId(NewPost["slug"]) "\">#</a> <a class=\"external\" href=\"" NewPost["link"] "\">" ToHtml(NewPost["title"]) "</a></p>\n"
  } else {
    post = post "<p class=\"title\"><a href=\"" chapter ".html#" ToId(NewPost["slug"]) "\">#</a> " ToHtml(NewPost["title"]) "</p>\n"
  }
  post = post "<span class=\"title-heading\">Wanderley Caloni, " date
  if( "update" in NewPost ) {
    post = post " (updated " NewPost["update"] ")"
  }
  post = post " "
  for( i in tags ) {
    post = post " <a href=\"" tags[i] ".html\">" tags[i] "</a>"
  }
  post = post " <a href=\"" chapter ".html\"> "
  post = post "<sup>[up]</sup></a> <a href=\"javascript:;\" onclick=\"copy_clipboard('section#section-" ToId(NewPost["slug"]) "')\"><sup>[copy]</sup></a></span>\n\n"
  for( i in NewPost["lines"] ) {
    post = post NewPost["lines"][i]["content"]
    if( NewPost["lines"][i]["type"] != "pre" ) {
      post = post "\n"
    }
  }
  post = post "</section><hr/>\n"
  PostsByMonth[chapter][date] = PostsByMonth[chapter][date] "\n" post
  NewPost["link"] = "<li><small><a href=\"" chapter ".html#" NewPost["slug"] "\">" ToHtml(NewPost["title"]) "</a></small></li>"
  PostLinksByMonth[chapter][date] = PostLinksByMonth[chapter][date] "\n" NewPost["link"]

  QuickSearch[NewPost["slug"]] = chapter ".html#" ToId(NewPost["slug"])
  delete NewPost
  NewPost["date"] = date
}


/^= / {
  if( "title" in NewPost ) {
    FlushNewPost()
  }
  NewPost["title"] = substr($0, 3)
  NewPost["slug"] = ToSlug(NewPost["title"])
}


/^[^=:]/ {
  newLine = FormatContent($0, NewPost["totalLines"])
  if( newLine ) {
    NewPost["totalLines"] = newLine
    if( length(NewPost["summary"]) < 200 ) {
      if( index($0, "{{") == 0 && index($0, "```") == 0 ) {
        NewPost["summary"] = NewPost["summary"] " " $0
      }
    }
  }
}


$1 == ":date:" { NewPost["date"] = $2 }
$1 == ":draft:" { NewPost["draft"] = 1 }
$1 == ":link:" { NewPost["link"] = $2 }
$1 == ":repost:" { NewPost["repost"] = $2 }
$1 == ":slug:" { NewPost["slug"] = $2 }
$1 == ":tags:" { $1 = "" ; NewPost["tags"] = $0 }
$1 == ":update:" { NewPost["update"] = $2 }


BEGIN {
  Blog["title"] = "Blogue do Caloni"
  "date" | getline Blog["build"]
}


END {
  if( "title" in NewPost ) {
    FlushNewPost()
  }

  PROCINFO["sorted_in"] = "@ind_num_asc"
  nChapter = "index"
  for( i in Chapters ) {
    nextChapter[Chapters[i]] = nChapter
    nChapter = Chapters[i]
  }
  PROCINFO["sorted_in"] = "@ind_num_desc"
  monthshtml = "public\\blog\\months.html"
  WriteToHtml(monthshtml, "caloni::months", "index.html", 0)
  lastyear = "2001"
  pChapter = "index"
  for( i in Chapters ) {
    prevChapter[Chapters[i]] = pChapter
    pChapter = Chapters[i]
    year = substr(Chapters[i], 1, 4)
    mon = substr(Chapters[i], 6, 2)
    if( year != lastyear ) {
      if( lastyear != "2001" ) {
        print "</p>" > monthshtml
      }
      print "<p id=\"" ToId(Chapters[i]) "\" class=\"toc\"><strong>" year "</strong>" > monthshtml
      lastyear = year
    }
    print "<a href=\"" Chapters[i] ".html\"> " ToHtml(mon) " </a>" > monthshtml
    QuickSearch[Chapters[i]] = Chapters[i] ".html"
    if( ! lastmonth ) {
      lastmonth = Chapters[i]
    }
  }
  print "</p>" > monthshtml
  WriteBottomHtml(monthshtml, 0)
  QuickSearch["months"] = "months.html"

  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( f in files ) {
    postsContent = ""
    file = "public\\blog\\" f ".html"
    if( f == "repost" ) {
      PROCINFO["sorted_in"] = "@ind_num_desc"
    } else {
      PROCINFO["sorted_in"] = "@ind_num_asc"
    }
    for( date in PostsByMonth[f] ) {
      postsContent = postsContent "\n" PostsByMonth[f][date]
    }
    if( f != "repost" ) {
      print "<ul style=\"list-style: none;\">" > file
      for( date in PostLinksByMonth[f] ) {
        print PostLinksByMonth[f][date] > file
      }
      print "</ul>" > file
    }
    print postsContent > file
    if( f != "repost" && f != "drafts" ) {
      WriteBottomHtml(file, 0, nextChapter[f] ".html", prevChapter[f] ".html")
    } else {
      WriteBottomHtml(file, 1)
    }
  }

  PROCINFO["sorted_in"] = "@ind_num_desc"
  for( t in TitlesByTagsAndDates ) {
    QuickSearch[t] = t ".html"
    file = "public\\blog\\" t ".html"
    WriteToHtml(file, "caloni::" t, "index.html", 1)
    for( d in TitlesByTagsAndDates[t] ) {
      for( title in TitlesByTagsAndDates[t][d] ) {
        slug = TitleToSlug[title]
        slugTerms = Index[slug]["tags"] 
        split(slugTerms, sslugTerms)
        ssslugTerms = ""
        for( st in sslugTerms ) {
          ssslugTerms = ssslugTerms " [" sslugTerms[st] "]"
        }
        print "<tr><td>" > file
        if( Index[slug]["image"] ) {
          print "<img src=\"img/" Index[slug]["image"] "\"/>" > file
        }
        print "<b><a href=\"" TitleToChapter[title] ".html#" ToId(slug) "\">" ToHtml(title) "</a></b>" > file
        print "<small><i>" Index[slug]["date"] ssslugTerms " " Index[slug]["summary"] "</small></i>" > file
        print "</td></tr>" > file
      }
    }
    WriteBottomHtml(file, 1)
  }

  tagshtml = "public\\blog\\tags.html"
  WriteToHtml(tagshtml, "caloni::tags", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( t in TitlesByTagsAndDates ) {
    titles = ""
    totalTitles = 0
    PROCINFO["sorted_in"] = "@ind_num_desc"
    for( d in TitlesByTagsAndDates[t] ) {
      for( title in TitlesByTagsAndDates[t][d] ) {
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
    print "<tr><td><b><a href=\"" t ".html" "\">" ToHtml(t) "</a></b>" > tagshtml
    print "<small><i>" titles "</small></i>" > tagshtml
    print "</td></tr>" > tagshtml
  }
  WriteBottomHtml(tagshtml, 1)
  QuickSearch["tags"] = "tags.html"

  postshtml = "public\\blog\\posts.html"
  WriteToHtml(postshtml, "caloni::posts", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_desc"
  for( date in entries ) {
    for( slug in entries[date] ) {
      title = entries[date][slug]
      split(Index[slug]["tags"], a)
      s = ""
      for( i in a ) {
        s = s " [" a[i] "]"
      }
      print "<tr><td>" > postshtml
      if( Index[slug]["image"] ) {
        print "<img src=\"img/" Index[slug]["image"] "\"/>" > postshtml
      }
      print "<b><a href=\"" TitleToChapter[title] ".html#" ToId(slug) "\">" ToHtml(title) "</a></b>" > postshtml
      print "<small><i>" Index[slug]["date"] s " " Index[slug]["summary"] " " slug "</small></i>" > postshtml
      print "</td></tr>" > postshtml
    }
  }
  WriteBottomHtml(postshtml, 1)
  QuickSearch["posts"] = "posts.html"

  indexhtml = "public\\blog\\index.html"
  WriteToHtml(indexhtml, Blog["title"], "2007-06.html#_about", 0, QuickSearch)
  print "<input type=\"text\" name=\"quick_search_name\" value=\"\" id=\"quick_search\" placeholder=\"&#x1F41E; digite algo / type something\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > indexhtml
  print "<big><a href=\"tag_coding.html\">coding</a></big><small><i>: programação, depuração, transpiração.</small></i></br>" > indexhtml
  print "<big><a href=\"tag_movies.html\">movies</a></big><small><i>: o finado Cine Tênis Verde veio parar aqui.</small></i></br>" > indexhtml
  print "<big><a href=\"tags.html\">tags</a></big><small><i>: todos os rótulos dos postes.</small></i></br>" > indexhtml
  print "<big><a href=\"" lastmonth ".html\">news</a></big><small><i>: postes publicados no último mês.</small></i></br>" > indexhtml
  print "<big><a href=\"months.html\">months</a></big><small><i>: lista dos meses com postes.</small></i></br>" > indexhtml
  print "<big><a href=\"posts.html\">posts</a></big><small><i>: lista com toooooooodos os postes do blogue.</small></i></br>" > indexhtml
  if( "repost" in QuickSearch ) {
    print "<big><a href=\"repost.html\">reposts</a></big><small><i>: vale a pena postar de novo.</small></i></br>" > indexhtml
  }
  if( "drafts" in QuickSearch ) {
    print "<big><a href=\"drafts.html\">drafts</a></big><small><i>: postes em progresso.</small></i></br>" > indexhtml
  }
  print "<div><big><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></big></div>" > indexhtml
  print "<table class=\"sortable\" style=\"width: 100%;\">" > indexhtml
  print "</table>" > indexhtml
  WriteBottomHtml(indexhtml, 0, "", "", Blog["build"])

  notfoundhtml = "public\\blog\\404.html"
  WriteToHtml(notfoundhtml, "caloni::404 page not found", "posts.html", 0)
  print "<div class=\"container\">" > notfoundhtml
  print "  <p class=\"title\">Opa, essa página não foi encontrada.</p>" > notfoundhtml
  print "    <div class=\"content\">" > notfoundhtml
  print "      <p>Não quer fazer uma <a href=\"/posts.html\">busca</a>? Às vezes eu mexo e remexo as coisas por aqui.</p>" > notfoundhtml
  print "    </div>" > notfoundhtml
  print "</div>" > notfoundhtml
  WriteBottomHtml(notfoundhtml, 0)
}

