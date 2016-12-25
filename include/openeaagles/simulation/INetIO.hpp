
#ifndef __oe_simulation_INetIO_H__
#define __oe_simulation_INetIO_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace simulation {

class INetIO : public base::Component
{
   DECLARE_SUBCLASS(INetIO, base::Component)

public:
   INetIO();

   //
   // interface required/used by Station class
   //

   // Updates the 'input' side of the network
   virtual void inputFrame(const double dt) =0;

   // Updates the 'output' side of the network
   virtual void outputFrame(const double dt) =0;

};

}
}

#endif
