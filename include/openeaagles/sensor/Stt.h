//------------------------------------------------------------------------------
// Class: Stt
// Description: Simple, Single-Target-Track Mode Radar
//
//------------------------------------------------------------------------------
#ifndef __oe_sensor_Stt_H__
#define __oe_sensor_Stt_H__

#include "openeaagles/simulation/Radar.h"

namespace oe {
namespace sensor {

class Stt : public simulation::Radar
{
    DECLARE_SUBCLASS(Stt,simulation::Radar)

public:
    Stt();

protected:
    void dynamics(const LCreal dt) override;
};

} // End sensor namespace
} // End oe namespace

#endif
