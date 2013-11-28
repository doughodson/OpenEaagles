//------------------------------------------------------------------------------
// Class: Aam
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Aam_H__
#define __Eaagles_Simulation_Aam_H__

#include "openeaagles/simulation/Missile.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Class: Aam
// Description: Base class for Air to Air (A/A)
//
//    Contains a simple aero and guidance model, which derived models can
//    override at will.
//
// Form name: AamMissile
//------------------------------------------------------------------------------
class Aam : public Missile
{
    DECLARE_SUBCLASS(Aam,Missile)

public:
    Aam();
    
    // Weapon interface
    virtual const char* getDescription() const;  // Returns "AIM"
    virtual const char* getNickname() const;     // Returns "GenericAam"
    virtual int getCategory() const;             // Returns MISSILE or GUIDED
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
