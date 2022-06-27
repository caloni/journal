#/bin/sh
mawk 'BEGIN {a = 0;} {if ($5 == "YetAnotherText") a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {a = 0;} {if ($0 ~ /YetAnotherText/) a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {a = 0;} /YetAnotherText/ {a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {a = 0;} {if (NF == 9) a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {a = 0;} {if ($1 == "SomeSampleText") a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {a = 0;} {if ($9 == "AndSomeMore") a ++;} END {print "a: " a;}' bigsample.txt
