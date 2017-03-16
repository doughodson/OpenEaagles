
#ifndef __oe_base_PeriodicTask_H__
#define __oe_base_PeriodicTask_H__

#include "openeaagles/base/concurrent/Thread.hpp"
#include "openeaagles/base/Statistic.hpp"

namespace oe {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: PeriodicTask
//
// Description: Abstract periodic task thread
//
//    User's will need to derive a class from this class and define the
//    work function, userFunc(), which is called at fixed rate of 'rate' Hz
//    until the parent component is shutdown.  A value of 1.0/rate is passed
//    to userFunc() as the delta time parameter.
//------------------------------------------------------------------------------
class PeriodicTask : public Thread
{
   DECLARE_SUBCLASS(PeriodicTask, Thread)

public:
   PeriodicTask(Component* const parent, const double priority, const double rate);

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
   PeriodicTask();

private:
   virtual unsigned long mainThreadFunc() override;

   double rate {};         // Loop rate (hz); until our parent shuts down
   Statistic bfStats {};   // Busted (overrun) frame statistics (windows only)
   unsigned int tcnt {};   // total frame count
   bool vdtFlg {};         // Variable delta time flag
};

}
}

#endif

