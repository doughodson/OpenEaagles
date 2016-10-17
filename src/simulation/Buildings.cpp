#include "openeaagles/simulation/Buildings.hpp"

#include "openeaagles/base/List.hpp"
#include "openeaagles/base/osg/Matrix"
#include "openeaagles/base/units/Angles.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Building,"Building")
EMPTY_SERIALIZER(Building)
EMPTY_DELETEDATA(Building)

Building::Building()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericBuilding");
    setType(&generic);
}

void Building::copyData(const Building& org, const bool)
{
    BaseClass::copyData(org);
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int Building::getMajorType() const
{
    return BUILDING;
}

}
}
