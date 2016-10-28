@echo off

cd resource
call mkres.bat
cd ..

..\..\bin\mkbuildinfo.exe BUILD src\build.cpp

pause
