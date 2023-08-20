@echo off
echo == categories ==
grep "guid" public\blog\categories\index.xml | sed "s/.*categories\/\([a-z]\+\).*/\1/" | sort
echo == tags ==
grep "guid" public\blog\tags\index.xml | sed "s/.*tags\/\([a-z]\+\).*/\1/" | sort
