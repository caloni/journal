@echo off
git pull
git push
hugo --buildDrafts --cleanDestinationDir -d public\blog --config themes/blog/config.toml

pushd public\blog
git add --all
git commit -m "publishing."
git push
popd
