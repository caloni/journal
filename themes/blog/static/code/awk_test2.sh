#/bin/sh
gawk 'BEGIN {FS = "<|:|=";} {if ($5 == "YetAnotherText") a ++;} END {print "a: " a;}' bigsample.txt
gawk 'BEGIN {FS = "<|:|=";} {if ($0 ~ /YetAnotherText/) a ++;} END {print "a: " a;}' bigsample.txt
gawk 'BEGIN {FS = "<|:|=";} /YetAnotherText/ {a ++;} END {print "a: " a;}' bigsample.txt
gawk 'BEGIN {FS = "<|:|=";} {if (NF == 8) a ++;} END {print "a: " a;}' bigsample.txt
gawk 'BEGIN {FS = "<|:|=";} {if ($2 == "SomeSampleText") a ++;} END {print "a: " a;}' bigsample.txt
gawk 'BEGIN {FS = "<|:|=";} {if ($8 == "AndSomeMore>") a ++;} END {print "a: " a;}' bigsample.txt
