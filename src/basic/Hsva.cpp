//------------------------------------------------------------------------------
// Hsva
//------------------------------------------------------------------------------
#include "openeaagles/basic/Hsva.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/units/Angles.h"

namespace oe {
namespace Basic {

IMPLEMENT_SUBCLASS(Hsva,"hsva")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Hsva)
    "alpha",    // alpha component,   range(0.0f .. 1.0f)
END_SLOTTABLE(Hsva)

// Map slot table to handles 
BEGIN_SLOT_MAP(Hsva)
    ON_SLOT(1,setAlpha,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Hsva::Hsva(const LCreal h, const LCreal s,
               const LCreal v, const LCreal a) : Hsv(h,s,v)
{
    STANDARD_CONSTRUCTOR()
    hsv[ALPHA] = a;
}

Hsva::Hsva()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
EMPTY_COPYDATA(Hsva)
EMPTY_DELETEDATA(Hsva)


//------------------------------------------------------------------------------
// Color map interpolation - given a value, min & max limit,
// and HSV colors for the min & max limits.  This routine will
// interpolate the HSV colors between the min & max limits to
// find the HSV color for the given value.
//------------------------------------------------------------------------------
bool Hsva::colorInterpolate(
      const LCreal value,      // Value
      const LCreal minValue,   // Minimum Value
      const LCreal maxValue,   // Maximum Value 
      const Hsva& minColor,  // Minimum HSV color
      const Hsva& maxColor   // Minimum HSV color
 )
{
   LCreal p = (value - minValue) / (maxValue - minValue );
   osg::Vec4 deltaColor = maxColor.hsv - minColor.hsv;
   deltaColor[Basic::Hsv::HUE] = lcAepcDeg(deltaColor[Basic::Hsv::HUE]);
   osg::Vec4 newColor = minColor.hsv + deltaColor * p;
   newColor[Basic::Hsv::HUE] = lcAepcDeg(newColor[Basic::Hsv::HUE]);
   if (newColor[Basic::Hsv::HUE] < 0) {
      newColor[Basic::Hsv::HUE] += 360.0f;
   }
   setHSVA(newColor);
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Hsva
//------------------------------------------------------------------------------
Object* Hsva::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Hsva::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "alpha:      " << alpha() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Basic namespace
} // End oe namespace
