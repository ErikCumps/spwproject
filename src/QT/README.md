# QT
The spwproject project is built with the opensource Qt version 4.3.3.

For your convenience I have included a copy of the original Qt source
archive, as well as a patch to apply to build Qt in the same way as it
was built for this project.

Use the following steps to build Qt:

* extract the qt-win-opensource-src-4.3.3.zip source archive in a directory of your choice

  >for the sake of this example, let's assume you choose C:\QT

* extract the qt_patch_4.3.3.zip archive in the C:\QT directory (confirm file overwrites)

* edit the C:\QT\bin\qtvars.bat file to:

  >set the QTDIR variable to the Qt source directory

  >set the MSVSDIR to your Visual Studio VC bin directory

* run the config.bat batch file

* run the build.bat batch file

* run the install.bat batchfile

* if all went well, your C:\QT directory will now contain a Qt ready for building the spwproject

* if something went wrong, you can find config and build logs in the QTLOG_*.TXT logfiles
