
include makedefs

# Library      : Requires
# ------------------------------------------------------------------------
# base         : -
# graphics     : freeglut, FTGL, freetype
# dafif        : -
# gui          :
# instruments  : -
# iodevice     : -
# linearsystem : -
# models       : JSBSim
# networks/dis : -
# networks/hla : HLA RTI
# maps         : -
# otw          : CIGICL 3.x
# recorder     : Google protocol buffers
# simulation   : -
# terrain      : -
#
PROJECTS = base
PROJECTS += dafif
PROJECTS += iodevice
PROJECTS += linearsystem 
PROJECTS += simulation terrain
PROJECTS += models
PROJECTS += maps
PROJECTS += recorder

#
# OpenGL-based graphics libraries
#
PROJECTS += graphics
PROJECTS += instruments

#
# GUI interface libraries
#
PROJECTS += "gui/glut"

#
# Interoperability interfaces
#
PROJECTS += networks/dis
#PROJECTS += networks/hla

# Out-the-Window interface classes to talk to visual systems
# - CIGI Class Library 3.x interface
# - download and install cigicl
PROJECTS += otw

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

