# OpenEaagles instrument library profile

# Define the Visual studio compiler version we wish to use (Windows only)
include(../config.pri)

# Target names
win32:CONFIG(release, debug|release): TARGET = oeInstruments
else:win32:CONFIG(debug, debug|release): TARGET = oeInstruments_d
else:unix:!macx:!symbian: TARGET = oeInstruments

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include                      \
   $${OE_3RD_PARTY_ROOT}/include

# Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/instruments/*.h              \
   $${OE_ROOT}/include/openeaagles/instruments/adi/*.h          \
   $${OE_ROOT}/include/openeaagles/instruments/buttons/*.h      \
   $${OE_ROOT}/include/openeaagles/instruments/dials/*.h        \
   $${OE_ROOT}/include/openeaagles/instruments/eadi3D/*.h       \
   $${OE_ROOT}/include/openeaagles/instruments/eng/*.h          \
   $${OE_ROOT}/include/openeaagles/instruments/gauges/*.h       \
   $${OE_ROOT}/include/openeaagles/instruments/landingGear/*.h  \
   $${OE_ROOT}/include/openeaagles/instruments/maps/*.h

# Source files
SOURCES += \
   $${OE_ROOT}/src/instruments/*.cpp              \
   $${OE_ROOT}/src/instruments/adi/*.cpp          \
   $${OE_ROOT}/src/instruments/buttons/*.cpp      \
   $${OE_ROOT}/src/instruments/dials/*.cpp        \
   $${OE_ROOT}/src/instruments/eadi3D/*.cpp       \
   $${OE_ROOT}/src/instruments/eng/*.cpp          \
   $${OE_ROOT}/src/instruments/gauges/*.cpp       \
   $${OE_ROOT}/src/instruments/landingGear/*.cpp  \
   $${OE_ROOT}/src/instruments/maps/*.cpp
