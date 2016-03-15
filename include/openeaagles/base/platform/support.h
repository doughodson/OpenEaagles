//------------------------------------------------------------------------------
// Common support data and functions
//
// Note: The "Lc" prefix stands for "Link class" in honor of Ed Link, who
//       invented the "Link Blue Box", which was the first flight simulator.
//------------------------------------------------------------------------------
#ifndef __oe_platform_support_H__
#define __oe_platform_support_H__

// ---
// Framework configuration file
// ---
#include "openeaagles/config.h"

// ---
// Common header files
// ---

#include <typeinfo>
#include <iostream>
#include <sys/types.h>
#include <cmath>
#include <cstring> // macros use std::strcmp

// ---
// Include OS/Processor unique stuff
// ---
#if defined(WIN32)
  #include "openeaagles/base/platform/windows/support0.h"
#else
  #include "openeaagles/base/platform/linux/support0.h"
#endif


#endif

