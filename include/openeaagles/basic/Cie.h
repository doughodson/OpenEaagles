//------------------------------------------------------------------------------
// Classes: Cie, MonitorMetrics
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Cie_H__
#define __Eaagles_Basic_Cie_H__

#include "openeaagles/basic/Color.h"
#include "openeaagles/basic/osg/Matrix"

namespace Eaagles {
namespace Basic {

class MonitorMetrics;
class Table1;
class List;

//------------------------------------------------------------------------------
// Class: Cie
// Base class: Object -> Color -> Cie
//
// Description:  Defines a color by Luminance, X, Y, and monitor-specific data.
//
// Form name: cie
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
    SPtr<const MonitorMetrics> monitor;
};


//------------------------------------------------------------------------------
// Class: MonitorMetrics
// Base class:  Object -> MonitorMetrics
//
// Description:  Defines the characteristics of a monitor, needed to accurately match color.
//
// Form name: monitorMetrics
// Slots:
//     red       <Table1>   ! Red luminance vs RGB level ( both luminance and RGB level range from 0..1 )
//                          ! (Default: redLumTbl)
//     green     <Table1>   ! Green luminance vs RGB level (default: greenLumTbl)
//     blue      <Table1>   ! Blue luminance vs RGB level (default: blueLumTbl)
//     phosphors <List>     ! Phosphor coordinates ( 6 element list, 2 each for red, green, blue )
//                          ! (default: phosphorCoordMatrix)
//     whiteRGB  <List>     ! RGB value of reference white ( 3 element list ) (default: whiteRGB)
//     whiteCIE  <List>     ! CIE coordinate of reference white ( 3 element list ) (default: whiteCIE)
//
// Events: None
//
//
// Public methods: Base class public methods, plus ...
//
//     MonitorMetrics(const Table1* redLuminance, const Table1* greenLuminance, const Table1* blueLuminance, 
//                    const osg::Matrix& phosphorCoordinates, const osg::Vec3& whiteRGB, const osg::Vec3& whiteCIE);
//         Special constructor to initialize the object with the given values.
//
//     cie2rgb(osg::Vec4& rgba, const osg::Vec3& cie)
//         Convert a CIE color into an RGB value for this monitor.
//------------------------------------------------------------------------------
class MonitorMetrics : public Object {
    DECLARE_SUBCLASS(MonitorMetrics,Object)

public:
    MonitorMetrics();
    MonitorMetrics(const Table1* redLuminance, const Table1* greenLuminance, const Table1* blueLuminance, 
                   const osg::Matrix& phosphorCoordinates, const osg::Vec3& whiteRGB, const osg::Vec3& whiteCIE);
    bool setSlotRed(const Table1* red);
    bool setSlotGreen(const Table1* green);
    bool setSlotBlue(const Table1* blue);
    bool setSlotPhosphors(const List* phosphors);
    bool setSlotWhiteRGB(const List* whiteRGB);
    bool setSlotWhiteCIE(const List* whiteCIE);

    void cie2rgb(osg::Vec4& rgba, const osg::Vec3& cie) const;

private:
    bool computeMatrix();

    // transform matrix from CIE to RGB for this monitor
    osg::Matrix transform;

    // RGB luminance vs RGB level on this monitor
    SPtr<const Table1> redLuminance;
    SPtr<const Table1> greenLuminance;
    SPtr<const Table1> blueLuminance;

    // CIE coordinates of Red, Green, and Blue for this monitor
    osg::Matrix phosphorCoordinates;

    // CIE and RGB coordinates of a reference white
    osg::Vec3 refwhiteRGB;
    osg::Vec3 refwhiteCIE;
};

} // End Basic namespace
} // End Eaagles namespace


#endif

