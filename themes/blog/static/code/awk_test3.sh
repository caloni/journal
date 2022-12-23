#/bin/sh
gawk '{if(NF==5)print("a")}' bigsample.txt
gawk '{if($4=="Hahaha")print("a")}' bigsample.txt
gawk '{if($1=="Hahaha")print("a")}' bigsample.txt
gawk '/Hahaha/{if($4=="Hahaha")print("a")}' bigsample.txt
