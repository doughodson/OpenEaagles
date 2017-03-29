rem
rem set premake location
rem
@SET Premake=..\..\..\OpenEaagles3rdParty\bin\premake5-alpha11.exe
rem
rem delete old solution/workspace files
rem
rmdir /S /Q ..\vs2013
rmdir /S /Q ..\vs2015
rmdir /S /Q ..\vs2017
rmdir /S /Q ..\obj
rem
rem delete old compiled libraries
rem
del ..\..\lib\*.lib
rem
rem create new solution/workspace files
rem
%Premake% --os=windows --file=premake5.lua vs2013
%Premake% --os=windows --file=premake5.lua vs2015
%Premake% --os=windows --file=premake5.lua vs2017

