/:tags:/ {
  tags = ":tags:"
  for ( i=2 ; i<=NF ; ++i ) {
    tags = tags " tag_" $i
  }
  print tags
  next
}

{
  print
}
