@echo off
if "%1"=="" goto usage
hugo --config themes/blog/config.toml new chess/%1/index.md
mv content/chess/%1 content/posts
goto :eof

:usage
echo How to use: new [slug]
