# Transform parsed text to html blog posts.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2025-01-30
# Depends on: MarkdownParser, Util.

BEGIN {
  "date" | getline Blog["build"]
  Blog["author"] = "Caloni"
  Blog["author-image"] = "/img/about-author.jpg"
  Blog["post-image"] = "/img/about-brand.png"
  Blog["description"] = "Write for computers, people and food."
  Blog["generator"] = "BlogWriter 0.2.0 " Settings["generator"]
  Blog["link"] = "http://www.caloni.com.br"
  Blog["output"] = "public\\blog"
  Blog["text_favorite_tags"] = "computer cinema"
  Blog["text_favorite_tags_description"]["computer"] = "programação, depuração, transpiração."
  Blog["text_favorite_tags_description"]["cinema"] = "o finado Cine Tênis Verde veio parar aqui."
  Blog["text_months"] = "lista dos meses com postes."
  Blog["text_notfound_description"] = "Não quer fazer uma <a href=\"/posts.html\">busca</a>? Às vezes eu mexo e remexo as coisas por aqui."
  Blog["text_notfound_title"] = "Opa, essa página não foi encontrada."
  Blog["text_page_prefix"] = "caloni"
  Blog["text_posts"] = "lista com toooooooodos os postes do blogue."
  Blog["text_quicksearch"] = "&#x1F41E; digite algo / type something"
  Blog["text_tags"] = "todos os rótulos dos postes."
  Blog["title"] = "Blogue do Caloni"
}

function FlushPost(slug,    tags, post, i, j, file, search, prefix, suffix, links)
{
  split(Index[slug]["tags"], tags)
  post = ""

  file = Blog["output"] "\\" Index[slug]["month"] ".html"
  if( ! (Index[slug]["month"] in Files) ) {
    WriteToHtml(file, Blog["text_page_prefix"] "::" Index[slug]["month"], "months.html", 0)
    Files[Index[slug]["month"]] = Index[slug]["month"]
  }
  for( i in Index[slug]["lines"] ) {
    prefix = ""
    suffix = ""
    links = 1
    if( Index[slug]["lines"][i]["content"] != "" ) {
      if( Index[slug]["lines"][i]["type"] != "pre" && Index[slug]["lines"][i]["type"] != "blockquote" ) {
        if ( substr(Index[slug]["lines"][i]["type"], 1, 1) == "h" && length(Index[slug]["lines"][i]["type"]) == 2 ) {
          gsub(/&/, "&amp;", Index[slug]["lines"][i]["content"])
        }
        if( "links" in Index[slug] ) {
          for( j in Index[slug]["links"] ) {
            if( Index[slug]["links"][j] in IndexMetadata ) {
              Index[slug]["links"][j] = "<a href=\"" IndexMetadata[Index[slug]["links"][j]]["chapter"] ".html#" Index[slug]["links"][j] "\">" j "</a>"
            }
            search = "\\[" j "\\]"
            gsub(search, Index[slug]["links"][j], Index[slug]["lines"][i]["content"])
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
        links = 0
      } else if ( Index[slug]["lines"][i]["type"] == "blockquote") {
        prefix = prefix "<blockquote>"
        suffix = suffix "</blockquote>"
        links = 0
      } else if ( Index[slug]["lines"][i]["type"] == "list") {
        prefix = prefix "<li>"
        suffix = suffix "</li>"
        if( Index[slug]["lines"][i-1]["type"] != "list" ) {
          prefix = "<ul>" prefix
        }
        if( Index[slug]["lines"][i+1]["type"] != "list" ) {
          suffix = suffix "</ul>"
        }
        links = 1
      } else if ( substr(Index[slug]["lines"][i]["type"], 1, 1) == "h" && length(Index[slug]["lines"][i]["type"]) == 2 ) {
        prefix = prefix "<h" substr(Index[slug]["lines"][i]["type"], 2, 1) ">"
        suffix = suffix "</h" substr(Index[slug]["lines"][i]["type"], 2, 1) ">\n"
        links = 1
      } else if ( Index[slug]["lines"][i]["type"] == "p") {
        prefix = prefix "<p>"
        suffix = suffix "</p>\n"
        links = 1
      } else if ( Index[slug]["lines"][i]["type"] == "img") {
        prefix = prefix "<img src=\"img/"
        suffix = suffix "\"/>\n"
        links = 0
      }

      if( links ) {
        Index[slug]["lines"][i]["content"] = gensub(/\[([^\]]+)\]/, "<a href=\"posts.html?q=\\1\">\\1</a>", "g", Index[slug]["lines"][i]["content"])
      }

      Index[slug]["lines"][i]["content"] = prefix Index[slug]["lines"][i]["content"] suffix
    }
  }

  post = "<span id=\"" slug "\" title=\"" ToHtml(Index[slug]["title"]) "\"/></span>\n"
  post = post "<section id=\"section_" slug "\">\n"
  if( "extlink" in Index[slug] ) {
    post = post "<p class=\"title\"><a href=\"" Index[slug]["month"] ".html#" slug "\">#</a> <a class=\"external\" href=\"" Index[slug]["extlink"] "\">" ToHtml(Index[slug]["title"]) "</a></p>\n"
  } else {
    post = post "<p class=\"title\"><a href=\"" Index[slug]["month"] ".html#" slug "\">#</a> " ToHtml(Index[slug]["title"]) "</p>\n"
  }
  post = post "<span class=\"title-heading\">" Blog["author"] ", " Index[slug]["date"]
  for( i in tags ) {
    post = post " <a href=\"" tags[i] ".html\">" tags[i] "</a>"
  }
  post = post "<a href=\"" Index[slug]["month"] ".html\"> "
  post = post "<sup>[up]</sup></a> <a href=\"javascript:;\" onclick=\"copy_clipboard('section#section_" slug "')\"><sup>[copy]</sup></a></span>\n\n"
  for( i in Index[slug]["lines"] ) {
    post = post Index[slug]["lines"][i]["content"]
    if( Index[slug]["lines"][i]["type"] != "pre" ) {
      post = post "\n"
    }
  }
  post = post "</section><hr/>\n"
  PostsByMonth[Index[slug]["month"]][Index[slug]["date"]] = PostsByMonth[Index[slug]["month"]][Index[slug]["date"]] "\n" post
  Index[slug]["extlink"] = "<li><small><a href=\"" Index[slug]["month"] ".html#" slug "\">" ToHtml(Index[slug]["title"]) "</a></small></li>"
  PostLinksByMonth[Index[slug]["month"]][Index[slug]["date"]] = PostLinksByMonth[Index[slug]["month"]][Index[slug]["date"]] "\n" Index[slug]["extlink"]

  QuickSearch[slug] = Index[slug]["month"] ".html#" slug
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

function WriteToHtml(file, title, backLink, filter, quickSearch,    originalSort, i)
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

function WriteBottomHtml(file, filter, nextLink, prevLink, build,    label, link)
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

function TiePreviousNextMonths(    i, c)
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

function FlushMonthsPage(    i, f, y, m, y2)
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

function FlushPostsPages(    i, j, p, f)
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( i in Files ) {
    p = ""
    f = Blog["output"] "\\" i ".html"
    PROCINFO["sorted_in"] = "@ind_num_asc"
    for( j in PostsByMonth[i] ) {
      p = p "\n" PostsByMonth[i][j]
    }
    print "<ul style=\"list-style: none;\">" > f
    for( j in PostLinksByMonth[i] ) {
      print PostLinksByMonth[i][j] > f
    }
    print "</ul>" > f
    print p > f
    WriteBottomHtml(f, 0, NextMonth[i] ".html", PrevMonth[i] ".html")
  }
}

function FlushTagsPages(    slug, tags, i, j, k, l, f, s)
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
          s = s " " tags[l]
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

function FlushTagsPage(    i, j, k, f, t)
{
  f = Blog["output"] "\\tags.html"
  WriteToHtml(f, Blog["text_page_prefix"] "::tags", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( i in SlugsByTagsAndDates ) {
    print "<button class=\"tagbutton\" style=\"font-size: 1rem;\" onclick=\"window.location = '" i ".html';\">" ToHtml(i) "</button>" > f
  }
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

function FlushPostsPage(    i, j, k, f, t, s)
{
  TiePreviousNextMonths()

  f = Blog["output"] "\\posts.html"
  WriteToHtml(f, Blog["text_page_prefix"] "::posts", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_desc"
  for( i in DateSlugTitle ) {
    for( j in DateSlugTitle[i] ) {
      t = DateSlugTitle[i][j]
      split(Index[j]["tags"], a)
      s = ""
      for( k in a ) {
        s = s " " a[k]
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

function FlushIndexPage(    favtags, i, c, f)
{
  split(Blog["text_favorite_tags"], favtags)
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( i in Months ) {
    c = Months[i]
    break
  }
  f = Blog["output"] "\\index.html"
  WriteToHtml(f, Blog["title"], c ".html#about", 0, QuickSearch)
  print "<input type=\"text\" name=\"quick_search_name\" value=\"\" id=\"quick_search\" placeholder=\"" Blog["text_quicksearch"] "\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > f
  for( i in favtags ) {
    print "<big><a href=\"" favtags[i] ".html\">" favtags[i] "</a></big><small><i>: " Blog["text_favorite_tags_description"][favtags[i]] "</small></i></br>" > f
  }
  print "<big><a href=\"tags.html\">tags</a></big><small><i>: " Blog["text_tags"] "</small></i></br>" > f
  print "<big><a href=\"months.html\">months</a></big><small><i>: " Blog["text_months"] "</small></i></br>" > f
  print "<big><a href=\"posts.html\">posts</a></big><small><i>: " Blog["text_posts"] "</small></i></br>" > f
  print "<div><big><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></big></div>" > f
  print "<table class=\"sortable\" style=\"width: 100%;\">" > f
  print "</table>" > f
  WriteBottomHtml(f, 0, "", "", Blog["build"])
}

function FlushNotFoundPage(    f)
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
  FlushPosts()
  FlushPostsPage()
  FlushPostsPages()
  FlushTagsPage()
  FlushTagsPages()
  FlushMonthsPage()
  FlushIndexPage()
  FlushNotFoundPage()
}
