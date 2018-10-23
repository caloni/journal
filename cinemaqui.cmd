@echo off
hugo new cinemaqui/%1.md
hugo new post/%1.md
gvim content\cinemaqui\%1.md content\post\%1.md
