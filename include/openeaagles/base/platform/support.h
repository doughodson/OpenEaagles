
#ifndef __oe_platform_support_H__
#define __oe_platform_support_H__

//------------------------------------------------------------------------------
// Common support data and functions
//------------------------------------------------------------------------------

// framework configuration file
#include "openeaagles/config.h"

// ---
// Include OS/Processor unique stuff
// ---
#if defined(WIN32)
  #include "openeaagles/base/platform/windows/support0.h"
#else
  #include "openeaagles/base/platform/linux/support0.h"
#endif

#endif

