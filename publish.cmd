@echo off
rm -fr public
hugo
pushd public
git init
git add --all
git commit -m "Publishing."
git remote add origin https://github.com/cinetenisverde/cinetenisverde.github.io
git push --force -u origin master
popd
git push
