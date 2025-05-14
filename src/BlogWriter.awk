# Transform parsed text to html blog posts.

BEGIN {
  G_SETTINGS["author"] = "Caloni"
  G_SETTINGS["author-image"] = "/img/about-author.jpg"
  G_SETTINGS["post-image"] = "/img/about-brand.png"
  G_SETTINGS["description"] = "Write for computers, people and food."
  G_SETTINGS["generator"] = G_SETTINGS["generator"]
  G_SETTINGS["link"] = "http://www.caloni.com.br"
  G_SETTINGS["output"] = "public\\blog"
  G_SETTINGS["text_favorite_tags"] = "computer cinema"
  G_SETTINGS["text_favorite_tags_description"]["computer"] = "programação, depuração, transpiração."
  G_SETTINGS["text_favorite_tags_description"]["cinema"] = "o finado Cine Tênis Verde veio parar aqui."
  G_SETTINGS["text_months"] = "lista dos meses com postes."
  G_SETTINGS["text_notfound_description"] = "Não quer fazer uma <a href=\"/posts.html\">busca</a>? Às vezes eu mexo e remexo as coisas por aqui."
  G_SETTINGS["text_notfound_title"] = "Opa, essa página não foi encontrada."
  G_SETTINGS["text_page_prefix"] = "caloni"
  G_SETTINGS["text_posts"] = "lista com toooooooodos os postes do blogue."
  G_SETTINGS["text_quicksearch"] = "&#x1F41E; digite algo / type something"
  G_SETTINGS["text_tags"] = "todos os rótulos dos postes."
  G_SETTINGS["title"] = "Blogue do Caloni"
}

function BlogWriter_FlushPost(slug,    tags, post, i, j, file, search, prefix, suffix, links)
{
  split(G_INDEX[slug]["tags"], tags)
  post = ""

  file = G_SETTINGS["output"] "\\" G_INDEX[slug]["month"] ".html"
  if( ! (G_INDEX[slug]["month"] in Files) ) {
    f_write_to_html(file, G_SETTINGS["text_page_prefix"] "::" G_INDEX[slug]["month"], "months.html", 0)
    Files[G_INDEX[slug]["month"]] = G_INDEX[slug]["month"]
  }
  for( i in G_INDEX[slug]["lines"] ) {
    prefix = ""
    suffix = ""
    links = 1
    if( G_INDEX[slug]["lines"][i]["content"] != "" ) {
      if( G_INDEX[slug]["lines"][i]["type"] != "pre" && G_INDEX[slug]["lines"][i]["type"] != "blockquote" ) {
        if ( substr(G_INDEX[slug]["lines"][i]["type"], 1, 1) == "h" && length(G_INDEX[slug]["lines"][i]["type"]) == 2 ) {
          gsub(/&/, "&amp;", G_INDEX[slug]["lines"][i]["content"])
        }
        if( "links" in G_INDEX[slug] ) {
          for( j in G_INDEX[slug]["links"] ) {
            if( G_INDEX[slug]["links"][j] in G_INDEX_METADATA ) {
              G_INDEX[slug]["links"][j] = "<a href=\"" G_INDEX_METADATA[G_INDEX[slug]["links"][j]]["chapter"] ".html#" G_INDEX[slug]["links"][j] "\">" j "</a>"
            }
            search = "\\[" j "\\]"
            gsub(search, G_INDEX[slug]["links"][j], G_INDEX[slug]["lines"][i]["content"])
          }
        }
      }

      if( G_INDEX[slug]["lines"][i]["type"] == "pre" ) {
        G_INDEX[slug]["lines"][i]["content"] = Util_TextToHtml(G_INDEX[slug]["lines"][i]["content"]) "\n"
        if( G_INDEX[slug]["lines"][i-1]["type"] != G_INDEX[slug]["lines"][i]["type"] ) {
          G_INDEX[slug]["lines"][i]["content"] = "<" G_INDEX[slug]["lines"][i]["type"] ">\n" G_INDEX[slug]["lines"][i]["content"]
        }
        if( G_INDEX[slug]["lines"][i+1]["type"] != G_INDEX[slug]["lines"][i]["type"] ) {
          G_INDEX[slug]["lines"][i]["content"] = G_INDEX[slug]["lines"][i]["content"] "</" G_INDEX[slug]["lines"][i]["type"] ">\n"
        }
        links = 0
      } else if ( G_INDEX[slug]["lines"][i]["type"] == "blockquote") {
        prefix = prefix "<blockquote>"
        suffix = suffix "</blockquote>"
        links = 0
      } else if ( G_INDEX[slug]["lines"][i]["type"] == "list") {
        prefix = prefix "<li>"
        suffix = suffix "</li>"
        if( G_INDEX[slug]["lines"][i-1]["type"] != "list" ) {
          prefix = "<ul>" prefix
        }
        if( G_INDEX[slug]["lines"][i+1]["type"] != "list" ) {
          suffix = suffix "</ul>"
        }
        links = 1
      } else if ( substr(G_INDEX[slug]["lines"][i]["type"], 1, 1) == "h" && length(G_INDEX[slug]["lines"][i]["type"]) == 2 ) {
        prefix = prefix "<h" substr(G_INDEX[slug]["lines"][i]["type"], 2, 1) ">"
        suffix = suffix "</h" substr(G_INDEX[slug]["lines"][i]["type"], 2, 1) ">\n"
        links = 1
      } else if ( G_INDEX[slug]["lines"][i]["type"] == "p") {
        prefix = prefix "<p>"
        suffix = suffix "</p>\n"
        links = 1
      } else if ( G_INDEX[slug]["lines"][i]["type"] == "img") {
        prefix = prefix "<img src=\"img/"
        suffix = suffix "\"/>\n"
        links = 0
      }

      if( links ) {
        G_INDEX[slug]["lines"][i]["content"] = gensub(/\[([^\]]+)\]/, "<a href=\"posts.html?q=\\1\">\\1</a>", "g", G_INDEX[slug]["lines"][i]["content"])
      }

      G_INDEX[slug]["lines"][i]["content"] = prefix G_INDEX[slug]["lines"][i]["content"] suffix
    }
  }

  post = "<span id=\"" slug "\" title=\"" Util_TextToHtml(G_INDEX[slug]["title"]) "\"/></span>\n"
  post = post "<section id=\"section_" slug "\">\n"
  if( "extlink" in G_INDEX[slug] ) {
    post = post "<p class=\"title\"><a href=\"" G_INDEX[slug]["month"] ".html#" slug "\">#</a> <a class=\"external\" href=\"" G_INDEX[slug]["extlink"] "\">" Util_TextToHtml(G_INDEX[slug]["title"]) "</a></p>\n"
  } else {
    post = post "<p class=\"title\"><a href=\"" G_INDEX[slug]["month"] ".html#" slug "\">#</a> " Util_TextToHtml(G_INDEX[slug]["title"]) "</p>\n"
  }
  post = post "<span class=\"title-heading\">" G_SETTINGS["author"] ", " G_INDEX[slug]["date"]
  for( i in tags ) {
    post = post " "
    if( "tag_nav" in G_INDEX[slug] && tags[i] in G_INDEX[slug]["tag_nav"] && "prev_in_tag" in G_INDEX[slug]["tag_nav"][tags[i]] ) {
      post = post "<a href=\"" G_INDEX[G_INDEX[slug]["tag_nav"][tags[i]]["prev_in_tag"]]["link"] "\">&lt;</a>"
    }
    post = post "<a href=\"" tags[i] ".html\">" tags[i] "</a>"
    if( "tag_nav" in G_INDEX[slug] && tags[i] in G_INDEX[slug]["tag_nav"] && "next_in_tag" in G_INDEX[slug]["tag_nav"][tags[i]] ) {
      post = post "<a href=\"" G_INDEX[G_INDEX[slug]["tag_nav"][tags[i]]["next_in_tag"]]["link"] "\">&gt;</a>"
    }
  }
  post = post "<a href=\"" G_INDEX[slug]["month"] ".html\"> "
  post = post "<sup>[up]</sup></a> <a href=\"javascript:;\" onclick=\"copy_clipboard('section#section_" slug "')\"><sup>[copy]</sup></a></span>\n\n"
  for( i in G_INDEX[slug]["lines"] ) {
    post = post G_INDEX[slug]["lines"][i]["content"]
    if( G_INDEX[slug]["lines"][i]["type"] != "pre" ) {
      post = post "\n"
    }
  }
  post = post "</section><hr/>\n"
  PostsByMonth[G_INDEX[slug]["month"]][G_INDEX[slug]["date"]] = PostsByMonth[G_INDEX[slug]["month"]][G_INDEX[slug]["date"]] "\n" post
  G_INDEX[slug]["extlink"] = "<li><small><a href=\"" G_INDEX[slug]["month"] ".html#" slug "\">" Util_TextToHtml(G_INDEX[slug]["title"]) "</a></small></li>"
  PostLinksByMonth[G_INDEX[slug]["month"]][G_INDEX[slug]["date"]] = PostLinksByMonth[G_INDEX[slug]["month"]][G_INDEX[slug]["date"]] "\n" G_INDEX[slug]["extlink"]

  QuickSearch[slug] = G_INDEX[slug]["month"] ".html#" slug
}

function BlogWriter_FlushPosts(    position, slug)
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( position in G_POST_SLUG_BY_POSITION ) {
    slug = G_POST_SLUG_BY_POSITION[position]
    if( !("date" in G_INDEX[slug]) ) {
      print "skipping", slug
      continue
    }
    BlogWriter_FlushPost(slug)
  }
}

function f_write_to_html(file, title, backLink, filter, quickSearch,    originalSort, i)
{
  print "<!DOCTYPE html>" > file
  print "<html lang=\"en-us\" dir=\"ltr\" itemscope itemtype=\"http://schema.org/Article\">" > file
  print "<head>" > file
  print "<meta charset=\"utf-8\" />" > file
  print "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/>" > file
  print "<title>" G_SETTINGS["title"] "</title>" > file
  print "<meta name=\"author\" content=\"" G_SETTINGS["author"] "\" />" > file
  print "<meta name=\"generator\" content=\"" G_SETTINGS["generator"] "\">" > file
  print "<meta property=\"og:title\" content=\"" G_SETTINGS["title"] "\"/>" > file
  print "<meta property=\"og:type\" content=\"website\"/>" > file
  print "<meta property=\"og:url\" content=\"" G_SETTINGS["link"] "\"/>" > file
  print "<meta property=\"og:image\" content=\"" G_SETTINGS["post-image"] "\"/>" > file
  print "<meta property=\"og:description\" content=\"" G_SETTINGS["description"] "\"/>" > file
  print "<link href=\"/index.xml\" rel=\"feed\" type=\"application/rss+xml\" title=\"" G_SETTINGS["title"] "\"/>" > file
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

function f_write_bottom_html(file, filter, nextLink, prevLink, build,    label, link)
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
    print "<p style=\"text-align: right;\" class=\"tiny\"><i>" build "</i></p>" > file
  }
  print "</div>" > file
  print "<div class=\"intentionally-blank\"></div>" > file
  print "</footer>" > file
  print "</body>" > file
  print "</html>" > file
}

function f_tie_previous_next_monts(    i, c)
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  c = "index"
  for( i in G_MONTHS ) {
    NextMonth[i] = c
    c = i
  }
  PROCINFO["sorted_in"] = "@ind_num_desc"
  c = "index"
  for( i in G_MONTHS ) {
    PrevMonth[i] = c
    c = i
  }
}

function BlogWriter_FlushMonthsPage(    i, f, y, m, y2)
{
  PROCINFO["sorted_in"] = "@ind_num_desc"
  f = G_SETTINGS["output"] "\\months.html"
  f_write_to_html(f, G_SETTINGS["text_page_prefix"] "::months", "index.html", 0)
  y = "2001"
  for( i in G_MONTHS ) {
    y2 = substr(i, 1, 4)
    m = substr(i, 6, 2)
    if( y2 != y ) {
      if( y != "2001" ) {
        print "</p>" > f
      }
      print "<p id=\"" i "\" class=\"toc\"><strong>" y2 "</strong>" > f
      y = y2
    }
    print "<a href=\"" i ".html\"> " Util_TextToHtml(m) " </a>" > f
    QuickSearch[i] = i ".html"
    if( ! LastMonth ) {
      LastMonth = i
    }
  }
  print "</p>" > f
  f_write_bottom_html(f, 0)
  QuickSearch["months"] = "months.html"
}

function BlogWriter_FlushPostsPages(    i, j, p, f)
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( i in Files ) {
    p = ""
    f = G_SETTINGS["output"] "\\" i ".html"
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
    f_write_bottom_html(f, 0, NextMonth[i] ".html", PrevMonth[i] ".html")
  }
}

function BlogWriter_FlushTagsPages(    slug, tags, i, j, k, l, f, s)
{
  PROCINFO["sorted_in"] = "@ind_num_desc"
  for( i in G_SLUGS_BY_TAGS_AND_DATES ) {
    QuickSearch[i] = i ".html"
    f = G_SETTINGS["output"] "\\" i ".html"
    f_write_to_html(f, G_SETTINGS["text_page_prefix"] "::" i, "index.html", 1)
    for( j in G_SLUGS_BY_TAGS_AND_DATES[i] ) {
      for( k in G_SLUGS_BY_TAGS_AND_DATES[i][j] ) {
        split(G_INDEX[k]["tags"], tags)
        s = ""
        for( l in tags ) {
          s = s " " tags[l]
        }
        print "<tr><td>" > f
        if( G_INDEX[k]["image"] ) {
          print "<img src=\"img/" G_INDEX[k]["image"] "\"/>" > f
        }
        print "<b><a href=\"" G_INDEX[k]["month"] ".html#" k "\">" Util_TextToHtml(G_INDEX[k]["title"]) "</a></b>" > f
        print "<small><i>" G_INDEX[k]["date"] s " " G_INDEX[k]["summary"] "</small></i>" > f
        print "</td></tr>" > f
      }
    }
    f_write_bottom_html(f, 1)
  }
}

function BlogWriter_FlushTagsPage(    i, j, k, f, t)
{
  f = G_SETTINGS["output"] "\\tags.html"
  f_write_to_html(f, G_SETTINGS["text_page_prefix"] "::tags", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( i in G_SLUGS_BY_TAGS_AND_DATES ) {
    print "<button class=\"tagbutton\" style=\"font-size: 1rem;\" onclick=\"window.location = '" i ".html';\">" Util_TextToHtml(i) "</button>" > f
  }
  for( i in G_SLUGS_BY_TAGS_AND_DATES ) {
    t = ""
    t2 = 0
    PROCINFO["sorted_in"] = "@ind_num_desc"
    for( j in G_SLUGS_BY_TAGS_AND_DATES[i] ) {
      for( k in G_SLUGS_BY_TAGS_AND_DATES[i][j] ) {
        if( t == "" ) {
          t = G_INDEX[k]["title"]
        } else {
          t = t " - " G_INDEX[k]["title"]
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
    print "<tr><td><b><a href=\"" i ".html" "\">" Util_TextToHtml(i) "</a></b>" > f
    print "<small><i>" t "</small></i>" > f
    print "</td></tr>" > f
  }
  f_write_bottom_html(f, 1)
  QuickSearch["tags"] = "tags.html"
}

function BlogWriter_FlushPostsPage(    i, j, k, f, t, s)
{
  f_tie_previous_next_monts()

  f = G_SETTINGS["output"] "\\posts.html"
  f_write_to_html(f, G_SETTINGS["text_page_prefix"] "::posts", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_desc"
  for( i in G_DATE_SLUG_TITLE ) {
    for( j in G_DATE_SLUG_TITLE[i] ) {
      t = G_DATE_SLUG_TITLE[i][j]
      split(G_INDEX[j]["tags"], a)
      s = ""
      for( k in a ) {
        s = s " " a[k]
      }
      print "<tr><td>" > f
      if( G_INDEX[j]["image"] ) {
        print "<img src=\"img/" G_INDEX[j]["image"] "\"/>" > f
      }
      print "<b><a href=\"" G_INDEX[j]["month"] ".html#" j "\">" Util_TextToHtml(t) "</a></b>" > f
      print "<small><i>" G_INDEX[j]["date"] s " " G_INDEX[j]["summary"] " " j "</small></i>" > f
      print "</td></tr>" > f
    }
  }
  f_write_bottom_html(f, 1)
  QuickSearch["posts"] = "posts.html"
}

function BlogWriter_FlushIndexPage(    favtags, i, c, f, build_link)
{
  split(G_SETTINGS["text_favorite_tags"], favtags)
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( i in G_MONTHS ) {
    c = G_MONTHS[i]
    break
  }
  f = G_SETTINGS["output"] "\\index.html"
  f_write_to_html(f, G_SETTINGS["title"], c ".html#about", 0, QuickSearch)
  print "<input type=\"text\" name=\"quick_search_name\" value=\"\" id=\"quick_search\" placeholder=\"" G_SETTINGS["text_quicksearch"] "\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > f
  for( i in favtags ) {
    print "<big><a href=\"" favtags[i] ".html\">" favtags[i] "</a></big><small><i>: " G_SETTINGS["text_favorite_tags_description"][favtags[i]] "</small></i></br>" > f
  }
  print "<big><a href=\"tags.html\">tags</a></big><small><i>: " G_SETTINGS["text_tags"] "</small></i></br>" > f
  print "<big><a href=\"months.html\">months</a></big><small><i>: " G_SETTINGS["text_months"] "</small></i></br>" > f
  print "<big><a href=\"posts.html\">posts</a></big><small><i>: " G_SETTINGS["text_posts"] "</small></i></br>" > f
  print "<div><big><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></big></div>" > f
  print "<table class=\"sortable\" style=\"width: 100%;\">" > f
  print "</table>" > f
  build_link = "<a href=\"" G_SETTINGS["public_repo_github_address"] "/commit/" G_SETTINGS["build"] "\">" G_SETTINGS["build"] "</a>"
  f_write_bottom_html(f, 0, "", "", G_SETTINGS["date"] "-" build_link)
}

function BlogWriter_FlushNotFoundPage(    f)
{
  f = G_SETTINGS["output"] "\\404.html"
  f_write_to_html(f, G_SETTINGS["text_page_prefix"] "::404 page not found", "posts.html", 0)
  print "<div class=\"container\">" > f
  print "  <p class=\"title\">" G_SETTINGS["text_notfound_title"] "</p>" > f
  print "    <div class=\"content\">" > f
  print "      <p>" G_SETTINGS["text_notfound_description"] "</p>" > f
  print "    </div>" > f
  print "</div>" > f
  f_write_bottom_html(f, 0)
}

END {
  BlogWriter_FlushPosts()
  BlogWriter_FlushPostsPage()
  BlogWriter_FlushPostsPages()
  BlogWriter_FlushTagsPage()
  BlogWriter_FlushTagsPages()
  BlogWriter_FlushMonthsPage()
  BlogWriter_FlushIndexPage()
  BlogWriter_FlushNotFoundPage()
}
