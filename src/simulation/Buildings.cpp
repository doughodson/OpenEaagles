#include "openeaagles/simulation/Buildings.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/units/Angles.h"

namespace oe {
namespace Simulation {

//==============================================================================
// class Building
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Building,"Building")
EMPTY_SERIALIZER(Building)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Building::Building()
{
    STANDARD_CONSTRUCTOR()
    static basic::String generic("GenericBuilding");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Building::copyData(const Building& org, const bool)
{
    BaseClass::copyData(org);
}

void Building::deleteData()
{
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int Building::getMajorType() const
{
    return BUILDING;
}

} // End Simulation namespace
} // End oe namespace
