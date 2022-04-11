@echo off
hugo
if %ERRORLEVEL% neq 0 goto :eof

git add --all
git commit -m "new posts, updates and promotions. %*"

pushd public
git add -f tags
git add --all
git commit -m "new posts, updates and promotions. %*"
popd

choice /M "Do you want to publish (default yes in 10 seconds)?" /D Y /T 10
if errorlevel 2 goto :eof

git pull
git push

pushd public
git push
popd

