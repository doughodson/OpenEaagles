//------------------------------------------------------------------------------
// Windows Visual Studio unique stuff
//------------------------------------------------------------------------------
#ifndef _BASIC_WINDOWS_SUPPORT0_H_
#define _BASIC_WINDOWS_SUPPORT0_H_

#include <winsock2.h>
#include <ERRNO.H>

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

