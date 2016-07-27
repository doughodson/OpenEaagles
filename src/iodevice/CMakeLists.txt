
file(GLOB sources "*.cpp")

if (WIN32)
  set (sources ${sources} platform/UsbJoystick_msvc.cpp)
else ()
  set (sources ${sources} platform/UsbJoystick_linux.cpp)
endif ()

file(GLOB_RECURSE headers "../../include/openeaagles/iodevice/*.h")

add_library (iodevice ${sources} ${headers})
set_target_properties(iodevice PROPERTIES PREFIX "oe")
set_target_properties (iodevice PROPERTIES DEBUG_POSTFIX _d)
