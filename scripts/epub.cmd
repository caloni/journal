hugo --buildDrafts --cleanDestinationDir -d public\epub --config themes/epub/config.toml
pushd public\epub
rm -f caloni.epub
zip -rX caloni.epub mimetype OEBPS META-INF
rem kindlegen.exe caloni.epub
popd
