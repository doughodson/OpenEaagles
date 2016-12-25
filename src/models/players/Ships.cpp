
#include "openeaagles/models/players/Ships.hpp"

#include "openeaagles/base/List.hpp"
#include "openeaagles/base/osg/Matrixd"
#include "openeaagles/base/units/Angles.hpp"

namespace oe {
namespace models {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Ship,"Ship")
EMPTY_SERIALIZER(Ship)

Ship::Ship()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericShip");
    setType(&generic);
}

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

}
}
