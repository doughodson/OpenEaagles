//------------------------------------------------------------------------------
// Class:  Color
//------------------------------------------------------------------------------
#ifndef __oe_basic_Color_H__
#define __oe_basic_Color_H__

#include "openeaagles/basic/Object.h"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/osg/Vec4"

namespace oe {
namespace basic {

class Number;

//------------------------------------------------------------------------------
// Class: Color
// Base class: Object-> Color
//
// Description: General Purpose Color class; used with RGB and HSV classes
//
// Factory name: Color
//
//
// Public methods:
//
//      LCreal red()
//      LCreal green()
//      LCreal blue()
//      LCreal alpha()
//          Data access routines.  Return the individual color components
//          as floats with a range of 0.0 to 1.0.
//
//      LCreal getDefaultAlpha()
//          Returns defaultAlpha (the default alpha value).
//
//      void setDefaultAlpha(LCreal alpha)
//          Sets defaultAlpha (the default alpha value).
//
//      const osg::Vec3*()
//      const osg::Vec3* getRGB()
//          Convert a color to an osg::Vec3* RGB vector.
//
//      const osg::Vec4*()
//      const osg::Vec4* getRGBA()
//          Converts a color to an osg::Vec4* RGBA vector.
//
//      Comparison operators: ==  !=
//          Are C++ equivalents.
//
// Enumerated:
//      { RED, GREEN, BLUE, ALPHA }
//          Used to index the Red, Green and Blue (RGB) color vectors
//------------------------------------------------------------------------------
class Color : public Object
{
    DECLARE_SUBCLASS(Color, Object)

public:
    // Components of RGB color
    enum { RED, GREEN, BLUE, ALPHA };

public:
    Color();

    operator const osg::Vec3*() const;
    operator const osg::Vec4*() const;

    LCreal red() const;
    LCreal green() const;
    LCreal blue() const;
    LCreal alpha() const;

    virtual bool setRed(const LCreal r);
    virtual bool setGreen(const LCreal g);
    virtual bool setBlue(const LCreal b);
    virtual bool setAlpha(const LCreal a);

    const osg::Vec3* getRGB() const;
    const osg::Vec4* getRGBA() const;

    static LCreal getDefaultAlpha();
    static void setDefaultAlpha(const LCreal alpha);

protected:
    osg::Vec4 color;                // RGBA color vector
    static LCreal defaultAlpha;
};


} // End basic namespace
} // End oe namespace

#endif
