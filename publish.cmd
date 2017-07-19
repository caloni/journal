@echo off
pushd public
rm -fr *
popd
hugo
pushd public
git add --all
git ci -m "Publishing."
git push
popd
