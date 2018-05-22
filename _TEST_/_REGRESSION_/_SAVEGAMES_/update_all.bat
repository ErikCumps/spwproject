@echo off

echo ^+--------------------------------------------^+
echo ^| Performing regression test baseline update ^|
echo ^+--------------------------------------------^+
echo.

for /D %%d in (set*) do (
	echo Updating baseline in %%d...
	pushd %%d >nul 2>nul
	call update.bat
	popd >nul 2>nul
)
echo.

pause
