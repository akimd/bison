#! /usr/bin/env perl

use warnings;
use 5.005;
use strict;

my %option;
while (<>)
{
    if (/^\s*          # Initial spaces.
        (?:(-\w),\s+)? # $1: Possible short option.
        (--[-\w]+)     # $2: Long option.
        (\[?)          # $3: '[' iff the argument is optional.
        (?:=([-\w]+))? # $4: Possible argument name.
        /x)
    {
	my ($short, $long, $opt, $arg) = ($1, $2, $3, $4);
	$short = defined $short ? '@option{' . $short . '}' : '';
	if ($arg)
	{
	    $arg =~ s/^=//;
	    $arg = '@var{' . lc ($arg) . '}';
	    $arg = '[' . $arg . ']'
		if $opt eq '[';
	    $option{"$long=$arg"} = $short ? "$short $arg" : '';
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
