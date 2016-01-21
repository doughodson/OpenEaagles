#include "openeaagles/simulation/Ships.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/units/Angles.h"

namespace oe {
namespace Simulation {

//==============================================================================
// class Ship
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Ship,"Ship")
EMPTY_SERIALIZER(Ship)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Ship::Ship()
{
    STANDARD_CONSTRUCTOR()
    static basic::String generic("GenericShip");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Ship::copyData(const Ship& org, const bool)
{
    BaseClass::copyData(org);
}

void Ship::deleteData()
{
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int Ship::getMajorType() const
{
    return SHIP;
}

} // End Simulation namespace
} // End oe namespace
