# spwawlib

These are the spwawlib savegames regression tests.

The spwreport tool is used to generate reports of savegames. These reports are
then compared with a baseline, so that changes can be detected.

Requirements:

These tests requires a test deployment of spwawlib (in ../../bin).

To perform the comparison with the baseline, the "diff" tool is used.
Please provide this in PATH before running the tests.


Note:

* _OOB_
  this folder contains the currently known OOB sets
* UNKNOWN_OOB
  this folder contains older regression test savegames for which the OOB set is not known
