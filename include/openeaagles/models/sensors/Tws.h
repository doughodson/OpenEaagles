//------------------------------------------------------------------------------
// Class: Tws
// Description: Simple, Track-While-Scan Mode Radar
//------------------------------------------------------------------------------
#ifndef __oe_models_Tws_H__
#define __oe_models_Tws_H__

#include "openeaagles/simulation/Radar.h"

namespace oe {
namespace models {

class Tws : public simulation::Radar
{
    DECLARE_SUBCLASS(Tws,simulation::Radar)

public:
    Tws();

private:

};

}
}

#endif
