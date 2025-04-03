@echo off

set PREMAKE_PATH="premake5.exe"
set PREMAKE_FILE_PATH="Mirror-Examples.lua"
set SOLUTION_FILE_PATH="Examples.sln"

IF EXIST "Scripts\" (
	set PREMAKE_PATH="Scripts\premake5.exe"
	set PREMAKE_FILE_PATH="Scripts\Mirror-Examples.lua"
)

%PREMAKE_PATH% --file=%PREMAKE_FILE_PATH% vs2022

IF NOT %errorlevel%==0 (
	echo Encountered error!
	pause
	exit 1
)

IF EXIST %SOLUTION_FILE_PATH% (
	tasklist /FI "IMAGENAME eq devenv.exe" | find /i "devenv.exe"
	
	rem Only open solution automatically for me
	IF Aaron==%username% (
		IF ERRORLEVEL 1 (
			rem Only open solution if no instances of visual studio are open
			echo starting %SOLUTION_FILE_PATH%
			START "" %SOLUTION_FILE_PATH%
			exit 0
		)
	)
)

pause