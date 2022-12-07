@echo off
if not exist k: goto :eof
mv "k:\documents\My Clippings.txt" static\txt\clippings.txt
git add static\txt\clippings.txt
