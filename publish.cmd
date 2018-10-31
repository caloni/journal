@echo off
hugo
pushd public
git add --all
git commit -m "Publishing."
git push
popd
git push
