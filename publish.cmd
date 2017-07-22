@echo off
pushd public
git add --all
git ci %*
git push
popd
git add --all
git ci %*
git push
