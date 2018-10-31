@echo off
hugo
pushd public
git add --all
git commit -m "Publishing."
git log --stat -1
git push
popd
git push
