# Qwt
The spwproject project is built with Qwt version 5.2.1.

For your convenience I have included a copy of the original Qwt source
archive, as well as a patch to apply to build Qwt in the same way as it
was built for this project.

It is *strongly* advised to build Qwt with the Qt that was built for this
project. (The build procedure is described [here](../Qt/README.md))

Use the following steps to build Qwt:

* extract the qwt-5.2.1.zip source archive in a directory of your choice

  >for the sake of this example, let's assume you choose C:\QWT\qwt-5.2.1

* extract the qwt_patch_5.2.1.zip archive in the C:\QWT\qwt-5.2.1 directory (confirm file overwrites)

* configure the Qt location with a qtdir.bat file (see example_qtdir.bat)

* run the prepare.bat batch file 

* run the build.bat batch file

* if all went well, your C:\QWT\qwt-5.2.1 directory will now contain a Qwt ready for building the spwproject

* if something went wrong, you can find build logs in the QWTBUILD.LOG logfile
