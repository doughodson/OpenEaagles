
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
SUBDIRS = basic
SUBDIRS += basicGL 
SUBDIRS += instruments
SUBDIRS += dafif
SUBDIRS += ioDevice
SUBDIRS += linearSys 
SUBDIRS += simulation sensors terrain
SUBDIRS += dis
SUBDIRS += dynamics
SUBDIRS += maps
SUBDIRS += recorder

#
# GUI interface libraries
#
SUBDIRS += "gui/glut"

# Out-the-Window interface classes to talk to visual systems
# - CIGI Class Library 3.x interface
# - download and install cigicl
SUBDIRS += otw

#
# HLA interface library
#
#SUBDIRS += "hla"

#
# Rules
#

all: $(SUBDIRS)

install: uninstall
	-cp -r ../include/openeaagles /usr/local/include/openeaagles
	-cp -r ../lib/linux /usr/local/lib/openeaagles

uninstall:
	-rm -rf /usr/local/include/openeaagles
	-rm -rf /usr/local/lib/openeaagles

clean: $(SUBDIRS)
	-rm -f *.o

$(SUBDIRS)::
	$(MAKE) -C $@ $(MAKECMDGOALS)