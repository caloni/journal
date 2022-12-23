#/bin/sh
mawk '{if(NF==5)print("a")}' bigsample.txt
mawk '{if($4=="Hahaha")print("a")}' bigsample.txt
mawk '{if($1=="Hahaha")print("a")}' bigsample.txt
mawk '/Hahaha/{if($4=="Hahaha")print("a")}' bigsample.txt
