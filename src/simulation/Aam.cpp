
#include "openeaagles/simulation/Aam.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Aam class -- Generic Air to Air Missile
//==============================================================================

IMPLEMENT_SUBCLASS(Aam,"AamMissile")
EMPTY_SLOTTABLE(Aam)
EMPTY_SERIALIZER(Aam)
EMPTY_COPYDATA(Aam)
EMPTY_DELETEDATA(Aam)

int Aam::getCategory() const               { return (MISSILE | GUIDED); }
const char* Aam::getDescription() const    { return "AIM"; }
const char* Aam::getNickname() const       { return "GenericAam"; }

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Aam::Aam()
{
    STANDARD_CONSTRUCTOR()
        
   static Basic::String gaam(getDescription());
   setType(&gaam);

   // Default guidance & dynamics for generic A/A missile
   // (derived models will override these values)
    setMaxTOF(60.0);
    setLethalRange(30.0f);
    setMaxBurstRng(150.0f);
    setSOBT(0.0f);
    setEOBT(50.0f);
    setTSG(1.0);
    setVpMin(0.0);
    setVpMax(800.0f);
    setVpMaxG(800.0f);
    setMaxG(5.0);
    setMaxAccel(50.0);
}

} // End Simulation namespace
} // End Eaagles namespace
