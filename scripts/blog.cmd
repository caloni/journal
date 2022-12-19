@echo off
git pull
hugo -D --cleanDestinationDir

pushd public
git add --all
git commit -m "publishing."
git push -f
popd
