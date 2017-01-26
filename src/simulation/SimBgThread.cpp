
#include "openeaagles/simulation/SimBgThread.hpp"

#include "openeaagles/simulation/Simulation.hpp"

#include "openeaagles/base/Component.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(SimBgThread, "SimBgThread")
EMPTY_SLOTTABLE(SimBgThread)
EMPTY_COPYDATA(SimBgThread)
EMPTY_DELETEDATA(SimBgThread)
EMPTY_SERIALIZER(SimBgThread)

SimBgThread::SimBgThread(base::Component* const parent, const double priority)
      : base::ThreadSyncTask(parent, priority)
{
   STANDARD_CONSTRUCTOR()

   pl0 = nullptr;
   dt0 = 0.0;
   idx0 = 0;
   n0 = 0;
}

void SimBgThread::start(
         base::PairStream* const pl1,
         const double dt1,
         const unsigned int idx1,
         const unsigned int n1
      )
{
   pl0 = pl1;
   dt0 = dt1;
   idx0 = idx1;
   n0 = n1;

   signalStart();
}

unsigned long SimBgThread::userFunc()
{
   // Make sure we've a player list and our index is valid ...
   if (pl0 != nullptr && idx0 > 0 && idx0 <= n0) {
      // then call the simulation executives update TC player list functions
      Simulation* sim = static_cast<Simulation*>(getParent());
      sim->updateBgPlayerList(pl0, dt0, idx0, n0);
   }

   return 0;
}

}
}

