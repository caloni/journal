/^= / { title = substr($0, 3) }
/^[^=]+/ { content = content "<p>" $0 "</p>\n" }
# END { print title ; print "" ; print content }
