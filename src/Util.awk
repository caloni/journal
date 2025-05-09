# Translate special characters to acceptable characters.

function Util_ToSlug(s)
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

function Util_ToHtml(s)
{
  gsub(/&/, "&amp;", s)
  gsub(/</, "\\&lt;", s)
  gsub(/>/, "\\&gt;", s)
  gsub(/"/, "\\&quot;", s)
  return s
}

function Util_ValidateId(s, id)
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
  Util_ValidateId(s, id)
  return id
}

function ToEpubId(s,    id)
{
  id = s
  gsub(/-/, "", id)
  gsub(/ /, "_", id)
  id = "_" id
  Util_ValidateId(s, id)
  return id
}

function Util_ToEpubLink(s)
{
  gsub(/\?[^"]+/, "", s)
  return s
}

function Util_IsNumeric(x, f)
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

function Util_ToLetter(s)
{
  if( !("Á" in G_CONVERT_LETTERS) ) {
    G_CONVERT_LETTERS["Á"] = "A"
    G_CONVERT_LETTERS["À"] = "A"
    G_CONVERT_LETTERS["Â"] = "A"
    G_CONVERT_LETTERS["Ã"] = "A"
    G_CONVERT_LETTERS["É"] = "E"
    G_CONVERT_LETTERS["Ê"] = "E"
    G_CONVERT_LETTERS["Ê"] = "E"
    G_CONVERT_LETTERS["Ô"] = "O"
    G_CONVERT_LETTERS["Õ"] = "O"
    G_CONVERT_LETTERS["Ó"] = "O"
    G_CONVERT_LETTERS["Ú"] = "U"
    G_CONVERT_LETTERS["Í"] = "I"
    G_CONVERT_LETTERS["Ï"] = "I"
    G_CONVERT_LETTERS["("] = "#"
    G_CONVERT_LETTERS[")"] = "#"
    G_CONVERT_LETTERS["'"] = "#"
    G_CONVERT_LETTERS["\""] = "#"
  }

  if( Util_IsNumeric(s) ) return "#"
  s = toupper(s)
  c = G_CONVERT_LETTERS[s]
  if( c != "" ) return c
  return s
}
