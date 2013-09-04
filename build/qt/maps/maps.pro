# OpenEaagles maps library profile

# Define the Visual studio compiler version we wish to use (Windows only)
include(../config.pri)

# Target names
win32:CONFIG(release, debug|release): TARGET = oeMaps
else:win32:CONFIG(debug, debug|release): TARGET = oeMaps_d
else:unix:!macx:!symbian: TARGET = oeMaps

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include                      \
   $${OE_3RD_PARTY_ROOT}/include

# Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/maps/*.h         \
   $${OE_ROOT}/include/openeaagles/maps/rpfMap/*.h  \
   $${OE_ROOT}/include/openeaagles/maps/vpfMap/*.h

# Source files
SOURCES += \
   $${OE_ROOT}/src/maps/*.cpp           \
   $${OE_ROOT}/src/maps/rpfMap/*.cpp    \
   $${OE_ROOT}/src/maps/vpfMap/*.cpp
