#include "openeaagles/simulation/Designator.h"

namespace oe {
namespace simulation {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Designator,"Designator")
EMPTY_SERIALIZER(Designator)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Designator::Designator() : data()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Designator::copyData(const Designator& org, const bool)
{
    BaseClass::copyData(org);
}

void Designator::deleteData()
{
}

}
}
