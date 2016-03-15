
#ifndef __oe_platform_support_H__
#define __oe_platform_support_H__

// framework configuration file
#include "openeaagles/config.h"

// ---
// Include OS/Processor unique stuff
// ---
#if defined(WIN32)
   // MinGW
   #if defined(__MINGW32__)
     // defines FLT_MIN and DBL_MAX
     #include <cfloat>
   #endif
#else
   #include <values.h>       // FLT_MIN
#endif

#include "openeaagles/base/util/platform_api.h"

#endif

