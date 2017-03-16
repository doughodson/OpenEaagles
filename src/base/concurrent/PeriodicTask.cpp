
#include "openeaagles/base/concurrent/PeriodicTask.hpp"

#include "openeaagles/base/Component.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(PeriodicTask, "AbstractPeriodicTask")
EMPTY_SLOTTABLE(PeriodicTask)
EMPTY_SERIALIZER(PeriodicTask)
EMPTY_DELETEDATA(PeriodicTask)

PeriodicTask::PeriodicTask(Component* const p, const double pri, const double rt) : Thread(p, pri), rate(rt)
{
   STANDARD_CONSTRUCTOR()
}

PeriodicTask::PeriodicTask() : Thread()
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "ThreadPeriodicTask(" << this << ")::ThreadPeriodicTask() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "ThreadPeriodicTask(" << this << ")::ThreadPeriodicTask() -- ERROR: Did NOT create the thread!" << std::endl;
}

void PeriodicTask::copyData(const PeriodicTask& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "Thread(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

//------------------------------------------------------------------------------
// get/set functions
//------------------------------------------------------------------------------
double PeriodicTask::getRate() const
{
   return rate;
}

const Statistic& PeriodicTask::getBustedFrameStats() const
{
   return bfStats;
}

unsigned int PeriodicTask::getTotalFrameCount() const
{
   return tcnt;
}

bool PeriodicTask::isVariableDeltaTimeEnabled() const
{
   return vdtFlg;
}

bool PeriodicTask::setVariableDeltaTimeFlag(const bool enable)
{
   vdtFlg = enable;
   return true;
}

}
}
