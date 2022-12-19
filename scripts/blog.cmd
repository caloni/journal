@echo off
git pull
git push
hugo -D --cleanDestinationDir

pushd public
git add --all
git commit -m "publishing."
git push
popd
