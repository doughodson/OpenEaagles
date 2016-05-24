
#include "openeaagles/simulation/Sam.h"

namespace oe {
namespace simulation {

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

   static base::String gsam(getDescription());
   setType(&gsam);
}

} // End simulation namespace
}
