//------------------------------------------------------------------------------
// Class: Jammer
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_Jammer_H__
#define __oe_Simulation_Jammer_H__

#include "openeaagles/simulation/RfSensor.h"

namespace oe {
namespace Simulation {
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
    void transmit(const LCreal dt) override;
};

} // End Simulation namespace
} // End oe namespace

#endif
