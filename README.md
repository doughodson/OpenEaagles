
OpenEaagles
===========

The most current version of OpenEaagles can be found at [www.OpenEaagles.org][1].


Directories
-----------

* build: using premake for creating Visual Studio/C++ projects and solutions (See premake project for more information)

* doc: changeLogs, uml diagrams and doxygen configuration files

* include: standard include path for all headers

* lib: contains libraries after compilation

* src: source code (.h/.cpp) files

To Build
---------

#### Windows
1. Load the visual studio solution file found in the build tree.
2. Select build options, and compile.
3. This will generate linkable libraries in the lib directory.

#### Linux
1. Edit setting in "setenv" file.
2. Load setting via "source setenv".
3. Review the makedefs file to ensure paths and prerequisites are met.
4. Enter src directory and run "make"

[1]: http://www.OpenEaagles.org

