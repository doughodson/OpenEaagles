
#ifndef __oe_models_Tws_H__
#define __oe_models_Tws_H__

#include "openeaagles/models/systems/Radar.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Tws
// Description: Simple, Track-While-Scan Mode Radar
//------------------------------------------------------------------------------
class Tws : public Radar
{
    DECLARE_SUBCLASS(Tws, Radar)

public:
    Tws();

private:

};

}
}

#endif
