
#ifndef __oe_base_MonitorMetrics_H__
#define __oe_base_MonitorMetrics_H__

#include "openeaagles/base/Color.hpp"
#include "openeaagles/base/osg/Matrixd"
#include "openeaagles/base/safe_ptr.hpp"
#include "openeaagles/base/osg/Vec3d"
#include "openeaagles/base/osg/Vec4d"

namespace oe {
namespace base {

class Table1;
class List;

//------------------------------------------------------------------------------
// Class: MonitorMetrics
//
// Description:  Defines the characteristics of a monitor, needed to accurately match color.
//
// Factory name: monitorMetrics
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
class MonitorMetrics : public Object
{
    DECLARE_SUBCLASS(MonitorMetrics, Object)

public:
    MonitorMetrics();
    MonitorMetrics(const Table1* redLuminance, const Table1* greenLuminance, const Table1* blueLuminance,
                   const osg::Matrixd& phosphorCoordinates, const osg::Vec3d& whiteRGB, const osg::Vec3d& whiteCIE);
    bool setSlotRed(const Table1* red);
    bool setSlotGreen(const Table1* green);
    bool setSlotBlue(const Table1* blue);
    bool setSlotPhosphors(const List* phosphors);
    bool setSlotWhiteRGB(const List* whiteRGB);
    bool setSlotWhiteCIE(const List* whiteCIE);

    void cie2rgb(osg::Vec4d& rgba, const osg::Vec3d& cie) const;

private:
    bool computeMatrix();

    // transform matrix from CIE to RGB for this monitor
    osg::Matrixd transform;

    // RGB luminance vs RGB level on this monitor
    safe_ptr<const Table1> redLuminance;
    safe_ptr<const Table1> greenLuminance;
    safe_ptr<const Table1> blueLuminance;

    // CIE coordinates of Red, Green, and Blue for this monitor
    osg::Matrixd phosphorCoordinates;

    // CIE and RGB coordinates of a reference white
    osg::Vec3d refwhiteRGB;
    osg::Vec3d refwhiteCIE;
};

}
}


#endif

