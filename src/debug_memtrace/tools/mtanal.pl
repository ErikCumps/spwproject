#!/usr/bin/perl

# debug_memtrace: an example trace analyzer script.
#
# Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
#
# License: GPL v2

use strict;

my @LOGFILES = (
	"warcab_memtrace.log",
	"drawhexes_memtrace.log",
	"debug_memtrace.log",
	);
my $LOGFILE = "";

my %CFG = {
	"sortid" => 1,
	"csv" => 0,
};

&Init (@ARGV);

if (!$LOGFILE) {
	my ($log);
	foreach $log (@LOGFILES) {
		if (-e $log) { $LOGFILE = $log; last; }
	}
}
die "No logfile found!\n" if (!$LOGFILE);

&Parse ($LOGFILE);

sub Usage
{
	print "Usage: $0 [-Si | -Sa] [-csv] [-h] [logfile]\n";
	print "Where: -h        print this help info\n";
	print "       -Si       sort leak report on sequence id\n";
	print "       -Sa       sort leak report on leak address\n";
	print "       -csv      generate CSV file with allocation statistics\n";
	print "       logfile   optional logfile to process\n";
	print "\n";
	print "Defaults:\n";
	if ($CFG{sortid}) {
		print "\tsort on sequence id\n";
	} else {
		print "\tsort on leak address\n";
	}
	if ($CFG{csv}) {
		print "\tgenerate CSV file\n";
	}
	
	print "\n";
	exit (0);
}

sub Init
{
	my (@ARGS) = @_;
	my ($i);

	for $i (0 .. $#ARGS) {
		if ($ARGS[$i] eq "-h") { &Usage; }
		if ($ARGS[$i] eq "-Si") {
			$CFG{sortid} = 1;
			next;
		}
		if ($ARGS[$i] eq "-Sa") {
			$CFG{sortid} = 0;
			next;
		}
		if ($ARGS[$i] eq "-csv") {
			$CFG{csv} = 1;
			next;
		}
		if ($LOGFILE) { &Usage; }
		$LOGFILE = $ARGS[$i];
	}
}

sub INFO_init
{
	my ($rINFO, $MARK) = @_;

	%{$rINFO} = ();

	$rINFO->{count} = 1;

	%{$rINFO->{tables}->[0]} = ();
	$rINFO->{tables}->[0]->{size} = 0;
	$rINFO->{tables}->[0]->{name} = $MARK;
	%{$rINFO->{tables}->[0]->{data}} = ();
	
	%{$rINFO->{stats}} = ();
}

sub INFO_table
{
	my ($rINFO, $name) = @_;
	my ($idx);

	$idx = $rINFO->{count}++;
	%{$rINFO->{tables}->[$idx]} = ();
	$rINFO->{tables}->[$idx]->{size} = 0;
	$rINFO->{tables}->[$idx]->{name} = $name;
	%{$rINFO->{tables}->[$idx]->{data}} = ();
}

sub INFO_count
{
	my ($rINFO) = @_;

	return ($rINFO->{count});
}

sub INFO_current
{
	my ($rINFO) = @_;
	my ($idx);

	$idx = $rINFO->{count};
	if ($idx) {
		return ($rINFO->{tables}->[$idx-1]);
	} else {
		return (0);
	}
}

sub INFO_indexed
{
	my ($rINFO, $idx) = @_;

	return ($rINFO->{tables}->[$idx]);
}

sub Parse
{
	my ($FILE) = @_;
	my ($lc, %INFO, @INFO);
	my ($addr, $size, $tid, $src, $mark, @del, $delsrc);

	INFO_init (\%INFO, "APPLICATION STARTUP");

	open (IN, "<$FILE") || die "failed to open \"$FILE\" for reading!\n";
	$lc = 0; @INFO = @del = ();
	while (<IN>) {
		chop; $lc++;
		if (/^\+\+\+/) {
			($addr, $size, $tid, $src) = /^\+\+\+\s+ptr=(0x[0-9a-fA-F]+)\s+size=(0x[0-9a-fA-F]+)\s+\[(0x[0-9a-fA-F]+)\]\s+\(([^)]+)\).*$/;
			defined ($addr) || die "invalid alloc syntax on line $lc: $_\n";
			&HandleAE (\%INFO, \@INFO, $lc, $addr, hex ($size), $src);
			next;
		}
		if (/^---/) {
			($addr, $tid, $src) = /^---\s+ptr=(0x[0-9a-fA-F]+)\s+\[(0x[0-9a-fA-F]+)\]\s+\(([^)]+)\).*$/;
			defined ($addr) || die "invalid free syntax on line $lc: $_\n";
			&HandleFE (\%INFO, \@INFO, $lc, $addr, $src);
			next;
		}
		if (/^###/) {
			($mark) = /^###\s+MARK\s+\(([^)]+)\).*$/;
			defined ($mark) || die "invalid mark syntax on line $lc: $_\n";
			&HandleME (\%INFO, $lc, $mark);
			next;
		}
		if (/^NEW/) {
			($addr, $size, $tid, $src) = /^NEW\s+ptr=(0x[0-9a-fA-F]+)\s+size=(0x[0-9a-fA-F]+)\s+\[(0x[0-9a-fA-F]+)\]\s+\(([^)]+)\).*$/;
			defined ($addr) || die "invalid new syntax on line $lc: $_\n";
			&HandleNE (\%INFO, \@INFO, $lc, $addr, hex ($size), $src);
			next;
		}
		if (/^DEL/) {
			($tid, $src) = /^DEL\s+ptr=0x00000000\s+\[(0x[0-9a-fA-F]+)\]\s+\(([^)]+)\).*$/;
			defined ($src) || die "invalid delete syntax on line $lc: $_\n";
			push @del, ( $src );
			next;
		}
		if (/^D--/) {
			($addr, $tid) = /^D--\s+ptr=(0x[0-9a-fA-F]+)\s+\[(0x[0-9a-fA-F]+)\].*$/;
			defined ($addr) || die "invalid deletefree syntax on line $lc: $_\n";
			$delsrc = pop @del;
			# deletefree may happen for objects for which no NEW tracking is provided, for
			# example internal allocations in Qt. These deletefree entries are silently ignored.
			# deletefree may also happen for objects for which no DEL tracking is provided.
			# These deletefree entries should not be ignored!
			if (defined ($delsrc)) {
				if (&HandleDE (\%INFO, \@INFO, $lc, $addr, $delsrc) != 0) {
					push @del, ( $delsrc );
				}
			} else {
				&HandleDE (\%INFO, \@INFO, $lc, $addr, "???");
			}
			next;
			
		}
		die "invalid syntax on line $lc: $_\n";
	}
	close (IN);
	
	my $delcnt = $#del+1;
	if ($delcnt) { die "$delcnt unaccounted delete entries discovered\n"; }

	&Report (\%INFO, \@INFO, $FILE);
}

sub HandleAE
{
	my ($rINFO, $rMINF, $id, $addr, $size, $src) = @_;
	my ($idx, $r, $p);

	for $idx (0 .. &INFO_count ($rINFO)) {
		$r = &INFO_indexed ($rINFO, $idx);
		$p = $r->{data}->{$addr};
		if ($p) { last; }
	}
	if ($p) {
		print "*** MYSTERY ALLOC on line $id: (ptr=$addr size=$size $src) == ([$p->[0]] ptr=$p->[1] size=$p->[2] $p->[3])\n";
	} else {
		$r = &INFO_current ($rINFO);
		$r->{data}->{$addr} = [ $id, $addr, $size, $src ];

		$rMINF->[0] += $size;
		if ($rMINF->[0] > $rMINF->[1]) { $rMINF->[1] = $rMINF->[0]; }
		
		&StatAlloc ($rINFO, $size, $src, $id);
	}
}

sub HandleFE
{
	my ($rINFO, $rMINF, $id, $addr, $src) = @_;
	my ($idx, $r, $p);

	for $idx (0 .. &INFO_count ($rINFO)) {
		$r = &INFO_indexed ($rINFO, $idx);
		$p = $r->{data}->{$addr};
		if ($p) { last; }
	}
	if (!$p) {
		print "*** MYSTERY FREE on line $id: (ptr=$addr $src)\n";
	} else {
		$rMINF->[0] -= $p->[2];
		delete ($r->{data}->{$addr});
		
		&StatFree ($rINFO, $src, $id);
	}
}

sub HandleME
{
	my ($rINFO, $id, $mark) = @_;

	&INFO_table ($rINFO, $mark);
}

sub HandleNE
{
	my ($rINFO, $rMINF, $id, $addr, $size, $src) = @_;
	my ($idx, $r, $p);

	for $idx (0 .. &INFO_count ($rINFO)) {
		$r = &INFO_indexed ($rINFO, $idx);
		$p = $r->{data}->{$addr};
		if ($p) { last; }
	}
	if ($p) {
		print "*** MYSTERY NEW on line $id: (ptr=$addr size=$size $src) == ([$p->[0]] ptr=$p->[1] size=$p->[2] $p->[3])\n";
	} else {
		$r = &INFO_current ($rINFO);
		$r->{data}->{$addr} = [ $id, $addr, $size, $src ];

		$rMINF->[0] += $size;
		if ($rMINF->[0] > $rMINF->[1]) { $rMINF->[1] = $rMINF->[0]; }
		
		&StatAlloc ($rINFO, $size, $src, $id);
	}
}

sub HandleDE
{
	my ($rINFO, $rMINF, $id, $addr, $src) = @_;
	my ($idx, $r, $p, $altaddr);
	my ($RC);

	for $idx (0 .. &INFO_count ($rINFO)) {
		$r = &INFO_indexed ($rINFO, $idx);
		$p = $r->{data}->{$addr};
		if ($p) { last; }
	}
###	if (!$p) {
###		$altaddr = sprintf "0x%8.8x", ( hex($addr)+4 );
###		for $idx (0 .. &INFO_count ($rINFO)) {
###			$r = &INFO_indexed ($rINFO, $idx);
###			$p = $r->{data}->{$altaddr};
###			if ($p) { $addr = $altaddr; last; }
###		}
###	}
	if (!$p) {
		###print "*** MYSTERY DELETE on line $id: (ptr=$addr $src)\n";
		$RC = 1;
	} else {
		$rMINF->[0] -= $p->[2];
		delete ($r->{data}->{$addr});

		&StatFree ($rINFO, $src, $id);
		
		$RC = 0;
	}
	return ($RC);
}

sub StatAlloc
{
	my ($rINFO, $size, $src, $lc) = @_;
	my ($file, $line, $func, $floc);
	
	($file, $line, $func) = $src =~ /^([^:]+):(\d+),\s+(.*),\s+\d+\s+bytes.*$/;
	if (!defined($file) && ($src eq "???")) {
		($file, $line, $func) = ("???", 0, "???");
	}
	defined($file) || die "invalid src syntax on line $lc: \"$src\"\n";
	
	$floc = "$func:$line";
	
	$rINFO->{stats}->{cnt}->[0]++;
	$rINFO->{stats}->{cnt}->[1]++;
	if ($size == $rINFO->{stats}->{max}->[0]) { $rINFO->{stats}->{max}->[1]++; }
	if ($size > $rINFO->{stats}->{max}->[0]) { $rINFO->{stats}->{max} = [ $size, 1 ]; }
	
	if (!defined($rINFO->{stats}->{lst}->{$file}->{cnt})) { $rINFO->{stats}->{lst}->{$file}->{cnt} = [ 0, 0, 0 ] };
	if (!defined($rINFO->{stats}->{lst}->{$file}->{max})) { $rINFO->{stats}->{lst}->{$file}->{max} = [ 0, 0 ] };
	$rINFO->{stats}->{lst}->{$file}->{cnt}->[0]++;
	$rINFO->{stats}->{lst}->{$file}->{cnt}->[1]++;
	if ($size == $rINFO->{stats}->{lst}->{$file}->{max}->[0]) { $rINFO->{stats}->{lst}->{$file}->{max}->[1]++; }
	if ($size > $rINFO->{stats}->{lst}->{$file}->{max}->[0]) { $rINFO->{stats}->{lst}->{$file}->{max} = [ $size, 1 ]; }
	
	if (!defined($rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{cnt})) { $rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{cnt} = [ 0, 0, 0 ] };
	if (!defined($rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{max})) { $rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{max} = [ 0, 0 ] };
	$rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{cnt}->[0]++;
	$rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{cnt}->[1]++;
	if ($size == $rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{max}->[0]) { $rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{max}->[1]++; }
	if ($size > $rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{max}->[0]) { $rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{max} = [ $size, 1 ]; }
}

sub StatFree
{
	my ($rINFO, $src, $lc) = @_;
	my ($file, $line, $func, $floc);
	
	($file, $line, $func) = $src =~ /^([^:]+):(\d+),\s+(.*)$/;
	if (!defined($file) && ($src eq "???")) {
		($file, $line, $func) = ("???", 0, "???");
	}
	defined($file) || die "invalid src syntax on line $lc: \"$src\"\n";
	
	$floc = "$func:$line";
	
	$rINFO->{stats}->{cnt}->[0]--;
	$rINFO->{stats}->{cnt}->[2]++;
	
	if (!defined($rINFO->{stats}->{lst}->{$file}->{cnt})) { $rINFO->{stats}->{lst}->{$file}->{cnt} = [ 0, 0, 0 ] };
	if (!defined($rINFO->{stats}->{lst}->{$file}->{max})) { $rINFO->{stats}->{lst}->{$file}->{max} = [ 0, 0 ] };
	$rINFO->{stats}->{lst}->{$file}->{cnt}->[0]--;
	$rINFO->{stats}->{lst}->{$file}->{cnt}->[2]++;
	
	if (!defined($rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{cnt})) { $rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{cnt} = [ 0, 0, 0 ] };
	if (!defined($rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{max})) { $rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{max} = [ 0, 0 ] };
	$rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{cnt}->[0]--;
	$rINFO->{stats}->{lst}->{$file}->{lst}->{$floc}->{cnt}->[2]++;
}

sub CmpName
{
	my ($a, $b) = @_;
	my ($d);
	
	$d = ($a cmp $b);
	return ($d);
}

sub CmpNameSize
{
	my ($a, $b) = @_;
	my ($Na, $Sa, $Nb, $Sb, $d);
	
	($Na, $Sa) = $a =~ /^(\S+),\s+(\d+)\s+bytes.*$/;
	($Nb, $Sb) = $b =~ /^(\S+),\s+(\d+)\s+bytes.*$/;
	
	$d = $Na cmp $Nb;
	if (!$d) { $d = $Sa <=> $Sb; }
	return ($d);
}

sub CmpSizeName
{
	my ($aS, $bS, $aN, $bN) = @_;
	my ($d);
	
	$d = $aS <=> $bS;
	if (!$d) { $d = $aN cmp $bN; }
	return ($d);
}

sub CmpCntName
{
	my ($aC, $bC, $aN, $bN) = @_;
	my ($d);
	
	$d = $aC <=> $bC;
	if (!$d) { $d = &CmpName ($aN, $bN); }
	return ($d)
}

sub CmpCntNameSize
{
	my ($aC, $bC, $aN, $bN) = @_;
	my ($d);
	
	$d = $aC <=> $bC;
	if (!$d) { $d = &CmpNameSize ($aN, $bN); }
	return ($d)
}

sub CmpSizeNameSize
{
	my ($aS, $bS, $aN, $bN) = @_;
	my ($Na, $Sa, $Nb, $Sb, $d);
	
	($Na, $Sa) = $aN =~ /^(\S+),\s+(\d+)\s+bytes.*$/;
	($Nb, $Sb) = $bN =~ /^(\S+),\s+(\d+)\s+bytes.*$/;
	
	$d = $aS <=> $bS;
	if (!$d) { $d = $Na cmp $Nb; }
	if (!$d) { $d = $Sa <=> $Sb; }
	return ($d);
}

sub Report
{
	my ($rINFO, $rMINF, $FILE) = @_;
	my (@list, $key, $ptr, $size);
	my ($idx, $table);
	my (@l1list, $l1key, @l2list, $l2key, $func, $line);

	print "\n";
	print "Memory allocation report:\n";
	print "=========================\n";
	print "\n";
	printf "Current   : %lu\n", ($rMINF->[0]);
	printf "Peak      : %lu\n", ($rMINF->[1]);
	printf "Totals    : %d = %lu - %lu (saldo = allocs - frees)\n", @{$rINFO->{stats}->{cnt}};
	printf "Peak block: %lu bytes (%u times)\n", @{$rINFO->{stats}->{max}};
	
	print "\n";
	print "Memory leak report:\n";
	print "===================\n";
	print "\n";

	$size = 0;
	for $idx (0 .. (&INFO_count ($rINFO))) {
		$table = &INFO_indexed ($rINFO, $idx);
		if (!(keys %{$table->{data}})) { next; }

		if ($table->{name}) {
			print "MARK: $table->{name}\n";
		}

		if ($CFG{sortid}) {
			@list = sort {$table->{data}->{$a}->[0] <=> $table->{data}->{$b}->[0]} keys %{$table->{data}};
		} else {
			@list = sort keys %{$table->{data}};
		}

		foreach $key (@list) {
			$ptr = $table->{data}->{$key};
			printf "[%8.8x] $key: $ptr->[2] bytes ($ptr->[3])\n", ($ptr->[0]);
			$table->{size} += $ptr->[2];
		}
		print "\n";

		print "Leak size: $table->{size} bytes\n";
		print "\n";

		$size += $table->{size};
	}
	print "\n";
	print "Total leak size: $size bytes\n";
	print "\n";
	
	if ($CFG{csv}) {
		$FILE .= ".csv";
		open (CSV, ">$FILE") || die "failed to open statistics file \"$FILE\" for writing!\n";
		print CSV "Sep=;\n";
		print CSV "FILE;FUNC;LINE;SALDO;ALLOC;FREE;MAX;MAXCNT\n";
	
		printf CSV "Global;_SUM_;0;%d;%lu;%lu;%lu;%u\n", (
			@{$rINFO->{stats}->{cnt}},
			@{$rINFO->{stats}->{max}}
		);

		if ($CFG{sortid}) {
			@l1list = sort {&CmpCntName (
					$rINFO->{stats}->{lst}->{$b}->{cnt}->[0], 
					$rINFO->{stats}->{lst}->{$a}->{cnt}->[0], 
					$a, $b )
				} keys %{$rINFO->{stats}->{lst}};	
		} else {
			@l1list = sort {&CmpSizeName (
					$rINFO->{stats}->{lst}->{$b}->{max}->[0],
					$rINFO->{stats}->{lst}->{$a}->{max}->[0],
					$a, $b )
				} keys %{$rINFO->{stats}->{lst}};
		}
		foreach $l1key (@l1list) {
			printf CSV "%s;_SUM_;0;%d;%lu;%lu;%lu;%u\n", (
				$l1key,
				@{$rINFO->{stats}->{lst}->{$l1key}->{cnt}},
				@{$rINFO->{stats}->{lst}->{$l1key}->{max}}
			);
			if ($CFG{sortid}) {
				@l2list = sort {&CmpCntNameSize (
					$rINFO->{stats}->{lst}->{$l1key}->{lst}->{$b}->{cnt}->[0],
					$rINFO->{stats}->{lst}->{$l1key}->{lst}->{$a}->{cnt}->[0],
					$a, $b )
					} keys %{$rINFO->{stats}->{lst}->{$l1key}->{lst}};
			} else {
				@l2list = sort {&CmpSizeNameSize (
					$rINFO->{stats}->{lst}->{$l1key}->{lst}->{$b}->{max}->[0],
					$rINFO->{stats}->{lst}->{$l1key}->{lst}->{$a}->{max}->[0],
					$a, $b )
				} keys %{$rINFO->{stats}->{lst}->{$l1key}->{lst}};
			}
			foreach $l2key (@l2list) {
				($func, $line) = $l2key =~ /^(.*):(\d+)$/;
				printf CSV "%s;%s;%lu;%d;%lu;%lu;%lu;%u\n", (
					$l1key, $func, $line,
					@{$rINFO->{stats}->{lst}->{$l1key}->{lst}->{$l2key}->{cnt}},
					@{$rINFO->{stats}->{lst}->{$l1key}->{lst}->{$l2key}->{max}}
				);
			}
		}
		close (CSV);
		
		print "\n";
		print "Memory allocation statistics:\n";
		print "=============================\n";
		print "\n";
		print "See $FILE.\n";
		print "\n";
	}
}
