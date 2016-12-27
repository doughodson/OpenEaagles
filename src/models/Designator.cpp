
#include "openeaagles/models/Designator.hpp"
#include <iostream>

namespace oe {
namespace models {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Designator, "Designator")
EMPTY_SERIALIZER(Designator)
EMPTY_DELETEDATA(Designator)

Designator::Designator() : data()
{
    STANDARD_CONSTRUCTOR()
}

void Designator::copyData(const Designator& org, const bool)
{
    BaseClass::copyData(org);
}

}
}
