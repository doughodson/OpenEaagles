//------------------------------------------------------------------------------
// Class: Stt
// Description: Simple, Single-Target-Track Mode Radar
//
//------------------------------------------------------------------------------
#ifndef __oe_Sensor_Stt_H__
#define __oe_Sensor_Stt_H__

#include "openeaagles/simulation/Radar.h"

namespace oe {
namespace Sensor {

class Stt : public Simulation::Radar
{
    DECLARE_SUBCLASS(Stt,Simulation::Radar)

public:
    Stt();

protected:
    void dynamics(const LCreal dt) override;
};

} // End Sensor namespace
} // End oe namespace

#endif
