#  Try to find FTGL library and include files
#  FTGL_FOUND - System has libftgl
#  FTGL_INCLUDE_DIRS - The libftgl include directories
#  FTGL_LIBRARIES - The libraries needed to use ftgl
#  FTGL_DEFINITIONS - Compiler switches required for using ftgl


if (WIN32)
  find_path (FTGL_INCLUDE_DIR FTGL/ftgl.h
		$ENV{PROGRAMFILES}/FTGL/include		
		DOC "The directory where FTGL/ftgl.h resides")
	FIND_LIBRARY( FTGL_LIBRARY
		NAMES ftgl FTGL ftgl32 ftgl32s
		PATHS
		$ENV{PROGRAMFILES}/FTGL/lib
		DOC "The FTGL library")
else (WIN32)
  find_path (FTGL_INCLUDE_DIR FTGL/ftgl.h
    hints /usr/include /usr/local/include /sw/include /opt/local/include
		doc "The directory where FTGL/ftgl.h resides")
	find_library (FTGL_LIBRARY
		names FTGL ftgl
    hints 
    PATH_SUFFIXES lib64 lib 
    PATHS
    #hints /usr/lib /usr/lib64 /usr/local/lib /usr/local/lib64 /sw/lib
    #      /opt/local/lib
		doc "The FTGL library")
endif (WIN32)

set (FTGL_LIBRARIES ${FTGL_LIBRARY})
set (FTGL_INCLUDE_DIRS ${FTGL_INCLUDE_DIR})

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (FTGL  DEFAULT_MSG
                                   FTGL_LIBRARY FTGL_INCLUDE_DIR)

mark_as_advanced (FTGL_INCLUDE_DIR FTGL_LIBRARY )
