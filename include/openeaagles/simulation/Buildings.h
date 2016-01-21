//------------------------------------------------------------------------------
// Class: Building
// Description: Generic building
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_Building_H__
#define __oe_Simulation_Building_H__

#include "openeaagles/simulation/Player.h"

namespace oe {
namespace Simulation {

//==============================================================================
// Class: Building
// Description: Generic building
// Factory name: Building
//==============================================================================
class Building : public Player
{
    DECLARE_SUBCLASS(Building,Player)

public:
    Building();

    unsigned int getMajorType() const override;
};

} // End Simulation namespace
} // End oe namespace

#endif
