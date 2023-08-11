@echo off
scripts\merge.exe
gawk -f scripts\removeyaml.awk content\blog.md > content\blog.txt
rm -fr public\blog_awk
xcopy /E /I /Y themes\blog\static public\blog_awk
echo Generating single files...
setlocal
set LC_ALL=en_US.UTF-8
gawk -f scripts\txt2blog.awk content\blog.txt
endlocal
echo Blog generated
