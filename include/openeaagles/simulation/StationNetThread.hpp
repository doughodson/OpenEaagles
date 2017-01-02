#ifndef __oe_simulation_StationNetThread_H__
#define __oe_simulation_StationNetThread_H__

#include "openeaagles/base/Thread.hpp"

namespace oe {
namespace simulation {

// ---
// Interoperability Networks thread
// ---
class NetThread : public base::ThreadPeriodicTask
{
   DECLARE_SUBCLASS(NetThread,base::ThreadPeriodicTask)
   public: NetThread(base::Component* const parent, const double priority, const double rate);
   private: virtual unsigned long userFunc(const double dt) override;
};

}
}



#endif
