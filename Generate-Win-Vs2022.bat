@echo off

set PREMAKE_PATH="Scripts\premake5.exe"
set PREMAKE_FILE_PATH="Scripts\Mirror-Examples.lua"
set SOLUTION_FILE_PATH="Examples.sln"

%PREMAKE_PATH% --file=%PREMAKE_FILE_PATH% vs2022
echo.

IF NOT %errorlevel%==0 (
	echo Encountered error!
	pause
	exit 1
)

pause
exit 0