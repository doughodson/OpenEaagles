# OpenEaagles recorder library profile

# Define the Visual studio compiler version we wish to use (Windows only)
include(../config.pri)

# Target names
win32:CONFIG(release, debug|release): TARGET = oeRecorder
else:win32:CONFIG(debug, debug|release): TARGET = oeRecorder_d
else:unix:!macx:!symbian: TARGET = oeRecorder

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include                      \
   $${OE_3RD_PARTY_ROOT}/include

# Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/recorder/*.h

# Source files
SOURCES += \
   $${OE_ROOT}/src/recorder/*.cpp
