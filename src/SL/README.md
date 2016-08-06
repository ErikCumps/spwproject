#SL
This is Slay's library.

Copyright (C) 2001-2016 Erik Cumps <erik.cumps@gmail.com>

License: GPL V2.

This is my application support library.

It offers the following facilities to C and C++ applications:

* application information handling
* application termination handling
* boolean data type
* error handling
* error and abort event callbacks
* out-of-memory, panic, error and exit hooks
* lock handling
* safe memory management handling
* state debug reporting
* lowlevel system support
* thread handling
* some utility functions

NOTE
----

the following files are generated from blst defintion files in the gen directory:

* include\SL\init_ids.h
* include\SL\message_ids.h
* src\init_ids.c
* src\message_ids.c

They have been committed for your build convenience, but if the blst definitions
ever change, please remember to regenerate these header and source files using the
update.bat batch file in the gen directory. This runs the buildlist.pl tool, which
is a perl script, so you will need to have Perl installed on your system.

