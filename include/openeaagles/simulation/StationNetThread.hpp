
#ifndef __oe_simulation_StationNetThread_H__
#define __oe_simulation_StationNetThread_H__

#include "openeaagles/base/concurrent/PeriodicTask.hpp"

namespace oe {
namespace simulation {

// ---
// Interoperability Networks thread
// ---
class NetThread : public base::PeriodicTask
{
   DECLARE_SUBCLASS(NetThread, base::PeriodicTask)
   public: NetThread(base::Component* const parent, const double priority, const double rate);
   private: virtual unsigned long userFunc(const double dt) override;
};

}
}



#endif
