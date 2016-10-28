@echo off

set RESCLEAN=0

if not "x%WARCAB_PREPARE%" == "xok" goto ERR_SETUP
if exist resource\resource.h goto SKIPRES

set RESCLEAN=1
if not exist resource\resource.h echo. >>resource\resource.h
if not exist resource\resource.c echo. >>resource\resource.c

:SKIPRES
%QTDIR%\bin\qmake -t vcapp -spec win32-msvc.net QwtBase="%QWTDIR%" warcab.pro
if errorlevel 1 goto ERR_QMAKE
goto DONE

rem ----------------------------------------------------------------------

:ERR_SETUP
echo.
echo *** Error: build preparation not done! ***
echo.
goto DONE

:ERR_QMAKE
echo.
echo *** Error: failed to run qmake! ***
echo.
goto DONE

:DIE
:DONE

if not "x%RESCLEAN%" == "x1" goto BYE
del resource\resource.h >nul
del resource\resource.c >nul

:BYE
