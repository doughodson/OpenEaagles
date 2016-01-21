//------------------------------------------------------------------------------
// Class: Cmy
//------------------------------------------------------------------------------
#ifndef __oe_Basic_Cmy_H__
#define __oe_Basic_Cmy_H__

#include "openeaagles/basic/Color.h"

namespace oe {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  Cmy
// Base class:  Object -> Color -> Cmy
//
// Description:  Defines a color by Cyan, Magenta, and Yellow.
//
//
// Factory name: cmy
// Slots:
//    cyan     <Number>  ! Cyan component (0.0 to 1.0) (default: 0)
//    magenta  <Number>  ! Magenta component (0.0 to 1.0) (default: 0)
//    yellow   <Number>  ! Yellow component (0.0 to 1.0) (default: 0)
//
//
// Public methods: Base class public methods, plus ...
//
//      Cmy(const LCreal c, const LCreal m, const LCreal y)
//          Special constructor that initializes the object to c, m, and y
//
//      LCreal cyan()
//      LCreal magenta()
//      LCreal yellow()
//          Data access routines.  Returns the individual CMY components.
//
//      getCMY(osg::Vec3 cmy)
//          Returns the CMY components in a vector.
//
//      static void cmy2rgb(osg::Vec4& rgb, const osg::Vec3& cmy);
//      static void rgb2cmy(osg::Vec3& cmy, const osg::Vec4& rgb);
//          Static functions to convert CMY colors to RGB and visa versa.
//
// Enumerated:
//      { CYAN, MAGENTA, YELLOW }
//          Used to index the CMY color vectors.
//
//
// Note:  The operators osg::Vec3*() and osg::Vec4*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        CMY color vector.
//------------------------------------------------------------------------------
class Cmy : public Color {
    DECLARE_SUBCLASS(Cmy,Color)

public:
    // components of CMY color
    enum { CYAN, MAGENTA, YELLOW };

public:
    Cmy(const LCreal c, const LCreal m, const LCreal y);
    Cmy();

    LCreal cyan() const;
    LCreal magenta() const;
    LCreal yellow() const;
    void getCMY(osg::Vec3& cmy) const;

    virtual bool setCyan(Number* const msg);
    virtual bool setMagenta(Number* const msg);
    virtual bool setYellow(Number* const msg);

    static void cmy2rgb(osg::Vec4& rgb, const osg::Vec3& cmy);
    static void rgb2cmy(osg::Vec3& cmy, const osg::Vec4& rgb);

protected:
    osg::Vec3 cmy;
};


} // End Basic namespace
} // End oe namespace

#endif
