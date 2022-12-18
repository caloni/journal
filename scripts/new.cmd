@echo off
if "%1"=="" goto usage
hugo new blog/%1.md
goto :eof

:usage
echo How to use: new [slug]
