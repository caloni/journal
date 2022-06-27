#/bin/sh
mawk 'BEGIN{FS="[<:=]"}NF>=4{a++}END{print a+0}' bigsample.txt
mawk 'BEGIN{FS="<|:|="}NF>=4{a++}END{print a+0}' bigsample.txt
mawk 'BEGIN{FS="<|:|="}NF>=4&&/:SubNetwork/{a++}END{print a+0}' bigsample.txt
mawk 'BEGIN{FS=":SubNetwork"}NF>=2{a++}END{print a+0}' bigsample.txt
mawk '/:SubNetwork/{a++}END{print a}' bigsample.txt
