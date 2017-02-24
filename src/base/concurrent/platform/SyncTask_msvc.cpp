
#include "openeaagles/base/concurrent/SyncTask.hpp"

#include "openeaagles/base/Component.hpp"
#include "openeaagles/base/util/system.hpp"

/* ----------------------------------------------------------------- */
/* Define 'USE_REALTIME_PRI_CLASS' to use 'REALTIME_PRIORITY_CLASS'  */
/* The default is HIGH_PRIORITY_CLASS                                */
/* ----------------------------------------------------------------- */
//#define USE_REALTIME_PRI_CLASS

namespace oe {
namespace base {

// max number of processors we'll allow
static const unsigned int MAX_CPUS = 32;

//-----------------------------------------------------------------------------
// create the signals
//-----------------------------------------------------------------------------
bool SyncTask::createSignals()
{
   // create the start semaphore already set, signalStart() will release it.
   startSig = CreateSemaphore(NULL, 0, 1, NULL);

   // create the completed semaphore already set, signalCompleted() will release it.
   completedSig  = CreateSemaphore(NULL, 0, 1, NULL);

   return true;
}

//-----------------------------------------------------------------------------
// Close the signals
//-----------------------------------------------------------------------------
void SyncTask::closeSignals()
{
   CloseHandle(startSig);
   startSig = 0;

   CloseHandle(completedSig);
   completedSig = 0;
}

//-----------------------------------------------------------------------------
// Signal 'start'
//-----------------------------------------------------------------------------
void SyncTask::signalStart()
{
   ReleaseSemaphore(static_cast<HANDLE>(startSig),1,NULL);
}

//-----------------------------------------------------------------------------
// Wait for the 'start' signal
//-----------------------------------------------------------------------------
void SyncTask::waitForStart()
{
   WaitForSingleObject(static_cast<HANDLE>(startSig), INFINITE);
}

//-----------------------------------------------------------------------------
// Signal 'completed'
//-----------------------------------------------------------------------------
void SyncTask::signalCompleted()
{
   ReleaseSemaphore(static_cast<HANDLE>(completedSig), 1, NULL);
}

//-----------------------------------------------------------------------------
// Wait for the 'completed' signal
//-----------------------------------------------------------------------------
void SyncTask::waitForCompleted()
{
   WaitForSingleObject(static_cast<HANDLE>(completedSig), INFINITE);
}

//-----------------------------------------------------------------------------
// Wait for all of these threads to complete.
//-----------------------------------------------------------------------------
void SyncTask::waitForAllCompleted(SyncTask** threads, const unsigned int num)
{
   if (threads != 0 && num > 0) {
      HANDLE handles[MAXIMUM_WAIT_OBJECTS];
      DWORD count = 0;
      for (unsigned int i = 0; i < num; i++) {
         if (threads[i] != nullptr) {
            handles[count++] = static_cast<HANDLE>(threads[i]->completedSig);
         }
      }

      if (count > 0) {
         WaitForMultipleObjects(count, handles, TRUE, INFINITE);
      }
   }
}

//-----------------------------------------------------------------------------
// Wait for any of these threads to complete.
//-----------------------------------------------------------------------------
int SyncTask::waitForAnyCompleted(SyncTask** threads, const unsigned int num)
{
   if (threads != nullptr && num > 0) {
      HANDLE handles[MAXIMUM_WAIT_OBJECTS];
      //Need to keep a mapping of handle index -> thread index (since we skip invalid ones)
      int indexes[MAXIMUM_WAIT_OBJECTS];
      DWORD count = 0;
      for (unsigned int i = 0; i < num; i++) {
         if (threads[i] != nullptr) {
            handles[count] = static_cast<HANDLE>(threads[i]->completedSig);
            indexes[count++] = i;
         }
      }

      if (count > 0) {
         DWORD ret = WaitForMultipleObjects(count, handles, FALSE, INFINITE);
         if (ret >= WAIT_OBJECT_0 && ret <= (WAIT_OBJECT_0 + num - 1)) {
            return indexes[ret - WAIT_OBJECT_0];
         }
         if (ret >= WAIT_ABANDONED_0 && ret <= (WAIT_ABANDONED_0 + num - 1)) {
            return indexes[ret - WAIT_ABANDONED_0];
         }
      }
   }
   return -1;
}

}
}
