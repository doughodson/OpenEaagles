
#ifndef __oe_base_SyncTask_H__
#define __oe_base_SyncTask_H__

#include "openeaagles/base/concurrent/AbstractThread.hpp"

namespace oe {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: SyncTask
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
class SyncTask : public AbstractThread
{
   DECLARE_SUBCLASS(SyncTask, AbstractThread)

public:
   SyncTask(Component* const parent, const double priority);

   void signalStart();
   void waitForCompleted();
   static void waitForAllCompleted(SyncTask** threads, const unsigned int num);

   //Returns the index of the first thread that is completed, or -1 if an error
   static int waitForAnyCompleted(SyncTask** threads, const unsigned int num);

   virtual bool terminate() override;

   // User defined work function
   private:  virtual unsigned long userFunc() =0;

protected:
   SyncTask();

   void waitForStart();
   void signalCompleted();

   virtual bool configThread() override;

private:
   bool createSignals();
   void closeSignals();

   virtual unsigned long mainThreadFunc() override;

   // Implementation dependent
   void* startSig {};      // Start signal
   void* completedSig {};  // completed signal
};

}
}

#endif

