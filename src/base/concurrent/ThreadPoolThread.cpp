
#include "openeaagles/base/concurrent/ThreadPoolThread.hpp"

#include "openeaagles/base/concurrent/ThreadPool.hpp"
#include "openeaagles/base/concurrent/ThreadPoolManager.hpp"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(ThreadPoolThread, "ThreadPoolThread")
EMPTY_SLOTTABLE(ThreadPoolThread)
EMPTY_COPYDATA(ThreadPoolThread)
EMPTY_SERIALIZER(ThreadPoolThread)

ThreadPoolThread::ThreadPoolThread(Component* const parent, ThreadPool* const pool, ThreadPoolManager* const mgr, const double priority, Object* const obj)
   : ThreadSyncTask(parent, priority), threadPool(pool), manager(mgr), persistentObj(obj), currentObj(nullptr)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long ThreadPoolThread::userFunc()
{
   //Execute the thread callback methods
   if (manager != nullptr)
   {
      manager->execute(persistentObj);
      manager->execute(persistentObj, currentObj);
   }

   //Clear the current callback object because we're done with it
   currentObj = nullptr;

   //Add the thread back to the pool
   threadPool->threadAvailable(this);

   return 0;
}

void ThreadPoolThread::deleteData()
{
   if (manager != nullptr)
      manager->destroy(persistentObj);
}

Object* ThreadPoolThread::getPersistentObj()
{
   return persistentObj;
}

void ThreadPoolThread::setCurrentObj(Object* const obj)
{
   currentObj = obj;
}

}
}
