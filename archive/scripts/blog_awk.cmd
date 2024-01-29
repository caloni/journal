@echo off
pushd public\blog_awk
rm -fr *
popd
xcopy /E /I /Y themes\blog\static public\blog_awk
scripts\merge.exe --copy-images
gawk -f scripts\removeyaml.awk content\blog.md > content\blog.txt
echo Single blog file converted from md+yaml to txt.
setlocal
set LC_ALL=en_US.UTF-8
gawk -f scripts\txt2blog.awk content\blog.txt
endlocal
echo Blog generated

rem pushd public\blog_awk
rem git add --all
rem git commit -m "Publish new version."
rem git push
rem popd
