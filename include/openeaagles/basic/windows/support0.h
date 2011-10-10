//------------------------------------------------------------------------------
// Windows Visual Studio unique stuff
//------------------------------------------------------------------------------
#ifndef _BASIC_WINDOWS_SUPPORT0_H_
#define _BASIC_WINDOWS_SUPPORT0_H_

#include <winsock2.h>
#include <ERRNO.H>

#if(_MSC_VER>=1600)   // VC10+
   #include <stdint.h>
#else
   typedef signed char      int8_t;
   typedef signed short     int16_t;
   typedef signed int       int32_t;
   typedef signed __int64   int64_t;
   typedef unsigned char    uint8_t;
   typedef unsigned short   uint16_t;
   typedef unsigned int     uint32_t;
   typedef unsigned __int64 uint64_t;
#endif

// Visual Studio intrinsics (MinGW doesn't support this)
#if !defined(__MINGW32__)
  #include <intrin.h>
#endif

// MinGW
#if defined(__MINGW32__)
  // defines FLT_MIN
  #include <float.h>
#endif

namespace Eaagles {

typedef LONGLONG  Integer64;
typedef ULONGLONG LCuint64;



} // End Eaagles namespace

#endif // _BASIC_WINDOWS_VSTUDIO_SUPPORT0_H_

