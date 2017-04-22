
#ifndef __oe_base_Hsv_H__
#define __oe_base_Hsv_H__

#include "openeaagles/base/Color.hpp"
#include "openeaagles/base/osg/Vec3d"
#include "openeaagles/base/osg/Vec4d"

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: Hsv
//
// Description:  Defines a color by Hue, Saturation and Value (see below).
//
//  Hues:  degrees around a color wheel
//      red          0.0
//      yellow      60.0
//      green      120.0
//      cyan       180.0
//      blue       240.0
//      magenta    300.0
//      red        360.0
//
//  Saturation:  pure hue    1.0
//         no hue (white)    0.0
//
//  Value:  max intensity    1.0
//    no intensity (black)   0.0
//
//
// Factory name: hsv
// Slots:
//     hue        <Number>  ! Set the hue component (0.0 to 360.0, default: 0.0)
//     saturation <Number>  ! Set the saturation component (0.0 to 1.0, default: 0.0)
//     value      <Number>  ! Set the value component (0.0 to 1.0, default: 0.0)
//
//
// Public methods: Base class public methods, plus ...
//
//      Hsv(const double h, const double s, const double v)
//          Special constructor that initializes the object to h, s, and v
//
//      double hue()
//      double saturation()
//      double value()
//          Data access routines.  Returns the individual HSV components.
//
//      getHSV(Vec3d hsv)
//          Returns the HSV components in a vector.
//
//      getHSV(Vec3d& vec) const;
//          Returns the hsv vector
//
//      setHSV(const Vecd3& vec);
//          Sets the hsv vector
//
//      virtual bool setHue(Number* const msg);
//          Sets the hue (see Hsv.hpp)
//      virtual bool setSaturation(Number* const msg);
//          Sets the saturation (see Hsv.hpp)
//      virtual bool setValue(Number* const msg);
//          Sets the value (see Hsv.hpp)
//      virtual bool setAlpha(Number* const msg);
//          Sets the alpha value
//
//      static void hsv2rgb(Vec4d& rgb, const Vec4d& hsv);
//      static void rgb2hsv(Vec4d& hsv, const Vec4d& rgb);
//          Static functions to convert HSVA colors to RGBA and visa versa.
//
//      static void hsv2rgb(Vec3d& rgb, const Vec3d& hsv);
//      static void rgb2hsv(Vec3d& hsv, const Vec3d& rgb);
//          Static functions to convert HSVA colors to RGB and visa versa.
//
// Enumerated:
//      { HUE, SATURATION, VALUE, ALPHA }
//          Used to index the HSV color vectors.
//
//
// Note:  The operators Vec3d*() and Vec4d*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        HSVA color vector.
//------------------------------------------------------------------------------
class Hsv : public Color
{
    DECLARE_SUBCLASS(Hsv, Color)

public:
    // Components of HSV color
    enum { HUE, SATURATION, VALUE, /* ALPHA */ };

public:
    Hsv(const double h, const double s, const double v);
    Hsv();

    double hue() const;                          // Hue value (0.0 to 360.0) (see Hsv.h)
    double saturation() const;                   // Saturation: 0.0 (white) to 1.0 (pure color)
    double value() const;                        // Value: 0.0 (black) to 1.0 (full)

    void getHSV(Vec3d& vec) const;               // Returns the hsv vector
    bool setHSV(const Vec3d& vec);               // Sets the hsv vector

    virtual bool setHue(Number* const msg);
    virtual bool setSaturation(Number* const msg);
    virtual bool setValue(Number* const msg);
    virtual bool setAlpha(Number* const msg);

    static void hsv2rgb(Vec4d& rgb, const Vec4d& hsv); // Converts an HSVA color vector to a RGBA color vector
    static void hsv2rgb(Vec3d& rgb, const Vec3d& hsv); // Converts an HSV color vector to a RGB color vector
    static void rgb2hsv(Vec4d& hsv, const Vec4d& rgb); // Converts a RGBA color vector to a HSVA color vector
    static void rgb2hsv(Vec3d& hsv, const Vec3d& rgb); // Converts a RGB color vector to a HSV color vector

protected:
    void getHSVA(Vec4d& hsva) const;
    bool setHSVA(const Vec4d& vec);

    Vec4d hsv;
};

}
}

#endif
