
#ifndef __oe_base_util_platform_api_msvc_H__
#define __oe_base_util_platform_api_msvc_H__

//------------------------------------------------------------------------------
// Visual Studio
//------------------------------------------------------------------------------

//
// The Winsock2.h header file internally includes core elements from the Windows.h
// header file, so there is not usually an #include line for the Windows.h header
// file in Winsock applications. If an #include line is needed for the Windows.h
// header file, this should be preceded with the #define WIN32_LEAN_AND_MEAN macro.
// For historical reasons, the Windows.h header defaults to including the Winsock.h
// header file for Windows Sockets 1.1. The declarations in the Winsock.h header
// file will conflict with the declarations in the Winsock2.h header file required
// by Windows Sockets 2.0. The WIN32_LEAN_AND_MEAN macro prevents the Winsock.h
// from being included by the Windows.h header.
//
//
// windows.h is a Windows-specific header file for the C/C++ programming language
// which contains declarations for all of the functions in the Windows API, all
// the common macros used by Windows programmers, and all the data types used
// by the various functions and subsystems.
//

// exclude rarely used header files
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

// and don't define min and max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <winsock2.h>

//
// defines multimedia functions - needed for usb joystick functions such as
// JOYCAPS and joyGetDevCaps() used by UsbJoystick
//
#include <Mmsystem.h>

//
// visual studio 2012 has a bug with some intrinsics
// we disable the one of concern
//
#if(_MSC_VER>=1700)   // VC11+
#include <cmath>
#pragma function(sqrt)
#endif

#endif
