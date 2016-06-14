rem
rem set premake location
rem
@SET Premake=..\..\..\OpenEaagles3rdParty\bin\premake5-alpha9.exe
rem
rem delete old solution/workspace files
rem
rmdir /S /Q ..\codeblocks
rmdir /S /Q ..\codelite
rmdir /S /Q ..\vs2013
rmdir /S /Q ..\vs2015
rem
rem delete old compiled libraries
rem
rmdir /S /Q ..\..\lib\mingw
rmdir /S /Q ..\..\lib\vs2013
rmdir /S /Q ..\..\lib\vs2015
rem
rem create new solution/workspace files
rem
%Premake% vs2013
%Premake% vs2015

