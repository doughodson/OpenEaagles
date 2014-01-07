//------------------------------------------------------------------------------
// Class: Building
// Description: Generic building
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Building_H__
#define __Eaagles_Simulation_Building_H__

#include "openeaagles/simulation/Player.h"

namespace Eaagles {
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

    // Player interface
    virtual unsigned int getMajorType() const;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
