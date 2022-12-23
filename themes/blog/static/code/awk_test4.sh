#/bin/sh
gawk 'BEGIN{FS="[<:=]"}NF>=4{a++}END{print a+0}' bigsample.txt
gawk 'BEGIN{FS="<|:|="}NF>=4{a++}END{print a+0}' bigsample.txt
gawk 'BEGIN{FS="<|:|="}NF>=4&&/:SubNetwork/{a++}END{print a+0}' bigsample.txt
gawk 'BEGIN{FS=":SubNetwork"}NF>=2{a++}END{print a+0}' bigsample.txt
gawk '/:SubNetwork/{a++}END{print a}' bigsample.txt
