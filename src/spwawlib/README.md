#spwawlib
This is the SP:WaW and winSPWW2 library.

Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>

License: GPL V2.

This library provides (almost complete) access to SP:WaW and winSPWW2 savegames.

It can create snapshots from these savegames and it can add snapshots to a
dossier for progress and history tracking.

Not all details of the SP:WaW and winSPWW2 savegame content have (yet) been
decoded, but most useful information for turn, batle and campaign tracking is
available.

The library also provides access to the SP:WaW and winSPWW2 OOB file content.

The snapshot and dossier files can be saved with zlib compression.

NOTE
----

the following files are generated from blst definition files in the gen directory:

* include\spwawlib_spwoob_types.h
* src\spwoob\spwawlib_spwoob_types.c

They have been committed for your build convenience, but if the blst definitions
ever change, please remember to regenerate these header and source files using the
update.bat batch file in the gen directory. This runs the buildlist.pl tool, which
is a perl script, so you will need to have Perl installed on your system.

