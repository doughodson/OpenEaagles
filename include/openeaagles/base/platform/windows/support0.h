//------------------------------------------------------------------------------
// Windows Visual Studio unique stuff
//------------------------------------------------------------------------------
#ifndef __oe_platform_windows_support0_H__
#define __oe_platform_windows_support0_H__

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

// VS2012 has a bug with some intrinsics
// For now, we will not use a few of them
#if(_MSC_VER>=1700)   // VC11+
#pragma function(sqrt)
#endif

// MinGW
#if defined(__MINGW32__)
  // defines FLT_MIN and DBL_MAX
  #include <cfloat>
#endif

namespace oe {

typedef LONGLONG  Integer64;
typedef ULONGLONG LCuint64;

}

#endif

