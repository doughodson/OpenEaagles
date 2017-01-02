
#include "openeaagles/simulation/StationBgThread.hpp"

#include "openeaagles/simulation/Station.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(BgThread,"BgThread")
EMPTY_SLOTTABLE(BgThread)
EMPTY_COPYDATA(BgThread)
EMPTY_DELETEDATA(BgThread)
EMPTY_SERIALIZER(BgThread)

BgThread::BgThread(base::Component* const parent, const double priority, const double rate)
      : base::ThreadPeriodicTask(parent, priority, rate)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long BgThread::userFunc(const double dt)
{
   Station* station = static_cast<Station*>(getParent());
   station->processBackgroundTasks(dt);
   return 0;
}

}
}
