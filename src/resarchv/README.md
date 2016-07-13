#resarchv
This is the Resource Archive library and tools.

Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>

License: GPL V2.

A Resource Archive is a structured file containing resources.

The supported resources are:

* a generic binary blob (binary data of unspecified type)
* an image (data that can be used by Qt to create a QPixmap)
* an image collection (a single image containing many sub-images)
* an RGB color triplets (an #RRGGBB value)
* an RGBA color quadlets (an #RRGGBBAA value)
* an XPM resource (an image in XPM format)

The resarchv library provides an API to create and access a resource archive.

The buildres tool allows creating a resource archive, with the content
specified by a resource configuration file.

The resrpt tool creates a content listing for a resource archive, and also
allows dumping the content of the resource archive to separate files.
