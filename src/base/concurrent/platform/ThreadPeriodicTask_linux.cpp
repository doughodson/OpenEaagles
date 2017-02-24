
#include "openeaagles/base/concurrent/ThreadPeriodicTask.hpp"

#include "openeaagles/base/Component.hpp"
#include "openeaagles/base/util/math_utils.hpp"
#include "openeaagles/base/util/system.hpp"

#include <signal.h>
#include <iostream>

namespace oe {
namespace base {

// max number of processors we'll allow
static const unsigned int MAX_CPUS = 32;

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long ThreadPeriodicTask::mainThreadFunc()
{
   if (getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "Thread(" << this << ")::mainLoopFunc(): Parent = " << getParent() << std::endl;
      std::cout << "Thread(" << this << ")::mainLoopFunc(): Starting main loop ..." << std::endl;
   }

   // Delta time
   const double dt = 1.0/static_cast<double>(getRate());
   int sec0 = static_cast<int>(dt);
   int nsec0 = static_cast<int>((dt - static_cast<double>(sec0)) * 1000000000.0);

   // Reference time
   struct timespec tp;
   clock_gettime(CLOCK_REALTIME, &tp);

   // Mutex
   pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_mutex_lock(&mutex);

   // ---
   // Inital wait for one frame --
   // --- Linux seems to need this otherwise the userFunc() call failes.
   // ---
   tp.tv_nsec += (nsec0);
   if (tp.tv_nsec >= 1000000000) {
      tp.tv_sec++;
      tp.tv_nsec -= 1000000000;
   }
   pthread_cond_timedwait(&cond, &mutex, &tp);

   while (!getParent()->isShutdown()) {

      // ---
      // User defined tasks
      // ---
      this->userFunc(dt);
      tcnt++;

      // ---
      // Wait for the end of frame
      // ---
      tp.tv_nsec += (nsec0);
      if (tp.tv_nsec >= 1000000000) {
         tp.tv_sec++;
         tp.tv_nsec -= 1000000000;
      }
      pthread_cond_timedwait(&cond, &mutex, &tp);

   }

   pthread_mutex_unlock(&mutex);

   if (getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "Thread(" << this << ")::mainLoopFunc(): ... end of main loop." << std::endl;
   }

   return 0;
}

}
}

