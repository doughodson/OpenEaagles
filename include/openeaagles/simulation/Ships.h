//------------------------------------------------------------------------------
// Class: Ship
// Description: Generic ship models
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Ship_H__
#define __Eaagles_Simulation_Ship_H__

#include "openeaagles/simulation/Player.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class Ship
// Description: Generic ship model
// Factory name: Ship
//==============================================================================
class Ship : public Player
{
    DECLARE_SUBCLASS(Ship, Player)

public:
    Ship();

    unsigned int getMajorType() const override;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
