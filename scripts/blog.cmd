@echo off
git add --all
git commit -m "new posts."
git pull
git push

hugo -D --cleanDestinationDir

pushd public
git add -f tags
git add --all
git commit -m "new posts."
git push
popd
