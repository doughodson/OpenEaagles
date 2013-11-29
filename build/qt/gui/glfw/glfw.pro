# OpenEaagles GLFW3 library profile

# Define the Visual studio compiler version we wish to use (Windows only)
include(../../config.pri)

# Target names
win32:CONFIG(release, debug|release): TARGET = oeGlfw
else:win32:CONFIG(debug, debug|release): TARGET = oeGlfw_d
else:unix:!macx:!symbian: TARGET = oeGlfw

# Include paths
INCLUDEPATH += \
   $${OE_ROOT}/include                      \
   $${OE_3RD_PARTY_ROOT}/include

# Header files
HEADERS += \
   $${OE_ROOT}/include/openeaagles/gui/glfw/*.h

# Source files
SOURCES += \
   $${OE_ROOT}/src/gui/glfw/*.cpp
