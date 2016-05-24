//------------------------------------------------------------------------------
// Class: Building
// Description: Generic building
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Building_H__
#define __oe_simulation_Building_H__

#include "openeaagles/simulation/Player.h"

namespace oe {
namespace simulation {

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

} // End simulation namespace
}

#endif
