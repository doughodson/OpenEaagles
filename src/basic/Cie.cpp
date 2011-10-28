//------------------------------------------------------------------------------
// Cie
//------------------------------------------------------------------------------
#include "openeaagles/basic/Cie.h"

#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Tables.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(Cie,"cie")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Cie)
    "luminance", // 1: ... Luminance component, range(0.0 to 1.0)
    "x",         // 2: ... X component, range(0.0 to 1.0)
    "y",         // 3: ... Y component, range(0.0 to 1.0)
    "monitor",   // 4: ... Characteristics of intended display
END_SLOTTABLE(Cie)

// Map slot table to handles 
BEGIN_SLOT_MAP(Cie)
    ON_SLOT(1,setLuminance,Number)
    ON_SLOT(2,setX,Number)
    ON_SLOT(3,setY,Number)
    ON_SLOT(4,setMonitor,MonitorMetrics)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Cie::Cie(const MonitorMetrics*, const LCreal l, const LCreal x, const LCreal y)
{
   STANDARD_CONSTRUCTOR()
   cie[LUMINANCE] = l;   // set the values
   cie[X]         = x;
   cie[Y]         = y;
   cie2rgb(color,cie,monitor);   // set the rgb values
}

Cie::Cie()
{
   STANDARD_CONSTRUCTOR()
   cie[LUMINANCE] = 0.0f;  // default to black
   cie[X]         = 0.0f;
   cie[Y]         = 0.0f;
   monitor = new MonitorMetrics(); // use a default monitor configuration
   cie2rgb(color,cie,monitor);   // set the rgb values
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Cie::copyData(const Cie& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) monitor = 0;

   cie = org.cie;
   monitor = org.monitor;
}

void Cie::deleteData()
{
   monitor = 0;
}


//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------
LCreal Cie::luminance() const
{
    return cie[LUMINANCE];
}

LCreal Cie::x() const
{
    return cie[X];
}

LCreal Cie::y() const
{
    return cie[Y];
}

void Cie::getCIE(osg::Vec3& hhh) const
{
    hhh.set(cie[LUMINANCE],cie[X],cie[Y]);
}

//------------------------------------------------------------------------------
// setLuminance() -- set the luminance value
//------------------------------------------------------------------------------
bool Cie::setLuminance(Number* const msg)
{
    if (msg == 0) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { cie[LUMINANCE] = value; cie2rgb(color,cie,monitor); }
    else std::cerr << "Cie::setLuminance: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setX() -- set the X value
//------------------------------------------------------------------------------
bool Cie::setX(Number* const msg)
{
    if (msg == 0) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { cie[X] = value; cie2rgb(color,cie,monitor); }
    else std::cerr << "Cie::setX: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setY() -- set the X value
//------------------------------------------------------------------------------
bool Cie::setY(Number* const msg)
{
    if (msg == 0) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { cie[Y] = value; cie2rgb(color,cie,monitor); }
    else std::cerr << "Cie::setY: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setMonitor() -- set the monitor parameters
//------------------------------------------------------------------------------
bool Cie::setMonitor(MonitorMetrics* const msg)
{
    if (msg == 0) return false;
    monitor = msg;
    cie2rgb(color,cie,monitor);
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Cie
//------------------------------------------------------------------------------
Object* Cie::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// cie2rgb() -- converts a CIE color to a Red, Green, Blue (RGB) value.
//------------------------------------------------------------------------------
void Cie::cie2rgb(osg::Vec4& rgb, const osg::Vec3& cie, const MonitorMetrics* m)
{
   if ( m == 0 )
      return;

   m->cie2rgb(rgb, cie);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Cie::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "Luminance: " << luminance() << std::endl;

    indent(sout,i+j);
    sout << "X:         " << x() << std::endl;

    indent(sout,i+j);
    sout << "Y:         " << y() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}


//------------------------------------------------------------------------------
// MonitorMetrics
//------------------------------------------------------------------------------

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

   if (cc)
   {
	   redLuminance = 0;
	   greenLuminance = 0;
	   blueLuminance = 0;
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
	redLuminance = 0;
	greenLuminance = 0;
	blueLuminance = 0;
}

bool MonitorMetrics::setSlotRed(const Table1* red)
{ 
	if ( red == 0 ) return false;
	redLuminance = red;
	return computeMatrix();
}

bool MonitorMetrics::setSlotGreen(const Table1* green)
{
	if ( green == 0 ) return false;
	greenLuminance = green;
	return computeMatrix();
}

bool MonitorMetrics::setSlotBlue(const Table1* blue){
	if ( blue == 0 ) return false;
	blueLuminance = blue;
	return computeMatrix();
}

bool MonitorMetrics::setSlotPhosphors(const List* phosphors)
{
	LCreal listItems[6];

	if ( phosphors == 0 ) return false;
	if ( phosphors->entries() != 6 ) return false;
	if ( phosphors->getNumberList(listItems, 6) != 6 ) return false;

	phosphorCoordinates.set( listItems[0], listItems[1], 1-listItems[0]-listItems[1], 0,
		                     listItems[2], listItems[3], 1-listItems[2]-listItems[3], 0,
							 listItems[4], listItems[5], 1-listItems[4]-listItems[5], 0,
							 0,            0,            0,                           1 );

	return computeMatrix();
}

bool MonitorMetrics::setSlotWhiteRGB(const List* whiteRGB)
{
	LCreal listItems[3];

	if ( whiteRGB == 0 ) return false;
	if ( whiteRGB->entries() != 6 ) return false;
	if ( whiteRGB->getNumberList(listItems, 3) != 3 ) return false;

	refwhiteRGB.set( listItems[0], listItems[1], listItems[2] );
	return computeMatrix();
}

bool MonitorMetrics::setSlotWhiteCIE(const List* whiteCIE)
{
	LCreal listItems[3];

	if ( whiteCIE == 0 ) return false;
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
	if ( phosInv.invert( phosphorCoordinates ) == false )
	{
		fprintf(stderr, "MonitorMetrics: Cannot invert phosphor coordinate matrix!");
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
			       0,                 0,                 0,                 1 );
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

} // End Basic namespace
} // End Eaagles namespace
