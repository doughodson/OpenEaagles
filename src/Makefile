
include makedefs

# Library         : Requires
# ------------------------------------------------------------------------
# base            : -
# dafif           : -
# graphics        : OpenGL, FTGL, freetype
# gui_glut        : freeglut
# instruments     : -
# interop         : -
# interop_dis     : -
# interop_hla     : HLA RTI
# interop_rprfom  : HLA RTI
# iodevice        : -
# linearsystem    : -
# map_rpf         : -
# models          : JSBSim
# otw             : CIGICL 3.x
# recorder        : Google protocol buffers
# simulation      : -
# terrain         : -
#
PROJECTS = base
PROJECTS += dafif
PROJECTS += iodevice
PROJECTS += linearsystem 
PROJECTS += simulation terrain
PROJECTS += models
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
PROJECTS += interop/common
PROJECTS += interop/dis
#PROJECTS += interop/hla
#PROJECTS += interop/rprfom

# Out-the-Window interface classes to talk to visual systems
# - CIGI Class Library 3.x interface
# - download and install cigicl
PROJECTS += otw

#
# Map format reader libraries
#
PROJECTS += map/rpf

.PHONY: all clean $(PROJECTS)

#
# Rules
#
all: $(PROJECTS)

$(PROJECTS):
	$(MAKE) -C $@

install: uninstall
	-cp -r ../include/openeaagles /usr/local/include/openeaagles
	-cp -r ../lib/linux /usr/local/lib/openeaagles

uninstall:
	-rm -rf /usr/local/include/openeaagles
	-rm -rf /usr/local/lib/openeaagles

clean:
	-for d in $(PROJECTS); do (cd $$d; $(MAKE) clean ); done

