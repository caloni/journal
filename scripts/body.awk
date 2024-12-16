BEGIN {
  Tendency = 57.2
  WeigthCurve = 0.05
}

/^$/ { exit }

NF == 3 {
  if( $2 ) {
    if( Tendency ) {
      Tendency = Tendency + WeigthCurve * ($2 - Tendency)
    } else {
      Tendency = $2
    }
  }
  printf "day %s weigth %s tendency %.1f exercise %s\n", $1, $2, Tendency, $3
}
