
#include "openeaagles/simulation/Sam.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Sam class -- Generic Surface to Air Missile
//==============================================================================

IMPLEMENT_SUBCLASS(Sam,"Sam")
EMPTY_SLOTTABLE(Sam)
EMPTY_SERIALIZER(Sam)
EMPTY_COPYDATA(Sam)
EMPTY_DELETEDATA(Sam)

int Sam::getCategory() const               { return (MISSILE | GUIDED); }
const char* Sam::getDescription() const    { return "SAM"; }
const char* Sam::getNickname() const       { return "GenericAam"; }

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Sam::Sam()
{
    STANDARD_CONSTRUCTOR()

   static Basic::String gsam(getDescription());
   setType(&gsam);
}

} // End Simulation namespace
} // End Eaagles namespace
