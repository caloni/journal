@echo off
if not exist public mkdir public
if exist public\blog.bkp rm -fr public\blog.bkp
if exist public\blog mv public\blog public\blog.bkp
git clone git@github.com:Caloni/caloni.github.io.git public\blog
pushd public\blog
git config --local core.autocrlf false
git config --local core.excludesfile false
popd
echo all good; type scripts\blog to test and good luck!
