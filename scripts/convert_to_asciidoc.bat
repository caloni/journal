@echo off
awk "{ print } /---/ { if( b == 1 ) exit; b = 1 }" index.md > index.adoc
echo. >> index.adoc
pandoc -f markdown -t asciidoc --wrap=none index.md >> index.adoc
