#!/usr/bin/perl

# This program generates a simkey10.dat (100, 1000, etc) each
# containing 100 random keys of length 10 (100, 1000, etc).
# These files can then be fed into keystats to observe that
# the time to add or find the keys is directly proportional to
# keylength n [in other words, O(n)].
#
# The conclusion is that really long keys (e.g. 100k) are not
# efficient.                                       TDH 23Jan07

use strict;
use warnings;


#for my $len (10,100,1000,10000,100000,1000000) {
for my $len (100) {
   open OUTFILE, ">simkeys$len.dat" or die "can't open: $!\n";
   # we'll do 100 keys of $len
   print "keylen $len\n";
   for my $i (0..99) {
       my $key = pack "I", $len;
       $key .= pack "C", (int(rand(256))) for (1..$len);
       print OUTFILE $key;
   }
   close OUTFILE;
}

