del *.sln

del /Q /F /S /AH bin
rmdir /Q /S bin

del /Q /F /S /AH .vs
rmdir /Q /S ".vs"

pushd Mirror
call Cleanup.bat
popd

pushd Projects
call Cleanup.bat
popd
