//------------------------------------------------------------------------------
// Thread classes
//------------------------------------------------------------------------------

#include "openeaagles/basic/Thread.h"
#include "openeaagles/basic/Component.h"

namespace oe {
namespace basic {

//------------------------------------------------------------------------------
// Window/Linux specific code
//------------------------------------------------------------------------------
#if defined(WIN32)
  #include "windows/Thread.cxx"
#else
  #include "linux/Thread.cxx"
#endif

//==============================================================================
// class Thread
//==============================================================================

IMPLEMENT_ABSTRACT_SUBCLASS(Thread,"Thread")
EMPTY_SLOTTABLE(Thread)
EMPTY_SERIALIZER(Thread)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Thread::Thread(Component* const p, const LCreal pri) : parent(p), priority(pri)
{
   STANDARD_CONSTRUCTOR()

   theThread = nullptr;
   killed = false;
   stackSize = 0;
}

Thread::Thread() : parent(nullptr), priority(0), killed(false), stackSize(0), theThread(nullptr)
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "Thread(" << this << ")::Thread() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "Thread(" << this << ")::Thread() -- ERROR: Did NOT create the thread!" << std::endl;

   theThread = nullptr;
   killed = false;
   stackSize = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Thread::copyData(const Thread& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "Thread(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Thread::deleteData()
{
   if (!isTerminated()) {
      terminate();
   }
   closeThread();
}

//------------------------------------------------------------------------------
// Create the thread
//------------------------------------------------------------------------------
bool Thread::create()
{
   bool ok = true;

   // Check for a parent pointer
   if (parent == nullptr) {
      std::cerr << "Thread(" << this << ")::create() -- ERROR: Parent pointer is not set!" << std::endl;
      ok = false;
   }

   // Check for valid priority
   if (priority < 0.0f || priority > 1.0f) {
      std::cerr << "Thread(" << this << ")::create() -- ERROR: Invalid priority! Use [ 0.0 ... 1.0 ]" << std::endl;
      ok = false;
   }

   // Create the thread
   if (ok) ok = createThread();

   if (!ok) {
      std::cerr << "Thread(" << this << ")::create() -- ERROR: Did NOT create the thread!" << std::endl;
   }

   killed = !ok;

   return ok;
}

//-----------------------------------------------------------------------------
// Get functions
//-----------------------------------------------------------------------------
bool Thread::isTerminated() const
{
   return killed;
}

LCreal Thread::getPriority() const
{
   return priority;
}

Component* Thread::getParent()
{
   return parent;
}

const void* Thread::getThreadHandle() const
{
   return theThread;
}

// thread stack size in bytes (or zero if using the default stack size)
size_t Thread::getStackSize() const
{
   return stackSize;
}

//-----------------------------------------------------------------------------
// Set functions
//-----------------------------------------------------------------------------

// Set the thread's stack size in bytes (or zero to use the system default size)
bool Thread::setStackSize(const size_t size)
{
   stackSize = size;
   return true;
}

// Set the terminated flag
void Thread::setTerminated()
{
   killed = true;
}

//==============================================================================
// class ThreadSingleTask
//==============================================================================

IMPLEMENT_ABSTRACT_SUBCLASS(ThreadSingleTask,"ThreadSingleTask")
EMPTY_SLOTTABLE(ThreadSingleTask)
EMPTY_SERIALIZER(ThreadSingleTask)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
ThreadSingleTask::ThreadSingleTask(Component* const p, const LCreal pri) : Thread(p,pri)
{
   STANDARD_CONSTRUCTOR()
}

ThreadSingleTask::ThreadSingleTask()
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "ThreadSingleTask(" << this << ")::ThreadSingleTask() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "ThreadSingleTask(" << this << ")::ThreadSingleTask() -- ERROR: Did NOT create the thread!" << std::endl;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ThreadSingleTask::copyData(const ThreadSingleTask& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "ThreadSingleTask(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void ThreadSingleTask::deleteData()
{
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long ThreadSingleTask::mainThreadFunc()
{
   unsigned long rtn = 0;

   // Configure this thread
   configThread();

   if ( getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "ThreadSingleTask(" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
   }

   // Call the user function only once
   rtn = this->userFunc();

   return rtn;
}

//==============================================================================
// class ThreadSyncTask
//==============================================================================

IMPLEMENT_ABSTRACT_SUBCLASS(ThreadSyncTask,"ThreadSyncTask")
EMPTY_SLOTTABLE(ThreadSyncTask)
EMPTY_SERIALIZER(ThreadSyncTask)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
ThreadSyncTask::ThreadSyncTask(Component* const p, const LCreal pri) : Thread(p, pri)
{
   STANDARD_CONSTRUCTOR()

   startSig = nullptr;
   completedSig = nullptr;
}

ThreadSyncTask::ThreadSyncTask() : startSig(nullptr), completedSig(nullptr)
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "ThreadSyncTask(" << this << ")::ThreadSyncTask() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "ThreadSyncTask(" << this << ")::ThreadSyncTask() -- ERROR: Did NOT create the thread!" << std::endl;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ThreadSyncTask::copyData(const ThreadSyncTask& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "ThreadSyncTask(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
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


//==============================================================================
// class ThreadPeriodicTask
//==============================================================================

IMPLEMENT_ABSTRACT_SUBCLASS(ThreadPeriodicTask,"ThreadPeriodicTask")
EMPTY_SLOTTABLE(ThreadPeriodicTask)
EMPTY_SERIALIZER(ThreadPeriodicTask)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
ThreadPeriodicTask::ThreadPeriodicTask(Component* const p, const LCreal pri, const LCreal rt)
                                       : Thread(p, pri), rate(rt), bfStats(), tcnt(0), vdtFlg(false)
{
   STANDARD_CONSTRUCTOR()
}

ThreadPeriodicTask::ThreadPeriodicTask()
                                       : Thread(), rate(0), bfStats(), tcnt(0), vdtFlg(false)
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "ThreadPeriodicTask(" << this << ")::ThreadPeriodicTask() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "ThreadPeriodicTask(" << this << ")::ThreadPeriodicTask() -- ERROR: Did NOT create the thread!" << std::endl;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ThreadPeriodicTask::copyData(const ThreadPeriodicTask& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "Thread(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void ThreadPeriodicTask::deleteData()
{
}

//------------------------------------------------------------------------------
// get/set functions
//------------------------------------------------------------------------------
LCreal ThreadPeriodicTask::getRate() const
{
   return rate;
}

const Statistic& ThreadPeriodicTask::getBustedFrameStats() const
{
   return bfStats;
}

unsigned int ThreadPeriodicTask::getTotalFrameCount() const
{
   return tcnt;
}

bool ThreadPeriodicTask::isVariableDeltaTimeEnabled() const
{
   return vdtFlg;
}

bool ThreadPeriodicTask::setVariableDeltaTimeFlag(const bool enable)
{
   vdtFlg = enable;
   return true;
}

} // End basic namespace
} // End oe namespace

