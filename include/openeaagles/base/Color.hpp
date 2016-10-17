
#ifndef __oe_base_Color_H__
#define __oe_base_Color_H__

#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/osg/Vec3"
#include "openeaagles/base/osg/Vec4"

namespace oe {
namespace base {

class Number;

//------------------------------------------------------------------------------
// Class: Color
//
// Description: General Purpose Color class; used with RGB and HSV classes
//
// Factory name: Color
//
//
// Public methods:
//
//      double red()
//      double green()
//      double blue()
//      double alpha()
//          Data access routines.  Return the individual color components
//          as floats with a range of 0.0 to 1.0.
//
//      double getDefaultAlpha()
//          Returns defaultAlpha (the default alpha value).
//
//      void setDefaultAlpha(double alpha)
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

    double red() const;
    double green() const;
    double blue() const;
    double alpha() const;

    virtual bool setRed(const double r);
    virtual bool setGreen(const double g);
    virtual bool setBlue(const double b);
    virtual bool setAlpha(const double a);

    const osg::Vec3* getRGB() const;
    const osg::Vec4* getRGBA() const;

    static double getDefaultAlpha();
    static void setDefaultAlpha(const double alpha);

protected:
    osg::Vec4 color;                // RGBA color vector
    static double defaultAlpha;
};


}
}

#endif
