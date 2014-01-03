//------------------------------------------------------------------------------
// Class: Sam
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Sam_H__
#define __Eaagles_Simulation_Sam_H__

#include "openeaagles/simulation/Missile.h"

namespace Eaagles {
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
    
    // Weapon interface
    virtual const char* getDescription() const;
    virtual const char* getNickname() const;
    virtual int getCategory() const;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
