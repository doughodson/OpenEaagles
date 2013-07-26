# OpenEaagles BasicGL library profile

# Define the Visual studio compiler version we wish to use (Windows only)
include(../config.pri)

DEFINES += FTGL_LIBRARY_STATIC

# Target names
win32:CONFIG(release, debug|release): TARGET = oeBasicGL
else:win32:CONFIG(debug, debug|release): TARGET = oeBasicGL_d
else:unix:!macx:!symbian: TARGET = oeBasicGL

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include                      \
   $${OE_3RD_PARTY_ROOT}/include            \
   $${OE_3RD_PARTY_ROOT}/include/freetype2

# Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/basicGL/*.h                    

# Source files
SOURCES += \
   $${OE_ROOT}/src/basicGL/*.cpp                  
