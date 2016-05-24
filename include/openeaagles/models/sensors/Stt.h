//------------------------------------------------------------------------------
// Class: Stt
// Description: Simple, Single-Target-Track Mode Radar
//
//------------------------------------------------------------------------------
#ifndef __oe_models_Stt_H__
#define __oe_models_Stt_H__

#include "openeaagles/simulation/Radar.h"

namespace oe {
namespace models {

class Stt : public simulation::Radar
{
    DECLARE_SUBCLASS(Stt, simulation::Radar)

public:
    Stt();

protected:
   virtual void dynamics(const double dt) override;
};

}
}

#endif
