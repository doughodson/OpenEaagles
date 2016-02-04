
include makedefs

# Library      : Requires
# ------------------------------------------------------------------------
# basic        : -
# graphics     : freeglut, FTGL, freetype
# dafif        : -
# dis          : -
# gui          :
# hla          : HLA RTI
# instruments  : -
# iodevice     : -
# linearsystem : -
# models       : JSBSim
# maps         : -
# otw          : CIGICL 3.x
# recorder     : Google protocol buffers
# simulation   : -
# terrain      : -
#
PROJECTS = basic
PROJECTS += graphics instruments
PROJECTS += dafif
PROJECTS += iodevice
PROJECTS += linearsystem 
PROJECTS += simulation terrain
PROJECTS += dis
PROJECTS += models
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

