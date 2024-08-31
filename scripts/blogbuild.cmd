@echo off
if not exist public mkdir public
if not exist public\blog mkdir public\blog
pushd public\blog
rm -fr *
popd
xcopy /Q /E /I /Y blog public\blog
xcopy /Q /E /I /Y img\blog public\blog\img
setlocal
set LC_ALL=en_US.UTF-8
gawk -f scripts\util.awk -f scripts\metadata.awk journal.txt
gawk -f scripts\util.awk -f scripts\txt2blog.awk public\metadata.txt journal.txt
endlocal
echo Blog generated
