
#ifndef __oe_base_Cmy_H__
#define __oe_base_Cmy_H__

#include "openeaagles/base/Color.hpp"

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class:  Cmy
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
//      Cmy(const double c, const double m, const double y)
//          Special constructor that initializes the object to c, m, and y
//
//      double cyan()
//      double magenta()
//      double yellow()
//          Data access routines.  Returns the individual CMY components.
//
//      getCMY(Vec3d cmy)
//          Returns the CMY components in a vector.
//
//      static void cmy2rgb(Vec4d& rgb, const Vec3d& cmy);
//      static void rgb2cmy(Vec3d& cmy, const Vec4d& rgb);
//          Static functions to convert CMY colors to RGB and visa versa.
//
// Enumerated:
//      { CYAN, MAGENTA, YELLOW }
//          Used to index the CMY color vectors.
//
//
// Note:  The operators Vec3d*() and Vec4d*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        CMY color vector.
//------------------------------------------------------------------------------
class Cmy : public Color
{
    DECLARE_SUBCLASS(Cmy, Color)

public:
    // components of CMY color
    enum { CYAN, MAGENTA, YELLOW };

public:
    Cmy(const double c, const double m, const double y);
    Cmy();

    double cyan() const;
    double magenta() const;
    double yellow() const;
    void getCMY(Vec3d& cmy) const;

    virtual bool setCyan(Number* const msg);
    virtual bool setMagenta(Number* const msg);
    virtual bool setYellow(Number* const msg);

    static void cmy2rgb(Vec4d& rgb, const Vec3d& cmy);
    static void rgb2cmy(Vec3d& cmy, const Vec4d& rgb);

protected:
    Vec3d cmy;
};

}
}

#endif
