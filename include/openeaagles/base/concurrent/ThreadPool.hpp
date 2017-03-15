
#ifndef __oe_base_ThreadPool_H__
#define __oe_base_ThreadPool_H__

#include "openeaagles/base/Object.hpp"

#include <array>

namespace oe {
namespace base {
class Number;
class Component;
class ThreadPoolManager;
class ThreadPoolThread;

//------------------------------------------------------------------------------
// Class: ThreadPool
//
// Description:
//    General purpose thread pool. You should pass in a ThreadPoolManager
//    derivative to provide the logic for different stages of threaded
//    operation (such as thread initialization or execution). Separating
//    the callbacks from the ThreadPool class keeps the ThreadPool itself
//    completely generic. It can be used programmatically or as part of
//    a Component, and the number of threads to use as well as their
//    priority are configurable and slottable. Selecting 0 for the number
//    of threads puts the ThreadPool in single-threaded mode. This will
//    still operate as if threads were being used except all calls (such
//    as execute()) will block the caller until completed.
//
// Possible improvements:
// - Provide another slot/variable to specify a wait timeout while waiting for
//   a thread to free from the pool. If the wait is exceeded, could continue
//   without executing, or even better, execute a timeout() callback in the
//   manager class to handle timeouts.
//------------------------------------------------------------------------------
class ThreadPool : public Object
{
   DECLARE_SUBCLASS(ThreadPool, Object)
   friend class ThreadPoolThread;

public:
   ThreadPool();
   ThreadPool(ThreadPoolManager* mgr);
   ThreadPool(ThreadPoolManager* mgr, const unsigned int num);
   ThreadPool(ThreadPoolManager* mgr, const unsigned int num, const double pri);

   // Sets a manager for the thread pool. Will often need to call this if
   // the thread pool is created as a slot (as opposed to creating it
   // using the constructor that passes a manager). If a manager is not
   // provided to the thread pool one way or the other, it will run but do nothing.
   void setManager(ThreadPoolManager* mgr);

   // Initializes the thread pool. The initialize() function in the ThreadPoolManager
   // may return a callback object that will be provided to other callbacks during
   // subsequent phases.
   void initialize(Component* const parent);

   // Blocks until a thread from the thread pool is available and then uses
   // it to call the execute() method in the provided ThreadPoolManager in a
   // separate thread. If the ThreadPool is in single-threaded mode, this will
   // not use a new thread and will instead call the execute() method from the
   // ThreadPoolManager in the calling thread and block until completed.
   void execute();

   // Same as execute() except allows the caller to provide an additional current
   // callback object. This allows additional use cases like using the current
   // callback object to adjust the behavior of the ThreadPoolManager for
   // different threads.
   void execute(Object* cur);

   // Destroys all threads in the thread pool.
   void destroy();

   // Slot functions
   virtual bool setSlotNumThreads(const Number* const);
   virtual bool setSlotPriority(const Number* const);

protected:
   void threadAvailable(ThreadPoolThread* availableThread);

private:
   static const unsigned int MAX_THREADS = 32;
   void initData();
   ThreadPoolThread* getAvailableThread();

   ThreadPoolManager* manager {};
   unsigned int numThreads {};
   double priority {0.5};

   unsigned int actualThreads {};
   std::array<ThreadPoolThread*, MAX_THREADS> allThreads {};

   // Keeps track of which threads are available to avoid performance penalty of checking signal state
   std::array<ThreadPoolThread*, MAX_THREADS> availableThreads {};

   // Semaphore to protect the available thread pool
   mutable long availableThreadsLock {};

   // Callback object for when we're not using threading
   Object* unthreadedObj {};
};

}
}

#endif
