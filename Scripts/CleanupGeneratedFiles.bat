cd ../

del *.sln

del /Q /F /S /AH bin
rmdir /Q /S bin

del /Q /F /S /AH .vs
rmdir /Q /S ".vs"

cd Examples
del *.vcxproj
del *.user

exit 0