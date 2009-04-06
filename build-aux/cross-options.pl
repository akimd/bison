#! /usr/bin/env perl

use warnings;
use 5.005;
use strict;

my %option;
while (<>)
{
    if (/^\s*             # Initial spaces.
        (?:(-\w),\s+)?    # $1: $short: Possible short option.
        (--[-\w]+)        # $2: $long:  Long option.
        (\[?)             # $3: $opt:   '[' iff the argument is optional.
        (?:=(\S+))?       # $4: $arg:   Possible argument name.
        \s                # Spaces.
        /x)
    {
	my ($short, $long, $opt, $arg) = ($1, $2, $3, $4);
	$short = defined $short ? '@option{' . $short . '}' : '';
	if ($arg)
	{
            # if $opt, $arg contains the closing ].
            substr ($arg, -1) = ''
                if $opt eq '[';
	    $arg =~ s/^=//;
            $arg = lc ($arg);
            # If the argument is compite (e.g., for --define[=NAME[=VALUE]]),
            # put each word in @var, to build @var{name}[=@var{value}], not
            # @var{name[=value]}].
	    $arg =~ s/(\w+)/\@var{$1}/g;
	    my $long_arg = "=$arg";
	    if ($opt eq '[') {
	      $long_arg = "[$long_arg]";
	      $arg = "[$arg]";
	    }
	    $option{"$long$long_arg"} = $short ? "$short $arg" : '';
	}
	else
	{
	    $option{"$long"} = "$short";
	}
    }
}

foreach my $long (sort keys %option)
{
    # Avoid trailing spaces.
    printf ("\@item %-40s \@tab%s\n",
            '@option{' . $long . '}',
            $option{$long} ? " $option{$long}" : "");
}
