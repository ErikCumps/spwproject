#spwawlib
This is the Steel Panthers World at War library.

Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>

License: GPL V2.

This library provides (almost complete) access to SPWaW savegames.

It can create snapshots from these savegames and it can add snapshots
to a dossier for progress and history tracking.

Not all details of the SPWaW savegame content have (yet) been decoded,
but most useful information for turn, batle and campaign tracking is
available. There is some confusion with regards to the extra campaign
information. for example, the number of major/minor victories and losses
in a campaign is not captured.

The library also provides access to the game's OOB file content.

The snapshot and dossier files can be saved with zlib compression.

Please note that there seem to be some incompatibilities with the new
SPWaW 2016 release. These are under investigation.

NOTE
----

the following files are generated from blst definition files in the gen directory:

* include\spwawlib_spwoob_types.h
* src\spwoob\spwawlib_spwoob_types.c

They have been committed for your build convenience, but if the blst definitions
ever change, please remember to regenerate these header and source files using the
update.bat batch file in the gen directory. This runs the buildlist.pl tool, which
is a perl script, so you will need to have Perl installed on your system.

