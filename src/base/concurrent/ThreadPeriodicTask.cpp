
#include "openeaagles/base/concurrent/ThreadPeriodicTask.hpp"

#include "openeaagles/base/Component.hpp"
//#include "openeaagles/base/util/math_utils.hpp"
//#include "openeaagles/base/util/system.hpp"
//#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(ThreadPeriodicTask, "ThreadPeriodicTask")
EMPTY_SLOTTABLE(ThreadPeriodicTask)
EMPTY_SERIALIZER(ThreadPeriodicTask)
EMPTY_DELETEDATA(ThreadPeriodicTask)

ThreadPeriodicTask::ThreadPeriodicTask(Component* const p, const double pri, const double rt)
                                       : Thread(p, pri), rate(rt)
{
   STANDARD_CONSTRUCTOR()
}

ThreadPeriodicTask::ThreadPeriodicTask()
                                       : Thread()
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "ThreadPeriodicTask(" << this << ")::ThreadPeriodicTask() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "ThreadPeriodicTask(" << this << ")::ThreadPeriodicTask() -- ERROR: Did NOT create the thread!" << std::endl;
}

void ThreadPeriodicTask::copyData(const ThreadPeriodicTask& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "Thread(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

//------------------------------------------------------------------------------
// get/set functions
//------------------------------------------------------------------------------
double ThreadPeriodicTask::getRate() const
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

}
}
