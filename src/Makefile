
include makedefs

# Library      : Requires
# ------------------------------------------------------------------------
# basic        : -
# basicGL      : freeglut, FTGL, freetype
# dafif        : -
# dis          : -
# dynamics     : JSBSim
# gui          :
# hla          : HLA RTI
# instruments  : -
# ioDevice     : -
# linearSys    : -
# maps         : -
# otw          : CIGICL 3.x
# recorder     : Google protocol buffers
# sensors      : -
# simulation   : -
# terrain      : -
#
PROJECTS = basic
PROJECTS += basicGL instruments
PROJECTS += dafif
PROJECTS += ioDevice
PROJECTS += linearSys 
PROJECTS += simulation sensors terrain
PROJECTS += dis
PROJECTS += dynamics
PROJECTS += maps
PROJECTS += recorder

#
# GUI interface libraries
#
PROJECTS += "gui/glut"

# Out-the-Window interface classes to talk to visual systems
# - CIGI Class Library 3.x interface
# - download and install cigicl
PROJECTS += otw

#
# HLA interface library
#
#PROJECTS += "hla"

#
# Rules
#
all:
	@echo ${OE_ROOT}
	for subdir in $(PROJECTS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

install: uninstall
	-cp -r ../include/openeaagles /usr/local/include/openeaagles
	-cp -r ../lib/linux /usr/local/lib/openeaagles

uninstall:
	-rm -rf /usr/local/include/openeaagles
	-rm -rf /usr/local/lib/openeaagles

clean:
	-rm -f *.o
	for subdir in $(PROJECTS); do \
	    echo $@ in $$subdir; \
	    (cd $$subdir && $(MAKE) $@) || exit 1; \
	done

