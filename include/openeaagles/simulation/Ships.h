//------------------------------------------------------------------------------
// Class: Ship
// Description: Generic ship models
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_Ship_H__
#define __oe_Simulation_Ship_H__

#include "openeaagles/simulation/Player.h"

namespace oe {
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
} // End oe namespace

#endif
