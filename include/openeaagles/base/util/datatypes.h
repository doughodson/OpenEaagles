
#ifndef __oe_base_util_datatypes_H__
#define __oe_base_util_datatypes_H__

#if defined(WIN32)
  #if defined(__MINGW32__)
    // MinGW
    #include "openeaagles/base/util/platform/datatypes_mingw.h"
  #else
    // Visual Studio
    #include "openeaagles/base/util/platform/datatypes_msvc.h"
  #endif
#else
  // Linux GCC
  #include "openeaagles/base/util/platform/datatypes_linux.h"
#endif

#endif

