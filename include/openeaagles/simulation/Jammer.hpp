
#ifndef __oe_simulation_Jammer_H__
#define __oe_simulation_Jammer_H__

#include "openeaagles/simulation/RfSensor.hpp"

namespace oe {
namespace simulation {
//------------------------------------------------------------------------------
// Class: Jammer
// Description: Example Jammer
// Factory name: Jammer
//
// Default R/F sensor type ID is "JAMMER"
//------------------------------------------------------------------------------
class Jammer : public RfSensor
{
    DECLARE_SUBCLASS(Jammer, RfSensor)

public:
    Jammer();

protected:
   virtual void transmit(const double dt) override;
};

}
}

#endif
