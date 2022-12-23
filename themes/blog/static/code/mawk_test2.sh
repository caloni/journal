#/bin/sh
mawk 'BEGIN {FS = "<|:|=";} {if ($5 == "YetAnotherText") a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {FS = "<|:|=";} {if ($0 ~ /YetAnotherText/) a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {FS = "<|:|=";} /YetAnotherText/ {a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {FS = "<|:|=";} {if (NF == 8) a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {FS = "<|:|=";} {if ($2 == "SomeSampleText") a ++;} END {print "a: " a;}' bigsample.txt
mawk 'BEGIN {FS = "<|:|=";} {if ($8 == "AndSomeMore>") a ++;} END {print "a: " a;}' bigsample.txt
