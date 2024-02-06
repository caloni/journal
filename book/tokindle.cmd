rem kindlegen.exe caloni.epub -o caloni.mobi
pushd MOBI
kindlegen.exe book.opf -o caloni.mobi
if exist caloni.mobi copy /y caloni.mobi ..
popd
