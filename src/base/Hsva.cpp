
#include "openeaagles/base/Hsva.hpp"
#include "openeaagles/base/Float.hpp"
#include "openeaagles/base/units/Angles.hpp"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Hsva, "hsva")
EMPTY_COPYDATA(Hsva)
EMPTY_DELETEDATA(Hsva)

BEGIN_SLOTTABLE(Hsva)
    "alpha",    // alpha component,   range(0.0f .. 1.0f)
END_SLOTTABLE(Hsva)

// Map slot table to handles 
BEGIN_SLOT_MAP(Hsva)
    ON_SLOT(1,setAlpha,Number)
END_SLOT_MAP()

Hsva::Hsva(const double h, const double s,
               const double v, const double a) : Hsv(h,s,v)
{
    STANDARD_CONSTRUCTOR()
    hsv[ALPHA] = a;
}

Hsva::Hsva()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// Color map interpolation - given a value, min & max limit,
// and HSV colors for the min & max limits.  This routine will
// interpolate the HSV colors between the min & max limits to
// find the HSV color for the given value.
//------------------------------------------------------------------------------
bool Hsva::colorInterpolate(
      const double value,      // Value
      const double minValue,   // Minimum Value
      const double maxValue,   // Maximum Value 
      const Hsva& minColor,  // Minimum HSV color
      const Hsva& maxColor   // Minimum HSV color
 )
{
   const double p = (value - minValue) / (maxValue - minValue );
   osg::Vec4d deltaColor = maxColor.hsv - minColor.hsv;
   deltaColor[base::Hsv::HUE] = base::Angle::aepcdDeg(deltaColor[base::Hsv::HUE]);
   osg::Vec4d newColor = minColor.hsv + deltaColor * p;
   newColor[base::Hsv::HUE] = base::Angle::aepcdDeg(newColor[base::Hsv::HUE]);
   if (newColor[base::Hsv::HUE] < 0) {
      newColor[base::Hsv::HUE] += 360.0f;
   }
   setHSVA(newColor);
   return true;
}

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

}
}
