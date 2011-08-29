//------------------------------------------------------------------------------
// Class:	Jammer
//------------------------------------------------------------------------------
#ifndef __Jammer_H_EDBC34AC_10D9_41ff_8521_33FC12BBE674__
#define __Jammer_H_EDBC34AC_10D9_41ff_8521_33FC12BBE674__

#include "openeaagles/simulation/RfSensor.h"

namespace Eaagles {
namespace Simulation {
//------------------------------------------------------------------------------
// Class: Jammer
// Base class: Basic::Object -> ... -> RfSystem -> RfSensor -> Jammer
//
// Description: Example Jammer
// Form name: Jammer
//
// Default R/F sensor type ID is "JAMMER"
//------------------------------------------------------------------------------
class Jammer : public RfSensor  
{
    DECLARE_SUBCLASS(Jammer,RfSensor)

public:
    Jammer();

    // RfSystem interface
    virtual void rfReceivedEmission(Emission* const em, Antenna* const ra, const LCreal raGain);

protected:
    // System Interface -- phase callbacks
    virtual void transmit(const LCreal dt);
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __Jammer_H_EDBC34AC_10D9_41ff_8521_33FC12BBE674__
