//------------------------------------------------------------------------------
// Class:	Tws
// Base class:	Basic::Object -> ... -> RfSystem -> RfSensor -> Radar -> Tws
//
// Description: Simple, Track-While-Scan Mode Radar
//
//------------------------------------------------------------------------------
#ifndef __TWS_H_BCCBFC13_3710_4aac_AE81_20EB503BF3D2__
#define __TWS_H_BCCBFC13_3710_4aac_AE81_20EB503BF3D2__

#include "openeaagles/simulation/Radar.h"

namespace Eaagles {
namespace Sensor {

class Tws : public Simulation::Radar  
{
    DECLARE_SUBCLASS(Tws,Simulation::Radar)

public:
    Tws();

private:

};

} // End Sensor namespace
} // End Eaagles namespace

#endif // __TWS_H_BCCBFC13_3710_4aac_AE81_20EB503BF3D2__
