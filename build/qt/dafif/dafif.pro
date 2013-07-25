# OpenEaagles dafif library profile

# Define the Visual studio compiler version we wish to use (Windows only)
MSVC_VER = vs2010

# Root directory
OE_ROOT=$$PWD/../../../
# OpenEaagles3rdParty Root
OE_3RD_PARTY_ROOT=$$PWD/../../../../OpenEaagles3rdParty

# Static library configuration
TEMPLATE = lib
CONFIG += static

# Store our objects in a temporary location
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
RCC_DIR = ./tmp/rcc

# destination and working directories
win32: DESTDIR = $${OE_ROOT}/lib/$${MSVC_VER}/
else:unix:!macx:!symbian: DESTDIR = $${OE_ROOT}/lib/linux

# Target names
win32:CONFIG(release, debug|release): TARGET = oeDafif
else:win32:CONFIG(debug, debug|release): TARGET = oeDafif_d
else:unix:!macx:!symbian: TARGET = oeDafif

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include                      \
   $${OE_3RD_PARTY_ROOT}/include

# Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/dafif/*.h

# Source files
SOURCES += \
   $${OE_ROOT}/src/dafif/*.cpp
