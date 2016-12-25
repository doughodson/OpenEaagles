
#include "openeaagles/models/navigation/Bullseye.hpp"
#include <iostream>

namespace oe {
namespace models {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Bullseye, "Bullseye")
EMPTY_SERIALIZER(Bullseye)
EMPTY_DELETEDATA(Bullseye)

Bullseye::Bullseye()
{
    STANDARD_CONSTRUCTOR()
}

void Bullseye::copyData(const Bullseye& org, const bool)
{
    BaseClass::copyData(org);
}

}
}
