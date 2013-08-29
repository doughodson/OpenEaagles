Building OpenEaagles using Qt:

Qt is a cross-platform gui library with a robust set of functionalities.  It comes with the QtCreator IDE, which can be used
to compile and link libraries and applications using a multiple variety of compilers.  

We have added the Qt .pro files as an easy way for users who wish to have a cross-platform consistent IDE in which to compile and run
OpenEaagles and it's examples.

In order to succesfully compile OpenEaagles using QtCreator, you must:

// ---
// INSTALLING
// ---
Download QtCreator (which comes with Qt)
http://qt-project.org/downloads

Version 5.0.1 and later are supported with OpenEaagles, although testing has been successful with 4.8.4 onward.

WINDOWS NOTES:
1) From Qt5.0.0 onward, Qt broke out the support for OpenGL 1.1.  This means that in order for legacy openGL code to run (such as these examples), you need
to make sure you install Qt with openGL legacy support (the download link will say 'opengl').

Once you have QtCreator installed, all you have to do is open the oe.pro file from within this directory... and voila!  

// ---
BUILDING
// ---
When you initially open the .pro file, Qt is going to ask you to setup the project.  By default Qt will build and install your applications in a separate
directory (shadow) from the original files.. usually something like /OpenEaagles/build/build-oe-Desktop_Qt_5_0_2_GCC_64bit-Debug.  This is a little confusing,
and can cause problems when your application are trying to find the library files.  To Fix this:
	- When Qt asks you to set up the project, accept the defaults (YES - this does enable shadow building, but it's easier this way, trust me).
	- After the project loads, click on the Project tab (left side of QtCreator), and under Build Settings -> General -> UNCHECK the shadow build checkbox. 
This will ensure that your library will build in the default location for any example to find it.

Note that OpenEaagles can be built with either Qt or any other supported compiler and IDE of your choice.  For example, you could build OpenEaagles using visual studio,
and then the examples using Qt.  Just make sure that Qt is using the same compiler as OpenEaagles.

// ---
SETTINGS
// ---
To change default settings (WINDOWS or LINUX)
There is a config.pri file located under OpenEaagles/build/qt that will define the location of OpenEaagles, OpenEaagles3rdParty, and any other 3rd party files you would
need.  All of the projects use this file, so you can change the default directories from here.
Also, on Windows - the MSVC_VER flag lets you determine how to name the output folder of your applications or libraries.  This DOES NOT change the compiler type.  You
may set this flag here to vs2008 or vs2010 to work out of the box.  This branch has not been tested under 2012 as of yet.


