# OpenEaagles Basic library profile

# Define the Visual studio compiler version we wish to use (Windows only)
include(../config.pri)

# Target names
win32:CONFIG(release, debug|release): TARGET = oeBasic
else:win32:CONFIG(debug, debug|release): TARGET = oeBasic_d
else:unix:!macx:!symbian: TARGET = oeBasic

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include           \
   $${OE_3RD_PARTY_ROOT}/include

# Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/basic/*.h                    \
   $${OE_ROOT}/include/openeaagles/basic/distributions/*.h      \
   $${OE_ROOT}/include/openeaagles/basic/osg/*.*                \
   $${OE_ROOT}/include/openeaagles/basic/ubf/*.h                \
   $${OE_ROOT}/include/openeaagles/basic/units\*.h

# Source files
SOURCES += \
   $${OE_ROOT}/src/basic/*.cpp                  \
   $${OE_ROOT}/src/basic/distributions/*.cpp    \
   $${OE_ROOT}/src/basic/osg/*.cpp              \
   $${OE_ROOT}/src/basic/ubf/*.cpp              \
   $${OE_ROOT}/src/basic/units/*.cpp            \

