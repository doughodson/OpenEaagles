
#include "openeaagles/models/sensor/Tws.hpp"
#include <iostream>

namespace oe {
namespace models {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Tws, "Tws")
EMPTY_DELETEDATA(Tws)

Tws::Tws()
{
    STANDARD_CONSTRUCTOR()
}

void Tws::copyData(const Tws& org, const bool)
{
    BaseClass::copyData(org);
}

std::ostream& Tws::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    // ###DPG need to print slots 
    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
