# Translate special characters from text to
# acceptable characters in final format.
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2024-08-30

function ToSlug(s)
{
  gsub(/[ÁÀÂÃáàâã]/, "a", s)
  gsub(/[ÉÊÊéêê]/, "e", s)
  gsub(/[ÔÕÓôõó]/, "o", s)
  gsub(/[Úú]/, "u", s)
  gsub(/[ÍÏíï]/, "i", s)
  gsub(/[Çç]/, "c", s)
  gsub(/[#()'",;:-]/, "", s)
  gsub(/&/, "and", s)
  gsub(/!/, "", s)
  gsub(/\?/, "", s)
  gsub(/\./, "", s)
  gsub(/%/, "perc", s)
  gsub(/</, "lt", s)
  gsub(/>/, "gt", s)
  gsub(/=/, "_", s)
  gsub(/ /, "_", s)
  gsub(/^_/, "", s)
  gsub(/_$/, "", s)
  gsub(/__/, "_", s)
  return tolower(s)
}

function ToHtml(s)
{
  gsub(/&/, "&amp;", s)
  gsub(/</, "\\&lt;", s)
  gsub(/>/, "\\&gt;", s)
  gsub(/"/, "\\&quot;", s)
  return s
}

function ValidateId(s, id)
{
  if( s == "_" ) {
    print "warning: empty id (_) to " id
  }
}

function ToId(s,    id)
{
  id = s
  gsub(/-/, "", id)
  gsub(/ /, "_", id)
  gsub(/#/, "sharp", id)
  id = "_" id
  ValidateId(s, id)
  return id
}

function ToEpubId(s,    id)
{
  id = s
  gsub(/-/, "", id)
  gsub(/ /, "_", id)
  id = "_" id
  ValidateId(s, id)
  return id
}

function ToEpubLink(s)
{
  gsub(/\?[^"]+/, "", s)
  return s
}

function IsNumeric(x, f)
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

function ToLetter(s)
{
  if( !("Á" in convertLetters) ) {
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

  if( IsNumeric(s) ) return "#"
  s = toupper(s)
  c = convertLetters[s]
  if( c != "" ) return c
  return s
}
