
#ifndef __oe_base_util_platform_api_msvc_H__
#define __oe_base_util_platform_api_msvc_H__

#include <winsock2.h>

// visual studio intrinsics
#include <intrin.h>

// visual studio 2012 has a bug with some intrinsics
// For now, we will not use a few of them
#if(_MSC_VER>=1700)   // VC11+
#include <cmath>
#pragma function(sqrt)
#endif

#endif
