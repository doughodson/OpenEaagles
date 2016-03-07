
#include "openeaagles/base/MonitorMetrics.h"

#include "openeaagles/base/Cie.h"
#include "openeaagles/base/Float.h"
#include "openeaagles/base/List.h"
#include "openeaagles/base/functors/Tables.h"
#include <cstdio>

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(MonitorMetrics,"monitorMetrics")
EMPTY_SERIALIZER(MonitorMetrics)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(MonitorMetrics)
    "red",       // 1: ... Red Luminance vs RGB level
    "green",     // 2: ... Green Luminance vs RGB level
    "blue",      // 3: ... Blue Luminance vs RGB level
    "phosphors", // 4: ... Phosphor coordinates
    "whiteRGB",  // 5: ... RGB coordinate of reference white
    "whiteCIE",  // 6: ... CIE coordinate of reference white
END_SLOTTABLE(MonitorMetrics)

// Map slot table to handles
BEGIN_SLOT_MAP(MonitorMetrics)
    ON_SLOT(1,setSlotRed,Table1)
    ON_SLOT(2,setSlotGreen,Table1)
    ON_SLOT(3,setSlotBlue,Table1)
    ON_SLOT(4,setSlotPhosphors,List)
    ON_SLOT(5,setSlotWhiteRGB,List)
    ON_SLOT(6,setSlotWhiteCIE,List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
MonitorMetrics::MonitorMetrics(const Table1* redLumTbl, const Table1* greenLumTbl, const Table1* blueLumTbl,
                               const osg::Matrix& phosphorCoordMatrix, const osg::Vec3& whiteRGB, const osg::Vec3& whiteCIE)
{
    STANDARD_CONSTRUCTOR()

    redLuminance = redLumTbl;
    greenLuminance = greenLumTbl;
    blueLuminance = blueLumTbl;
    phosphorCoordinates = phosphorCoordMatrix;
    refwhiteRGB = whiteRGB;
    refwhiteCIE = whiteCIE;

    computeMatrix();
}

MonitorMetrics::MonitorMetrics()
{
    // default configuration based on data from an unknown CRT monitor
    const int npoints = 21;
    LCreal luminanceLevels[npoints] = { 0.0f,     0.05f,    0.1f,    0.15f,   0.2f,    0.25f,    0.3f,
                                        0.35f,    0.4f,     0.45f,   0.5f,    0.55f,   0.6f,     0.65f,
                                        0.7f,     0.75f,    0.8f,    0.85f,   0.9f,    0.95f,    1.0f };
    LCreal luminanceRed[npoints] =    { 0.0f,     0.0f,     1.613f,  3.3f,    5.487f,  7.657f,  10.19f,
                                       13.02f,  16.49f,   19.36f,  22.57f,  26.21f,  30.32f,   33.67f,
                                       37.4f,   41.65f,   46.25f,  49.96f,  54.33f,  59.19f,   63.43f };
    LCreal luminanceGreen[npoints] =  { 0.0f,     0.0f,     3.14f,   7.482f, 13.28f,  20.13f,   27.91f,
                                       36.77f,  47.04f,   55.47f,  66.16f,  77.91f,  90.12f,  100.9f,
                                      113.4f, 127.3f,   142.2f,  152.7f,  165.8f,  178.7f,   191.0f };
    LCreal luminanceBlue[npoints] =   { 0.0f,     0.7603f,  2.025f,  3.653f,  5.554f,  7.427f,   9.427f,
                                       11.71f,  14.25f,   16.47f,  18.97f,  21.6f,   24.15f,   26.67f,
                                       29.51f,  32.16f,   35.22f,  37.61f,  40.53f,  43.4f,    45.99f };

    STANDARD_CONSTRUCTOR()

    redLuminance = new Table1(luminanceRed, npoints, luminanceLevels, npoints);
    greenLuminance = new Table1(luminanceGreen, npoints, luminanceLevels, npoints);
    blueLuminance = new Table1(luminanceBlue, npoints, luminanceLevels, npoints);
    phosphorCoordinates.set( 0.628f, 0.346f, 0.026f, 0.0f,
                             0.347f, 0.556f, 0.097f, 0.0f,
                             0.147f, 0.065f, 0.788f, 0.0f,
                             0.0f,   0.0f,   0.0f,   1.0f );
    refwhiteRGB.set( 0.211138f, 0.635777f, 0.153086f);
    refwhiteCIE.set( 0.276f, 0.239f, 0.485f );

    computeMatrix();
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void MonitorMetrics::copyData(const MonitorMetrics& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        redLuminance = nullptr;
        greenLuminance = nullptr;
        blueLuminance = nullptr;
    }

    transform = org.transform;
    redLuminance = org.redLuminance;
    greenLuminance = org.greenLuminance;
    blueLuminance = org.blueLuminance;
    phosphorCoordinates = org.phosphorCoordinates;
    refwhiteRGB = org.refwhiteRGB;
    refwhiteCIE = org.refwhiteCIE;
}

void MonitorMetrics::deleteData()
{
    redLuminance = nullptr;
    greenLuminance = nullptr;
    blueLuminance = nullptr;
}

bool MonitorMetrics::setSlotRed(const Table1* red)
{
    if ( red == nullptr ) return false;
    redLuminance = red;
    return computeMatrix();
}

bool MonitorMetrics::setSlotGreen(const Table1* green)
{
    if ( green == nullptr ) return false;
    greenLuminance = green;
    return computeMatrix();
}

bool MonitorMetrics::setSlotBlue(const Table1* blue)
{
    if ( blue == nullptr ) return false;
    blueLuminance = blue;
    return computeMatrix();
}

bool MonitorMetrics::setSlotPhosphors(const List* phosphors)
{
    LCreal listItems[6];

    if ( phosphors == nullptr ) return false;
    if ( phosphors->entries() != 6 ) return false;
    if ( phosphors->getNumberList(listItems, 6) != 6 ) return false;

    phosphorCoordinates.set( listItems[0], listItems[1], 1-listItems[0]-listItems[1], 0,
                             listItems[2], listItems[3], 1-listItems[2]-listItems[3], 0,
                             listItems[4], listItems[5], 1-listItems[4]-listItems[5], 0,
                                        0,            0,                           0, 1 );

    return computeMatrix();
}

bool MonitorMetrics::setSlotWhiteRGB(const List* whiteRGB)
{
    LCreal listItems[3];

    if ( whiteRGB == nullptr ) return false;
    if ( whiteRGB->entries() != 6 ) return false;
    if ( whiteRGB->getNumberList(listItems, 3) != 3 ) return false;

    refwhiteRGB.set( listItems[0], listItems[1], listItems[2] );
    return computeMatrix();
}

bool MonitorMetrics::setSlotWhiteCIE(const List* whiteCIE)
{
    LCreal listItems[3];

    if ( whiteCIE == nullptr ) return false;
    if ( whiteCIE->entries() != 6 ) return false;
    if ( whiteCIE->getNumberList(listItems, 3) != 3 ) return false;

    refwhiteCIE.set( listItems[0], listItems[1], listItems[2] );
    return computeMatrix();
}

bool MonitorMetrics::computeMatrix()
{
    osg::Matrix phosInv;
    osg::Vec3 k;

    // Invert phosphor matrix
    if ( phosInv.invert( phosphorCoordinates ) == false ) {
        std::fprintf(stderr, "MonitorMetrics: Cannot invert phosphor coordinate matrix!");
        return false;
   }

    // compute vector k
    k = refwhiteCIE / refwhiteCIE[1];
    k = k * phosInv;
    k[0] = refwhiteRGB[0] / k[0];
    k[1] = refwhiteRGB[1] / k[1];
    k[2] = refwhiteRGB[2] / k[2];

    // Compute transform matrix
    transform.set( phosInv(0,0)*k[0], phosInv(1,0)*k[0], phosInv(2,0)*k[0], 0,
                   phosInv(0,1)*k[1], phosInv(1,1)*k[1], phosInv(2,1)*k[1], 0,
                   phosInv(0,2)*k[2], phosInv(1,2)*k[2], phosInv(2,2)*k[2], 0,
                                   0,                 0,                 0, 1 );
    return true;
}

void MonitorMetrics::cie2rgb(osg::Vec4& rgba, const osg::Vec3& cie) const
{
    osg::Vec3 rgb, ciexyz;

    ciexyz.set(cie[Cie::X], cie[Cie::Y], 1-cie[Cie::X]-cie[Cie::Y]);
    rgb = transform * ciexyz;
    rgb *= cie[Cie::LUMINANCE]/(rgb[Color::RED]+rgb[Color::GREEN]+rgb[Color::BLUE]);

    rgba[Color::RED] = redLuminance->lfi( rgb[Color::RED] );
    rgba[Color::GREEN] = redLuminance->lfi( rgb[Color::GREEN] );
    rgba[Color::BLUE] = redLuminance->lfi( rgb[Color::BLUE] );
    rgba[Color::ALPHA] = Color::getDefaultAlpha();
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* MonitorMetrics::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}
}
