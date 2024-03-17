rem pushd ..
cd ../
rem del *.dll
rem del *.vcxproj

del *.sln

del /Q /F /S /AH bin
rmdir /Q /S bin

del /Q /F /S /AH .vs
rmdir /Q /S ".vs"

pushd App
call Cleanup.bat
popd

pushd cJSON
call Cleanup.bat
popd

pushd Mirror
call Cleanup.bat
popd
