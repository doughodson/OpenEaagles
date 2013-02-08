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

doc: changeLogs, uml diagrams and doxygen configuration files

include: standard include path for all headers

lib: contains libraries after compilation

src: source code (.h/.cpp) files

To build:
---------

::Windows::
Load the visual studio solution file found in the build tree.
Select build options, and compile. This will generate linkable
libraries in the lib directory.

::Linux::
Edit setting in "setenv" file.
Load setting via "source setenv"
Review the makedefs file to ensure paths and prerequisites are met.
Enter src directory and run "make"

