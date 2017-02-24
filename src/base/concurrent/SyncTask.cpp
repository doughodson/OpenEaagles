
#include "openeaagles/base/concurrent/SyncTask.hpp"

#include "openeaagles/base/Component.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(SyncTask, "AbstractSyncTask")
EMPTY_SLOTTABLE(SyncTask)
EMPTY_SERIALIZER(SyncTask)

SyncTask::SyncTask(Component* const p, const double pri) : AbstractThread(p, pri)
{
   STANDARD_CONSTRUCTOR()
}

SyncTask::SyncTask()
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "SyncTask(" << this << ")::SyncTask() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "SyncTask(" << this << ")::SyncTask() -- ERROR: Did NOT create the thread!" << std::endl;
}

void SyncTask::copyData(const SyncTask& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "SyncTask(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

void SyncTask::deleteData()
{
   closeSignals();
}

//-----------------------------------------------------------------------------
// Configure thread
//-----------------------------------------------------------------------------
bool SyncTask::configThread()
{
   bool ok = BaseClass::configThread();

   // Create the signals
   if (ok) ok = createSignals();

   if (!ok) {
      std::cerr << "SyncTask(" << this << ")::configThread() -- ERROR: Did NOT create the signals!" << std::endl;
   }

   return ok;
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long SyncTask::mainThreadFunc()
{
   unsigned long rtn = 0;

   // Configure this thread
   bool ok = configThread();

   if ( getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "SyncTask(" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
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
bool SyncTask::terminate()
{
   signalCompleted();
   return BaseClass::terminate();
}

}
}

