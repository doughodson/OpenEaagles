//------------------------------------------------------------------------------
// Class:	Stt
// Base class:	Basic::Object -> ... -> RfSystem -> RfSensor -> Radar -> Stt
//
// Description: Simple, Single-Target-Track Mode Radar
//
//------------------------------------------------------------------------------
#ifndef __STT_H_9FE7BD12_E414_4e0f_B542_6CDEE8D7E5B6__
#define __STT_H_9FE7BD12_E414_4e0f_B542_6CDEE8D7E5B6__

#include "openeaagles/simulation/Radar.h"

namespace Eaagles {
namespace Sensor {

class Stt : public Simulation::Radar  
{
    DECLARE_SUBCLASS(Stt,Simulation::Radar)

public:
    Stt();

protected:
    // System class -- phase callbacks
    virtual void dynamics(const LCreal dt);
};

} // End Sensor namespace
} // End Eaagles namespace

#endif // __STT_H_9FE7BD12_E414_4e0f_B542_6CDEE8D7E5B6__
