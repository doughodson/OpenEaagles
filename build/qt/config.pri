# Configuration file - used for setting library locations

MSVC_VER = vs2010
#MSVC_VER = vs2012

# Change these directories as needed to find the OpenEaagles and OpenEaagles
# 3rd party folders.
# Root directory
OE_ROOT=$$PWD/../../
# OpenEaagles3rdParty Root
OE_3RD_PARTY_ROOT=$$PWD/../../../OpenEaagles3rdParty

# freetype is in different places for linux vs windows
win32: FREETYPE2_INC_DIR = $${OE_3RD_PARTY_ROOT}
else:unix:!macx:!symbian: FREETYPE2_INC_DIR = /usr/

# destination and working directories
win32: DESTDIR = $${OE_ROOT}/lib/$${MSVC_VER}/
else:unix:!macx:!symbian: DESTDIR = $${OE_ROOT}/lib/linux

DEFINES += _CRT_SECURE_NO_WARNINGS

# Store our objects in a temporary location
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
RCC_DIR = ./tmp/rcc

# Static library configuration
TEMPLATE = lib
CONFIG += static
