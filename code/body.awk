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
  Day = $1
  #printf "day %s weigth %s tendency %.1f exercise %s\n", Day, $2, Tendency, $3
}

END {
  printf "Day %s Tendency %.1f\n", Day, Tendency
}
