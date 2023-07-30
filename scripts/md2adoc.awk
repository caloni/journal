/^---$/ {
  header = ! header;
  if( header ) {
    print ""
    delete fields
  } else {
    print "= " title
    for( f in fields )
      print ":" f fields[f]
  }
  next
}

/^[a-z]+:/ {
  if( header )
    if( $1 == "title:" )
      title = substr($0, length($1) + 2)
      titleStart = substr(title, 1, 1);
      if( titleStart == "\"" || titleStart == "'" ) {
        title = substr(title, 2, length(title) - 2);
      }
    else {
      field = $1
      if( NF > 1 && $2 != "[]" )
        fields[field] = substr($0, length($1) + 1)
    }
}

/^ - [a-z]+$/ {
  if( header )
    fields[field] = fields[field] " " $2
}

{
  if( ! header )
    print
}
