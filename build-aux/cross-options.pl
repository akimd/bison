#! /usr/bin/env perl

use warnings;
use 5.005;
use strict;

my %option;
while (<>)
{
    if (/^\s*(?:(-\w), )?(--[-\w]+)(\[?)(=[-\w]+)?\]?/)
    {
	my ($short, $long, $opt, $arg) = ($1, $2, $3, $4);
	$short = defined $short ? '@option{' . $short . '}' : '';
	if ($arg)
	{
	    $arg =~ s/^=//;
	    $arg = '@var{' . lc ($arg) . '}';
	    $arg = '[' . $arg . ']'
		if defined $opt;
	    $option{"$long=$arg"} = "$short $arg";
	}
	else
	{
	    $option{"$long"} = "$short";
	}
    }
}

foreach my $long (sort keys %option)
{
    printf "\@item %-40s \@tab %s\n", '@option{' . $long . '}', $option{$long};
}
