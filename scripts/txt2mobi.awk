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
  gsub(/"/, "\\&quot;", str)
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
  gsub(/&/, "and", str)
  gsub(/!/, "", str)
  gsub(/ /, "-", str)
  return tolower(str)
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

function readfile(file,     tmp, save_rs)
{
  save_rs = RS
  RS = "^$"
  getline tmp < file
  close(file)
  RS = save_rs
  return tmp
}

function writepost()
{
  ++postCount
  if( slug == "" ) {
    slug = toslug(title)
  }
  entries[substr(title, 1, 1),title] = title
  sterms = ""
  split(tags, stags)
  for( t in stags ) {
    terms[stags[t]][title] = title
    sterms = sterms " <a href=\"toc" toid(stags[t]) ".xhtml\">" stags[t] "</a>"
  }
  slugs[slug]["slug"] = slug
  slugs[slug]["title"] = title
  slugs[slug]["date"] = date
  titleToSlug[title] = slug
  titleToChapter[title] = chapter
  if( draft ) {
    draftToSlug[title] = slug
    drafts = "public\\book\\drafts.txt"
    print "= " title > drafts
    print draftContent > drafts
    print "\n\n" > drafts
  }
  fchapter = toid(chapter)
  file = "public\\book\\MOBI\\" fchapter ".html"
  if( ! (fchapter in files) ) {
    print "<!DOCTYPE html>" > file
    print "<head>" > file
    print "<meta name=\"generator\" content=\"AWK\">" > file
    print "<title>" tohtml(chapter) "</title>" > file
    print "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf8\">" > file
    print "</head>" > file
    print "<body>" > file
    print "<h2 class=\"chapter-title\" style=\"page-break-before: always\" id=\"" toid(chapter) "\">" tohtml(chapter) "</h2>" > file
    files[fchapter] = fchapter
  }
  print "<h3 class=\"title\" style=\"page-break-before: always\" id=\"" toid(slug) "\">" tohtml(title) "</h3>" > file
  print "<i>Wanderley Caloni, " date "</i>" > file
  print content > file
  print "<span>" > file
  print "</span>" > file
}


function formatContent(content)
{
  prefix = "\n"
  suffix = ""
  paragraph = 1

  do {
    #todo: test
    #if( index(content, "```") == 1 ) {
    #  content = ""
    #  if( contentState["```"] ) {
    #    prefix = "</pre>"
    #    contentState["```"] = 0
    #  } else {
    #    contentState["```"] = 1
    #    prefix = prefix "<pre>"
    #  }
    #  break
    #} else if( contentState["```"] ) {
    #  break
    #}

    #todo: test
    #if( content ~ /^ *- */ ) {
    #  content = gensub(/ *- *(.*)/, "\\1", "g", content)
    #  if( ! contentState["-"] ) {
    #    prefix = prefix "<ul>"
    #    contentState["-"] = 1
    #  }
    #  prefix = prefix "<li>"
    #  suffix = "</li>" suffix
    #  paragraph = 0
    #} else if ( contentState["-"] ) {
    #    prefix = "</ul>\n"
    #    contentState["-"] = 0
    #}

    #todo: test
    #if( content ~ /^ +/ ) {
    #  sub(/^ /, "", content)
    #  if( ! contentState[" "] ) {
    #    prefix = prefix "<pre>"
    #    contentState[" "] = 1
    #  }
    #  break
    #} else if ( contentState[" "] ) {
    #    prefix = "</pre>\n"
    #    contentState[" "] = 0
    #}

    #todo: test
    #if( content ~ /^#+ / ) {

    #  if( content ~ /^# / ) {
    #    headerLevel = 2
    #  } else if( content ~ /^## / ) {
    #    headerLevel = 3
    #  } else if( content ~ /^### / ) {
    #    headerLevel = 4
    #  } else {
    #    headerLevel = 5
    #  }
    #  gsub(/^#+ /, "", content)

    #  prefix = prefix "<h" headerLevel ">"
    #  suffix = "</h" headerLevel ">" suffix
    #  paragraph = 0
    #}

    if( content ~ /^\[[^]]+\]:/ ) {
      endName = index(content, ":")
      name = substr(content, 2, endName - 3)
      link = substr(content, endName + 2)

      if( link ~ /[a-z]:\/\// ) {
        link = "<a href=\"" link "\">" name "</a>"
      }
      else {
        link = gensub(/(.*)/, "index.xhtml", "g", link)
        link = "<a href=\"" link "\">" name "</a>"
      }

      links[name] = link
      content = ""
      break
    }

    if( index(content, "{{< image src=") == 1 ) {
      newImage = gensub(/.*{{< image src="([^"]+)".*/, "img/" slug "-\\1", "g", content)
      g_postImages[newImage] = newImage
      content = gensub(/{{< image src="([^"]+)".*>}}/, "<img src=\"img/" slug "-\\1\"/>", "g", content)
      break
    }

    gsub(/&/, "&amp;", content)
    gsub(/</, "\\&lt;", content)
    gsub(/>/, "\\&gt;", content)
    content = gensub(/\[([^&]]+)\]\(([^)]+)\)/, "<a href=\"\\2\">\\1</a>", "g", content)

    if( paragraph ) {
      content = "<p>" content "</p>"
    }

  } while( 0 )

  return prefix content suffix
}


/^= / {
  if( content ) {
    writepost()
    content = ""
    draftContent = ""
    slug = ""
    tags = ""
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
    date = $2
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
  else if( $1 == ":draft:" ) {
    draft = 1
  }
}

/^[^=:]/ {
  draftContent = draftContent "\n\n" $0
  newContent = formatContent($0)
  if( content ) {
    content = content newContent
  } else {
    summary = $0
    content = newContent
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
    draftContent = ""
  }

  for( f in files ) {
    file = "public\\book\\MOBI\\" f ".html"
    print "</body>" > file
    print "</html>" > file
  }

  package = "public\\book\\MOBI\\book.opf"
  print "<?xml version=\"1.0\" encoding=\"utf-8\"?>" > package
  print "<package unique-identifier=\"uid\" xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:asd=\"http://www.idpf.org/asdfaf\">" > package
  print "<metadata>" > package
  print "<dc-metadata  xmlns:dc=\"http://purl.org/metadata/dublin_core\" xmlns:oebpackage=\"http://openebook.org/namespaces/oeb-package/1.0/\">" > package
  print "<dc:Title>Blogue do Caloni: Programação, Depuração, Transpiração</dc:Title>" > package
  print "<dc:Language>pt</dc:Language>" > package
  print "<dc:Creator>Wanderley Caloni</dc:Creator>" > package
  print "<dc:Copyrights>Copyright Wanderley Caloni</dc:Copyrights>" > package
  print "<dc:Publisher>Blogue do Caloni</dc:Publisher>" > package
  print "<x-metadata>" > package
  print "<EmbeddedCover>cover.jpg</EmbeddedCover>" > package
  print "</x-metadata>" > package
  print "</dc-metadata>" > package
  print "</metadata>" > package
  print "<manifest>" > package
  print "<item id=\"content\" media-type=\"text/x-oeb1-document\" href=\"index.html\"></item>" > package
  contentId = 1
  for( chapter in chapters ) {
    print "<item id=\"content" contentId++ "\" media-type=\"text/x-oeb1-document\" href=\"" toid(chapter) ".html\"></item>" > package
  }
  print "</manifest>" > package
  print "<spine toc=\"toc\">" > package
  print "<itemref idref=\"content\"/>" > package
  print "</spine>" > package
  print "<guide>" > package
  print "<reference href=\"index.html#toc\" type =\"toc\" title =\"#include <map>\"/>" > package
  for( chapter in chapters ) {
    print "<reference href=\"" toid(chapter) ".html\" type =\"text\" title =\"another chapter\"/>" > package
  }
  print "</guide>" > package
  print "</package>" > package

  tocncx = "public\\book\\MOBI\\book.ncx"
  print "<?xml version=\"1.0\"?>" > tocncx
  print "<!DOCTYPE ncx PUBLIC \"-//NISO//DTD ncx 2005-1//EN\"" > tocncx
  print "\"http://www.daisy.org/z3986/2005/ncx-2005-1.dtd\">" > tocncx
  print "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\">" > tocncx
  print "<head>" > tocncx
  print "</head>" > tocncx
  print "<docTitle>" > tocncx
  print "<text>Blogue do Caloni: Programação, Depuração, Transpiração</text>" > tocncx
  print "</docTitle>" > tocncx
  print "<navMap>" > tocncx
  print "<navPoint id=\"toc\" playOrder=\"1\"><navLabel> <text>#include <map></text> </navLabel> <content src=\"index.html#toc\"/> </navPoint>" > tocncx
  playOrder = 2
  for( chapter in chapters ) {
    print "<navPoint id=\"text" playOrder "\" playOrder=\"" playOrder "\"><navLabel> <text>int main</text> </navLabel> <content src=\"" toid(chapter) ".html\"/> </navPoint>" > tocncx
    playOrder = playOrder + 1
  }
  print "</navMap>" > tocncx
  print "</ncx>" > tocncx

  tocxhtml = "public\\book\\MOBI\\index.html"
  print "<!DOCTYPE html>" > tocxhtml
  print "<head>" > tocxhtml
  print "<meta name=\"generator\" content=\"AWK\">" > tocxhtml
  print "<title>Blogue do Caloni</title>" > tocxhtml
  print "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf8\">" > tocxhtml
  print "</head>" > tocxhtml
  print "<body style=\"min-height:100vh;display:flex;flex-direction:column\">" > tocxhtml
  print "<h2 id=\"text\" style=\"page-break-before: always;\">E Deus disse: 'int main() {}'. E pronto: surgiu o primeiro erro de compilação...</h2>" > tocxhtml
  print "<h3 class=\"title\" style=\"page-break-before: always\" id=\"about\">Wanderley Caloni</h3>" > tocxhtml
  print "<i>Wanderley Caloni, 2007-06-14</i>" > tocxhtml
  print "<p>Quer entrar em contato? Mande o bom e velho <a href=\"mailto:wanderleycaloni@gmail.com\">email</a>.</p>" > tocxhtml
  print "<p>Wanderley Caloni é um programador C/C++ especializado em backend para Windows que decidiu ter seu próprio blogue técnico a pedidos insistentes do seu amigo Rodrigo Strauss, que estava blogando já fazia alguns anos no <a href=\"https://www.1bit.com.br\">www.1bit.com.br</a>. Busco mantê-lo atualizado por esses longos anos de programação, depuração e transpiração com minhas peripécias do dia-a-dia. Eventualmente me tornei crítico de cinema e juntei aqui essas duas escovas de dentes, textos técnicos e cinematográficos, o que acabou tornando o saite gigante a ponto de eu precisar trocar meu static site generator para algo mais rápido como Hugo.</p>" > tocxhtml
  print "<span>" > tocxhtml
  print "</span>" > tocxhtml
  print "<h2 id=\"toc\" style=\"page-break-before: always;\">Index</h2>" > tocxhtml
  print "<h2>" > tocxhtml
  print "<a href=\"#letterW\">W</a>" > tocxhtml
  print "</h2>" > tocxhtml
  print "<h2 id=\"categories\">Categories</h2>" > tocxhtml
  print "<ul>" > tocxhtml
  print "<li><a href=\"#categories-blog\">Blog(14)</a></li>" > tocxhtml
  print "</ul>" > tocxhtml
  print "<h2 id=\"tags\">Tags</h2>" > tocxhtml
  print "<ul>" > tocxhtml
  print "<li><a href=\"#tags-ccpp\">Ccpp(36)</a></li>" > tocxhtml
  print "</ul>" > tocxhtml
  print "<h2><a href=\"#text\">Posts</a></h2>" > tocxhtml
  print "<ul>" > tocxhtml
  for( e in entries ) {
    split(e, letterAndTitle, SUBSEP)
    title = letterAndTitle[2]
    print "<li><a href=\"" toid(titleToChapter[title]) ".html#" toid(titleToSlug[title]) "\">" tohtml(title) "</a></li>" > tocxhtml
  }
  print "</ul>" > tocxhtml
  print "<h2 id=\"letters\" style=\"page-break-before: always;\">Letters</h2>" > tocxhtml
  print "<ul>" > tocxhtml
  print "<h3 id=\"letterW\">W</h3>" > tocxhtml
  print "<li><a href=\"#about\">Wanderley Caloni</a></li>" > tocxhtml
  print "</ul>" > tocxhtml
  print "<h2 id=\"categories\" style=\"page-break-before: always;\">Categories</h2>" > tocxhtml
  print "<ul>" > tocxhtml
  print "<h3 id=\"categories-blog\">Blog</h3>" > tocxhtml
  print "<li><a href=\"#about\">Wanderley Caloni</a></li>" > tocxhtml
  print "</ul>" > tocxhtml
  "date" | getline currentDate
  print "<div id=\"end\" style=\"page-break-before: always;\"><p>Obrigado por ler! =)</p><p>Blogue do Caloni " currentDate ". </p></div>" > tocxhtml
  print "</body>" > tocxhtml
  print "</html>" > tocxhtml
}

