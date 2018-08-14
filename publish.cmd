@echo off
hugo
pushd public
git add --all
git commit --allow-empty -m "Publishing site."
git push
popd
