//------------------------------------------------------------------------------
// Class: Tws
// Description: Simple, Track-While-Scan Mode Radar
//------------------------------------------------------------------------------
#ifndef __Eaagles_Sensor_Tws_H__
#define __Eaagles_Sensor_Tws_H__

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

#endif
