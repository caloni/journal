@echo off
hugo new post/%1.md
update.py content\post\%1.md %2
gvim content\post\%1.md
