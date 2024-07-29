# parser using CommonMark spec to blog files
# https://spec.commonmark.org
# Wanderley Caloni 2024-07-27
# v. WIP

BEGIN {
  commonMarkFile = "public\\commonmark.txt"
  currentBlock = ""
}

/^`{3,}/ {
  currentBlock = currentBlock == "fcode" ? "" : "fcode"
  next
}

currentBlock != "" {
  print currentBlock ": " $0 > commonMarkFile
  next
}

# todo: convert journal.txt titles from asciidoc to commonmark
/^= / {
  print "title: " $0 > commonMarkFile
  next
}

/^#{2,6} / {
  print "heading: " $0 > commonMarkFile
  next
}

/^ {4,}/ {
  print "icode: " $0 > commonMarkFile
  next
}

/^\[.*\]: *[^ ]+ *(".*")?/ {
  print "linkrefdef: " $0 > commonMarkFile
  next
}

{
  print "unclassified: " $0 > commonMarkFile
  next
}

