# Read weigth records and print the weight tendency
# Record is in the format YYYY-MM-DD W E where:
# W is the weight (if not recorded keep it empty)
# E is the number of exercises in the day (if none keep it empty)
#
# Wanderley Caloni <wanderley.caloni@gmail.com>
# 2025-01-07

BEGIN {
  TendencyCurve = 0.03 # bigger == more sensitive
  LastWeigth = 0
  Exercises = 0
  Tendency = 0
  Verbose = 1
}

NF >= 3 {
  Exercises += $3
}

NF >= 2 && $2 > 0 {
  LastWeigth = $2
  if( Tendency ) {
    Tendency = Tendency + TendencyCurve * (LastWeigth - Tendency)
  } else {
    Tendency = $2
  }
}

NF >= 1 {
  Day = $1
  if( Verbose ) {
    printf "day %s weigth %s tendency %.1f exercise %s\n", Day, LastWeigth, Tendency, Exercises
  }
}

/^$/ { exit }

END {
  printf "the final day is %s, there were %s exercises in total, the final tendency is %.1f\n", Day, Exercises, Tendency
}
