# Transform pseudo-markdown text to html blog posts.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2024-08-30

#include util.awk

BEGIN {
  "date" | getline Blog["build"]
  Blog["author"] = "Caloni"
  Blog["author-image"] = "/img/about-author.jpg"
  Blog["post-image"] = "/img/about-brand.png"
  Blog["description"] = "Write for computers, people and food."
  Blog["generator"] = "txt2blog 0.0.1"
  Blog["link"] = "http://www.caloni.com.br"
  Blog["output"] = "public\\blog"
  Blog["text_drafts"] = "postes em progresso."
  Blog["text_favorite_tags"]["coding"] = "programação, depuração, transpiração."
  Blog["text_favorite_tags"]["movies"] = "o finado Cine Tênis Verde veio parar aqui."
  Blog["text_lists"] = "quem não gosta de listas?"
  Blog["text_months"] = "lista dos meses com postes."
  Blog["text_news"] = "postes publicados no último mês."
  Blog["text_notfound_description"] = "Não quer fazer uma <a href=\"/posts.html\">busca</a>? Às vezes eu mexo e remexo as coisas por aqui."
  Blog["text_notfound_title"] = "Opa, essa página não foi encontrada."
  Blog["text_page_prefix"] = "caloni"
  Blog["text_posts"] = "lista com toooooooodos os postes do blogue."
  Blog["text_quicksearch"] = "&#x1F41E; digite algo / type something"
  Blog["text_reposts"] = "vale a pena postar de novo."
  Blog["text_tags"] = "todos os rótulos dos postes."
  Blog["title"] = "Blogue do Caloni"
}


function WriteToHtml(file, title, backLink, filter, quickSearch,    originalSort)
{
  print "<!DOCTYPE html>" > file
  print "<html lang=\"en-us\" dir=\"ltr\" itemscope itemtype=\"http://schema.org/Article\">" > file
  print "<head>" > file
  print "<meta charset=\"utf-8\" />" > file
  print "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/>" > file
  print "<title>" Blog["title"] "</title>" > file
  print "<meta name=\"author\" content=\"" Blog["author"] "\" />" > file
  print "<meta name=\"generator\" content=\"" Blog["generator"] "\">" > file
  print "<meta property=\"og:title\" content=\"" Blog["title"] "\"/>" > file
  print "<meta property=\"og:type\" content=\"website\"/>" > file
  print "<meta property=\"og:url\" content=\"" Blog["link"] "\"/>" > file
  print "<meta property=\"og:image\" content=\"" Blog["post-image"] "\"/>" > file
  print "<meta property=\"og:description\" content=\"" Blog["description"] "\"/>" > file
  print "<link href=\"/index.xml\" rel=\"feed\" type=\"application/rss+xml\" title=\"" Blog["title"] "\"/>" > file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/custom.css\"/>" > file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/jquery-ui.css\"/>" > file
  print "<script src=\"/js/jquery-1.12.4.js\"></script>" > file
  print "<script src=\"/js/jquery-ui.js\"></script>" > file
  print "<script src=\"/js/copy_clipboard.js\"></script>" > file
  print "<script>" > file
  print "var quick_search_posts = [ " > file
  originalSort = PROCINFO["sorted_in"]
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( i in quickSearch ) {
    print "\"" quickSearch[i] "\"," > file
  }
  PROCINFO["sorted_in"] = originalSort
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


function WriteBottomHtml(file, filter, nextLink, prevLink, build,    label)
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
  if( build ) {
    print "<p><small><i>Build: " build "</i></small></p>" > file
  }
  print "</div>" > file
  print "</footer>" > file
  print "</body>" > file
  print "</html>" > file
}


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

    if( line ~ /^>/ ) {
      sub(/^> ?/, "", line)
      ContentType = "blockquote"
      prefix = "<blockquote>"
      suffix = "</blockquote>"
      break
    }

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

    if( index(line, "image::") == 1 ) {
      NewPost["image"] = gensub(/image::(.*)\[.*\]/, NewPost["slug"] "_\\1", "g", line)
      line = gensub(/image::(.*)\[.*\]/, "<img src=\"img/" NewPost["slug"] "_\\1\"/>", "g", line)
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


function FlushContentState(    lastLine)
{
  lastLine = length(NewPost["lines"])
  if ( ContentState["-"] ) {
    NewPost["lines"][lastLine]["content"] = NewPost["lines"][lastLine]["content"] "</ul>\n"
    ContentState["-"] = 0
  }
  delete ContentState
}


function FlushNewPost(    slug, date, month, tags, post)
{
  slug = NewPost["slug"]
  date = NewPost["date"]
  month = substr(date, 1, 7)
  split(NewPost["tags"], tags)
  post = ""

  FlushContentState()

  Months[month] = month
  DateSlugTitle[date][slug] = NewPost["title"]

  if( "draft" in NewPost ) {
    month = "drafts"
    QuickSearch["drafts"] = "drafts.html"
  }

  Index[NewPost["slug"]]["slug"] = slug
  Index[NewPost["slug"]]["title"] = NewPost["title"]
  Index[NewPost["slug"]]["date"] = date
  Index[NewPost["slug"]]["month"] = month
  Index[NewPost["slug"]]["link"] = month ".html#" slug
  Index[NewPost["slug"]]["summary"] = NewPost["summary"]
  Index[NewPost["slug"]]["tags"] = NewPost["tags"]
  Index[NewPost["slug"]]["image"] = NewPost["image"]
  TitleToSlug[NewPost["title"]] = slug
  for( i in tags ) {
    SlugsByTagsAndDates[tags[i]][date][slug] = slug
  }

  if ( "repost" in NewPost ) {
    QuickSearch["repost"] = "repost.html"
    file = Blog["output"] "\\repost.html"
    if( ! ("repost" in Files) ) {
      WriteToHtml(file, Blog["text_page_prefix"] "::repost", "index.html", 1)
      Files["repost"] = "repost"
    }
    post = "<tr><td><b><a href=\"" month ".html#" slug "\">" ToHtml(NewPost["title"]) "</a></b>\n"
    post = post "<small><i>" NewPost["repost"] " [" date "] "
    for( i in tags ) {
      post = post " [" tags[i] "]"
    }
    post = post " " NewPost["summary"] "</small></i>\n"
    post = post "</td></tr>\n"
    PostsByMonth["repost"][NewPost["repost"]] = PostsByMonth["repost"][NewPost["repost"]] "\n" post
  }

  file = Blog["output"] "\\" month ".html"
  if( ! (month in Files) ) {
    WriteToHtml(file, Blog["text_page_prefix"] "::" month, "months.html", 0)
    Files[month] = month
  }
  for( i in NewPost["lines"] ) {
    if( NewPost["lines"][i]["content"] != "" ) {
      if( NewPost["lines"][i]["type"] != "pre" && NewPost["lines"][i]["type"] != "blockquote" ) {
        if( "links" in NewPost ) {
          for( j in NewPost["links"] ) {
            search = "\\[" j "\\]"
            gsub(search, NewPost["links"][j], NewPost["lines"][i]["content"])
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

  post = "<span id=\"" slug "\" title=\"" ToHtml(NewPost["title"]) "\"/></span>\n"
  post = post "<section id=\"section_" slug "\">\n"
  if( "link" in NewPost ) {
    post = post "<p class=\"title\"><a href=\"" month ".html#" slug "\">#</a> <a class=\"external\" href=\"" NewPost["link"] "\">" ToHtml(NewPost["title"]) "</a></p>\n"
  } else {
    post = post "<p class=\"title\"><a href=\"" month ".html#" slug "\">#</a> " ToHtml(NewPost["title"]) "</p>\n"
  }
  post = post "<span class=\"title-heading\">" Blog["author"] ", " date
  if( "update" in NewPost ) {
    post = post " (updated " NewPost["update"] ")"
  }
  for( i in tags ) {
    post = post " <a href=\"" tags[i] ".html\">" tags[i] "</a>"
  }
  post = post "<a href=\"" month ".html\"> "
  post = post "<sup>[up]</sup></a> <a href=\"javascript:;\" onclick=\"copy_clipboard('section#section_" slug "')\"><sup>[copy]</sup></a></span>\n\n"
  for( i in NewPost["lines"] ) {
    post = post NewPost["lines"][i]["content"]
    if( NewPost["lines"][i]["type"] != "pre" ) {
      post = post "\n"
    }
  }
  post = post "</section><hr/>\n"
  PostsByMonth[month][date] = PostsByMonth[month][date] "\n" post
  NewPost["link"] = "<li><small><a href=\"" month ".html#" slug "\">" ToHtml(NewPost["title"]) "</a></small></li>"
  PostLinksByMonth[month][date] = PostLinksByMonth[month][date] "\n" NewPost["link"]

  QuickSearch[NewPost["slug"]] = month ".html#" slug
  delete NewPost
  NewPost["date"] = date
}


$1 == "metadata_slug" { Index[$2]["link"] = $3 ; next }


/^# / && !ContentState["```"] {
  if( "title" in NewPost ) {
    FlushNewPost()
  }
  NewPost["title"] = substr($0, 3)
  NewPost["slug"] = ToSlug(NewPost["title"])
  next
}


/^\[[^]]+\]:/ && !ContentState["```"] {
  if( match($0, /^\[([^]]+)\]: *([^"]+) *"?([^"]+)?"?/, a) ) {
    if( a[2] ~ /^(https?)|(ftp)|(mailto):/ ) {
      a[2] = "<a href=\"" a[2] "\">" a[1] "</a>"
    }
    else if( a[2] ~ /^(bib_)|(idx_)/ ) {
      Lists[a[1] " [" a[2] "]"][NewPost["slug"]] = NewPost["slug"]
      a[2] = "<a href=\"lists.html?q=" a[1] "\">" a[1] "</a>"
    }
    else if( a[2] in Index ) {
      a[2] = Index[a[2]]["link"]
      a[2] = "<a href=\"" a[2] "\">" a[1] "</a>"
    }
    else {
      print "warning: link", a[2], "not found. Line:"
      print line
      a[2] = gensub(/(.*)/, "posts.html?q=\\1", "g", a[2])
      a[2] = "<a href=\"" a[2] "\">" a[1] "</a>"
    }
    NewPost["links"][a[1]] = a[2]
  } else {
    print "error: invalid link at", $0
  }
  next
}


/^[^:]/ {
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


$1 == ":date:" { NewPost["date"] = $2 }
$1 == ":draft:" { NewPost["draft"] = 1 }
$1 == ":link:" { NewPost["link"] = $2 }
$1 == ":repost:" { NewPost["repost"] = $2 }
$1 == ":slug:" { NewPost["slug"] = $2 }
$1 == ":tags:" { $1 = "" ; NewPost["tags"] = $0 }
$1 == ":update:" { NewPost["update"] = $2 }


function TiePreviousNextMonths()
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  c = "index"
  for( i in Months ) {
    NextMonth[i] = c
    c = i
  }
  PROCINFO["sorted_in"] = "@ind_num_desc"
  c = "index"
  for( i in Months ) {
    PrevMonth[i] = c
    c = i
  }
}


function FlushMonthsPage()
{
  PROCINFO["sorted_in"] = "@ind_num_desc"
  f = Blog["output"] "\\months.html"
  WriteToHtml(f, Blog["text_page_prefix"] "::months", "index.html", 0)
  y = "2001"
  for( i in Months ) {
    y2 = substr(i, 1, 4)
    m = substr(i, 6, 2)
    if( y2 != y ) {
      if( y != "2001" ) {
        print "</p>" > f
      }
      print "<p id=\"" i "\" class=\"toc\"><strong>" y2 "</strong>" > f
      y = y2
    }
    print "<a href=\"" i ".html\"> " ToHtml(m) " </a>" > f
    QuickSearch[i] = i ".html"
    if( ! LastMonth ) {
      LastMonth = i
    }
  }
  print "</p>" > f
  WriteBottomHtml(f, 0)
  QuickSearch["months"] = "months.html"
}


function FlushPostsPages()
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( i in Files ) {
    p = ""
    f = Blog["output"] "\\" i ".html"
    if( i == "repost" ) {
      PROCINFO["sorted_in"] = "@ind_num_desc"
    } else {
      PROCINFO["sorted_in"] = "@ind_num_asc"
    }
    for( j in PostsByMonth[i] ) {
      p = p "\n" PostsByMonth[i][j]
    }
    if( i != "repost" ) {
      print "<ul style=\"list-style: none;\">" > f
      for( j in PostLinksByMonth[i] ) {
        print PostLinksByMonth[i][j] > f
      }
      print "</ul>" > f
    }
    print p > f
    if( i != "repost" && i != "drafts" ) {
      WriteBottomHtml(f, 0, NextMonth[i] ".html", PrevMonth[i] ".html")
    } else {
      WriteBottomHtml(f, 1)
    }
  }
}


function FlushTagsPages(    slug, tags)
{
  PROCINFO["sorted_in"] = "@ind_num_desc"
  for( i in SlugsByTagsAndDates ) {
    QuickSearch[i] = i ".html"
    f = Blog["output"] "\\" i ".html"
    WriteToHtml(f, Blog["text_page_prefix"] "::" i, "index.html", 1)
    for( j in SlugsByTagsAndDates[i] ) {
      for( k in SlugsByTagsAndDates[i][j] ) {
        split(Index[k]["tags"], tags)
        s = ""
        for( l in tags ) {
          s = s " [" tags[l] "]"
        }
        print "<tr><td>" > f
        if( Index[k]["image"] ) {
          print "<img src=\"img/" Index[k]["image"] "\"/>" > f
        }
        print "<b><a href=\"" Index[k]["month"] ".html#" k "\">" ToHtml(Index[k]["title"]) "</a></b>" > f
        print "<small><i>" Index[k]["date"] s " " Index[k]["summary"] "</small></i>" > f
        print "</td></tr>" > f
      }
    }
    WriteBottomHtml(f, 1)
  }
}


function FlushTagsPage()
{
  f = Blog["output"] "\\tags.html"
  WriteToHtml(f, Blog["text_page_prefix"] "::tags", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( i in SlugsByTagsAndDates ) {
    t = ""
    t2 = 0
    PROCINFO["sorted_in"] = "@ind_num_desc"
    for( j in SlugsByTagsAndDates[i] ) {
      for( k in SlugsByTagsAndDates[i][j] ) {
        if( t == "" ) {
          t = Index[k]["title"]
        } else {
          t = t " - " Index[k]["title"]
        }
        t2 = t2 + 1
        if( t2 > 15 ) {
          break
        }
      }
      if( t2 > 15 ) {
        break
      }
    }
    print "<tr><td><b><a href=\"" i ".html" "\">" ToHtml(i) "</a></b>" > f
    print "<small><i>" t "</small></i>" > f
    print "</td></tr>" > f
  }
  WriteBottomHtml(f, 1)
  QuickSearch["tags"] = "tags.html"
}


function FlushListsPage(    slugsByDate, slug)
{
  slug = ""

  if( length(Lists) == 0 ) return
  f = Blog["output"] "\\lists.html"
  WriteToHtml(f, Blog["text_page_prefix"] "::lists", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( i in Lists ) {
    tt = ""
    if( m = match(i, "(.*) \\[(.*)\\]", a) ) {
      print "<tr><td><a href=\"#" a[2] "\">#</a>" ToHtml(a[1]) " " > f
    } else {
      print "<tr><td>" ToHtml(i) " " > f
    }
    for( j in Lists[i] ) {
      slugsByDate[Index[j]["date"], j] = j
    }
    for( j in slugsByDate ) {
      slug = slugsByDate[j]
      t = "<a href=\"" Index[slug]["month"] ".html#" slug "\">" Index[slug]["title"] "</a>"
      if( tt == "" ) {
        tt = t
      } else {
        tt = tt " - " t
      }
    }
    delete slugsByDate
    print "<small><i>" tt "</small></i>" > f
    print "</td></tr>" > f
  }
  WriteBottomHtml(f, 1)
  QuickSearch["lists"] = "lists.html"
}


function FlushPostsPage()
{
  f = Blog["output"] "\\posts.html"
  WriteToHtml(f, Blog["text_page_prefix"] "::posts", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_desc"
  for( i in DateSlugTitle ) {
    for( j in DateSlugTitle[i] ) {
      t = DateSlugTitle[i][j]
      split(Index[j]["tags"], a)
      s = ""
      for( k in a ) {
        s = s " [" a[k] "]"
      }
      print "<tr><td>" > f
      if( Index[j]["image"] ) {
        print "<img src=\"img/" Index[j]["image"] "\"/>" > f
      }
      print "<b><a href=\"" Index[j]["month"] ".html#" j "\">" ToHtml(t) "</a></b>" > f
      print "<small><i>" Index[j]["date"] s " " Index[j]["summary"] " " j "</small></i>" > f
      print "</td></tr>" > f
    }
  }
  WriteBottomHtml(f, 1)
  QuickSearch["posts"] = "posts.html"
}


function FlushIndexPage()
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( i in Months ) {
    c = Months[i]
    break
  }
  f = Blog["output"] "\\index.html"
  WriteToHtml(f, Blog["title"], c ".html#about", 0, QuickSearch)
  print "<input type=\"text\" name=\"quick_search_name\" value=\"\" id=\"quick_search\" placeholder=\"" Blog["text_quicksearch"] "\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > f
  for( i in Blog["text_favorite_tags"] ) {
    print "<big><a href=\"tag_" i ".html\">" i "</a></big><small><i>: " Blog["text_favorite_tags"][i] "</small></i></br>" > f
  }
  print "<big><a href=\"tags.html\">tags</a></big><small><i>: " Blog["text_tags"] "</small></i></br>" > f
  print "<big><a href=\"" LastMonth ".html\">news</a></big><small><i>: " Blog["text_news"] "</small></i></br>" > f
  print "<big><a href=\"months.html\">months</a></big><small><i>: " Blog["text_months"] "</small></i></br>" > f
  print "<big><a href=\"posts.html\">posts</a></big><small><i>: " Blog["text_posts"] "</small></i></br>" > f
  if( "repost" in QuickSearch ) {
    print "<big><a href=\"repost.html\">reposts</a></big><small><i>: " Blog["text_reposts"] "</small></i></br>" > f
  }
  if( "lists" in QuickSearch ) {
    print "<big><a href=\"lists.html\">lists</a></big><small><i>: " Blog["text_lists"] "</small></i></br>" > f
  }
  if( "drafts" in QuickSearch ) {
    print "<big><a href=\"drafts.html\">drafts</a></big><small><i>: " Blog["text_drafts"] "</small></i></br>" > f
  }
  print "<div><big><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></big></div>" > f
  print "<table class=\"sortable\" style=\"width: 100%;\">" > f
  print "</table>" > f
  WriteBottomHtml(f, 0, "", "", Blog["build"])
}


function FlushNotFoundPage()
{
  f = Blog["output"] "\\404.html"
  WriteToHtml(f, Blog["text_page_prefix"] "::404 page not found", "posts.html", 0)
  print "<div class=\"container\">" > f
  print "  <p class=\"title\">" Blog["text_notfound_title"] "</p>" > f
  print "    <div class=\"content\">" > f
  print "      <p>" Blog["text_notfound_description"] "</p>" > f
  print "    </div>" > f
  print "</div>" > f
  WriteBottomHtml(f, 0)
}


END {
  if( "title" in NewPost ) {
    FlushNewPost()
  }
  TiePreviousNextMonths()

  FlushPostsPage()
  FlushPostsPages()
  FlushTagsPage()
  FlushListsPage()
  FlushTagsPages()
  FlushMonthsPage()
  FlushIndexPage()
  FlushNotFoundPage()
}

