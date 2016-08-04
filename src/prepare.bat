@echo off

rem
rem This is the build preparation batch file for the spwproject.
rem

set CWD=%CD%

rem Obtain the Qt directory
if not exist qtdir.bat goto ERR_NOQT
call qtdir.bat

echo ^+-------------------------------------^+
echo ^|   Preparing the spwproject builds   ^|
echo ^+-------------------------------------^+
echo.
echo     QTDIR: %QTDIR%
echo     CWD  : %CWD%
echo.

if "x%QTENVSETUP%" == "xok" goto ENV_OK
if not exist %QTDIR%\bin\qtvars.bat goto ERR_QT
call %QTDIR%\bin\qtvars.bat
if not "x%QTENVSETUP%" == "xok" goto ERR_ENV

set QMAKEPATH=%CWD%;%QMAKEPATH%
set WARCAB_PREPARE=ok

:ENV_OK

echo --^> mkxpm
cd mkxpm
call prepare.bat
cd ..

echo --^> tilesplit
cd tilesplit
call prepare.bat
cd ..

goto DONE

rem ----------------------------------------------------------------------

:ERR_NOQT
echo.
echo *** Error: Qt directory not configured: please create qtdir.bat! ***
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
