
#ifndef __oe_simulation_SimTcThread_H__
#define __oe_simulation_SimTcThread_H__

#include "openeaagles/base/Thread.hpp"

namespace oe {
namespace base { class Component; class PairStream; }
namespace simulation {

//------------------------------------------------------------------------------
// Class: SimTcThread
// Description: Time critical thread
//------------------------------------------------------------------------------
class SimTcThread : public base::ThreadSyncTask
{
   DECLARE_SUBCLASS(SimTcThread, base::ThreadSyncTask)

public:
   SimTcThread(base::Component* const parent, const double priority);

   // Parent thread signals start to this child thread with these parameters.
   void start(
      base::PairStream* const pl0,
      const double dt0,
      const unsigned int idx0,
      const unsigned int n0
   );

private:
   // ThreadSyncTask class function -- our userFunc()
   virtual unsigned long userFunc() override;

private:
   base::PairStream* pl0;
   double dt0;
   unsigned int idx0;
   unsigned int n0;
};

}
}

#endif
