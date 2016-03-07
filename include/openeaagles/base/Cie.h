
#ifndef __oe_base_Cie_H__
#define __oe_base_Cie_H__

#include "openeaagles/base/Color.h"
#include "openeaagles/base/osg/Matrix"

namespace oe {
namespace base {

class MonitorMetrics;
class Table1;
class List;

//------------------------------------------------------------------------------
// Class: Cie
// Base class: Object -> Color -> Cie
//
// Description:  Defines a color by Luminance, X, Y, and monitor-specific data.
//
// Factory name: cie
// Slots:
//     luminance <Number>         ! Luminance component (0.0 to 1.0) (default: 0)
//     x         <Number>         ! Green component     (0.0 to 1.0) (default: 0)
//     y         <Number>         ! Blue component      (0.0 to 1.0, x+y<=1) (default: 0)
//     monitor   <MonitorMetrics> ! Monitor characteristics
//
//
// Public methods: Base class public methods, plus ...
//
//     Cie(const MonitorMetrics* m, const LCreal l, const LCreal x, const LCreal y);
//         Special constructor to initialize the CIE color with the given values.
//
//     LCreal luminance()
//     LCreal x()
//     LCreal y()
//         Data access routines.  Returns the CIE component.
//
//     getCIE(osg::Vec3& cie)
//         Returns the CIE components in a vector.
//
//     static void cie2rgb(osg::Vec4& rgba, const osg::Vec3& cie, const MonitorMetrics* m)
//         Static function to convert a CIE color into RGB.
//
// Enumerated:
//     { LUMINANCE, X, Y }
//         Used to index the CIE color vector.
//
//
// Note:  The operators osg::Vec3*() and osg::Vec4*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        CIE color vector.
//------------------------------------------------------------------------------
class Cie : public Color {
    DECLARE_SUBCLASS(Cie,Color)

public:
    // components of CIE color
    enum { LUMINANCE, X, Y };

public:
    Cie(const MonitorMetrics* m, const LCreal l, const LCreal x, const LCreal y);
    Cie();

    LCreal luminance() const;
    LCreal x() const;
    LCreal y() const;
    void getCIE(osg::Vec3& cie) const;

    virtual bool setMonitor(MonitorMetrics* const msg);
    virtual bool setLuminance(Number* const msg);
    virtual bool setX(Number* const msg);
    virtual bool setY(Number* const msg);

    static void cie2rgb(osg::Vec4& rgba, const osg::Vec3& cie, const MonitorMetrics* m);

protected:
    osg::Vec3 cie;
    safe_ptr<const MonitorMetrics> monitor;
};


}
}

#endif

