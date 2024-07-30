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

/^> / {
  print "quote: " $0 > commonMarkFile
  next
}

/^ - / {
  print "list: " $0 > commonMarkFile
  next
}

# todo: convert journal.txt titles from asciidoc to commonmark
/^= / {
  print "title: " $0 > commonMarkFile
  next
}

# todo: convert journal.txt heading entries from asciidoc to invisible linkref items in commonmark
/^:.*: .+/ {
  print "head: " $0 > commonMarkFile
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
  print "linkdef: " $0 > commonMarkFile
  next
}

/.+/ {
  print "p: " $0 > commonMarkFile
  next
}

/^$/ {
  print $0 > commonMarkFile
  next
}

{
  print "undef: " $0 > commonMarkFile
  next
}

