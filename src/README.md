# src

The spwproject source projects are built with:
* Microsoft Visual C++ .NET 2003
* Qt 4.3.3
* Qwt 5.2.1
* zlib 1.2.8

To prepare the projects for building, please:
* Build Qt 4.3.3,
  using the procedure described [here](QT/README.md)
* Build Qwt 5.2.1,
  using the procedure described [here](Qwt/README.md),
  with the Qt built in the previous step
* configure the Qt location with a qtdir.bat file (see example_qtdir.bat)
* configure the Qwt location with a qwtdir.bat file (see example_qwtdir.bat)
* run the prepare.bat batch file

These are the spwproject source projects:
* [sys](sys/README.md)
* [debug_memtrace](debug_memtrace/README.md)
* [ad_api](ad_api/README.md)
* [isfilenewer](isfilenewer/README.md)
* [mkbuildinfo](mkbuildinfo/README.md)
* [resarchv](resarchv/README.md)
* [SL](SL/README.md)
* [mkxpm](mkxpm/README.md)
* [spwawlib](spwawlib/README.md)
* [spwreport](spwreport/README.md)
* [spwdecompose](spwdecompose/README.md)
* [spwreconstruct](spwreconstruct/README.md)
* [warcab](warcab/README.md)

And also:
* [devtools](devtools/README.md)

The following external libraries are used to build the project:
* [Qt](QT/README.md)
* [zlib](ZLIB/README.md)
* [Qwt](QWT/README.md)
