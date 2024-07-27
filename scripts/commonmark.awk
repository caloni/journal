# parser using CommonMark spec to blog files
# https://spec.commonmark.org
# Wanderley Caloni 2024-07-27
# v. WIP

# todo: convert journal.txt titles from asciidoc to commonmark
/^= / {
  commonmark = "public\\commonmark.txt"
  print $0 > commonmark
}

