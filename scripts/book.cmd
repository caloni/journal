@echo off
echo Building drafts...
hugo --buildDrafts --theme=drafts -d drafts
pushd drafts
sed -i "s/href=\"\//href=\"#/g" index.html
sed -i "s/src=\"\//src=\"/g" index.html
sed -i "s/href=\"#[a-z]\+\/[a-z.-]\+/href=\"#/g" index.html
kindlegen.exe book.opf -o caloni-drafts.mobi
cp caloni-drafts.mobi k:\documents
rm k:\documents\caloni-drafts.han
popd

choice /M "Do you want to build the entire book (default yes in 10 seconds)?" /D Y /T 10
if errorlevel 2 goto :eof
hugo --buildDrafts --theme=book -d book
pushd book
sed -i "s/href=\"\//href=\"#/g" index.html
sed -i "s/src=\"\//src=\"/g" index.html
sed -i "s/href=\"#[a-z]\+\/[a-z.-]\+/href=\"#/g" index.html
kindlegen.exe book.opf -o caloni.mobi
cp caloni.mobi k:\documents
popd

