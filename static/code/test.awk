BEGIN {
  print "FS:", FS;
  print "RS:", RS;
  print "OFS:", OFS;
  print "ORS:", ORS;
}
{
  print "NF:", NF
  print "NR:", NR
  print "$0:", $0;
  print "$1:", $1;
  print "$2:", $2;
  print "$NR:", $NR;
  print "";
}
