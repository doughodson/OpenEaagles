
#include "openeaagles/basic/ThreadPool.h"
#include "openeaagles/basic/Thread.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// ThreadPoolThread
//
// Description: The implementation of ThreadSyncTask used for the ThreadPool
//==============================================================================

class ThreadPoolThread : public ThreadSyncTask {
   DECLARE_SUBCLASS(ThreadPoolThread, ThreadSyncTask)
   friend class ThreadPool;

public:
   ThreadPoolThread(Component* const parent, ThreadPool*const pool, ThreadPoolManager*const mgr, const LCreal priority, Object* const obj);

protected:
   Object* getPersistentObj();
   void setCurrentObj(Object* const obj);

private:
   virtual unsigned long userFunc();

private:
   ThreadPool* threadPool;
   ThreadPoolManager* manager;
   Object* persistentObj; //The persistent callback object for this thread
   Object* currentObj;    //The active callback object for this thread
};


IMPLEMENT_SUBCLASS(ThreadPoolThread,"ThreadPoolThread")
EMPTY_SLOTTABLE(ThreadPoolThread)
EMPTY_COPYDATA(ThreadPoolThread)
EMPTY_SERIALIZER(ThreadPoolThread)

ThreadPoolThread::ThreadPoolThread(Component* const parent, ThreadPool*const pool, ThreadPoolManager*const mgr, const LCreal priority, Object* const obj)
   : threadPool(pool), manager(mgr), persistentObj(obj), ThreadSyncTask(parent, priority)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long ThreadPoolThread::userFunc()
{
   //Execute the thread callback methods
   if(manager != 0)
   {
      manager->execute(persistentObj);
      manager->execute(persistentObj, currentObj);
   }

   //Clear the current callback object because we're done with it
   currentObj = 0;

   //Add the thread back to the pool
   threadPool->threadAvailable(this);

   return 0;
}

void ThreadPoolThread::deleteData()
{
   if(manager != 0)
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


//==============================================================================
// ThreadPoolManager
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(ThreadPoolManager,"ThreadPoolManager")
EMPTY_SLOTTABLE(ThreadPoolManager)
EMPTY_CONSTRUCTOR(ThreadPoolManager)
EMPTY_COPYDATA(ThreadPoolManager)
EMPTY_DELETEDATA(ThreadPoolManager)
EMPTY_SERIALIZER(ThreadPoolManager)

//------------------------------------------------------------------------------
// Default callback implementations (do nothing)
//------------------------------------------------------------------------------

Object* ThreadPoolManager::initialize()
{
   return 0;
}

void ThreadPoolManager::prepare( Object* const obj )
{
}

void ThreadPoolManager::execute( Object* const obj )
{
}

void ThreadPoolManager::execute( Object* const obj, Object* cur )
{
}

void ThreadPoolManager::destroy( Object* const obj )
{
}

//==============================================================================
// ThreadPool
//==============================================================================

IMPLEMENT_SUBCLASS(ThreadPool,"ThreadPool")
EMPTY_SERIALIZER(ThreadPool)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------

BEGIN_SLOTTABLE(ThreadPool)
   "numThreads",  // Number of threads to use - 0 = don't use threading
   "priority"     // Thread priority (zero(0) is lowest, one(1) is highest)
END_SLOTTABLE(ThreadPool)

BEGIN_SLOT_MAP(ThreadPool)
   ON_SLOT( 1,  setSlotNumThreads, Number)
   ON_SLOT( 2,  setSlotPriority,   Number)
END_SLOT_MAP()

   //------------------------------------------------------------------------------
   // Constructors
   //------------------------------------------------------------------------------

ThreadPool::ThreadPool()
   : manager(0), numThreads(0), priority(0.5)
{
   STANDARD_CONSTRUCTOR()
   initData();
}

ThreadPool::ThreadPool( ThreadPoolManager* mgr )
   : manager(0), numThreads(0), priority(0.5)
{
   STANDARD_CONSTRUCTOR()
   setManager(mgr);
   initData();
}

ThreadPool::ThreadPool( ThreadPoolManager* mgr, const unsigned int num )
   : manager(0), numThreads(num), priority(0.5)
{
   STANDARD_CONSTRUCTOR()
   setManager(mgr);
   initData();
}

ThreadPool::ThreadPool( ThreadPoolManager* mgr, const unsigned int num, const LCreal pri )
   : manager(0), numThreads(num), priority(pri)
{
   STANDARD_CONSTRUCTOR()
   setManager(mgr);
   initData();
}

void ThreadPool::initData()
{
   actualThreads = 0;
   availableThreadsLock = 0;
   unthreadedObj = 0;
}


//------------------------------------------------------------------------------
// Public interface
//------------------------------------------------------------------------------

void ThreadPool::setManager(ThreadPoolManager* mgr)
{
   if (manager != 0) {
      manager->unref();
   }
   manager = mgr;
   if (manager != 0) {
      manager->ref();
   }
}

void ThreadPool::initialize(Component* const parent)
{
   destroy();

   // Create the thread pool
   if (numThreads > 0)
   {
      std::cout << "Running thread pool in multi-threaded mode" << std::endl;
      for (unsigned int i = 0; i < numThreads; i++)
      {
         //Get the callback object for this thread
         Object* callbackObj = 0;
         if(manager != 0)
            callbackObj = manager->initialize();

         //Add the thread to the master array
         allThreads[actualThreads] = new ThreadPoolThread(parent, this, manager, priority, callbackObj);

         //Create the thread
         bool ok = allThreads[actualThreads]->create();
         if (ok)
         {
            std::cout << "Created thread pool thread[" << actualThreads << "] = " << allThreads[actualThreads] << std::endl;
            availableThreads[actualThreads] = allThreads[actualThreads];
            actualThreads++;
         }
         else
         {
            allThreads[actualThreads]->unref();
            allThreads[actualThreads] = 0;
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "ThreadPool::initialize(): ERROR, failed to create a thread pool thread!" << std::endl;
            }
         }
      }
   }

   //Use single-threaded mode if we're not using threads or if threading failed
   if(actualThreads == 0)
   {
      std::cout << "Running thread pool in single-threaded mode" << std::endl;
      if(manager != 0)
         unthreadedObj = manager->initialize();
   }
}

void ThreadPool::execute()
{
   execute(0);
}

void ThreadPool::execute(Object* cur)
{
   //If we're unthreaded, just use this thread
   if(actualThreads == 0)
   {
      if(manager != 0)
      {
         manager->prepare(unthreadedObj);
         manager->execute(unthreadedObj);
         manager->execute(unthreadedObj, cur);
      }
      return;
   }

   //Try to get an available thread from the pool
   ThreadPoolThread* availableThread = getAvailableThread();

   //If we didn't get one, we'll have to wait
   if(availableThread == 0)
   {
      //Wait for one to become available
      if(ThreadSyncTask::waitForAnyCompleted((ThreadSyncTask**)&allThreads[0], actualThreads) == -1)
      {
         //Error
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "ThreadPool::execute(): ERROR, unknown error while waiting for completed thread signal!" << std::endl;
         }
         return;
      }

      //Should have at least one now - try again
      availableThread = getAvailableThread();
   }

   //Do we have one now (we should)?
   if(availableThread == 0)
   {
      //Error
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "ThreadPool::execute(): ERROR, could not get an available thread!" << std::endl;
      }
      return;
   }

   //Prepare the thread
   if(manager != 0)
      manager->prepare(availableThread->getPersistentObj());

   //Launch the thread
   availableThread->setCurrentObj(cur);
   availableThread->signalStart();
}

ThreadPoolThread* ThreadPool::getAvailableThread()
{
   ThreadPoolThread* availableThread = 0;
   lcLock(availableThreadsLock);
   for(int i = actualThreads - 1 ; i >= 0 ; i--)
   {
      if(availableThreads[i] != 0)
      {
         availableThread = availableThreads[i];
         availableThreads[i] = 0;
         break;
      }
   }
   lcUnlock(availableThreadsLock);
   return availableThread;
}

void ThreadPool::threadAvailable(ThreadPoolThread* availableThread)
{
   lcLock(availableThreadsLock);
   for(unsigned int i = 0 ; i < actualThreads ; i++)
   {
      if(availableThreads[i] == 0)
      {
         availableThreads[i] = availableThread;
         break;
      }
   }
   lcUnlock(availableThreadsLock);
}

void ThreadPool::destroy()
{
   //Delete all threads
   for (unsigned int i = 0; i < actualThreads; i++) {
      allThreads[i]->terminate();
      allThreads[i]->unref();
      allThreads[i] = 0;
   }
   lcLock(availableThreadsLock);
   for (unsigned int i = 0; i < actualThreads; i++) {
      availableThreads[i] = 0;
   }
   lcUnlock(availableThreadsLock);
   actualThreads = 0;

   //Destroy the unthreaded object
   if(unthreadedObj != 0)
   {
      if(manager != 0)
         manager->destroy(unthreadedObj);
      unthreadedObj = 0;
   }
}

//------------------------------------------------------------------------------
// Object overloads
//------------------------------------------------------------------------------

void ThreadPool::copyData(const ThreadPool& org, const bool cc)
{
   BaseClass::copyData(org);
   destroy();
   if(cc)
      initData();

   //Copy the manager, number of threads, and priority
   if(org.manager != 0)
      setManager((ThreadPoolManager*)org.manager->clone());
   else
      setManager(0);
   numThreads = org.numThreads;
   priority = org.priority;
}

void ThreadPool::deleteData()
{
   destroy();
   setManager(0);
}

//------------------------------------------------------------------------------
// Slot methods
//------------------------------------------------------------------------------

bool ThreadPool::setSlotNumThreads(const Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int num = msg->getInt();
      if (num >= 0 && num <= (int)MAX_THREADS) {
         numThreads = (unsigned int)num;
         ok = true;
      }
      else {
         std::cerr << "ThreadPool::setSlotNumThreads: numThreads is invalid, range: [0 .. " << MAX_THREADS << "]" << std::endl;
      }
   }
   return ok;
}

bool ThreadPool::setSlotPriority(const Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal pri = msg->getReal();
      if (pri >= 0 && pri <= 1.0f) {
         priority = pri;
         ok = true;
      }
      else {
         std::cerr << "ThreadPool::setSlotPriority: priority is invalid, range: [0 .. 1]" << std::endl;
      }
   }
   return ok;
}

Object* ThreadPool::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

} // End Basic namespace
} // End Eaagles namespace
