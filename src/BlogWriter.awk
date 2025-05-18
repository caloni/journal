# Transform parsed text to html blog posts.
#
# The following information is available after parsing:
#
# - G_FILES list of months that have some post
# - G_NEXT_MONTH next month (or default page) indexed by current month
# - G_POSTS_BY_MONTH texts of posts indexed by month and date
# - G_POST_LINKS_BY_MONTH permalinks of posts indexed by month and date
# - G_PREV_MONTH previous month (or default page) indexed by current month
# - G_QUICK_SEARCH pages list to be used in quick search

# TODO get hardcoded personal info from metadata file
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

# Write slug indexed post content to month page
function BlogWriter_WritePost(a_slug,    l_tags, l_postText, l_file, l_prefix, l_suffix, l_key, l_key2)
{
  split(G_INDEX[a_slug]["tags"], l_tags)
  l_postText = ""

  l_file = G_SETTINGS["output"] "\\" G_INDEX[a_slug]["month"] ".html"
  # first post in the month
  if( ! (G_INDEX[a_slug]["month"] in G_FILES) )
  {
    BlogWriter_WriteHead(l_file, G_SETTINGS["text_page_prefix"] "::" G_INDEX[a_slug]["month"], "months.html", 0)
    G_FILES[G_INDEX[a_slug]["month"]] = G_INDEX[a_slug]["month"]
  }
  # for each post line
  for( l_key in G_INDEX[a_slug]["lines"] )
  {
    l_prefix = ""
    l_suffix = ""
    l_transformBracketsToSearchLink = 1

    # line is non empty
    if( G_INDEX[a_slug]["lines"][l_key]["content"] != "" )
    {
      # is not preformatted nor quote block
      if( G_INDEX[a_slug]["lines"][l_key]["type"] != "pre" && G_INDEX[a_slug]["lines"][l_key]["type"] != "blockquote" )
      {
        # it is a header (h1, h2, ...)
        if ( substr(G_INDEX[a_slug]["lines"][l_key]["type"], 1, 1) == "h" && length(G_INDEX[a_slug]["lines"][l_key]["type"]) == 2 )
        {
          # replace ampersand only for headers for some reason
          gsub(/&/, "&amp;", G_INDEX[a_slug]["lines"][l_key]["content"])
        }
        # TODO links translation, need more clarification
        for( l_key2 in G_INDEX[a_slug]["links"] )
        {
          if( G_INDEX[a_slug]["links"][l_key2] in G_INDEX_METADATA )
          {
            G_INDEX[a_slug]["links"][l_key2] = "<a href=\"" G_INDEX_METADATA[G_INDEX[a_slug]["links"][l_key2]]["chapter"] ".html#" G_INDEX[a_slug]["links"][l_key2] "\">" l_key2 "</a>"
          }
          gsub("\\[" l_key2 "\\]", G_INDEX[a_slug]["links"][l_key2], G_INDEX[a_slug]["lines"][l_key]["content"])
        }
      }

      # preformatted content
      if( G_INDEX[a_slug]["lines"][l_key]["type"] == "pre" )
      {
        G_INDEX[a_slug]["lines"][l_key]["content"] = Util_TextToHtml(G_INDEX[a_slug]["lines"][l_key]["content"]) "\n"
        if( G_INDEX[a_slug]["lines"][l_key-1]["type"] != G_INDEX[a_slug]["lines"][l_key]["type"] )
        {
          G_INDEX[a_slug]["lines"][l_key]["content"] = "<" G_INDEX[a_slug]["lines"][l_key]["type"] ">\n" G_INDEX[a_slug]["lines"][l_key]["content"]
        }
        if( G_INDEX[a_slug]["lines"][l_key+1]["type"] != G_INDEX[a_slug]["lines"][l_key]["type"] )
        {
          G_INDEX[a_slug]["lines"][l_key]["content"] = G_INDEX[a_slug]["lines"][l_key]["content"] "</" G_INDEX[a_slug]["lines"][l_key]["type"] ">\n"
        }
        l_transformBracketsToSearchLink = 0
      }
      # quote block
      else if ( G_INDEX[a_slug]["lines"][l_key]["type"] == "blockquote")
      {
        l_prefix = l_prefix "<blockquote>"
        l_suffix = l_suffix "</blockquote>"
        l_transformBracketsToSearchLink = 0
      }
      # list
      else if ( G_INDEX[a_slug]["lines"][l_key]["type"] == "list")
      {
        l_prefix = l_prefix "<li>"
        l_suffix = l_suffix "</li>"
        if( G_INDEX[a_slug]["lines"][l_key-1]["type"] != "list" )
        {
          l_prefix = "<ul>" l_prefix
        }
        if( G_INDEX[a_slug]["lines"][l_key+1]["type"] != "list" )
        {
          l_suffix = l_suffix "</ul>"
        }
        l_transformBracketsToSearchLink = 1
      }
      # headers h1, h2, ...
      else if ( substr(G_INDEX[a_slug]["lines"][l_key]["type"], 1, 1) == "h" && length(G_INDEX[a_slug]["lines"][l_key]["type"]) == 2 )
      {
        l_prefix = l_prefix "<h" substr(G_INDEX[a_slug]["lines"][l_key]["type"], 2, 1) ">"
        l_suffix = l_suffix "</h" substr(G_INDEX[a_slug]["lines"][l_key]["type"], 2, 1) ">\n"
        l_transformBracketsToSearchLink = 1
      }
      # paragraph
      else if ( G_INDEX[a_slug]["lines"][l_key]["type"] == "p")
      {
        l_prefix = l_prefix "<p>"
        l_suffix = l_suffix "</p>\n"
        l_transformBracketsToSearchLink = 1
      }
      # image
      else if ( G_INDEX[a_slug]["lines"][l_key]["type"] == "img")
      {
        l_prefix = l_prefix "<img src=\"img/"
        l_suffix = l_suffix "\"/>\n"
        l_transformBracketsToSearchLink = 0
      }

      # no link found, replace [this] for search?q=this
      if( l_transformBracketsToSearchLink )
      {
        G_INDEX[a_slug]["lines"][l_key]["content"] = gensub(/\[([^\]]+)\]/, "<a href=\"posts.html?q=\\1\">\\1</a>", "g", G_INDEX[a_slug]["lines"][l_key]["content"])
      }

      G_INDEX[a_slug]["lines"][l_key]["content"] = l_prefix G_INDEX[a_slug]["lines"][l_key]["content"] l_suffix
    }
  }

  # let's begin to mount post content
  # first the span with anchor
  l_postText = "<span id=\"" a_slug "\" title=\"" Util_TextToHtml(G_INDEX[a_slug]["title"]) "\"/></span>\n"
  # next begin section
  l_postText = l_postText "<section id=\"section_" a_slug "\">\n"
  # put blog post external link
  if( "extlink" in G_INDEX[a_slug] )
  {
    l_postText = l_postText "<p class=\"title\"><a href=\"" G_INDEX[a_slug]["month"] ".html#" a_slug "\">#</a> <a class=\"external\" href=\"" G_INDEX[a_slug]["extlink"] "\">" Util_TextToHtml(G_INDEX[a_slug]["title"]) "</a></p>\n"
  }
  # or it is just random text post
  else
  {
    l_postText = l_postText "<p class=\"title\"><a href=\"" G_INDEX[a_slug]["month"] ".html#" a_slug "\">#</a> " Util_TextToHtml(G_INDEX[a_slug]["title"]) "</p>\n"
  }
  # author and date
  l_postText = l_postText "<span class=\"title-heading\">" G_SETTINGS["author"] ", " G_INDEX[a_slug]["date"]
  # tags
  for( l_key in l_tags )
  {
    l_postText = l_postText " "
    # previous post with same tag navigation
    if( "tag_nav" in G_INDEX[a_slug] && l_tags[l_key] in G_INDEX[a_slug]["tag_nav"] && "prev_in_tag" in G_INDEX[a_slug]["tag_nav"][l_tags[l_key]] )
    {
      l_postText = l_postText "<a href=\"" G_INDEX[G_INDEX[a_slug]["tag_nav"][l_tags[l_key]]["prev_in_tag"]]["link"] "\">&lt;</a>"
    }
    # posts with same tag listing
    l_postText = l_postText "<a href=\"" l_tags[l_key] ".html\">" l_tags[l_key] "</a>"
    # next post with same tag navigation
    if( "tag_nav" in G_INDEX[a_slug] && l_tags[l_key] in G_INDEX[a_slug]["tag_nav"] && "next_in_tag" in G_INDEX[a_slug]["tag_nav"][l_tags[l_key]] )
    {
      l_postText = l_postText "<a href=\"" G_INDEX[G_INDEX[a_slug]["tag_nav"][l_tags[l_key]]["next_in_tag"]]["link"] "\">&gt;</a>"
    }
  }
  # top month page link, copy text link
  l_postText = l_postText "<a href=\"" G_INDEX[a_slug]["month"] ".html\"> "
  l_postText = l_postText "<sup>[up]</sup></a> <a href=\"javascript:;\" onclick=\"copy_clipboard('section#section_" a_slug "')\"><sup>[copy]</sup></a></span>\n\n"
  # finally post lines
  for( l_key in G_INDEX[a_slug]["lines"] )
  {
    l_postText = l_postText G_INDEX[a_slug]["lines"][l_key]["content"]
    # non preformatted, insert new line for html asthetics
    if( G_INDEX[a_slug]["lines"][l_key]["type"] != "pre" )
    {
      l_postText = l_postText "\n"
    }
  }
  # end of post section
  l_postText = l_postText "</section><hr/>\n"
  # append final post final text to posts of this month
  G_POSTS_BY_MONTH[G_INDEX[a_slug]["month"]][G_INDEX[a_slug]["date"]] = G_POSTS_BY_MONTH[G_INDEX[a_slug]["month"]][G_INDEX[a_slug]["date"]] "\n" l_postText
  # append link to post to post links to this month
  # TODO why using extlink?
  G_INDEX[a_slug]["extlink"] = "<li><small><a href=\"" G_INDEX[a_slug]["month"] ".html#" a_slug "\">" Util_TextToHtml(G_INDEX[a_slug]["title"]) "</a></small></li>"
  G_POST_LINKS_BY_MONTH[G_INDEX[a_slug]["month"]][G_INDEX[a_slug]["date"]] = G_POST_LINKS_BY_MONTH[G_INDEX[a_slug]["month"]][G_INDEX[a_slug]["date"]] "\n" G_INDEX[a_slug]["extlink"]
  # add post permalink to quick search listing
  G_QUICK_SEARCH[a_slug] = G_INDEX[a_slug]["month"] ".html#" a_slug
  # cleanup
  delete l_tags
  l_postText = l_file = l_prefix = l_suffix = l_key = l_key2 = ""
}

# Write all posts to months page ordered by journal numerical position
function BlogWriter_FlushPosts(    l_position, l_slug)
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( l_position in G_POST_SLUG_BY_POSITION )
  {
    l_slug = G_POST_SLUG_BY_POSITION[l_position]
    # posts without a date are treated as invisible drafts
    if( !("date" in G_INDEX[l_slug]) )
    {
      print "skipping", l_slug
      continue
    }
    BlogWriter_WritePost(l_slug)
  }
  l_position = l_slug = ""
}

function BlogWriter_WriteHead(a_file, a_title, a_backLink, a_filter, a_quickSearch,    l_originalSort, l_key)
{
  print "<!DOCTYPE html>" > a_file
  print "<html lang=\"en-us\" dir=\"ltr\" itemscope itemtype=\"http://schema.org/Article\">" > a_file
  print "<head>" > a_file
  print "<meta charset=\"utf-8\" />" > a_file
  print "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/>" > a_file
  print "<title>" G_SETTINGS["title"] "</title>" > a_file
  print "<meta name=\"author\" content=\"" G_SETTINGS["author"] "\" />" > a_file
  print "<meta name=\"generator\" content=\"" G_SETTINGS["generator"] "\">" > a_file
  print "<meta property=\"og:title\" content=\"" G_SETTINGS["title"] "\"/>" > a_file
  print "<meta property=\"og:type\" content=\"website\"/>" > a_file
  print "<meta property=\"og:url\" content=\"" G_SETTINGS["link"] "\"/>" > a_file
  print "<meta property=\"og:image\" content=\"" G_SETTINGS["post-image"] "\"/>" > a_file
  print "<meta property=\"og:description\" content=\"" G_SETTINGS["description"] "\"/>" > a_file
  print "<link href=\"/index.xml\" rel=\"feed\" type=\"application/rss+xml\" title=\"" G_SETTINGS["title"] "\"/>" > a_file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/custom.css\"/>" > a_file
  print "<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/jquery-ui.css\"/>" > a_file
  print "<script src=\"/js/jquery-1.12.4.js\"></script>" > a_file
  print "<script src=\"/js/jquery-ui.js\"></script>" > a_file
  print "<script src=\"/js/copy_clipboard.js\"></script>" > a_file
  print "<script>" > a_file
  print "var quick_search_posts = [ " > a_file
  l_originalSort = PROCINFO["sorted_in"]
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( l_key in a_quickSearch )
  {
    print "\"" a_quickSearch[l_key] "\"," > a_file
  }
  PROCINFO["sorted_in"] = l_originalSort
  print " ]; " > a_file
  print "</script>" > a_file
  print "<script src=\"/js/quick_search.js\"></script>" > a_file
  print "<script src=\"/js/list.js\"></script>" > a_file
  print "<link rel=\"icon\" href=\"/img/favicon.ico\"/>" > a_file
  print "</head>" > a_file
  print "<body style=\"min-height:100vh;display:flex;flex-direction:column\">" > a_file
  print "<nav class=\"navbar has-shadow is-white\"" > a_file
  print "role=\"navigation\" aria-label=\"main navigation\">" > a_file
  print "<div class=\"container\">" > a_file
  print "<div class=\"navbar-brand\">" > a_file
  print "&nbsp;" > a_file
  print "<a class=\"navbar-item\" href=\"" a_backLink "\">" > a_file
  print "<div class=\"is-4\"><b>" a_title "</b></div>" > a_file
  print "</a>" > a_file
  print "</div>" > a_file
  print "</div>" > a_file
  print "</nav>" > a_file
  print "<div class=\"container\">" > a_file
  print "<div class=\"column\">" > a_file
  print "<div style=\"min-height:56vh\">" > a_file
  print "<div style=\"padding-bottom: 1em;\"></div>" > a_file
  if( a_filter )
  {
    print "<input type=\"text\" name=\"filter\" value=\"\" id=\"filter\" placeholder=\"enter to select\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > a_file
    print "<button id=\"filterbutton\" style=\"font-size: 1rem;\" onclick=\"ApplyFilter($('#filter').val());\">select</button>" > a_file
    print "<button id=\"removebutton\" style=\"font-size: 1rem;\" onclick=\"ApplyNotFilter($('#filter').val());\">remove</button>" > a_file
    print "<button id=\"randombutton\" style=\"font-size: 1rem;\" onclick=\"window.location = randomPost;\">random</button>" > a_file
    print "<div><big><b><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></b></big></div>" > a_file
    print "<table class=\"sortable\" style=\"width: 100%;\">" > a_file
  }
  l_originalSort = l_key = ""
}

function BlogWriter_WriteBottom(a_file, a_filter, a_nextLink, a_prevLink, a_build,    l_label, l_link)
{
  if( a_filter )
  {
    print "</table>" > a_file
  }
  print "<span style=\"float: left;\">" > a_file
  if( a_nextLink && a_nextLink != "" )
  {
    l_link = a_nextLink
    sub(/\.html/, "", a_nextLink)
    l_label = a_nextLink
    print " <a href=\"" l_link "\">[" l_label "]</a>" > a_file
  }
  if( a_prevLink && a_prevLink != "" )
  {
    l_link = a_prevLink
    sub(/\.html/, "", a_prevLink)
    l_label = a_prevLink
    print " <a href=\"" l_link "\">[" l_label "]</a>" > a_file
  }
  print "</span>" > a_file
  print "</div>" > a_file
  print "</div>" > a_file
  print "</section>" > a_file
  print "<footer class=\"footer\">" > a_file
  print "<div class=\"container\">" > a_file
  if( a_build )
  {
    print "<p style=\"text-align: right;\" class=\"tiny\"><i>" a_build "</i></p>" > a_file
  }
  print "</div>" > a_file
  print "<div class=\"intentionally-blank\"></div>" > a_file
  print "</footer>" > a_file
  print "</body>" > a_file
  print "</html>" > a_file
  l_label = l_link = ""
}

# Link months with each other chronologically
function BlogWriter_TiePreviousMonths(    l_key, l_defaultLink)
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  l_defaultLink = "index"
  for( l_key in G_MONTHS )
  {
    G_NEXT_MONTH[l_key] = l_defaultLink
    l_defaultLink = l_key
  }
  PROCINFO["sorted_in"] = "@ind_num_desc"
  l_defaultLink = "index"
  for( l_key in G_MONTHS )
  {
    G_PREV_MONTH[l_key] = l_defaultLink
    l_defaultLink = l_key
  }
  l_key = l_defaultLink = ""
}

function BlogWriter_FlushMonthsPage(    l_key, l_file, l_year, l_month, l_year2)
{
  PROCINFO["sorted_in"] = "@ind_num_desc"
  l_file = G_SETTINGS["output"] "\\months.html"
  BlogWriter_WriteHead(l_file, G_SETTINGS["text_page_prefix"] "::months", "index.html", 0)
  l_year = "2001"
  for( l_key in G_MONTHS )
  {
    l_year2 = substr(l_key, 1, 4)
    l_month = substr(l_key, 6, 2)
    if( l_year2 != l_year )
    {
      if( l_year != "2001" )
      {
        print "</p>" > l_file
      }
      print "<p id=\"" l_key "\" class=\"toc\"><strong>" l_year2 "</strong>" > l_file
      l_year = l_year2
    }
    print "<a href=\"" l_key ".html\"> " Util_TextToHtml(l_month) " </a>" > l_file
    G_QUICK_SEARCH[l_key] = l_key ".html"
    if( ! LastMonth )
    {
      LastMonth = l_key
    }
  }
  print "</p>" > l_file
  BlogWriter_WriteBottom(l_file, 0)
  G_QUICK_SEARCH["months"] = "months.html"
  l_key = l_file = l_year = l_month = l_year2 = ""
}

function BlogWriter_FlushPostsPages(    l_key, l_key2, l_posts, l_file)
{
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( l_key in G_FILES )
  {
    l_posts = ""
    l_file = G_SETTINGS["output"] "\\" l_key ".html"
    PROCINFO["sorted_in"] = "@ind_num_asc"
    for( l_key2 in G_POSTS_BY_MONTH[l_key] )
    {
      l_posts = l_posts "\n" G_POSTS_BY_MONTH[l_key][l_key2]
    }
    print "<ul style=\"list-style: none;\">" > l_file
    for( l_key2 in G_POST_LINKS_BY_MONTH[l_key] )
    {
      print G_POST_LINKS_BY_MONTH[l_key][l_key2] > l_file
    }
    print "</ul>" > l_file
    print l_posts > l_file
    BlogWriter_WriteBottom(l_file, 0, G_NEXT_MONTH[l_key] ".html", G_PREV_MONTH[l_key] ".html")
  }
  l_key = l_key2 = l_posts = l_file = ""
}

function BlogWriter_FlushTagsPages(    l_key, l_key2, l_key3, l_key4, l_file, l_tags, l_tagsText)
{
  PROCINFO["sorted_in"] = "@ind_num_desc"
  for( l_key in G_SLUGS_BY_TAGS_AND_DATES )
  {
    G_QUICK_SEARCH[l_key] = l_key ".html"
    l_file = G_SETTINGS["output"] "\\" l_key ".html"
    BlogWriter_WriteHead(l_file, G_SETTINGS["text_page_prefix"] "::" l_key, "index.html", 1)
    for( l_key2 in G_SLUGS_BY_TAGS_AND_DATES[l_key] )
    {
      for( l_key3 in G_SLUGS_BY_TAGS_AND_DATES[l_key][l_key2] )
      {
        split(G_INDEX[l_key3]["tags"], l_tags)
        l_tagsText = ""
        for( l_key4 in l_tags )
        {
          l_tagsText = l_tagsText " " l_tags[l_key4]
        }
        print "<tr><td>" > l_file
        if( G_INDEX[l_key3]["image"] )
        {
          print "<img src=\"img/" G_INDEX[l_key3]["image"] "\"/>" > l_file
        }
        print "<b><a href=\"" G_INDEX[l_key3]["month"] ".html#" l_key3 "\">" Util_TextToHtml(G_INDEX[l_key3]["title"]) "</a></b>" > l_file
        print "<small><i>" G_INDEX[l_key3]["date"] l_tagsText " " G_INDEX[l_key3]["summary"] "</small></i>" > l_file
        print "</td></tr>" > l_file
      }
    }
    BlogWriter_WriteBottom(l_file, 1)
  }
  delete l_tags
  l_key = l_key2 = l_key3 = l_key4 = l_file = l_tagsText = ""
}

function BlogWriter_FlushTagsPage(    l_key, l_key2, l_key3, l_file, l_titleText, l_titleMax)
{
  l_file = G_SETTINGS["output"] "\\tags.html"
  BlogWriter_WriteHead(l_file, G_SETTINGS["text_page_prefix"] "::tags", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_asc"
  for( l_key in G_SLUGS_BY_TAGS_AND_DATES )
  {
    print "<button class=\"tagbutton\" style=\"font-size: 1rem;\" onclick=\"window.location = '" l_key ".html';\">" Util_TextToHtml(l_key) "</button>" > l_file
  }
  for( l_key in G_SLUGS_BY_TAGS_AND_DATES )
  {
    l_titleText = ""
    l_titleMax = 0
    PROCINFO["sorted_in"] = "@ind_num_desc"
    for( l_key2 in G_SLUGS_BY_TAGS_AND_DATES[l_key] )
    {
      for( l_key3 in G_SLUGS_BY_TAGS_AND_DATES[l_key][l_key2] )
      {
        if( l_titleText == "" )
        {
          l_titleText = G_INDEX[l_key3]["title"]
        }
        else
        {
          l_titleText = l_titleText " - " G_INDEX[l_key3]["title"]
        }
        l_titleMax = l_titleMax + 1
        if( l_titleMax > 15 )
        {
          break
        }
      }
      if( l_titleMax > 15 )
      {
        break
      }
    }
    print "<tr><td><b><a href=\"" l_key ".html" "\">" Util_TextToHtml(l_key) "</a></b>" > l_file
    print "<small><i>" l_titleText "</small></i>" > l_file
    print "</td></tr>" > l_file
  }
  BlogWriter_WriteBottom(l_file, 1)
  G_QUICK_SEARCH["tags"] = "tags.html"
  l_key = l_key2 = l_key3 = l_file = l_titleText = l_titleMax = ""
}

# Write page with all posts slug, title, tags and summary ordered by date
function BlogWriter_FlushPostsPage(    l_date, l_slug, l_tags, l_tag, l_file, l_title, l_tagsText)
{
  BlogWriter_TiePreviousMonths()

  l_file = G_SETTINGS["output"] "\\posts.html"
  BlogWriter_WriteHead(l_file, G_SETTINGS["text_page_prefix"] "::posts", "index.html", 1)
  PROCINFO["sorted_in"] = "@ind_str_desc"
  for( l_date in G_DATE_SLUG_TITLE )
  {
    # for each post
    for( l_slug in G_DATE_SLUG_TITLE[l_date] )
    {
      l_title = G_DATE_SLUG_TITLE[l_date][l_slug]
      split(G_INDEX[l_slug]["tags"], l_tags)
      l_tagsText = ""
      for( l_tag in l_tags )
      {
        l_tagsText = l_tagsText " " l_tags[l_tag]
      }
      print "<tr><td>" > l_file
      if( G_INDEX[l_slug]["image"] )
      {
        print "<img src=\"img/" G_INDEX[l_slug]["image"] "\"/>" > l_file
      }
      print "<b><a href=\"" G_INDEX[l_slug]["month"] ".html#" l_slug "\">" Util_TextToHtml(l_title) "</a></b>" > l_file
      print "<small><i>" G_INDEX[l_slug]["date"] l_tagsText " " G_INDEX[l_slug]["summary"] " " l_slug "</small></i>" > l_file
      print "</td></tr>" > l_file
    }
  }
  BlogWriter_WriteBottom(l_file, 1)
  G_QUICK_SEARCH["posts"] = "posts.html"
  #cleanup
  delete l_tags
  l_date = l_slug = l_tag = l_file = l_title = l_tagsText = ""
}

function BlogWriter_FlushIndexPage(    l_favtags, l_key, l_month, l_file, l_build_link)
{
  split(G_SETTINGS["text_favorite_tags"], l_favtags)
  PROCINFO["sorted_in"] = "@ind_num_asc"
  for( l_key in G_MONTHS )
  {
    l_month = G_MONTHS[l_key]
    break
  }
  l_file = G_SETTINGS["output"] "\\index.html"
  BlogWriter_WriteHead(l_file, G_SETTINGS["title"], l_month ".html#about", 0, G_QUICK_SEARCH)
  print "<input type=\"text\" name=\"quick_search_name\" value=\"\" id=\"quick_search\" placeholder=\"" G_SETTINGS["text_quicksearch"] "\" style=\"width: 100%; font-size: 1.5rem; margin-top: 1em; margin-bottom: 0.5em;\" title=\"\"/></br>" > l_file
  for( l_key in l_favtags )
  {
    print "<big><a href=\"" l_favtags[l_key] ".html\">" l_favtags[l_key] "</a></big><small><i>: " G_SETTINGS["text_favorite_tags_description"][l_favtags[l_key]] "</small></i></br>" > l_file
  }
  print "<big><a href=\"tags.html\">tags</a></big><small><i>: " G_SETTINGS["text_tags"] "</small></i></br>" > l_file
  print "<big><a href=\"months.html\">months</a></big><small><i>: " G_SETTINGS["text_months"] "</small></i></br>" > l_file
  print "<big><a href=\"posts.html\">posts</a></big><small><i>: " G_SETTINGS["text_posts"] "</small></i></br>" > l_file
  print "<div><big><span style=\"visibility: hidden; padding: 5px;\" name=\"results\" id=\"results\">...</span></big></div>" > l_file
  print "<table class=\"sortable\" style=\"width: 100%;\">" > l_file
  print "</table>" > l_file
  l_build_link = "<a href=\"" G_SETTINGS["public_repo_github_address"] "/commit/" G_SETTINGS["build"] "\">" G_SETTINGS["build"] "</a>"
  BlogWriter_WriteBottom(l_file, 0, "", "", G_SETTINGS["date"] "-" l_build_link)
  delete l_favtags
  l_key = l_month = l_file = l_build_link = ""
}

function BlogWriter_FlushNotFoundPage(    l_file)
{
  l_file = G_SETTINGS["output"] "\\404.html"
  BlogWriter_WriteHead(l_file, G_SETTINGS["text_page_prefix"] "::404 page not found", "posts.html", 0)
  print "<div class=\"container\">" > l_file
  print "  <p class=\"title\">" G_SETTINGS["text_notfound_title"] "</p>" > l_file
  print "    <div class=\"content\">" > l_file
  print "      <p>" G_SETTINGS["text_notfound_description"] "</p>" > l_file
  print "    </div>" > l_file
  print "</div>" > l_file
  BlogWriter_WriteBottom(l_file, 0)
  l_file = ""
}

# Flush posts and metadata to html files
END {
  BlogWriter_FlushPosts()
  BlogWriter_FlushPostsPage()
  BlogWriter_FlushPostsPages()
  BlogWriter_FlushTagsPage()
  BlogWriter_FlushTagsPages()
  BlogWriter_FlushMonthsPage()
  BlogWriter_FlushIndexPage()
  BlogWriter_FlushNotFoundPage()

  # cleanup
  delete G_FILES
  delete G_NEXT_MONTH
  delete G_POSTS_BY_MONTH
  delete G_POST_LINKS_BY_MONTH
  delete G_PREV_MONTH
  delete G_QUICK_SEARCH
}
