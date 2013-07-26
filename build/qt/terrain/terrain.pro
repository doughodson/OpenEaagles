# OpenEaagles terrain library profile

# Define the Visual studio compiler version we wish to use (Windows only)
include(../config.pri)

# Target names
win32:CONFIG(release, debug|release): TARGET = oeTerrain
else:win32:CONFIG(debug, debug|release): TARGET = oeTerrain_d
else:unix:!macx:!symbian: TARGET = oeTerrain

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include                      \
   $${OE_3RD_PARTY_ROOT}/include

# Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/terrain/*.h          \
   $${OE_ROOT}/include/openeaagles/terrain/ded/*.h      \
   $${OE_ROOT}/include/openeaagles/terrain/dted/*.h     \
   $${OE_ROOT}/include/openeaagles/terrain/srtm/*.h

# Source files
SOURCES += \
   $${OE_ROOT}/src/terrain/*.cpp        \
   $${OE_ROOT}/src/terrain/ded/*.cpp    \
   $${OE_ROOT}/src/terrain/dted/*.cpp   \
   $${OE_ROOT}/src/terrain/srtm/*.cpp
