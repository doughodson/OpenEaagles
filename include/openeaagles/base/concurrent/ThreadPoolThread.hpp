
#include "openeaagles/base/concurrent/SyncTask.hpp"

namespace oe {
namespace base {
class ThreadPool;
class ThreadPoolManager;

//------------------------------------------------------------------------------
// ThreadPoolThread
//
// Description: The implementation of ThreadSyncTask used for the ThreadPool
//------------------------------------------------------------------------------
class ThreadPoolThread : public SyncTask
{
   DECLARE_SUBCLASS(ThreadPoolThread, SyncTask)
   friend class ThreadPool;

public:
   ThreadPoolThread(Component* const parent, ThreadPool* const pool, ThreadPoolManager* const mgr, const double priority, Object* const obj);

protected:
   Object* getPersistentObj();
   void setCurrentObj(Object* const obj);

private:
   virtual unsigned long userFunc() override;

private:
   ThreadPool* threadPool {};
   ThreadPoolManager* manager {};
   Object* persistentObj {};      // The persistent callback object for this thread
   Object* currentObj {};         // The active callback object for this thread
};

}
}
