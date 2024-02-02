@echo off
pushd public
rm -r -f *
popd
hugo
pushd public
git add --all
git commit -m "Publishing site."
git push
popd
