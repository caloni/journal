@echo off
scripts\merge.exe
gawk -f scripts\md2adoc.awk content\blog.md > content\blog.txt
scripts\publish.exe

rem pushd public\blog_alpha
rem git add --all
rem git commit -m "Publish new version."
rem git push
rem popd
