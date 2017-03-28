
#include "openeaagles/base/concurrent/Thread.hpp"

#include "openeaagles/base/Component.hpp"
#include "openeaagles/base/util/system_utils.hpp"
#include <iostream>

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

}
}
