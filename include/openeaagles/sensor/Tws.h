//------------------------------------------------------------------------------
// Class: Tws
// Description: Simple, Track-While-Scan Mode Radar
//------------------------------------------------------------------------------
#ifndef __oe_sensor_Tws_H__
#define __oe_sensor_Tws_H__

#include "openeaagles/simulation/Radar.h"

namespace oe {
namespace sensor {

class Tws : public Simulation::Radar
{
    DECLARE_SUBCLASS(Tws,Simulation::Radar)

public:
    Tws();

private:

};

} // End sensor namespace
} // End oe namespace

#endif
