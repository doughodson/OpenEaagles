
#include "openeaagles/simulation/StationTcThread.hpp"

#include "openeaagles/simulation/Station.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(TcThread, "TcThread")
EMPTY_SLOTTABLE(TcThread)
EMPTY_COPYDATA(TcThread)
EMPTY_DELETEDATA(TcThread)
EMPTY_SERIALIZER(TcThread)

TcThread::TcThread(base::Component* const parent, const double priority, const double rate): base::PeriodicTask(parent, priority, rate)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long TcThread::userFunc(const double dt)
{
   Station* station = static_cast<Station*>(getParent());
   station->processTimeCriticalTasks(dt);
   return 0;
}

}
}
