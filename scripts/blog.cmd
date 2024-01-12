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
gawk -f scripts\txt2blog.awk blog.txt
endlocal
echo Blog generated
git add --all
git commit -m "Another day flawlessly finished."
git push
echo Source published
choice /M "Do you want to publish the PUBLIC blog to the world wide web (default yes in 10 seconds)?" /D Y /T 10
if errorlevel 2 goto :eof
pushd public\blog
git add --all
git commit -m "Here comes great news."
git push
popd
