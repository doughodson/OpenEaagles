//------------------------------------------------------------------------------
// Class: Hsva
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Hsva_H__
#define __Eaagles_Basic_Hsva_H__

#include "openeaagles/basic/Hsv.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  Hsva
// Base class:  Object -> Color -> Hsv -> Hsva
//
// Description:  Defines a color by Hue, Saturation, Value and Alpha
//
// Factory name: hsva
// Slots:
//    alpha      <Number>  ! Set the alpha component (0.0 to 1.0) (default: 1.0)
//
//
// Public methods: Base class public methods, plus ...
//
//      Hsva(LCreal h, LCreal s, LCreal v, LCreal a)
//          Special constructor that initializes the object to h, s, v and a
//
//      getHSVA(osg::Vec4 hsv)
//          Returns the HSVA components in a vector.
//
//      setHSVA(const osg::Vec4& vec)
//          Sets the HSVA components in a vector.
//
//      bool Hsva::colorInterpolate( const LCreal value, const LCreal minValue,
//                                   const LCreal maxValue,  const Hsva& minColor,
//                                   const Hsva& maxColor}
//          Color map interpolation - given a value, min & max limit, and HSV colors
//          for the min & max limits.  This routine will interpolate the HSV colors
//          between the min & max limits to find the HSV color for the given value.
//
// Note:  The operators osg::Vec3*() and osg::Vec4*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        HSVA color vector.
//
//------------------------------------------------------------------------------
class Hsva : public Hsv {
    DECLARE_SUBCLASS(Hsva,Hsv)

public:
    Hsva(const LCreal h, const LCreal s, const LCreal v, const LCreal a);
    Hsva();

    void getHSVA(osg::Vec4& hsva) const   { Hsv::getHSVA(hsva);        }
    bool setHSVA(const osg::Vec4& vec)    { return Hsv::setHSVA(vec);  }

    bool colorInterpolate(
      const LCreal value,      // Value
      const LCreal minValue,   // Minimum Value
      const LCreal maxValue,   // Maximum Value
      const Hsva& minColor,  // Minimum HSV color
      const Hsva& maxColor   // Minimum HSV color
    );
};

} // End Basic namespace
} // End Eaagles namespace

#endif
