# OpenEaagles vehicles library profile

# Define the Visual studio compiler version we wish to use (Windows only)
include(../config.pri)

# Target names
win32:CONFIG(release, debug|release): TARGET = oeVehicles
else:win32:CONFIG(debug, debug|release): TARGET = oeVehicles_d
else:unix:!macx:!symbian: TARGET = oeVehicles

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include                      \
   $${OE_3RD_PARTY_ROOT}/include            \
   $${OE_3RD_PARTY_ROOT}/include/JSBSim

# Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/vehicles/*.h

# Source files
SOURCES += \
   $${OE_ROOT}/src/vehicles/*.cpp
