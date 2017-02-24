
#include "openeaagles/base/concurrent/AbstractThread.hpp"

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
// Static thread function
//-----------------------------------------------------------------------------
void* AbstractThread::staticThreadFunc(void* lpParam)
{
   const auto thread = static_cast<AbstractThread*>(lpParam);
   Component* parent = thread->getParent();

   // Make sure that our AbstractThread class and its parent are not going to go a way.
   thread->ref();
   parent->ref();

   // The main thread function, which is a AbstractThread class memeber function,
   // will handle the rest.
   unsigned long rtn = thread->mainThreadFunc();
   thread->setTerminated();

   parent->unref();
   thread->unref();

   return reinterpret_cast<void*>(rtn);
}

//-----------------------------------------------------------------------------
// Static function returns the number of processors assigned to this process
//-----------------------------------------------------------------------------
unsigned short AbstractThread::getNumProcessors()
{
   unsigned short num = 0;

   cpu_set_t mask;
   int rtn = sched_getaffinity(0, sizeof(cpu_set_t), &mask);
   if (rtn == 0) {
      // we're checking the number of bits that are set in 'mask'
      for (unsigned int cpu = 0; cpu < MAX_CPUS; cpu++) {
         if ( CPU_ISSET(cpu, &mask) != 0 ) num++;
      }
   }

   return num;
}

//-----------------------------------------------------------------------------
// Create the thread
//-----------------------------------------------------------------------------
bool AbstractThread::createThread()
{
   // thread attributes
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   // ---
   // Set the thread schedule priority
   //   priority > 0 use SCHED_FIFO polity with % of FIFO priority
   //   priority == 0; use SCHED_OTHER
   // ---
   struct sched_param param;
   {
      if (priority > 0.0f && priority <= 1.0f) {
         float maxp = sched_get_priority_max(SCHED_FIFO);
         float minp = sched_get_priority_min(SCHED_FIFO);
         float value = (maxp - minp);
         if (priority      == 1.0f) value *= 1.0f;
         else if (priority >= 0.9f) value *= 0.9f;
         else if (priority >= 0.8f) value *= 0.8f;
         else if (priority >= 0.7f) value *= 0.7f;
         else if (priority >= 0.6f) value *= 0.6f;
         else if (priority >= 0.5f) value *= 0.5f;
         else if (priority >= 0.4f) value *= 0.4f;
         else if (priority >= 0.3f) value *= 0.3f;
         else if (priority >= 0.2f) value *= 0.2f;
         else if (priority  > 0.1f) value *= 0.1f;
         else value = 0.0f;
         param.sched_priority = nint(value + minp);
         pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
      }
      else {
         param.sched_priority = sched_get_priority_max(SCHED_OTHER);
         pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
      }

      pthread_attr_setschedparam(&attr, &param);
   }

   // ---
   // Thread stack size
   // ---
   if (stackSize > 0) {
      pthread_attr_setstacksize(&attr, stackSize);
   }

   // ---
   // Create the thread
   // ---
   pthread_t* thread = new pthread_t;
   pthread_create(thread, &attr, staticThreadFunc, this);

   //if ( stat != 0 && parent->isMessageEnabled(MSG_INFO) ) {
      std::cout << "AbstractThread(" << this << ")::createThread(): pthread_create() thread = " << thread << ", pri = " << param.sched_priority << std::endl;
   //}

   theThread = thread;

   return (theThread != nullptr);
}

//-----------------------------------------------------------------------------
// Configure thread
//-----------------------------------------------------------------------------
bool AbstractThread::configThread()
{
   // Nothing to do
   return true;
}

//-----------------------------------------------------------------------------
// Close the thread
//-----------------------------------------------------------------------------
void AbstractThread::closeThread()
{
}

//-----------------------------------------------------------------------------
// Treminate the thread
//-----------------------------------------------------------------------------
bool AbstractThread::terminate()
{
   if (theThread != nullptr && !killed) {
      if ( getParent()->isMessageEnabled(MSG_INFO) ) {
         std::cout << "AbstractThread(" << this << ")::terminate(): handle = " << theThread << std::endl;
      }

      pthread_t* thread = static_cast<pthread_t*>(theThread);
      pthread_kill(*thread, SIGKILL);
      theThread = nullptr;
      killed = true;

      // The staticThreadFunc() function ref()'s 'this' Thread class and our
      // parent object, but since we've been terminated, the thread won't be
      // returning via staticThreadFunc(), so we need to unref() here.
      getParent()->unref();
      this->unref();
   }
   return killed;
}

}
}

