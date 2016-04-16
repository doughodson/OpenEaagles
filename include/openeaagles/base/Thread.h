//------------------------------------------------------------------------------
// Classes:
//
//    Thread               -- Base class for all thread classes.
//
//    ThreadSingleTask     -- Single task thread; single call to the user's
//                            function
//
//    ThreadPeriodicTask   -- Periodic task thread; multiple calls to the user's
//                            function at a fixed rate
//
//    ThreadSyncTask       -- Synchronized task thread; multiple calls to the
//                            user's function with start/complete signals
//
//
//    ### All of these classes are abstract. ###
//
// Examples of how to use the ThreadSingleTask, ThreadPeriodicTask and
// ThreadSyncTask thread classes that are used can be found in the "otw/OtwCigiCl.cpp",
// "simulation/Station.cpp" and "simulation/Simulation.cpp" files, respectively.
//------------------------------------------------------------------------------
#ifndef __oe_base_Thread_H__
#define __oe_base_Thread_H__

#include "openeaagles/base/Object.h"
#include "openeaagles/base/Statistic.h"
#include "openeaagles/base/util/platform_api.h"

namespace oe {
namespace base {

class Component;

//------------------------------------------------------------------------------
// Class:  Thread
//
// Description: Abstract thread class
//
//    User's will need to define the work function, mainThreadFunc() that is derived
//    from this class.
//
//    Thread objects and their parent objects are ref()'d as the child threads
//    starts, and are unref()'d when the threads terminate.  This prevents the
//    objects from being deleted before the thread terminates.
//
//    On Windows applications, the process is converted to a "High priority
//    class" process during the creation of the first Thread.
//
//    The constructor requires several parameters, so the default constructors are
//    'protected', and because of the nature of threads, this class can not be copied
//    or cloned.
//
//
// Thread priority mapping: range [ 0.0 ... 1.0 ] (default: 0.5 -> Normal)
//
//       Priority       Windows(HIGH_PRIORITY_CLASS)              Linux
//    -------------     ------------------------------      --------------------
//        1.0           THREAD_PRIORITY_TIME_CRITICAL(15)   SCHED_FIFO( max FIFO pri )
//    [ 0.9, 1.0 )      THREAD_PRIORITY_HIGHEST(2)          SCHED_FIFO( 90% FIFO pri )
//    [ 0.8, 0.9 )      THREAD_PRIORITY_HIGHEST(2)          SCHED_FIFO( 80% FIFO pri )
//    [ 0.7, 0.8 )      THREAD_PRIORITY_ABOVE_NORMAL(1)     SCHED_FIFO( 70% FIFO pri )
//    [ 0.6, 0.7 )      THREAD_PRIORITY_ABOVE_NORMAL(1)     SCHED_FIFO( 60% FIFO pri )
//    [ 0.5, 0.6 )      THREAD_PRIORITY_NORMAL(0)           SCHED_FIFO( 50% FIFO pri )
//    [ 0.4, 0.5 )      THREAD_PRIORITY_NORMAL(0)           SCHED_FIFO( 40% FIFO pri )
//    [ 0.3, 0.4 )      THREAD_PRIORITY_BELOW_NORMAL(-1)    SCHED_FIFO( 30% FIFO pri )
//    [ 0.2, 0.3 )      THREAD_PRIORITY_BELOW_NORMAL(-1)    SCHED_FIFO( 20% FIFO pri )
//    [ 0.1, 0.2 )      THREAD_PRIORITY_LOWEST(-2)          SCHED_FIFO( 10% FIFO pri )
//    ( 0.0, 0.1 )      THREAD_PRIORITY_LOWEST(-2)          SCHED_FIFO( min FIFO pri )
//        0.0           THREAD_PRIORITY_IDLE(-15)           SCHED_OTHER(0)
//
//    Windows Only: the default process priority class is 'HIGH_PRIORITY_CLASS'.  To use
//    the Windows highest process priority class, 'REALTIME_PRIORITY_CLASS', define the
//    macro 'USE_REALTIME_PRI_CLASS' in the file 'src/base/windows/Thread.cxx', which
//    will set REALTIME_PRIORITY_CLASS class and use the thread priorities below.
//
//            Priority        Windows(REALTIME_PRIORITY_CLASS)
//          -------------     --------------------------------
//              1.0           THREAD_PRIORITY_TIME_CRITICAL(15)
//          [ 0.9, 1.0 )                (5)
//          [ 0.8, 0.9 )                (4)
//          [ 0.7, 0.8 )                (3)
//          [ 0.6, 0.7 )      THREAD_PRIORITY_HIGHEST(2)
//          ( 0.5, 0.6 )      THREAD_PRIORITY_ABOVE_NORMAL(1)
//              0.5           THREAD_PRIORITY_NORMAL(0)
//          [ 0.4, 0.5 )      THREAD_PRIORITY_BELOW_NORMAL(-1)
//          [ 0.3, 0.4 )      THREAD_PRIORITY_LOWEST(-2)
//          [ 0.2, 0.3 )               (-3)
//          [ 0.1, 0.2 )               (-4)
//          ( 0.0, 0.1 )               (-5)
//              0.0           THREAD_PRIORITY_IDLE(-15)
//
//------------------------------------------------------------------------------
class Thread : public Object
{
   DECLARE_SUBCLASS(Thread, Object)

public:
   Thread(Component* const parent, const double priority);

   double getPriority() const;
   bool isTerminated() const;

   // thread stack size in bytes (or zero if using the default stack size)
   size_t getStackSize() const;

   // Create/start the child thread
   virtual bool create();

   // Terminate the child thread
   virtual bool terminate();

   // Set the thread's stack size (zero to use the system default size)
   // -- set before creating the thread --
   bool setStackSize(const size_t size);

   // number of processors assigned to this process
   static unsigned short getNumProcessors();

protected: // Functions
   Thread();
   Component* getParent();
   const void* getThreadHandle() const;

   virtual void setTerminated(); // Indicate that the thread has terminated natually

   virtual bool configThread(); // called by the child thread

private: // Functions
   bool createThread();
   void closeThread();

   // Our main thread function
   virtual unsigned long mainThreadFunc() =0;

   // The static thread function ---
   #if defined(WIN32)
      static DWORD WINAPI staticThreadFunc(LPVOID lpParam);
   #else
      static void* staticThreadFunc(void* lpParam);
   #endif

private: // Data
   Component* parent;   // Our parent component that created us.
   double priority;     // Thread priority (0->lowest, 1->highest)
   bool killed;         // Are we terminated?
   size_t stackSize;    // Stack size in bytes (zero to use the system default stack size)

   // Implementation dependent
   void* theThread;     // Thread handle
};

//------------------------------------------------------------------------------
// Class:  ThreadSingleTask
// Description: Abstract single task thread class
//
//    User's will need to derive a class from this class and define the
//    work function, userFunc(), which is only called once.
//------------------------------------------------------------------------------
class ThreadSingleTask : public Thread
{
   DECLARE_SUBCLASS(ThreadSingleTask, Thread)

public:
   ThreadSingleTask(Component* const parent, const double priority);

   // User defined work function
   private:  virtual unsigned long userFunc() =0;

protected:
   ThreadSingleTask();

private:
   // Thread class function
   unsigned long mainThreadFunc() override;
};

//------------------------------------------------------------------------------
// Class:  ThreadPeriodicTask
//
// Description: Abstract periodic task thread
//
//    User's will need to derive a class from this class and define the
//    work function, userFunc(), which is called at fixed rate of 'rate' Hz
//    until the parent component is shutdown.  A value of 1.0/rate is passed
//    to userFunc() as the delta time parameter.
//------------------------------------------------------------------------------
class ThreadPeriodicTask : public Thread
{
   DECLARE_SUBCLASS(ThreadPeriodicTask, Thread)

public:
   ThreadPeriodicTask(Component* const parent, const double priority, const double rate);

   double getRate() const;                         // Update rate (must be greater than zero)
   unsigned int getTotalFrameCount() const;        // Total frame count

   // Busted (overrun) frames statistics; overrun frames time (seconds)
   // (Windows only)
   const Statistic& getBustedFrameStats() const;

   // Variable delta time flag (windows only).
   // If false (default), delta time is always passed as one over the update rate;
   // If true and there's a frame overrun then a delta time adjusted for the overrun
   // is used.
   bool isVariableDeltaTimeEnabled() const;
   bool setVariableDeltaTimeFlag(const bool enable);

   // User defined work function
   private: virtual unsigned long userFunc(const double dt) =0;

protected:
   ThreadPeriodicTask();

private:
   unsigned long mainThreadFunc() override;

   double rate;         // Loop rate (hz); until our parent shuts down
   Statistic bfStats;   // Busted (overrun) frame statistics (windows only)
   unsigned int tcnt;   // total frame count
   bool vdtFlg;         // Variable delta time flag
};

//------------------------------------------------------------------------------
// Class:  ThreadSyncTask
//
// Description: Abstract sync task (i.e., start/complete loop) thread;
//
//    User's will need to derive a class from this class and define the
//    work function, userFunc(), which is called once after each 'start' signal,
//    signalStart().  This class will then signal 'completed' after each return
//    from the userFunc(). Users can use waitForCompleted() to wait for the
//    'completed' signal, or use the static function waitForAllCompleted() to
//    wait for several sync task threads.  Loop will end with the shutdown of
//    the parent.
//------------------------------------------------------------------------------
class ThreadSyncTask : public Thread
{
   DECLARE_SUBCLASS(ThreadSyncTask, Thread)

public:
   ThreadSyncTask(Component* const parent, const double priority);

   void signalStart();
   void waitForCompleted();
   static void waitForAllCompleted(ThreadSyncTask** threads, const unsigned int num);

   //Returns the index of the first thread that is completed, or -1 if an error
   static int waitForAnyCompleted(ThreadSyncTask** threads, const unsigned int num);

   bool terminate() override;

   // User defined work function
   private:  virtual unsigned long userFunc() =0;

protected:
   ThreadSyncTask();

   void waitForStart();
   void signalCompleted();

   bool configThread() override;

private:
   bool createSignals();
   void closeSignals();

   unsigned long mainThreadFunc() override;

   // Implementation dependent
   void* startSig;      // Start signal
   void* completedSig;  // completed signal
};

}
}

#endif

