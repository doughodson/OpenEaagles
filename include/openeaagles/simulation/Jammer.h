//------------------------------------------------------------------------------
// Class: Jammer
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Jammer_H__
#define __oe_simulation_Jammer_H__

#include "openeaagles/simulation/RfSensor.h"

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
    DECLARE_SUBCLASS(Jammer,RfSensor)

public:
    Jammer();

protected:
    void transmit(const double dt) override;
};

} // End simulation namespace
}

#endif
