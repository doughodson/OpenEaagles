
#include "openeaagles/base/Thread.hpp"
#include "openeaagles/base/Component.hpp"
#include "openeaagles/base/util/math_utils.hpp"
#include "openeaagles/base/util/system.hpp"
#include <iostream>

/* ----------------------------------------------------------------- */
/* Define 'USE_REALTIME_PRI_CLASS' to use 'REALTIME_PRIORITY_CLASS'  */
/* The default is HIGH_PRIORITY_CLASS                                */
/* ----------------------------------------------------------------- */
//#define USE_REALTIME_PRI_CLASS

//==============================================================================
// class Thread
//==============================================================================

namespace oe {
namespace base {

// max number of processors we'll allow
static const unsigned int MAX_CPUS = 32;

//-----------------------------------------------------------------------------
// Static thread function
//-----------------------------------------------------------------------------
DWORD WINAPI Thread::staticThreadFunc(LPVOID lpParam)
{
   const auto thread = static_cast<Thread*>(lpParam);
   Component* parent = thread->getParent();

   // Make sure that our Thread class and its parent are not going to go a way.
   thread->ref();
   parent->ref();

   // The main thread function, which is a Thread class memeber function,
   // will handle the rest.
   DWORD rtn = thread->mainThreadFunc();
   thread->setTerminated();

   parent->unref();
   thread->unref();

   return rtn;
}

//-----------------------------------------------------------------------------
// Static function returns the number of processors assigned to this process
//-----------------------------------------------------------------------------
unsigned short Thread::getNumProcessors()
{
   unsigned short num = 0;

   DWORD_PTR mask = 0;
   DWORD_PTR sysMask = 0;
   HANDLE hProcess = GetCurrentProcess();
   BOOL rtn = GetProcessAffinityMask(hProcess, &mask, &sysMask);
   if (rtn != 0) {
      // we're checking the number of bits that are set in 'mask'
      unsigned tbit = 0x01;
      for (unsigned int i = 0; i < MAX_CPUS; i++) {
         if ( (tbit & mask) != 0 ) num++;
         tbit = (tbit << 1);
      }
   }

   return num;
}

//-----------------------------------------------------------------------------
// Create the thread
//-----------------------------------------------------------------------------
bool Thread::createThread()
{
   // Creation flags
   DWORD dwCreationFlags = 0;

   // create the thread
   HANDLE hnd =
      CreateThread(
         NULL,             // no security attributes
         stackSize,        // stack size (bytes)
         staticThreadFunc, // static thread function
         this,             // argument passed to static function
         dwCreationFlags,  // Creation flags
         NULL              // returns the thread identifier
      );

   if ( hnd != 0 && parent->isMessageEnabled(MSG_INFO) ) {
      std::cout << "Thread(" << this << ")::createThread(): CreateThread() handle = " << hnd << std::endl;
   }

   theThread = hnd;

   return (hnd != 0);
}

//-----------------------------------------------------------------------------
// Configure thread
//-----------------------------------------------------------------------------
bool Thread::configThread()
{
   // Get handles
   HANDLE hProcess = GetCurrentProcess();
   HANDLE hThread  = GetCurrentThread();

   if (parent->isMessageEnabled(MSG_INFO)) {
      std::cout << "Thread(" << this << ")::configThread(): process handle = " << hProcess << std::endl;
      std::cout << "Thread(" << this << ")::configThread(): thread handle = " << hThread  << std::endl;
   }

   // ---
   // (if it isn't already) Set our process priority class to high priority class
   // ---
   {
      #ifdef USE_REALTIME_PRI_CLASS
         DWORD pclass = REALTIME_PRIORITY_CLASS;
      #else
         DWORD pclass = HIGH_PRIORITY_CLASS;
      #endif

      if (GetPriorityClass(hProcess) != pclass) {
         BOOL stat = SetPriorityClass(hProcess,pclass);

         if (stat == 0 && parent->isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Thread(" << this << ")::configThread(): Error: SetPriorityClass() failed! ";
            std::cerr << GetLastError() << std::endl;
         }
         else if (stat != 0 && parent->isMessageEnabled(MSG_INFO)) {
            std::cout << "Thread(" << this << ")::configThread(): SetPriorityClass() set!" << std::endl;
         }
      }
   }

   // ---
   // Map and set our priority
   // ---
   {
      int value = 0;
      #ifdef USE_RT_PRI_CLASS
         if (priority      == 1.0f) value = THREAD_PRIORITY_TIME_CRITICAL;
         else if (priority >= 0.9f) value = 5;
         else if (priority >= 0.8f) value = 4;
         else if (priority >= 0.7f) value = 3;
         else if (priority >= 0.6f) value = THREAD_PRIORITY_HIGHEST;
         else if (priority  > 0.5f) value = THREAD_PRIORITY_ABOVE_NORMAL;
         else if (priority == 0.5f) value = THREAD_PRIORITY_NORMAL;
         else if (priority >= 0.4f) value = THREAD_PRIORITY_BELOW_NORMAL;
         else if (priority >= 0.3f) value = THREAD_PRIORITY_LOWEST;
         else if (priority >= 0.2f) value = -3;
         else if (priority >= 0.1f) value = -4;
         else if (priority  > 0.0f) value = -5;
         else                       value = THREAD_PRIORITY_IDLE;
      #else
         if (priority      == 1.0f) value = THREAD_PRIORITY_TIME_CRITICAL;
         else if (priority >= 0.7f) value = THREAD_PRIORITY_HIGHEST;
         else if (priority >= 0.6f) value = THREAD_PRIORITY_ABOVE_NORMAL;
         else if (priority >= 0.4f) value = THREAD_PRIORITY_NORMAL;
         else if (priority >= 0.2f) value = THREAD_PRIORITY_BELOW_NORMAL;
         else if (priority >  0.0f) value = THREAD_PRIORITY_LOWEST;
         else                       value = THREAD_PRIORITY_IDLE;
      #endif

      BOOL stat = SetThreadPriority(hThread,value);

      if (stat == 0 && parent->isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Thread(" << this << ")::configThread(): Error: SetThreadPriority(" << value << ") failed! ";
         std::cerr << GetLastError()  << std::endl;
      }
      else if (stat != 0 && parent->isMessageEnabled(MSG_INFO)) {
         std::cout << "Thread(" << this << ")::configThread(): SetThreadPriority(" << value << ") set!" << std::endl;
      }
   }

   return true;
}

//-----------------------------------------------------------------------------
// Close the thread
//-----------------------------------------------------------------------------
void Thread::closeThread()
{
   CloseHandle(theThread);
   parent = 0;
   theThread = nullptr;
}

//-----------------------------------------------------------------------------
// Treminate the thread
//-----------------------------------------------------------------------------
bool Thread::terminate()
{
   if (theThread != nullptr && !killed) {
      if ( parent->isMessageEnabled(MSG_INFO) ) {
         std::cout << "Thread(" << this << ")::terminate(): handle = " << theThread << std::endl;
      }

      TerminateThread(theThread, 0);
      theThread = nullptr;
      killed = true;

      // The child thread ref()'s 'this' Thread class and our parent object,
      // but since the child thread is being terminated, the thread won't be
      // able to do the unref()'s, so we need to unref() here.
      parent->unref();
      this->unref();
   }
   return killed;
}

//==============================================================================
// class ThreadPeriodicTask
//==============================================================================

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long ThreadPeriodicTask::mainThreadFunc()
{
   unsigned long rtn = 0;

   // Configure this thread
   bool ok = configThread();

   if ( getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "ThreadPeriodicTask(" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
      std::cout << "ThreadPeriodicTask(" << this << ")::mainThreadFunc(): Parent = " << getParent() << std::endl;
      std::cout << "ThreadPeriodicTask(" << this << ")::mainThreadFunc(): Starting main loop ..." << std::endl;
   }

   // All of the real work is done by ...
   if (ok) {
      double refTime = 0.0;                                  // Reference time
      const double startTime = getComputerTime();;           // Computer's time of day (sec) run started
      double dt = 1.0/static_cast<double>(getRate());

      while (!getParent()->isShutdown()) {

         // ---
         // User defined tasks
         // ---
         this->userFunc(dt);

         // ---
         // Wait for the start of the next frame
         // ---
         {
            // Update reference time at the start of next frame
            refTime = (refTime + dt);

            // Current computer time
            const double time = getComputerTime();

            // Actual run time
            const double t0 = (time - startTime);

            // How long should we sleep for
            const double st = refTime - t0;
            const auto sleepFor = static_cast<int>(st*1000.0);

            // wait for the next frame
            if (sleepFor > 0) Sleep(sleepFor);

            // Compute next delta time and update frame stats
            dt = 1.0/static_cast<double>(getRate());
            if (st < 0) {
               const double overrun = -st;
               bfStats.sigma(overrun);
               if (vdtFlg) {
                  // adjust for overrun
                  dt += overrun;
                  refTime += overrun;
               }
            }
            tcnt++;
         }
      }
   }

   if (getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "ThreadPeriodicTask(" << this << ")::mainThreadFunc(): ... end of main loop." << std::endl;
   }

   return rtn;
}

//==============================================================================
// class ThreadSyncTask
//==============================================================================

//-----------------------------------------------------------------------------
// create the signals
//-----------------------------------------------------------------------------
bool ThreadSyncTask::createSignals()
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
void ThreadSyncTask::closeSignals()
{
   CloseHandle(startSig);
   startSig = 0;

   CloseHandle(completedSig);
   completedSig = 0;
}

//-----------------------------------------------------------------------------
// Signal 'start'
//-----------------------------------------------------------------------------
void ThreadSyncTask::signalStart()
{
   ReleaseSemaphore(static_cast<HANDLE>(startSig),1,NULL);
}

//-----------------------------------------------------------------------------
// Wait for the 'start' signal
//-----------------------------------------------------------------------------
void ThreadSyncTask::waitForStart()
{
   WaitForSingleObject(static_cast<HANDLE>(startSig), INFINITE);
}

//-----------------------------------------------------------------------------
// Signal 'completed'
//-----------------------------------------------------------------------------
void ThreadSyncTask::signalCompleted()
{
   ReleaseSemaphore(static_cast<HANDLE>(completedSig), 1, NULL);
}

//-----------------------------------------------------------------------------
// Wait for the 'completed' signal
//-----------------------------------------------------------------------------
void ThreadSyncTask::waitForCompleted()
{
   WaitForSingleObject(static_cast<HANDLE>(completedSig), INFINITE);
}

//-----------------------------------------------------------------------------
// Wait for all of these threads to complete.
//-----------------------------------------------------------------------------
void ThreadSyncTask::waitForAllCompleted(ThreadSyncTask** threads, const unsigned int num)
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
int ThreadSyncTask::waitForAnyCompleted(ThreadSyncTask** threads, const unsigned int num)
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
         if(ret >= WAIT_OBJECT_0 && ret <= (WAIT_OBJECT_0 + num - 1)) {
            return indexes[ret - WAIT_OBJECT_0];
         }
         if(ret >= WAIT_ABANDONED_0 && ret <= (WAIT_ABANDONED_0 + num - 1)) {
            return indexes[ret - WAIT_ABANDONED_0];
         }
      }
   }
   return -1;
}

}
}
