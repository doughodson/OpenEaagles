
#ifndef __oe_base_Color_H__
#define __oe_base_Color_H__

#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/osg/Vec4d"

namespace oe {
namespace base {
class Vec3d;
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
//      const Vec3d*()
//      const Vec3d* getRGB()
//          Convert a color to an Vec3d* RGB vector.
//
//      const Vec4d*()
//      const Vec4d* getRGBA()
//          Converts a color to an Vec4d* RGBA vector.
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

    operator const Vec3d*() const;
    operator const Vec4d*() const;

    double red() const;
    double green() const;
    double blue() const;
    double alpha() const;

    virtual bool setRed(const double r);
    virtual bool setGreen(const double g);
    virtual bool setBlue(const double b);
    virtual bool setAlpha(const double a);

    const Vec3d* getRGB() const;
    const Vec4d* getRGBA() const;

    static double getDefaultAlpha();
    static void setDefaultAlpha(const double alpha);

protected:
    Vec4d color;                // RGBA color vector
    static double defaultAlpha;
};

}
}

#endif
