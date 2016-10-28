@echo off

rem
rem This is the build preparation batch file for the spwproject.
rem

set CWD=%CD%

rem Obtain the Qt directory
if not exist qtdir.bat goto ERR_NOQT
call qtdir.bat

rem Obtain the Qwt directory
if not exist qwtdir.bat goto ERR_NOQWT
call qwtdir.bat

echo ^+-------------------------------------^+
echo ^|   Preparing the spwproject builds   ^|
echo ^+-------------------------------------^+
echo.
echo     QTDIR : %QTDIR%
echo     QWTDIR: %QWTDIR%
echo     CWD   : %CWD%
echo.

if "x%QTENVSETUP%" == "xok" goto ENV_OK
if not exist %QTDIR%\bin\qtvars.bat goto ERR_QT
call %QTDIR%\bin\qtvars.bat
if not "x%QTENVSETUP%" == "xok" goto ERR_ENV

:ENV_OK

set QMAKEPATH=%CWD%;%QMAKEPATH%
set WARCAB_PREPARE=ok

echo --^> mkxpm
cd mkxpm
call prepare.bat
cd ..

echo --^> tilesplit
cd tilesplit
call prepare.bat
cd ..

echo --^> warcab
cd warcab
call prepare.bat
cd ..

goto DONE

rem ----------------------------------------------------------------------

:ERR_NOQT
echo.
echo *** Error: Qt directory not configured: please create qtdir.bat! ***
echo.
goto DIE

:ERR_NOQWT
echo.
echo *** Error: Qwt directory not configured: please create qwtdir.bat! ***
echo.
goto DIE

:ERR_QT
echo.
echo *** Error: failed to locate Qt toolchain! ***
echo.
goto DIE

:ERR_ENV
echo.
echo *** Error: failed to setup Qt development environment! ***
echo.
goto DIE

:DIE
:DONE
echo.
echo Done. Press any key to quit.
pause >nul
