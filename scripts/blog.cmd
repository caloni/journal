@echo off
pushd public\blog
rm -fr *
popd
xcopy /Q /E /I /Y blog public\blog
xcopy /Q /E /I /Y img public\blog\img
setlocal
set LC_ALL=en_US.UTF-8
gawk -f scripts\txt2blog.awk blog.txt
endlocal
echo Blog generated
choice /M "Do you want to publish the blog to the world wide web (default yes in 10 seconds)?" /D Y /T 10
if errorlevel 2 goto :eof
git add --all
git commit -m "Publish new version."
git push
pushd public\blog
git add --all
git commit -m "Publish new version."
git push
popd
