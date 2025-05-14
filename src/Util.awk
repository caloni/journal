# Translate special characters to acceptable characters.

function Util_TitleToSlug(a_title)
{
  gsub(/[ÁÀÂÃáàâã]/, "a", a_title)
  gsub(/[ÉÊÊéêê]/, "e", a_title)
  gsub(/[ÔÕÓôõó]/, "o", a_title)
  gsub(/[Úú]/, "u", a_title)
  gsub(/[ÍÏíï]/, "i", a_title)
  gsub(/[Çç]/, "c", a_title)
  gsub(/[#()'",;:-]/, "", a_title)
  gsub(/&/, "and", a_title)
  gsub(/!/, "", a_title)
  gsub(/\?/, "", a_title)
  gsub(/\./, "", a_title)
  gsub(/%/, "perc", a_title)
  gsub(/</, "lt", a_title)
  gsub(/>/, "gt", a_title)
  gsub(/=/, "_", a_title)
  gsub(/ /, "_", a_title)
  gsub(/^_/, "", a_title)
  gsub(/_$/, "", a_title)
  gsub(/__/, "_", a_title)
  return tolower(a_title)
}

function Util_TextToHtml(a_text)
{
  gsub(/&/, "&amp;", a_text)
  gsub(/</, "\\&lt;", a_text)
  gsub(/>/, "\\&gt;", a_text)
  gsub(/"/, "\\&quot;", a_text)
  return a_text
}

