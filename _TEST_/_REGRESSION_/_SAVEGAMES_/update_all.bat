@echo off

echo ^+--------------------------^+
echo ^| Starting baseline update ^|
echo ^+--------------------------^+
echo.

for /D %%d in (set*) do (
	echo Performing baseline update in %%d...
	pushd %%d >nul 2>nul
	call update.bat
	popd >nul 2>nul
)
echo.

pause
