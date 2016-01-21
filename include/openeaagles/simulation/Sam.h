//------------------------------------------------------------------------------
// Class: Sam
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_Sam_H__
#define __oe_Simulation_Sam_H__

#include "openeaagles/simulation/Missile.h"

namespace oe {
namespace Simulation {

//------------------------------------------------------------------------------
// Class: Sam
//
// Description: Base class for Surface to Air (SAM) Missiles
//
// Factory name: Sam
//------------------------------------------------------------------------------
class Sam : public Missile
{
    DECLARE_SUBCLASS(Sam,Missile)

public:
    Sam();

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
};

} // End Simulation namespace
} // End oe namespace

#endif
