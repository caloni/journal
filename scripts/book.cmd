@echo off
rm -fr public\book
xcopy /Q /E /I /Y book public\book
xcopy /Q /E /I /Y img public\book\EPUB\img
echo Generating single files...
setlocal
set LC_ALL=en_US.UTF-8
gawk -f scripts\txt2epub.awk blog.txt
endlocal
pushd public\book
call repack.cmd
call tokindle.cmd
copy /y caloni.mobi k:\documents
if %ERRORLEVEL% EQU 0 echo === BOOK COPIED SUCCESSFULLY ===
if %ERRORLEVEL% NEQ 0 echo === ERROR COPYING BOOK ===
popd
copy /y ..\journal.txt k:\documents
if %ERRORLEVEL% EQU 0 echo === JOURNAL UPDATED ===
if %ERRORLEVEL% NEQ 0 echo === ERROR UPDATING JOURNAL ===
copy /y ..\clippings.txt k:\documents
if %ERRORLEVEL% EQU 0 echo === CLIPPINGS UPDATED ===
if %ERRORLEVEL% NEQ 0 echo === ERROR UPDATING CLIPPINGS ===
