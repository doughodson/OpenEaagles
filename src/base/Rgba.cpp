//------------------------------------------------------------------------------
// Rgba
//------------------------------------------------------------------------------
#include "openeaagles/base/Rgba.h"
#include "openeaagles/base/Float.h"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Rgba,"rgba")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Rgba)
    "alpha",    // 1: alpha component,   range(0.0 to 1.0)
END_SLOTTABLE(Rgba)

// Map slot table to handles 
BEGIN_SLOT_MAP(Rgba)
    ON_SLOT(1,setSlotAlpha,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
EMPTY_COPYDATA(Rgba)
EMPTY_DELETEDATA(Rgba)



//------------------------------------------------------------------------------
// getSlotByIndex() for Rgba
//------------------------------------------------------------------------------
Object* Rgba::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
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

} // End base namespace
} // End oe namespace
