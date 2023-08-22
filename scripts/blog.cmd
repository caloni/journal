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

pushd public\blog
git add --all
git commit -m "Publish new version."
git push
popd
