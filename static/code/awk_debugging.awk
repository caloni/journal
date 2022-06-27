BEGIN {
  counter = 0;
}
END {
  print counter;
}
{
  if( $1 == "this" )
    counter++;
}

