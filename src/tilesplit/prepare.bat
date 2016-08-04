@echo off

if not "x%WARCAB_PREPARE%" == "xok" goto ERR_SETUP

%QTDIR%\bin\qmake -t vcapp -spec win32-msvc.net tilesplit.pro
if errorlevel 1 goto ERR_QMAKE
goto DONE

rem ----------------------------------------------------------------------

:ERR_SETUP
echo.
echo *** Error: build preparation not done! ***
echo.
goto DIE

:ERR_QMAKE
echo.
echo *** Error: failed to run qmake! ***
echo.
goto DIE

:DIE
:DONE
