
#ifndef __oe_base_util_atomics_linux_H__
#define __oe_base_util_atomics_linux_H__

// ---
// Simple semaphore spinlock and unlock functions: 
//    lock(long int& s)      -- gets the semaphore w/spinlock wait
//    unlock(long int& s)    -- frees the semaphore
//
//    where 's' is the semaphore that must be initialized to zero.
//
// Linux version
// ---

namespace oe {
namespace base {

inline void lock(long int& semaphore)
{

#if 1

   auto sem = static_cast<long int*>(&semaphore);
   while (__sync_lock_test_and_set(sem, 1));

#else
   //long* semaphore = (long*) &sem;
   // Try to get the semaphore (i.e., set it to one), but make
   // sure that it was free (i.e., previously set to zero).
   // Otherwise wait (spin) until it is free.
   __asm__ __volatile__ (
      "1:\n\t"
          "movl $1,%%eax\n\t"
          "lock xchgl %%eax,%0\n\t"
          "testl %%eax,%%eax\n\t"
          "je   3f\n\t"
      "2:\n\t"
          "pause\n\t"
          "cmpl $0,%0\n\t"
          "je   1b\n\t"
          "jmp  2b\n\t"
      "3:\n\t"
          :"+m" (semaphore)
          :
          : "memory", "%eax"
   );
#endif

}

inline void unlock(long int& semaphore)
{
#if 1

   auto sem = static_cast<long int*>(&semaphore);
   __sync_lock_release(sem);

#else

   //long* semaphore = (long*) &sem;
   // free the semaphore (i.e., just set it to zero)
   __asm__ __volatile__ (
       "movl $0,%%eax\n\t"
       "lock xchgl %%eax,%0\n\t"
       :"+m" (semaphore)
       :
       : "memory", "%eax"
   );
#endif

}

}
}

#endif
