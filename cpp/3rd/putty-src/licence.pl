#!/usr/bin/env perl -w

# This script generates licence.h (containing the PuTTY licence in the
# form of macros expanding to C string literals) from the LICENCE
# master file. It also regenerates the licence-related Halibut input
# files.

use File::Basename;

# Read the input file.
$infile = "LICENCE";
open my $in, $infile or die "$infile: open: $!\n";
my @lines = ();
while (<$in>) {
    chomp;
    push @lines, $_;
}
close $in;

# Format into paragraphs.
my @paras = ();
my $para = undef;
for my $line (@lines) {
    if ($line eq "") {
        $para = undef;
    } elsif (!defined $para) {
        push @paras, $line;
        $para = \$paras[$#paras];
    } else {
        $$para .= " " . $line;
    }
}

# Get the copyright years and short form of copyright holder.
die "bad format of first paragraph\n"
    unless $paras[0] =~ m!copyright ([^\.]*)\.!i;
$shortdetails = $1;

# Write out licence.h.

$outfile = "licence.h";
open my $out, ">", $outfile or die "$outfile: open: $!\n";
select $out;

print "/*\n";
print " * $outfile - macro definitions for the PuTTY licence.\n";
print " *\n";
print " * Generated by @{[basename __FILE__]} from $infile.\n";
print " * You should edit those files rather than editing this one.\n";
print " */\n";
print "\n";

print "#define LICENCE_TEXT(parsep) \\\n";
for my $i (0..$#paras) {
    my $lit = &stringlit($paras[$i]);
    print "    parsep \\\n" if $i > 0;
    print "    \"$lit\"";
    print " \\" if $i < $#paras;
    print "\n";
}
print "\n";

printf "#define SHORT_COPYRIGHT_DETAILS \"%s\"\n", &stringlit($shortdetails);

sub stringlit {
    my ($lit) = @_;
    $lit =~ s!\\!\\\\!g;
    $lit =~ s!"!\\"!g;
    return $lit;
}

close $out;

# Write out doc/licence.but.

$outfile = "doc/licence.but";
open $out, ">", $outfile or die "$outfile: open: $!\n";
select $out;

print "\\# Generated by @{[basename __FILE__]} from $infile.\n";
print "\\# You should edit those files rather than editing this one.\n\n";

print "\\A{licence} PuTTY \\ii{Licence}\n\n";

for my $i (0..$#paras) {
    my $para = &halibutescape($paras[$i]);
    if ($i == 0) {
        $para =~ s!copyright!\\i{copyright}!; # index term in paragraph 1
    }
    print "$para\n\n";
}

close $out;

# And write out doc/copy.but, which defines a macro used in the manual
# preamble blurb.

$outfile = "doc/copy.but";
open $out, ">", $outfile or die "$outfile: open: $!\n";
select $out;

print "\\# Generated by @{[basename __FILE__]} from $infile.\n";
print "\\# You should edit those files rather than editing this one.\n\n";

printf "\\define{shortcopyrightdetails} %s\n\n",
    &halibutescape($shortdetails);

close $out;

sub halibutescape {
    my ($text) = @_;
    $text =~ s![\\{}]!\\$&!g; # Halibut escaping
    $text =~ s!"([^"]*)"!\\q{$1}!g; # convert quoted strings to \q{}
    return $text;
}
