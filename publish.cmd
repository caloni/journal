@echo off
pushd public
rm -fr *
popd
hugo
pushd public
git add --all
git ci %*
git push
popd
git add --all
git ci %*
git push
