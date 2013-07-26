# OpenEaagles ioDevice library profile

# Define the Visual studio compiler version we wish to use (Windows only)
include(../config.pri)

# Target names
win32:CONFIG(release, debug|release): TARGET = oeIoDevice
else:win32:CONFIG(debug, debug|release): TARGET = oeIoDevice_d
else:unix:!macx:!symbian: TARGET = oeIoDevice

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include                      \
   $${OE_3RD_PARTY_ROOT}/include

# Common Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/ioDevice/*.h

# Common Source files
SOURCES += \
   $${OE_ROOT}/src/ioDevice/*.cpp

# OS specific Header and source files
win32:SOURCES += $${OE_ROOT}/src/ioDevice/windows/*.cpp
else:unix:!macx:!symbian: SOURCES += $${OE_ROOT}/src/ioDevice/linux/*.cpp

