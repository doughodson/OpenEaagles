
#include "openeaagles/base/concurrent/ThreadSyncTask.hpp"

#include "openeaagles/base/Component.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(ThreadSyncTask, "ThreadSyncTask")
EMPTY_SLOTTABLE(ThreadSyncTask)
EMPTY_SERIALIZER(ThreadSyncTask)

ThreadSyncTask::ThreadSyncTask(Component* const p, const double pri) : Thread(p, pri)
{
   STANDARD_CONSTRUCTOR()
}

ThreadSyncTask::ThreadSyncTask()
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "ThreadSyncTask(" << this << ")::ThreadSyncTask() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "ThreadSyncTask(" << this << ")::ThreadSyncTask() -- ERROR: Did NOT create the thread!" << std::endl;
}

void ThreadSyncTask::copyData(const ThreadSyncTask& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "ThreadSyncTask(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

void ThreadSyncTask::deleteData()
{
   closeSignals();
}

//-----------------------------------------------------------------------------
// Configure thread
//-----------------------------------------------------------------------------
bool ThreadSyncTask::configThread()
{
   bool ok = BaseClass::configThread();

   // Create the signals
   if (ok) ok = createSignals();

   if (!ok) {
      std::cerr << "ThreadSyncTask(" << this << ")::configThread() -- ERROR: Did NOT create the signals!" << std::endl;
   }

   return ok;
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long ThreadSyncTask::mainThreadFunc()
{
   unsigned long rtn = 0;

   // Configure this thread
   bool ok = configThread();

   if ( getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "ThreadSyncTask(" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
   }

   // Main start-complete loop ...
   while ( ok && getParent()->isNotShutdown() ) {

      // Wait for the start signal
      waitForStart();

      // Just in case we've been shutdown while we were waiting
      if (getParent()->isShutdown()) {
         signalCompleted();
         break;
      }

      // User defined tasks
      this->userFunc();

      // Signal that we've completed
      signalCompleted();
   }

   return rtn;
}

//-----------------------------------------------------------------------------
// Terminate the thread
//-----------------------------------------------------------------------------
bool ThreadSyncTask::terminate()
{
   signalCompleted();
   return BaseClass::terminate();
}

}
}

