
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

function ToId(str)
{
  gsub(/-/, "", str)
  gsub(/#/, "sharp", str)
  str = "_" str
  return str
}

