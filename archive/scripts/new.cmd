@echo off
if "%1"=="" goto usage
hugo --config themes/blog/config.toml new posts/%1/index.md
goto :eof

:usage
echo How to use: new [slug]
