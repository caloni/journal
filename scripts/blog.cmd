@echo off
git pull
git push
hugo -D --cleanDestinationDir -d public\blog

pushd public\blog
git add --all
git commit -m "publishing."
git push
popd
