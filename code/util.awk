
function ToSlug(s)
{
  gsub(/[ÁÀÂÃáàâã]/, "a", s)
  gsub(/[ÉÊÊéêê]/, "e", s)
  gsub(/[ÔÕÓôõó]/, "o", s)
  gsub(/[Úú]/, "u", s)
  gsub(/[ÍÏíï]/, "i", s)
  gsub(/[#()'",;:-]/, "", s)
  gsub(/&/, "and", s)
  gsub(/!/, "", s)
  gsub(/\?/, "", s)
  gsub(/\./, "", s)
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

function ToId(s)
{
  gsub(/-/, "", s)
  gsub(/ /, "_", s)
  gsub(/#/, "sharp", s)
  s = "_" s
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

