//------------------------------------------------------------------------------
// Class: Hls
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Hls_H__
#define __Eaagles_Basic_Hls_H__

#include "openeaagles/basic/Color.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  Hls
// Base class:  Object -> Color -> Hls
//
// Description:  Defines a color by Hue, Saturation and Lightness.
//
//
// Form name: hls
// Slots:
//     hue         <Number>  ! hue component (0.0 to 360.0, default: 0.0)
//     saturation  <Number>  ! saturation component (0.0 to 1.0, default: 0.0)
//     lightness   <Number>  ! lightness component (0.0 to 1.0, default: 0.0)
//
//
// Public methods: Base class public methods, plus ...
//
//      Hls(const LCreal h, const LCreal l, const LCreal s)
//          Special constuctor that initializes the object to h, l,  and s
//
//      LCreal hue()
//      LCreal saturation()
//      LCreal lightness()
//          Data access routines.  Returns the individual HLS components.
//
//      getHLS(osg::Vec3 hls)
//          Returns the HLS components in a vector.
//
//      static void hls2rgb(osg::Vec4& rgb, const osg::Vec3& hls);
//      static void rgb2hls(osg::Vec3& hls, const osg::Vec4& rgb);
//          Static functions to convert HLS colors to RGB and visa versa.
//
// Enumerated:
//      { HUE, LIGHTNESS, SATURATION }
//          Used to index the HLS color vectors.
//
//
// Note:  The operators osg::Vec3*() and osg::Vec4*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        HLS color vector.
//------------------------------------------------------------------------------
class Hls : public Color {
    DECLARE_SUBCLASS(Hls,Color)

public:
	// components of HLS color
	enum { HUE, LIGHTNESS, SATURATION };

public:
    Hls(const LCreal h, const LCreal l, const LCreal s);
    Hls();

    LCreal hue() const;
    LCreal lightness() const;
    LCreal saturation() const;
    void getHLS(osg::Vec3& hls) const;

    virtual bool setHue(Number* const msg);
    virtual bool setLightness(Number* const msg);
    virtual bool setSaturation(Number* const msg);

    static void hls2rgb(osg::Vec4& rgb, const osg::Vec3& hls);
    static void rgb2hls(osg::Vec3& hls, const osg::Vec4& rgb);

private:
    static LCreal value(LCreal n1, LCreal n2, LCreal hue);

protected:
    osg::Vec3 hls;
};

} // End Basic namespace
} // End Eaagles namespace

#endif
