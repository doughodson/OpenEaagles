
#include "openeaagles/base/Rgba.hpp"
#include "openeaagles/base/Float.hpp"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Rgba, "rgba")
EMPTY_COPYDATA(Rgba)
EMPTY_DELETEDATA(Rgba)

BEGIN_SLOTTABLE(Rgba)
    "alpha",    // 1: alpha component,   range(0.0 to 1.0)
END_SLOTTABLE(Rgba)

BEGIN_SLOT_MAP(Rgba)
    ON_SLOT(1,setSlotAlpha,Number)
END_SLOT_MAP()

Rgba::Rgba(const double r, const double g,
               const double b, const double a) : Rgb(r,g,b)
{
   STANDARD_CONSTRUCTOR()
   color[Color::ALPHA] = a;
}

Rgba::Rgba()
{
   STANDARD_CONSTRUCTOR()
}

std::ostream& Rgba::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "alpha: " << alpha() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
