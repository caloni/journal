@echo off
grep -o "http.*caloni.*com.br[^\"]\+" public/blog/news/index.html
