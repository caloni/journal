@echo off
echo Building drafts...
hugo --buildDrafts --config themes\drafts\config.toml -d public\drafts --cleanDestinationDir
pushd public\drafts
sed -i "s/href=\"\//href=\"#/g" index.html
sed -i "s/src=\"\//src=\"/g" index.html
sed -i "s/href=\"#[a-z]\+\/[a-z.-]\+/href=\"#/g" index.html
for /r %%i in (*.png) do mv %%i .
for /r %%i in (*.jpg) do mv %%i .
for /r %%i in (*.gif) do mv %%i .
kindlegen.exe book.opf -o caloni-drafts.mobi
if not exist k: echo !! WARNING !! NO KINDLE CONNECTED IN THIS COMPUTER !!
copy /y caloni-drafts.mobi k:\documents
if %ERRORLEVEL% EQU 0 echo === DRAFT COPIED SUCCESSFULLY ===
if %ERRORLEVEL% NEQ 0 echo === ERROR COPYING DRAFT ===
rm k:\documents\caloni-drafts.han
popd

choice /M "Do you want to build the entire book (default yes in 10 seconds)?" /D Y /T 10
if errorlevel 2 goto :eof
hugo --buildDrafts --config themes\book\config.toml -d public\book --cleanDestinationDir
pushd public\book
sed -i "s/href=\"\//href=\"#/g" index.html
sed -i "s/src=\"\//src=\"/g" index.html
sed -i "s/href=\"#[a-z]\+\/[a-z.-]\+/href=\"#/g" index.html
for /r %%i in (*.png) do mv %%i .
for /r %%i in (*.jpg) do mv %%i .
for /r %%i in (*.gif) do mv %%i .
if not exist k: echo !! WARNING !! NO KINDLE CONNECTED IN THIS COMPUTER !!
kindlegen.exe book.opf -o caloni.mobi
copy /y caloni.mobi k:\documents
if %ERRORLEVEL% EQU 0 echo === BOOK COPIED SUCCESSFULLY ===
if %ERRORLEVEL% NEQ 0 echo === ERROR COPYING BOOK ===
popd

