==================
OpenEaagles ReadMe
==================

-------------------------------------------------------
The most current version of OpenEaagles can be found
at www.OpenEaagles.org.
-------------------------------------------------------


Directories
-----------

build: using premake for creating Visual Studio/C++ projects and solutions 
(See premake project for more information)

doc: doxygen configuration files (see doxygen project for more info)

include: standard include path for all headers

lib: contains framework libraries after compilation

src: source code (.cpp) files


To build:
---------

For Windows, just load the visual studio solution file found in the build tree.
Select build options, and compile. This will generate linkable
libraries.

For Linux, enter the src directory and type "make". Review the
makedefs file to ensure paths and prerequisites are met. After compile,
type "make install" to install headers and libraries to:

/usr/local/include/openeaagles
/usr/local/lib/openeaagles

