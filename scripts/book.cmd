@echo off
if not exist public mkdir public
if not exist public\book mkdir public\book
rm -fr public\book
xcopy /Q /E /I /Y book public\book
xcopy /Q /E /I /Y img\journal public\blog\img
xcopy /Q /E /I /Y img\blog public\blog\img
setlocal
set LC_ALL=en_US.UTF-8
gawk -f scripts\txt2epub.awk blog.txt journal.txt
endlocal
pushd public\book
call repack.cmd
if exist c:\users\caloni\ebooks\caloni.epub copy /y c:\users\caloni\ebooks\caloni.epub c:\users\caloni\ebooks\caloni-old.epub
if exist c:\users\caloni\ebooks copy /y caloni.epub c:\users\caloni\ebooks
echo Book generated
popd
pushd public\book
choice /M "Do you want to convert and copy to kindle (default yes in 10 seconds)?" /D Y /T 10
if errorlevel 2 goto :end_of_road
call tokindle.cmd
if exist c:\users\caloni\ebooks\caloni.mobi copy /y c:\users\caloni\ebooks\caloni.mobi c:\users\caloni\ebooks\caloni-old.mobi
if exist c:\users\caloni\ebooks copy /y caloni.mobi c:\users\caloni\ebooks
copy /y caloni.mobi k:\documents
if %ERRORLEVEL% EQU 0 echo === BOOK COPIED SUCCESSFULLY ===
if %ERRORLEVEL% NEQ 0 echo === ERROR COPYING BOOK ===
:end_of_road
popd
