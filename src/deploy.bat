@echo off

rem
rem This is the simple deployment tool of the spwproject.
rem

if "%1x" == "x" goto USAGE

set CWD=%CD%

rem Obtain the Qt directory
if not exist qtdir.bat goto ERR_NOQT
call qtdir.bat

rem Obtain the Qwt directory
if not exist qwtdir.bat goto ERR_NOQWT
call qwtdir.bat

rem The zlib location is fixed
set ZLIBDIR=ZLIB

echo ^+-------------------------------------^+
echo ^|   Deploying the spwproject builds   ^|
echo ^+-------------------------------------^+
echo.
echo     QTDIR  : %QTDIR%
echo     QWTDIR : %QWTDIR%
echo     ZLIBDIR: %ZLIBDIR%
echo     CWD    : %CWD%
echo.

rem Determine the deployment type
set TYPE=
if "%1x" == "testx"		set TYPE=test
if "%1x" == "debugx"		set TYPE=debug
if "%1x" == "releasex"		set TYPE=release
if "%TYPE%x" == "x" goto USAGE

rem Determine the Qt/Qwt deployment dll suffix
set QTDLL=4
if "%TYPE%" == "debug"		set QTDLL=d4
set QWTDLL=5
if "%TYPE%" == "debug"		set QWTDLL=d5

rem Determine the deployment directory
if "%TYPE%" == "test"		set DST=..\_TEST_\bin
if "%TYPE%" == "debug"		set DST=..\deploy\Debug\warcab
if "%TYPE%" == "release"	set DST=..\deploy\Release\warcab

echo     TYPE   : %TYPE%
echo     DST    : %DST%
echo.

rem Prepare the target directory
if exist %DST% rmdir /S /Q %DST% >nul 2>nul
mkdir %DST% >nul 2>nul

rem Prepare the Qt plugins directory
set PLUGINS=%DST%\qtplugins\imageformats
mkdir %PLUGINS% >nul 2>nul

rem Prepare the Save directory
set SAVE=%DST%\save
mkdir %SAVE% >nul 2>nul

rem A test install is a bit special
if "%TYPE%" == "test" goto TESTINSTALL

rem Perform a regular install

xcopy /Y qt.conf					%DST%
xcopy /Y %QTDIR%\lib\QtCore%QTDLL%.dll			%DST%
xcopy /Y %QTDIR%\lib\QtGui%QTDLL%.dll			%DST%
xcopy /Y %QTDIR%\bin\QtSvg%QTDLL%.dll			%DST%
xcopy /Y %QTDIR%\bin\QtXml%QTDLL%.dll			%DST%
xcopy /Y %QTDIR%\plugins\imageformats\qgif%QTDLL%.dll	%PLUGINS%
xcopy /Y %QTDIR%\plugins\imageformats\qjpeg%QTDLL%.dll	%PLUGINS%
xcopy /Y %QTDIR%\plugins\imageformats\qmng%QTDLL%.dll	%PLUGINS%
xcopy /Y %QTDIR%\plugins\imageformats\qsvg%QTDLL%.dll	%PLUGINS%
xcopy /Y %QTDIR%\plugins\imageformats\qtiff%QTDLL%.dll	%PLUGINS%
xcopy /Y %QWTDIR%\lib\qwt%QWTDLL%.dll			%DST%
xcopy /Y %ZLIBDIR%\lib\zlib1.dll			%DST%
xcopy /Y debug_memtrace\%TYPE%\debug_memtrace.dll	%DST%
xcopy /Y spwawlib\%TYPE%\spwawlib.dll			%DST%
xcopy /Y spwreport\%TYPE%\spwreport.exe			%DST%
xcopy /Y spwdecompose\%TYPE%\spwdecompose.exe		%DST%
xcopy /Y spwreconstruct\%TYPE%\spwreconstruct.exe	%DST%
xcopy /Y warcab\%TYPE%\warcab.exe			%DST%
xcopy /Y warcab\resource\warcab.res			%DST%
xcopy /Y DATA\demo.warcab				%SAVE%
xcopy /Y DATA\README.txt				%DST%
xcopy /Y DATA\CHANGES					%DST%
xcopy /Y ..\LICENSE					%DST%

if "%TYPE%" == "release" goto DONE

rem For a Debug deployment some additional files must be deployed
xcopy /Y debug_memtrace\%TYPE%\debug_memtrace.pdb	%DST%
xcopy /Y spwawlib\%TYPE%\spwawlib.pdb			%DST%
xcopy /Y spwreport\%TYPE%\spwreport.pdb			%DST%
xcopy /Y spwdecompose\%TYPE%\spwdecompose.pdb		%DST%
xcopy /Y spwreconstruct\%TYPE%\spwreconstruct.pdb	%DST%
xcopy /Y warcab\warcab.pdb				%DST%

goto DONE

:TESTINSTALL

rem Perform a development-testing deployment
xcopy /Y qt.conf					%DST%
xcopy /Y %QTDIR%\lib\QtCore*.dll			%DST%
xcopy /Y %QTDIR%\lib\QtGui*.dll				%DST%
xcopy /Y %QTDIR%\bin\QtSvg*.dll				%DST%
xcopy /Y %QTDIR%\bin\QtXml*.dll				%DST%
xcopy /Y %QTDIR%\plugins\imageformats\qgif*.dll		%PLUGINS%
xcopy /Y %QTDIR%\plugins\imageformats\qjpeg*.dll	%PLUGINS%
xcopy /Y %QTDIR%\plugins\imageformats\qmng*.dll		%PLUGINS%
xcopy /Y %QTDIR%\plugins\imageformats\qsvg*.dll		%PLUGINS%
xcopy /Y %QTDIR%\plugins\imageformats\qtiff*.dll	%PLUGINS%
xcopy /Y %QWTDIR%\lib\qwt5.dll				%DST%
xcopy /Y %QWTDIR%\lib\qwtd5.dll				%DST%
xcopy /Y %ZLIBDIR%\lib\zlib1.dll			%DST%
xcopy /Y devtools\bindump\Debug\bindump.exe		%DST%
xcopy /Y devtools\bytedistrpt\Debug\bytedistrpt.exe	%DST%
xcopy /Y ..\bin\resrpt.exe				%DST%
xcopy /Y debug_memtrace\Debug\debug_memtrace.dll	%DST%
xcopy /Y debug_memtrace\Debug\debug_memtrace.pdb	%DST%
xcopy /Y tilesplit\Debug\tilesplit.exe			%DST%
xcopy /Y mkxpm\Debug\mkxpm.exe				%DST%
xcopy /Y spwawlib\Debug\spwawlib.dll			%DST%
xcopy /Y spwawlib\Debug\spwawlib.pdb			%DST%
xcopy /Y spwreport\Debug\spwreport.exe			%DST%
xcopy /Y spwdecompose\Debug\spwdecompose.exe		%DST%
xcopy /Y spwreconstruct\Debug\spwreconstruct.exe	%DST%
xcopy /Y warcab\Debug\warcab.exe			%DST%
xcopy /Y warcab\warcab.pdb				%DST%
xcopy /Y warcab\resource\warcab.res			%DST%
copy /Y warcab\Release\warcab.exe			%DST%\warcab_release.exe
xcopy /Y DATA\demo.warcab				%SAVE%

goto DONE

:USAGE
echo Usage: deploy.bat type
echo Where: type   deployment type
echo.
echo Supported deployment types are: test, debug and release.
echo.
goto DONE

:DONE
pause
