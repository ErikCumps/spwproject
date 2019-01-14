#!/usr/bin/perl
#
# buildlist.pl - code generator for named enum lists.
#
# Copyright (C) 2011-2016 Erik Cumps <erik.cumps@gmail.com>
#
# License: GPL v2

use strict;
use IO::File;
use File::Basename;
use Data::Dumper;

our $DEBUG = 0;

sub Usage
{
	my ($msg) = @_;

	if ($msg) { print "Error: $msg\n\n"; }

	print "Usage: $0 in [stem] [-C dir] [-I dir] [-H path] [-noc] [-noi]\n";
	print "Where: in        input filename\n";
	print "       stem      optional output filename stem\n";
	print "       -C dir    target directory for .c file\n";
	print "       -I dir    target directory for .h file\n";
	print "       -H path   relative path for header file include\n";
	print "       -noc      don't generate .c file\n";
	print "       -noi      don't generate .h file\n";
	print "\n";
	exit 1;
}

my ($IN, $IN_set, $STEM, $STEM_set) = ("", 0, "", 0);
my ($CDIR, $CDIR_set, $IDIR, $IDIR_set, $HPATH, $HPATH_set, $NOC, $NOI) = ("", 0, "", 0, "", 0, 0, 0);
my ($arg);
while ($#ARGV >= 0) {
	$arg = shift;
	if ($arg eq "-C") {
		if ($CDIR_set) { &Usage ("multiple -C options!"); }
		$CDIR = shift; $CDIR_set++;
		next;
	}
	if ($arg eq "-I") {
		if ($IDIR_set) { &Usage ("multiple -I options!"); }
		$IDIR = shift; $IDIR_set++;
		next;
	}
	if ($arg eq "-H") {
		if ($HPATH_set) { &Usage ("multiple -H options!"); }
		$HPATH = shift; $HPATH_set++;
		next;
	}
	if ($arg eq "-noc") {
		if ($NOC) { &Usage ("multiple -noc options!"); }
		$NOC = 1;
		next;
	}
	if ($arg eq "-noi") {
		if ($NOI) { &Usage ("multiple -noi options!"); }
		$NOI = 1;
		next;
	}
	if (!$IN_set) {
		$IN = $arg; $IN_set++;
		next;
	}
	if (!$STEM_set) {
		$STEM = $arg; $STEM_set++;
		next;
	}
	&Usage ("too many options!");
}
if (!$IN) { &Usage; }

if (!$STEM) {
	$STEM = basename ($IN);
	$STEM =~ s/\.[^.]*$//;
}
if (!$CDIR) { $CDIR = "." } $CDIR =~ s/\/+$//g;
if (!$IDIR) { $IDIR = "." } $IDIR =~ s/\/+$//g;
if (!$HPATH) { $HPATH = "" } else { $HPATH =~ s/\/*$/\//g; }

my $COUT = "$CDIR/$STEM.c";
my $IOUT = "$IDIR/$STEM.h";
my $HOUT = "$HPATH$STEM.h";

# Parser data structures
our %PARSER_ERROR = (
	"BAD_ITEM"		=> "item header syntax error",
	"BAD_ITEM_TYPE"		=> "invalid item type",
	"BAD_ITEM_ID"		=> "unexpected item id value",
	"DUP_ITEM"		=> "duplicate item name",
	"DUP_ITEM_VALUE"	=> "duplicate item value data",
	"DUP_ITEM_DATA"		=> "duplicate item data",
	"NO_CURRENT_ITEM"	=> "no current item type",
	"NO_CURRENT_CLASS"	=> "no current item class",
	"BAD_ENUMFLAG_BASE"	=> "base enum for enumflag does not exist",
	"BAD_CLSENUM_TYPE"	=> "missing class enum type for clsenum",
	"BAD_RAWENUM_TYPE"	=> "missing data type for rawenum",
	"BAD_RAWEREF_DTYPE"	=> "missing data type for raweref",
	"BAD_RAWEREF_RTYPE"	=> "missing reference type for raweref",
);
our %PARSER = (
	"title"		=> [ \&ParseTitle,	010, 0			],
	"enum"		=> [ \&ParseEnums,	100, \&RenderEnums 	],
	"enumflag"	=> [ \&ParseEnumflags,	150, \&RenderEnumflags	],
	"clsenum"	=> [ \&ParseClsEnums,	110, \&RenderClsEnums 	],
	###"rawenum"	=> [ \&ParseRawEnums,	200, \&RenderRawEnums 	],
	"rawenum"	=> [ \&ParseRawEnums,	105, \&RenderRawEnums 	],
	"raweref"	=> [ \&ParseRawErefs,	210, \&RenderRawErefs 	],
);

our ($lc, $line);

#
# Reports specification parser error
#
sub HandleParserError
{
	my ($error) = @_;
	my ($msg);

	$msg = $PARSER_ERROR{$error};
	if (!$msg) { $msg = "syntax error"; }

	print STDERR "*** line $::lc: $msg\n";
	print STDERR "*** $::line\n\n";
	exit (1);
}

#
# Parse UserSection (Header data)
#
sub ParseUserSectionHeader
{
	my ($fh, $rDATA) = @_;

	while (<$fh>) {
		chop; $::line = $_; $::lc++;

		# Detect end of UserSection
		if (/^%$/) { return; }

		# Store data
		push @{$rDATA->{"UserSectionHeader"}}, $::line;
	}

}

#
# Parse UserSection (Footer data)
#
sub ParseUserSectionFooter
{
	my ($fh, $rDATA) = @_;

	while (<$fh>) {
		chop; $::line = $_; $::lc++;

		# Detect end of UserSection
		if (/^%$/) { return; }

		# Store data
		push @{$rDATA->{"UserSectionFooter"}}, $::line;
	}

}

#
# Parse [title]
#
sub ParseTitle
{
	my ($NAME, $TYPE, $rSEEN, $rLIST, $rDATA, $rCODE) = @_;

	# This is a NOP
}

#
# Parse [enum]
#
sub ParseEnums
{
	my ($NAME, $TYPE, $rSEEN, $rLIST, $rDATA, $rCODE) = @_;
	my (@data, $item, $iid);

	# Process item data
	@data = split (/\t+/, $::line);
	$item = shift (@data);

	# Check presence of item id
	if ($item =~ /^[^=]+=\d+$/) {
		($item, $iid) = split("=", $item);
	} else {
		$iid = -1;
	}

	# Validate item data name
	if ($rSEEN->{$TYPE}{$NAME}[1]->{$item}) {
		&HandleParserError ("DUP_ITEM_DATA");
	}
	$rSEEN->{$TYPE}{$NAME}[1]->{$item} = 1;

	# Massage item data fields
	($data[0]) = $data[0] =~ /^"([^"]+)"$/;
	if ($data[1]) {
		($data[1]) = $data[1] =~ /^"([^"]+)"$/;
		${$rCODE} = 1;
	}

	# Store data
	push @{$rLIST}, ( $item );
	$rDATA->{$item} = [ $iid, [ @data ]];
}

#
# Parse [enumflag]
#
sub ParseEnumflags
{
	my ($NAME, $TYPE, $rSEEN, $rLIST, $rDATA, $rCODE) = @_;
	my (@data, $item);

	# Process item data
	@data = split (/\t+/, $::line);
	$item = shift (@data);

	# Check presence of item id
	if ($item =~ /^[^=]+=\d+$/) { &HandleParserError ("BAD_ITEM_ID"); }

	# Validate item name
	if ($rSEEN->{$TYPE}{$NAME}[1]->{$item}) {
		&HandleParserError ("DUP_ITEM_DATA");
	}
	$rSEEN->{$TYPE}{$NAME}[1]->{$item} = 1;

	# Massage item data fields
	($data[0]) = $data[0] =~ /^"([^"]+)"$/;
	if ($data[1]) {
		($data[1]) = $data[1] =~ /^"([^"]+)"$/;
		${$rCODE} = 1;
	}

	# Store data
	push @{$rLIST}, ( $item );
	$rDATA->{$item} = [ @data ];
}

#
# Parse [clsenum]
#
sub ParseClsEnums
{
	my ($NAME, $TYPE, $rSEEN, $rLIST, $rDATA, $rCODE) = @_;
	my (@data, $item, $desc);
	
	@data = split (/:/, $::line);
	
	if ($data[0] eq "\@class") {
		# Process class data
		$rDATA->{class} = $data[1];
		push @{$rDATA->{class_list}}, ( $data[1] );
	} else {
		# Validate class data is present
		if (!$rDATA->{class}) {
			&HandleParserError ("NO_CURRENT_CLASS");
		}

		# Process item data
		@data = split (/\t+/, $::line);
		$item = shift (@data);
		$desc = shift (@data);

		# Validate item name
		if ($rSEEN->{$TYPE}{$NAME}[1]->{$item}) {
			&HandleParserError ("DUP_ITEM_DATA");
		}
		$rSEEN->{$TYPE}{$NAME}[1]->{$item} = 1;
	
		# Massage item description
		($desc) = $desc =~ /^"([^"]+)"$/; #"
	
		# Store data
		push @{$rLIST}, ( $item );
		$rDATA->{items}->{$item} = [ $desc ];
		push @{$rDATA->{classes}->{$rDATA->{class}}}, ( $item );
	}
}

#
# Parse [rawenum]
#
sub ParseRawEnums
{
	my ($NAME, $TYPE, $rSEEN, $rLIST, $rDATA, $rCODE) = @_;
	my (@data, $value, $item);

	# Process item data
	@data = split (/\t+/, $::line);
	$value = shift (@data);
	$item  = shift (@data);

	# Validate item value
	if ($rSEEN->{$TYPE}{$NAME}[1]->{$value}) {
		&HandleParserError ("DUP_ITEM_VALUE");
	}
	$rSEEN->{$TYPE}{$NAME}[1]->{$value} = 1;

	# Validate item name
	if ($rSEEN->{$TYPE}{$NAME}[2]->{$item}) {
		&HandleParserError ("DUP_ITEM_DATA");
	}
	$rSEEN->{$TYPE}{$NAME}[2]->{$item} = 1;

	# Massage item data fields
	($data[0]) = $data[0] =~ /^"([^"]+)"$/;
	if ($data[1]) {
		($data[1]) = $data[1] =~ /^"([^"]+)"$/;
		${$rCODE} = 1;
	}

	# Store data
	push @{$rLIST}, ( $item );
	$rDATA->{$item} = [ $value, [ @data ]];
}

#
# Parse [raweref]
#
sub ParseRawErefs
{
	my ($NAME, $TYPE, $rSEEN, $rLIST, $rDATA, $rCODE) = @_;
	my (@data, $value, $ref);

	# Process item data
	@data  = split (/\t+/, $::line);
	$value = shift (@data);
	$ref   = shift (@data);

	# Validate item value
	if ($rSEEN->{$TYPE}{$NAME}[1]->{$value}) {
		&HandleParserError ("DUP_ITEM_VALUE");
	}
	$rSEEN->{$TYPE}{$NAME}[1]->{$value} = 1;

	# Store data
	push @{$rLIST}, ( $value );
	$rDATA->{$value} =  [ $ref ];
}

#
# Parse specification file
#
sub Parse
{
	my ($FILE, $rDATA) = @_;
	my ($fh);
	my ($type, $ntype, $name, $nname, $desc, $ndesc, $code);
	my (@list, %data, %seen) = ((), (), ());
	my (@data, $item, $iid);

	$fh = new IO::File;
	$fh->open ("<$FILE") || die "failed to open \"$FILE\" for reading!\n";
	$::lc = 0; $type = 0; $code = 0;
	while (<$fh>) {
		chop; $::line = $_; $::lc++;

		# Detect start of UserSection
		if (/^%$/) {
			if (! $rDATA->{"UserSectionHeader"}) {
				&ParseUserSectionHeader ($fh, $rDATA);
			} else {
				&ParseUserSectionFooter ($fh, $rDATA);
			}
			next;
		}

		# Clean up comments
		$::line =~ s/#.*$//;
		
		# Clean up leading/trailing whitespace
		$::line =~ s/^\s+//;
		$::line =~ s/\s+$//;

		# Skip empty lines
		if ($::line =~ /^\s*$/) { next; }

		# New item header?
		if (/^\[/) {
			# Extract item header info and validate syntax
			($ntype, $nname, $ndesc) = /^\[(\S+)\s+(\S+)\]\s+(\S.*)$/;
			if (!defined ($ntype)) {
				&HandleParserError ("BAD_ITEM");
			}
			if (!defined ($PARSER{$ntype})) {
				&HandleParserError ("BAD_ITEM_TYPE");
			}

			# Finalize previous item data first
			if ($type) {
				@{$rDATA->{$type}->{"data"}->{$name}->{"list"}} = @list;
				%{$rDATA->{$type}->{"data"}->{$name}->{"data"}} = %data;
				$rDATA->{$type}->{"data"}->{$name}->{"desc"} = $desc;
				$rDATA->{$type}->{"data"}->{$name}->{"code"} = $code;

				push @{$rDATA->{$type}->{"list"}}, ( $name );

				@list = %data = %{$seen{$type}{$name}[1]} = ();
			}

			# Validate item
			if ($ntype eq "enumflag") {
				my $enum = (split (/:/, $nname))[1];
				if (!$enum || !$seen{"enum"}{$enum}[0]) {
					&HandleParserError ("BAD_ENUMFLAG_BASE");
				}
			} elsif ($ntype eq "clsenum") {
				my $ctype = (split (/:/, $nname))[1];
				if (!$ctype) {
					&HandleParserError ("BAD_CLSENUM_TYPE");
				}
			} elsif ($ntype eq "rawenum") {
				my $dtype = (split (/:/, $nname))[1];
				if (!$dtype) {
					&HandleParserError ("BAD_RAWENUM_TYPE");
				}
			} elsif ($ntype eq "raweref") {
				my $dtype = (split (/:/, $nname))[1];
				if (!$dtype) {
					&HandleParserError ("BAD_RAWEREF_DTYPE");
				}
				my $rtype = (split (/:/, $nname))[2];
				if (!$rtype) {
					&HandleParserError ("BAD_RAWEREF_RTYPE");
				}
			}

			# Start new item
			if ($seen{$ntype}{$nname}[0]) {
				&HandleParserError ("DUP_ITEM");
			}
			$seen{$ntype}{$nname}[0] = 1;

			$type = $ntype; undef ($ntype);
			$name = $nname; undef ($nname);
			$desc = $ndesc; undef ($ndesc);
			$code = 0;

			# Fixup for title item
			if ($type eq "title") {
				$code = $name;
				$name = "title";
			}
		} else {
			if (!defined ($type)) {
				&HandleParserError ("NO_CURRENT_ITEM");
			}
			&{$PARSER{$type}[0]} ($name, $type, \%seen, \@list, \%data, \$code);
		}
	}
	$fh->close;

	# Finalize final item data if any
	if ($type) {
		@{$rDATA->{$type}->{"data"}->{$name}->{"list"}} = @list;
		%{$rDATA->{$type}->{"data"}->{$name}->{"data"}} = %data;
		$rDATA->{$type}->{"data"}->{$name}->{"desc"} = $desc;
		$rDATA->{$type}->{"data"}->{$name}->{"code"} = $code;

		push @{$rDATA->{$type}->{"list"}}, ( $name );
	}

	# Create a debug dump of the parsed data
	if ($DEBUG && open(DBG,">parsed_data.dump")) {
		my $d = Data::Dumper->new ([$rDATA]); $d->Indent (1); $d->{xpad} = "\t"; $d->Sortkeys(1);
		print DBG $d->Dump; print DBG "\n";
		close (DBG);
	}
}

my %DATA = ();
&Parse ($IN, \%DATA);

sub RenderTitle
{
	my ($rDATA, $Cfh, $Hfh, $OC, $OH) = @_;
	my ($desc, $tmpl, $buffer, $fh);

	if (!defined ($rDATA->{"title"})) { return; }

	$desc = $rDATA->{"title"}->{"data"}->{"title"}->{"desc"};
	$tmpl = $rDATA->{"title"}->{"data"}->{"title"}->{"code"};
	$buffer = "";

	$fh = new IO::File;
	if ($fh->open ("<$tmpl")) {
		while (<$fh>) { s/\[TITLE\]/$desc/g; $buffer .= $_; }
		$fh->close;
	} else {
		$buffer = "/*** $desc ***/\n\n";
	}

	if (!$NOC) {
		print $Cfh "$buffer\n";
	}
	if (!$NOI) {
		print $Hfh "$buffer\n";
	}

}

sub RenderEnums
{
	my ($rDATA, $Cfh, $Hfh, $OC, $OH) = @_;
	my ($enum, $INFO);
	my ($item, $iid, $f, $data, $comm);
	my ($len, @maxlen, $fmt);
	my ($l, @maxcomm);

	if (!$NOC) {
		print $Cfh "#include <$OH>\n\n";
		print $Cfh "\n\n";
	}

	foreach $enum (@{$rDATA->{"enum"}->{"list"}})
	{
		$INFO = $rDATA->{"enum"}->{"data"}->{$enum};

		if (!$NOI) {
			$maxcomm[0] = 0;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};
				$l = length ($data->[1]->[0]) + 3;
				if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
			}
			$l = length ("\\internal start code");
			if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
			$l = length ("\\internal final code");
			if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }

			print $Hfh "/*! ".$INFO->{"desc"}." */\n";
			print $Hfh "typedef enum e_$enum {\n";

			@maxlen = (); $l = "";
			foreach $item (@{$INFO->{"list"}}) { $l = $item; }
			$maxlen[0] = 45;

			$fmt = "\t%-$maxlen[0]s\t\t/*!< %-$maxcomm[0]s */\n";
			$f = 1;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};
				$iid  = $data->[0];
				$comm = $data->[1]->[0];

				if ($f) {
					printf $Hfh $fmt, ("$enum\_START = 0,", "\\internal start code");
					printf $Hfh $fmt, ("$enum\_$item = 0,", $comm);
					$f = 0;
				} else {
					printf $Hfh $fmt, ("$enum\_$item,", $comm);
				}
			}
			printf $Hfh $fmt, ("$enum\_LIMIT = $enum\_$l", "\\internal final code");
			print $Hfh "} $enum;\n";
			print $Hfh "#define\t$enum\_CNT\t($enum\_LIMIT - $enum\_START + 1)\n\n";

			print $Hfh "/*! ".$INFO->{"desc"}." lookup function\n";
			print $Hfh " *\n";
			print $Hfh " * \\param e\t".$INFO->{"desc"}."\n";
			print $Hfh " * \\return\tPointer to const string with ".$INFO->{"desc"}." name\n";
			print $Hfh " */\n";
			print $Hfh "extern const char *$enum\_lookup ($enum e);\n";

			if ($INFO->{"code"}) {
				print $Hfh "/*! ".$INFO->{"desc"}." code lookup function\n";
				print $Hfh " *\n";
				print $Hfh " * \\param e\t".$INFO->{"desc"}."\n";
				print $Hfh " * \\return\tPointer to const string with ".$INFO->{"desc"}." code\n";
				print $Hfh " */\n";
				print $Hfh "extern const char *$enum\_lookup_code ($enum e);\n";
			}

			print $Hfh "\n\n\n";
		}

		if (!$NOC) {
			$maxcomm[0] = 0;
			foreach $item (@{$INFO->{"list"}}) {
				$l = length ("$enum\_$item") + 3;
				if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
			}

			print $Cfh "/*! ".$INFO->{"desc"}." */\n";
			print $Cfh "static const char *$enum\_names[$enum\_LIMIT+1] = {\n";

			@maxlen = ();
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};
				if ($data->[1]->[0]) {
					$len = length ("\"$data->[1]->[0]\",");
				} else {
					$len = length ("\"$enum\_$item\",");
				}
				if (!$maxlen[0] || ($len > $maxlen[0])) { $maxlen[0] = $len; }
			}
			if ($maxlen[0] < 45) { $maxlen[0] = 45; }

			$fmt = "\t%-$maxlen[0]s\t\t/*!< %-$maxcomm[0]s */\n";
			$f = 1;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};

				$comm = "$enum\_$item";
				if ($data->[1]->[0]) {
					printf $Cfh $fmt, ("\"$data->[1]->[0]\",", $comm);
				} else {
					printf $Cfh $fmt, ("\"$enum\_$item\",", $comm);
				}
			}
			print $Cfh "};\n\n";

			print $Cfh "/*! ".$INFO->{"desc"}." lookup function\n";
			print $Cfh " *\n";
			print $Cfh " * \\param e\t".$INFO->{"desc"}."\n";
			print $Cfh " * \\return\tPointer to const string with ".$INFO->{"desc"}." name\n";
			print $Cfh " */\n";
			print $Cfh "const char *\n$enum\_lookup ($enum e)\n{\n\treturn ($enum\_names[e]);\n}\n\n";
			print $Cfh "\n\n";

		}

		if (!$NOC && $INFO->{"code"}) {
			print $Cfh "/*! ".$INFO->{"desc"}.": code strings */\n";
			print $Cfh "static const char *$enum\_codes[$enum\_LIMIT+1] = {\n";

			@maxlen = ();
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};
				if ($data->[1]->[1]) {
					$len = length ("\"$data->[1]->[1]\",");
				} else {
					$len = length ("\"\",");
				}
				if (!$maxlen[0] || ($len > $maxlen[0])) { $maxlen[0] = $len; }
			}
			if ($maxlen[0] < 45) { $maxlen[0] = 45; }

			$fmt = "\t%-$maxlen[0]s\t\t/*!< %-$maxcomm[0]s */\n";
			$f = 1;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};

				$comm = "$enum\_$item";
				if ($data->[1]->[1]) {
					printf $Cfh $fmt, ("\"$data->[1]->[1]\",", $comm);
				} else {
					printf $Cfh $fmt, ("\"\",", $comm);
				}
			}
			print $Cfh "};\n\n";

			print $Cfh "/*! ".$INFO->{"desc"}." code lookup function\n";
			print $Cfh " *\n";
			print $Cfh " * \\param e\t".$INFO->{"desc"}."\n";
			print $Cfh " * \\return\tpointer to const string with ".$INFO->{"desc"}." code\n";
			print $Cfh " */\n";
			print $Cfh "const char *\n$enum\_lookup_code ($enum e)\n{\n\treturn ($enum\_codes[e]);\n}\n\n";
			print $Cfh "\n\n";
		}
	}
}

sub RenderEnumflags
{
	my ($rDATA, $Cfh, $Hfh, $OC, $OH) = @_;
	my ($enumflag, $desc, $bname, $benum, $INFO);
	my ($item, $iid, $f, $data, $comm);
	my ($len, @maxlen, $fmt);
	my ($l, @maxcomm);

	foreach $enumflag (@{$rDATA->{"enumflag"}->{"list"}})
	{
		$desc = $rDATA->{"enumflag"}->{"data"}->{$enumflag}->{"desc"};

		($bname, $benum) = split (/:/, $enumflag);
		$INFO = $rDATA->{"enum"}->{"data"}->{$benum};

		$maxcomm[0] = 0;
		foreach $item (@{$INFO->{"list"}}) {
			$data = $INFO->{"data"}->{$item};
			$l = length ($data->[1]->[0]) + 3;
			if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
		}

		if (!$NOI) {
			print $Hfh "/*! $desc */\n";
			print $Hfh "typedef unsigned long $bname;\n\n";

			@maxlen = ();
			foreach $item (@{$INFO->{"list"}}) {

				$len = length ("$bname\_$item");
				if (!$maxlen[0] || ($len > $maxlen[0])) { $maxlen[0] = $len; }

				$len = length ("(1 << $benum\_$item)");
				if (!$maxlen[1] || ($len > $maxlen[1])) { $maxlen[1] = $len; }
			}

			$fmt = "#define\t%-$maxlen[0]s\t%-$maxlen[1]s\t\t/*!< %-$maxcomm[0]s */\n";
			$f = 1;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};
				$iid  = $data->[0];
				$comm = $data->[1]->[0];

				if ($f) {
					printf $Hfh $fmt, ("$bname\_$item", "(0x00000000)", $comm);
					$f = 0;
				} else {
					printf $Hfh $fmt, ("$bname\_$item", "(1 << $benum\_$item)", $comm);
				}
			}

			print $Hfh "\n";
			print $Hfh "\n\n";
		}
	}
}

sub RenderClsEnums
{
	my ($rDATA, $Cfh, $Hfh, $OC, $OH) = @_;
	my ($enum, $ctype, $INFO);
	my ($item, $iid, $f, $data, $comm, $class);
	my ($len, @maxlen, $fmt);
	my ($l, @maxcomm);

	foreach $enum (@{$rDATA->{"clsenum"}->{"list"}})
	{
		$INFO = $rDATA->{"clsenum"}->{"data"}->{$enum};

		($enum, $ctype) = split (":", $enum);

		if (!$NOI) {
			$maxcomm[0] = 0;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{items}->{$item}[0];
				$l = length ($data) + 3;
				if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
			}
			$l = length ("\\internal start code");
			if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
			$l = length ("\\internal final code");
			if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }

			print $Hfh "/*! ".$INFO->{"desc"}." */\n";
			print $Hfh "typedef enum e_$enum {\n";

			@maxlen = (); $l = "";
			foreach $item (@{$INFO->{"list"}}) { $l = $item; }
			$maxlen[0] = 45;

			$fmt = "\t%-$maxlen[0]s\t\t/*!< %-$maxcomm[0]s */\n";
			$f = 1;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{items}->{$item}[0];
				$comm = $data;

				if ($f) {
					printf $Hfh $fmt, ("$enum\_START = 0,", "\\internal start code");
					printf $Hfh $fmt, ("$enum\_$item = 0,", $comm);
					$f = 0;
				} else {
					printf $Hfh $fmt, ("$enum\_$item,", $comm);
				}
			}
			printf $Hfh $fmt, ("$enum\__NONE,", "\\internal default code");
			printf $Hfh $fmt, ("$enum\_LIMIT = $enum\__NONE", "\\internal final code");
			print $Hfh "} $enum;\n";
			print $Hfh "#define\t$enum\_CNT\t($enum\_LIMIT - $enum\_START + 1)\n\n";

			print $Hfh "/*! ".$INFO->{"desc"}." lookup function\n";
			print $Hfh " *\n";
			print $Hfh " * \\param e\t".$INFO->{"desc"}."\n";
			print $Hfh " * \\return\tpointer to const string with ".$INFO->{"desc"}." name\n";
			print $Hfh " */\n";
			print $Hfh "extern const char *$enum\_lookup ($enum e);\n\n";

			print $Hfh "/*! ".$INFO->{"desc"}." classification function\n";
			print $Hfh " *\n";
			print $Hfh " * \\param e\t".$INFO->{"desc"}." value\n";
			print $Hfh " * \\return\t$ctype enum\n";
			print $Hfh " */\n";
			print $Hfh "extern $ctype $enum\_classify ($enum e);\n\n";

			print $Hfh "\n\n";
		}

		if (!$NOC) {
			$maxcomm[0] = 0;
			foreach $item (@{$INFO->{"list"}}) {
				$l = length ("$enum\_$item") + 3;
				if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
			}

			print $Cfh "/*! ".$INFO->{"desc"}." */\n";
			print $Cfh "static const char *$enum\_names[$enum\_LIMIT+1] = {\n";

			@maxlen = ();
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{items}->{$item}[0];
				if ($data) {
					$len = length ("\"$data\",");
				} else {
					$len = length ("\"$enum\_$item\",");
				}
				if (!$maxlen[0] || ($len > $maxlen[0])) { $maxlen[0] = $len; }
			}
			if ($maxlen[0] < 45) { $maxlen[0] = 45; }

			$fmt = "\t%-$maxlen[0]s\t\t/*!< %-$maxcomm[0]s */\n";
			$f = 1;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{items}->{$item}[0];

				$comm = "$enum\_$item";
				if ($data) {
					printf $Cfh $fmt, ("\"$data\",", $comm);
				} else {
					printf $Cfh $fmt, ("\"$enum\_$item\",", $comm);
				}
			}
			printf $Cfh $fmt, ("\"*unknown value*\"", "$enum\__NONE");
			print $Cfh "};\n\n";

			print $Cfh "/*! ".$INFO->{"desc"}." lookup function\n";
			print $Cfh " *\n";
			print $Cfh " * \\param e\t".$INFO->{"desc"}."\n";
			print $Cfh " * \\return\tpointer to const string with ".$INFO->{"desc"}." name\n";
			print $Cfh " */\n";
			print $Cfh "const char *\n$enum\_lookup ($enum e) {\n\treturn ($enum\_names[e]);\n}\n\n";
		}

		if (!$NOC) {
			print $Cfh "/*! ".$INFO->{"desc"}." classification function\n";
			print $Cfh " *\n";
			print $Cfh " * \\param e\t".$INFO->{"desc"}." value\n";
			print $Cfh " * \\return\t$ctype enum\n";
			print $Cfh " */\n";
			print $Cfh "$ctype\n$enum\_classify ($enum e)\n{\n\t$ctype\tc = $ctype\_LIMIT;\n\n\tswitch (e) {\n";

			foreach $class (@{$INFO->{data}->{class_list}}) {
				foreach $item (@{$INFO->{data}->{classes}->{$class}}) {
					print $Cfh "\t\tcase $enum\_$item:\n"
				}
				print $Cfh "\t\t\tc = $ctype\_$class;\n\t\t\tbreak;\n"
			}

			print $Cfh "\t}\n\treturn (c);\n}\n\n";
		}

		print $Cfh "\n\n";
	}
}

sub RenderRawEnums
{
	my ($rDATA, $Cfh, $Hfh, $OC, $OH) = @_;
	my ($enum, $dtype, $INFO);
	my ($item, $iid, $f, $data, $comm);
	my ($len, @maxlen, $fmt);
	my ($l, @maxcomm);

	foreach $enum (@{$rDATA->{"rawenum"}->{"list"}})
	{
		$INFO = $rDATA->{"rawenum"}->{"data"}->{$enum};

		($enum, $dtype) = split (":", $enum);

		if (!$NOI) {
			$maxcomm[0] = 0;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};
				$l = length ($data->[1]->[0]) + 3;
				if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
			}
			$l = length ("\\internal start code");
			if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
			$l = length ("\\internal final code");
			if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }

			print $Hfh "/*! ".$INFO->{"desc"}." */\n";
			print $Hfh "typedef enum e_$enum {\n";

			@maxlen = (); $l = "";
			foreach $item (@{$INFO->{"list"}}) { $l = $item; }
			$maxlen[0] = 45;

			$fmt = "\t%-$maxlen[0]s\t\t/*!< %-$maxcomm[0]s */\n";
			$f = 1;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};
				$iid  = $data->[0];
				$comm = $data->[1]->[0];

				if ($f) {
					printf $Hfh $fmt, ("$enum\_START = 0,", "\\internal start code");
					printf $Hfh $fmt, ("$enum\_$item = 0,", $comm);
					$f = 0;
				} else {
					printf $Hfh $fmt, ("$enum\_$item,", $comm);
				}
			}
			printf $Hfh $fmt, ("$enum\__NONE,", "\\internal default code");
			printf $Hfh $fmt, ("$enum\_LIMIT = $enum\__NONE", "\\internal final code");
			print $Hfh "} $enum;\n";
			print $Hfh "#define\t$enum\_CNT\t($enum\_LIMIT - $enum\_START + 1)\n\n";

			print $Hfh "/*! ".$INFO->{"desc"}." lookup function\n";
			print $Hfh " *\n";
			print $Hfh " * \\param e\t".$INFO->{"desc"}."\n";
			print $Hfh " * \\return\tpointer to const string with ".$INFO->{"desc"}." name\n";
			print $Hfh " */\n";
			print $Hfh "extern const char *$enum\_lookup ($enum e);\n\n";

			if ($INFO->{"code"}) {
				print $Hfh "/*! ".$INFO->{"desc"}." code lookup function\n";
				print $Hfh " *\n";
				print $Hfh " * \\param e\t".$INFO->{"desc"}."\n";
				print $Hfh " * \\return\tpointer to const string with ".$INFO->{"desc"}." code\n";
				print $Hfh " */\n";
				print $Hfh "extern const char *$enum\_lookup_code ($enum e);\n\n";
			}

			print $Hfh "/*! ".$INFO->{"desc"}." translation function\n";
			print $Hfh " *\n";
			print $Hfh " * \\param r\traw ".$INFO->{"desc"}." value\n";
			print $Hfh " * \\return\t".$INFO->{"desc"}."\n";
			print $Hfh " */\n";
			print $Hfh "extern $enum $enum\_xlt ($dtype r);\n\n";

			print $Hfh "\n\n";
		}

		if (!$NOC) {
			$maxcomm[0] = 0;
			foreach $item (@{$INFO->{"list"}}) {
				$l = length ("$enum\_$item") + 3;
				if ($l > $maxcomm[0]) { $maxcomm[0] = $l; }
			}

			print $Cfh "/*! ".$INFO->{"desc"}." */\n";
			print $Cfh "static const char *$enum\_names[$enum\_LIMIT+1] = {\n";

			@maxlen = ();
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};
				if ($data->[1]->[0]) {
					$len = length ("\"$data->[1]->[0]\",");
				} else {
					$len = length ("\"$enum\_$item\",");
				}
				if (!$maxlen[0] || ($len > $maxlen[0])) { $maxlen[0] = $len; }
			}
			if ($maxlen[0] < 45) { $maxlen[0] = 45; }

			$fmt = "\t%-$maxlen[0]s\t\t/*!< %-$maxcomm[0]s */\n";
			$f = 1;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};

				$comm = "$enum\_$item";
				if ($data->[1]->[0]) {
					printf $Cfh $fmt, ("\"$data->[1]->[0]\",", $comm);
				} else {
					printf $Cfh $fmt, ("\"$enum\_$item\",", $comm);
				}
			}
			printf $Cfh $fmt, ("\"*unknown value*\"", "$enum\__NONE");
			print $Cfh "};\n\n";

			print $Cfh "/*! ".$INFO->{"desc"}." lookup function\n";
			print $Cfh " *\n";
			print $Cfh " * \\param e\t".$INFO->{"desc"}."\n";
			print $Cfh " * \\return\tpointer to const string with ".$INFO->{"desc"}." name\n";
			print $Cfh " */\n";
			print $Cfh "const char *\n$enum\_lookup ($enum e) {\n\treturn ($enum\_names[e]);\n}\n\n";
		}

		if (!$NOC && $INFO->{"code"}) {
			print $Cfh "/*! ".$INFO->{"desc"}.": code strings */\n";
			print $Cfh "static const char *$enum\_codes[$enum\_LIMIT+1] = {\n";

			@maxlen = ();
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};
				if ($data->[1]->[1]) {
					$len = length ("\"$data->[1]->[1]\",");
				} else {
					$len = length ("\"\",");
				}
				if (!$maxlen[0] || ($len > $maxlen[0])) { $maxlen[0] = $len; }
			}
			if ($maxlen[0] < 45) { $maxlen[0] = 45; }

			$fmt = "\t%-$maxlen[0]s\t\t/*!< %-$maxcomm[0]s */\n";
			$f = 1;
			foreach $item (@{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};

				$comm = "$enum\_$item";
				if ($data->[1]->[1]) {
					printf $Cfh $fmt, ("\"$data->[1]->[1]\",", $comm);
				} else {
					printf $Cfh $fmt, ("\"\",", $comm);
				}
			}
			print $Cfh "};\n\n";

			print $Cfh "/*! ".$INFO->{"desc"}." code lookup function\n";
			print $Cfh " *\n";
			print $Cfh " * \\param e\t".$INFO->{"desc"}."\n";
			print $Cfh " * \\return\tpointer to const string with ".$INFO->{"desc"}." code\n";
			print $Cfh " */\n";
			print $Cfh "const char *\n$enum\_lookup_code ($enum e) {\n\treturn ($enum\_codes[e]);\n}\n\n";
		}

		if (!$NOC) {
			print $Cfh "/*! ".$INFO->{"desc"}." translation function\n";
			print $Cfh " *\n";
			print $Cfh " * \\param r\traw ".$INFO->{"desc"}." value\n";
			print $Cfh " * \\return\t".$INFO->{"desc"}."\n";
			print $Cfh " */\n";
			print $Cfh "$enum\n$enum\_xlt ($dtype r)\n{\n\t$enum\te;\n\n\tswitch (r) {\n";

			foreach $item (sort {$INFO->{"data"}->{$a}->[0] <=> $INFO->{"data"}->{$b}->[0]} @{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};

				print $Cfh "\t\tcase $data->[0]:\n\t\t\te = $enum\_$item;\n\t\t\tbreak;\n"
			}

			print $Cfh "\t\tdefault:\n\t\t\te = $enum\__NONE;\n\t\t\tbreak;\n\t}\n\treturn (e);\n}\n\n";
		}

		print $Cfh "\n\n";
	}
}

sub RenderRawErefs
{
	my ($rDATA, $Cfh, $Hfh, $OC, $OH) = @_;
	my ($enum, $dtype, $rtype, $INFO);
	my ($item, $data);
	my ($l, @maxcomm);

	foreach $enum (@{$rDATA->{"raweref"}->{"list"}})
	{
		$INFO = $rDATA->{"raweref"}->{"data"}->{$enum};

		($enum, $dtype, $rtype) = split (":", $enum);

		if (!$NOI) {

			print $Hfh "/*! raw ".$INFO->{"desc"}." translation function\n";
			print $Hfh " *\n";
			print $Hfh " * \\param r\traw ".$INFO->{"desc"}." value\n";
			print $Hfh " * \\return\t$rtype enum\n";
			print $Hfh " */\n";
			print $Hfh "extern $rtype $enum\_xlt ($dtype r);\n\n";

			print $Hfh "\n\n";
		}

		if (!$NOC) {
			print $Cfh "/*! raw ".$INFO->{"desc"}." translation function\n";
			print $Cfh " *\n";
			print $Cfh " * \\param r\traw ".$INFO->{"desc"}." value\n";
			print $Cfh " * \\return\t$rtype enum\n";
			print $Cfh " */\n";
			print $Cfh "$rtype\n$enum\_xlt ($dtype r)\n{\n\t$rtype\te;\n\n\tswitch (r) {\n";

			foreach $item (sort {$a <=> $b} @{$INFO->{"list"}}) {
				$data = $INFO->{"data"}->{$item};

				print $Cfh "\t\tcase $item:\n\t\t\te = $rtype\_$data->[0];\n\t\t\tbreak;\n"
			}

			print $Cfh "\t\tdefault:\n\t\t\te = $rtype\__NONE;\n\t\t\tbreak;\n\t}\n\treturn (e);\n}\n\n";
		}

		print $Cfh "\n\n";
	}
}

sub Render
{
	my ($rDATA, $OC, $OH, $IH) = @_;
	my ($Cfh, $Hfh, $HN, $type);

	$HN = uc ($OH);
	$HN =~ s/[^A-Z0-9_]/_/g;
	$HN = "protection_$HN";

	if (!$NOC) {
		$Cfh = new IO::File;
		$Cfh->open (">$OC") || die "failed to open \"$OC\" for writing!\n";
	}

	if (!$NOI) {
		$Hfh = new IO::File;
		$Hfh->open (">$OH") || die "failed to open \"$OH\" for writing!\n";
	}

	&RenderTitle ($rDATA, $Cfh, $Hfh, $OC, $HOUT);

	if (!$NOI) {
		print $Hfh "#ifndef\t$HN\n#define\t$HN\t1\n";

		if ($rDATA->{"UserSectionHeader"} && @{$rDATA->{"UserSectionHeader"}}) {
			my $line;
			print $Hfh "\n";
			foreach $line (@{$rDATA->{"UserSectionHeader"}}) { print $Hfh $line,"\n"; }
			print $Hfh "\n";
			print $Hfh "\n\n\n";
		}

		print $Hfh "#ifdef\t__cplusplus\nextern \"C\" {\n#endif\t/* __cplusplus */\n";
		print $Hfh "\n\n\n";
	}

	foreach $type (sort {$PARSER{$a}[1] <=> $PARSER{$b}[1]} keys %PARSER) {
		if (!$PARSER{$type}[2]) { next; }
		&{$PARSER{$type}[2]} ($rDATA, $Cfh, $Hfh, $OC, $HOUT);
	}

	if (!$NOC) {
		$Cfh->close;
	}

	if (!$NOI) {
		print $Hfh "#ifdef\t__cplusplus\n}\n#endif\t/* __cplusplus */\n";
		print $Hfh "#endif\t/* $HN */\n";
		
		if ($rDATA->{"UserSectionFooter"} && @{$rDATA->{"UserSectionFooter"}}) {
			my $line;
			print $Hfh "\n";
			foreach $line (@{$rDATA->{"UserSectionFooter"}}) { print $Hfh $line,"\n"; }
			print $Hfh "\n";
		}
		
		$Hfh->close;
	}

}

&Render (\%DATA, $COUT, $IOUT, $HOUT);
