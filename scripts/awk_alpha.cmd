@echo off
scripts\merge.exe
gawk -f scripts\md2adoc.awk content\blog.md > content\blog.txt
rm -fr public\epub_awk
xcopy /E /I /Y themes\epub_awk public\epub_awk
echo Generating single files...
setlocal
set LC_ALL=en_US.UTF-8
gawk -f scripts\adoc2epub.awk content\blog.txt
endlocal
