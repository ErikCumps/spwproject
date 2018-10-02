@echo off

echo ^+------------------------------------^+
echo ^| Performing regression test cleanup ^|
echo ^+------------------------------------^+
echo.

for /D %%d in (set*) do (
	echo Cleaning test data in %%d...
	pushd %%d >nul 2>nul
	call cleanup.bat
	popd >nul 2>nul
)
echo.

pause
