rem
rem set premake location
rem
@SET Premake=..\..\..\OpenEaagles3rdParty\bin\premake4.exe
rem
rem delete old solution/workspace files
rem
rmdir /S /Q ..\codeblocks
rmdir /S /Q ..\codelite
rmdir /S /Q ..\vs2008
rmdir /S /Q ..\vs2010
rmdir /S /Q ..\vs2012
rmdir /S /Q ..\vs2013
rem
rem delete old compiled libraries
rem
rmdir /S /Q ..\..\lib\mingw
rmdir /S /Q ..\..\lib\vs2008
rmdir /S /Q ..\..\lib\vs2010
rmdir /S /Q ..\..\lib\vs2012
rmdir /S /Q ..\..\lib\vs2013
rem
rem create new solution/workspace files
rem
%Premake% vs2008
%Premake% vs2010
%Premake% vs2012
%Premake% vs2013
