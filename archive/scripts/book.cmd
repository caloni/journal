@echo off
scripts\merge.exe
gawk -f scripts\removeyaml.awk content\blog.md > content\blog.txt
rm -fr public\epub_awk
xcopy /E /I /Y themes\epub_awk public\epub_awk
echo Generating single files...
setlocal
set LC_ALL=en_US.UTF-8
gawk -f scripts\txt2epub.awk content\blog.txt
endlocal
pushd public\epub_awk
call repack.cmd
call tokindle.cmd
copy /y caloni.mobi k:\documents
if %ERRORLEVEL% EQU 0 echo === BOOK COPIED SUCCESSFULLY ===
if %ERRORLEVEL% NEQ 0 echo === ERROR COPYING BOOK ===
popd
copy /y ..\journal.txt k:\documents
if %ERRORLEVEL% EQU 0 echo === JOURNAL UPDATED ===
if %ERRORLEVEL% NEQ 0 echo === ERROR UPDATING JOURNAL ===
