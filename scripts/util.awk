
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
  gsub(/ /, "_", s)
  return tolower(s)
}


function ToHtml(s)
{
  gsub(/&/, "&amp;", s)
  gsub(/</, "\\&lt;", s)
  gsub(/>/, "\\&gt;", s)
  return s
}

