#debug_memtrace
This is debug_memtrace, a poor man's heap memory management tracing facility.

Copyright (C) 2008-2019 Erik Cumps <erik.cumps@gmail.com>

License: GPL V2.

This project can be used to generate a trace of heap memory operations, which
can then be post-processed to detect memory leaks.

The following operations are traced:

	* for C: malloc, calloc, free, realloc and strdup
	* for C++: new, delete and delete[]


Each trace line provides the following information (where applicable):

	* type: allocation/free/new/delete
	* pointer value
	* allocation size
	* thread id
	* caller file name, line number and function name

An example trace analyzer script (written in Perl) is provided:

	tools\mtanal.pl.

