@echo off
scripts\merge.exe
gawk -f scripts\md2adoc.awk content\blog.md > content\blog.txt
xcopy /E /I /Y themes\epub_awk public\epub_awk
echo Generating single files...
gawk -f scripts\adoc2epub.awk content\blog.txt
