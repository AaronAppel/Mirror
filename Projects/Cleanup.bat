del *.vcxproj
del *.user

pushd cJSON
call Cleanup.bat
popd

pushd Examples
call Cleanup.bat
popd

pushd Testing
call Cleanup.bat
popd
