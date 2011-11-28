rem
rem delete old solution/workspace files
rem
rmdir /S /Q ..\codeblocks
rmdir /S /Q ..\vs2008
rmdir /S /Q ..\vs2010
rem
rem delete old compiled libraries
rem
rmdir /S /Q ..\..\lib\mingw
rmdir /S /Q ..\..\lib\vs2008
rmdir /S /Q ..\..\lib\vs2010
rem
rem create solution/workspace files
rem
premake4.exe codeblocks
premake4.exe vs2008
premake4.exe vs2010
