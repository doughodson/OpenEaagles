
#ifndef __oe_models_Tws_H__
#define __oe_models_Tws_H__

#include "openeaagles/simulation/Radar.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Tws
// Description: Simple, Track-While-Scan Mode Radar
//------------------------------------------------------------------------------
class Tws : public simulation::Radar
{
    DECLARE_SUBCLASS(Tws, simulation::Radar)

public:
    Tws();

private:

};

}
}

#endif
