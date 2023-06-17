@echo off
scripts\merge.exe
scripts\publish.exe
rem rm content\blog.txt

pushd public\blog_alpha
git add --all
git commit -m "Publish new version."
git push
popd
