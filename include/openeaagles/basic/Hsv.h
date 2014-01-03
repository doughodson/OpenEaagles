//------------------------------------------------------------------------------
// Class: Hsv
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Hsv_H__
#define __Eaagles_Basic_Hsv_H__

#include "openeaagles/basic/Color.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  Hsv
// Base class:  Object -> Color -> Hsv
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
//      Hsv(const LCreal h, const LCreal s, const LCreal v)
//          Special constructor that initializes the object to h, s, and v
//
//      LCreal hue()
//      LCreal saturation()
//      LCreal value()
//          Data access routines.  Returns the individual HSV components.
//
//      getHSV(osg::Vec3 hsv)
//          Returns the HSV components in a vector.
//
//      getHSV(osg::Vec3& vec) const;                
//          Returns the hsv vector

//      setHSV(const osg::Vec3& vec);                
//          Sets the hsv vector
//
//      virtual bool setHue(Number* const msg);           
//          Sets the hue (see Hsv.h)
//      virtual bool setSaturation(Number* const msg);    
//          Sets the saturation (see Hsv.h)
//      virtual bool setValue(Number* const msg);         
//          Sets the value (see Hsv.h)
//      virtual bool setAlpha(Number* const msg);         
//          Sets the alpha value
//
//      static void hsv2rgb(osg::Vec4& rgb, const osg::Vec4& hsv);
//      static void rgb2hsv(osg::Vec4& hsv, const osg::Vec4& rgb);
//          Static functions to convert HSVA colors to RGBA and visa versa.
//
//      static void hsv2rgb(osg::Vec3& rgb, const osg::Vec3& hsv);
//      static void rgb2hsv(osg::Vec3& hsv, const osg::Vec3& rgb);
//          Static functions to convert HSVA colors to RGB and visa versa.
//
// Enumerated:
//      { HUE, SATURATION, VALUE, ALPHA }
//          Used to index the HSV color vectors.
//
//
// Note:  The operators osg::Vec3*() and osg::Vec4*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        HSVA color vector.
//------------------------------------------------------------------------------
class Hsv : public Color {
    DECLARE_SUBCLASS(Hsv,Color)

public:
    // Components of HSV color
    enum { HUE, SATURATION, VALUE, /* ALPHA */ };

public:
    Hsv(const LCreal h, const LCreal s, const LCreal v);
    Hsv();

    LCreal hue() const;                               // Hue value (0.0 to 360.0) (see Hsv.h)
    LCreal saturation() const;                        // Saturation: 0.0 (white) to 1.0 (pure color)
    LCreal value() const;                             // Value: 0.0 (black) to 1.0 (full)

    void getHSV(osg::Vec3& vec) const;                // Returns the hsv vector
    bool setHSV(const osg::Vec3& vec);                // Sets the hsv vector

    virtual bool setHue(Number* const msg);         // Sets the hue (see Hsv.h)
    virtual bool setSaturation(Number* const msg);  // Sets the saturation (see Hsv.h)
    virtual bool setValue(Number* const msg);       // Sets the value (see Hsv.h)
    virtual bool setAlpha(Number* const msg);       // sets the alpha value

    static void hsv2rgb(osg::Vec4& rgb, const osg::Vec4& hsv); // Converts an HSVA color vector to a RGBA color vector
    static void hsv2rgb(osg::Vec3& rgb, const osg::Vec3& hsv); // Converts an HSV color vector to a RGB color vector
    static void rgb2hsv(osg::Vec4& hsv, const osg::Vec4& rgb); // Converts a RGBA color vector to a HSVA color vector
    static void rgb2hsv(osg::Vec3& hsv, const osg::Vec3& rgb); // Converts a RGB color vector to a HSV color vector

protected:
    void getHSVA(osg::Vec4& hsva) const;
    bool setHSVA(const osg::Vec4& vec);

    osg::Vec4 hsv;
};

} // End Basic namespace
} // End Eaagles namespace

#endif
