@echo off
git add --all
git commit -m "new posts, updates and promotions. %*"
git pull
git push

hugo

pushd public
git add -f tags
git add --all
git commit -m "new posts, updates and promotions. %*"
git push
popd
