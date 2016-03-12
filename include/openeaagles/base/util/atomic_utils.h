
#ifndef __oe_base_util_atomic_utils_H__
#define __oe_base_util_atomic_utils_H__

namespace oe {
namespace base {

// ---
// Simple semaphore spinlock and unlock functions:
//    lock(long& s)   -- locks the semaphore w/spinlock wait
//    unlock(long& s) -- frees the semaphore
// where 's' is the semaphore that must be initialized to zero.
// ---
#if defined(WIN32)
  #if defined(__MINGW32__)
    // MinGW lock
    #include "openeaagles/base/util/platform/mingw_lock.h"
  #else
    // Visual Studio lock
    #include "openeaagles/base/util/platform/msvc_lock.h"
  #endif
#else
  // Linux GCC lock
  #include "openeaagles/base/util/platform/linux_lock.h"
#endif

}
}

#endif

