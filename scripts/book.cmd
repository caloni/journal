@echo off
rm -fr public\book
xcopy /Q /E /I /Y book public\book
xcopy /Q /E /I /Y img public\book\EPUB\img
setlocal
set LC_ALL=en_US.UTF-8
gawk -f scripts\txt2epub.awk blog.txt
endlocal
pushd public\book
call repack.cmd
echo Book generated
popd
choice /M "Do you want to convert and copy to kindle (default yes in 10 seconds)?" /D Y /T 10
if errorlevel 2 goto :eof
pushd public\book
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
