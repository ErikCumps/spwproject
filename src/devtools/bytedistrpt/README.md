#bytedistrpt
A simple tool to report the byte distribution in a file.

Copyright (C) 2010-2016 Erik Cumps <erik.cumps@gmail.com>

License: GPL V2.

The tool analyzes a file and provides a number of distribution reports, each
of which list the absolute and relative number of occurrences of the 255
different byte values and of some character classes:

* a character class distribution report for

	* alphabetic characters: 'A' to 'Z' and 'a' to 'z'
	* numeric characters: '0' to '9'
	* punctuation: '!' to '/', ':' to '@', '[' to '`' and '{' to '~'
	* whitespace: ' ', '\t' and '\n'
	* null: the '\0' character
	* other: everything else
	
* a byte value distribution report, sorted on frequency

* a byte value report, sorted on byte value
