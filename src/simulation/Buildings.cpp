#include "openeaagles/simulation/Buildings.h"

#include "openeaagles/base/List.h"
#include "openeaagles/base/osg/Matrix"
#include "openeaagles/base/units/Angles.h"

namespace oe {
namespace simulation {

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
    static base::String generic("GenericBuilding");
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

} // End simulation namespace
}
