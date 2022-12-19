hugo --buildDrafts --cleanDestinationDir -d epub --config themes/epub/config.toml
pushd epub
rm -f caloni.epub
zip -rX caloni.epub mimetype OEBPS META-INF
rem kindlegen.exe caloni.epub
popd
