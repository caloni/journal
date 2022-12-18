@echo off
if "%1"=="" goto usage
hugo new chess/%1.md
mv content/chess/%1.md content/blog
goto :eof

:usage
echo How to use: new [slug]
