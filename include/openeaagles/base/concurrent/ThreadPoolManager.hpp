
#ifndef __oe_base_ThreadPoolManager_H__
#define __oe_base_ThreadPoolManager_H__

#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/Number.hpp"

namespace oe {
namespace base {
class Component;
class ThreadPoolThread;

//------------------------------------------------------------------------------
// Class: ThreadPoolManager
//
// Description:
//    Manages the initialization, execution, and destruction of threads
//    in the thread pool.  Should be overridden and an instance provided
//    to the ThreadPool class before use.  The single instance given to
//    the ThreadPool will be used for every thread in the pool.

//------------------------------------------------------------------------------
class ThreadPoolManager : public Object
{
   DECLARE_SUBCLASS(ThreadPoolManager, Object)
   friend class ThreadPool;
   friend class ThreadPoolThread;

public:
   ThreadPoolManager();

protected:
   // Called when each individual thread is being initialized. Should pass
   // back a callback object unique to each thread that will be passed to
   // the other callback methods.
   virtual Object* initialize();

   // Called just before a thread executes and gives the calling thread a
   // chance to manipulate the callback object before the thread executes.
   virtual void prepare(Object* const obj);

   // Called when a thread executes. Can and will be raised from multiple
   // thread contexts, so anything this method does must be thread safe.
   virtual void execute(Object* const obj);

   // Same as execute(Object*) except also passes the additional current
   // callback object provided to the ThreadPool using its alternate
   // execute() method. This version should be used if the alternate
   // version that passes a current callback object is used in the
   // ThreadPool. If both version are overridden, they will both get
   // called from the thread, this one last.
   virtual void execute(Object* const obj, Object* cur);

   // Called for each thread when the thread is destroyed and resources
   // (such as the callback object) need to be cleaned up.
   virtual void destroy(Object* const obj);
};

}
}

#endif

