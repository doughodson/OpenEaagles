//------------------------------------------------------------------------------
// Hsv
//------------------------------------------------------------------------------
#include "openeaagles/basic/Hsv.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(Hsv,"hsv")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Hsv)
    "hue",          // 1: hue component,        range(0.0 to 360.0)
    "saturation",   // 2: saturation component, range(0.0 to 1.0)
    "value",        // 3: value component,      range(0.0 to 1.0)
END_SLOTTABLE(Hsv)

// Map slot table to handles 
BEGIN_SLOT_MAP(Hsv)
    ON_SLOT(1,setHue,Number)
    ON_SLOT(2,setSaturation,Number)
    ON_SLOT(3,setValue,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Hsv::Hsv(const LCreal h, const LCreal s, const LCreal v)
{
   STANDARD_CONSTRUCTOR()
   hsv[HUE]        = h;     // set the values
   hsv[SATURATION] = s;
   hsv[VALUE]      = v;
   hsv[ALPHA]      = getDefaultAlpha();
   hsv2rgb(color,hsv);      // set the rgb values
}

Hsv::Hsv()
{
   STANDARD_CONSTRUCTOR()
   hsv[HUE]        = 0.0f;  // default to black
   hsv[SATURATION] = 0.0f;
   hsv[VALUE]      = 0.0f;
   hsv[ALPHA]      = getDefaultAlpha();
   hsv2rgb(color,hsv);      // set the rgb values
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Hsv::copyData(const Hsv& org, const bool)
{
   BaseClass::copyData(org);
   hsv = org.hsv;
}

EMPTY_DELETEDATA(Hsv)


//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------
LCreal Hsv::hue() const
{
    return hsv[HUE];
}

LCreal Hsv::saturation() const
{
    return hsv[SATURATION];
}

LCreal Hsv::value() const
{
    return hsv[VALUE];
}

void Hsv::getHSV(osg::Vec3& hhh) const
{
    hhh.set(hsv[HUE],hsv[SATURATION],hsv[VALUE]);
}

void Hsv::getHSVA(osg::Vec4& hhh) const
{
    hhh.set(hsv[HUE],hsv[SATURATION],hsv[VALUE],hsv[ALPHA]);
}

//------------------------------------------------------------------------------
// setHue() -- set the HSV hue
//------------------------------------------------------------------------------
bool Hsv::setHue(Number* const msg)
{
    if (msg == 0) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 360);
    if (ok) { hsv[HUE] = value; hsv2rgb(color,hsv); }
    else std::cerr << "Hsv::setHue: invalid entry(" << value << "), valid range: 0 to 360" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setSaturation() -- set the HSV staturation
//------------------------------------------------------------------------------
bool Hsv::setSaturation(Number* const msg)
{
    if (msg == 0) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { hsv[SATURATION] = value; hsv2rgb(color,hsv); }
    else std::cerr << "Hsv::setSaturation: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setValue() -- set the HSV value
//------------------------------------------------------------------------------
bool Hsv::setValue(Number* const msg)
{
    if (msg == 0) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { hsv[VALUE] = value; hsv2rgb(color,hsv); }
    else std::cerr << "Hsv::setValue: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setAlpha() -- set the ALPHA value
//------------------------------------------------------------------------------
bool Hsv::setAlpha(Number* const msg)
{
    if (msg == 0) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { hsv[ALPHA] = value; hsv2rgb(color,hsv); }
    else std::cerr << "Hsv::setAlpha: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setHSV() -- Sets the hsv vector
//------------------------------------------------------------------------------
bool Hsv::setHSV(const osg::Vec3& vec)
{
   bool ok = false;
   if (&vec != 0) {
      hsv[0] = vec[0];
      hsv[1] = vec[1];
      hsv[2] = vec[2];
      hsv2rgb(color,hsv);
      ok = true;
   }
   return ok;
}

bool Hsv::setHSVA(const osg::Vec4& vec)
{
   bool ok = false;
   if (&vec != 0) {
      hsv = vec;
      hsv2rgb(color,hsv);
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// hsv2rgb() -- converts a Hue, Saturation, Value (HSV) color value to an
//              Red, Gree, Blue (RGB) value.
//
// This code is based on '/usr/people/4Dgifts/iristools/libgutil/colormod.c'
//------------------------------------------------------------------------------
void Hsv::hsv2rgb(osg::Vec3& rgb, const osg::Vec3& hsv)
{
    // local HSV values
    LCreal h = lcAepcDeg(hsv[HUE]);
    if (h < 0.0f) h += 360.0f;
    LCreal s = hsv[SATURATION];
    LCreal v = hsv[VALUE];

    if (s != 0.0) {

        // The max hue value is the same as the min hue value.
        if (h == 360.0f) h = 0.0f;
        h /= 60.0f;

        // computer some parameters
        //int i = ffloor(h);
        int i = int(h);
        LCreal f = h - LCreal(i);
        LCreal p = v * (1.0f - s);
        LCreal q = v * (1.0f - (s * f));
        LCreal t = v * (1.0f - (s * (1.0f - f)));

        switch (i) {
            case 0 : {
                // when hue is >= red and < yellow
                rgb[RED] = v;
                rgb[GREEN] = t;
                rgb[BLUE] = p;
            }
            break;

            case 1 : {
                // when hue is >= yellow and < green
                rgb[RED] = q;
                rgb[GREEN] = v;
                rgb[BLUE] = p;
            }
            break;

            case 2 : {
                // when hue is >= green and < cyan
                rgb[RED] = p;
                rgb[GREEN] = v;
                rgb[BLUE] = t;
            }
            break;

            case 3 : {
                // when hue is >= cyan and < blue
                rgb[RED] = p;
                rgb[GREEN] = q;
                rgb[BLUE] = v;
            }
            break;

            case 4 : {
                // when hue is >= blue and < magenta
                rgb[RED] = t;
                rgb[GREEN] = p;
                rgb[BLUE] = v;
            }
            break;

            case 5 : {
                // when hue is >= magenta and < red
                rgb[RED] = v;
                rgb[GREEN] = p;
                rgb[BLUE] = q;
            }
            break;
        }
    }
    else {
        // when saturation is zero, the color is gray of intensity 'v'.
        rgb[RED] = v;
        rgb[GREEN] = v;
        rgb[BLUE] = v;
    }
}

void Hsv::hsv2rgb(osg::Vec4& rgb, const osg::Vec4& hsv)
{
   osg::Vec3 hsv3(hsv[0], hsv[1], hsv[2]);
   osg::Vec3 rgb3;
   hsv2rgb(rgb3, hsv3);

   // Copy to output
   rgb[RED]   = rgb3[RED];
   rgb[GREEN] = rgb3[GREEN];
   rgb[BLUE]  = rgb3[BLUE];

   // Just pass alpha
   rgb[ALPHA] = hsv[ALPHA];
}

//------------------------------------------------------------------------------
// rgb2hsv() -- converts a Red, Gree, Blue (RGB) color value to an
//              Hue, Saturation, Value (HSV) value.
//
// This code is based on '/usr/people/4Dgifts/iristools/libgutil/colormod.c'
//------------------------------------------------------------------------------
void Hsv::rgb2hsv(osg::Vec3& hsv, const osg::Vec3& rgb)
{
   LCreal cmax = lcMax( rgb[RED], lcMax(rgb[GREEN],rgb[BLUE]) ); 
   LCreal cmin = lcMin( rgb[RED], lcMin(rgb[GREEN],rgb[BLUE]) );
   LCreal cdelta = cmax - cmin;
   LCreal h = 0;
   LCreal s = 0;

   if ( cmax != 0.0 )
	   s = cdelta / cmax;
   
   if ( s != 0.0 )
   {
	   LCreal rc = (cmax - rgb[RED]) / cdelta;
	   LCreal gc = (cmax - rgb[GREEN]) / cdelta;
	   LCreal bc = (cmax - rgb[BLUE]) / cdelta;

	   if ( rgb[RED] == cmax )
		   h = bc - gc;
	   else if ( rgb[GREEN] == cmax )
		   h = 2.0f + rc - bc;
	   else if ( rgb[BLUE] == cmax )
		   h = 4.0f + gc - rc;

	   h *= 60.0f;
	   if ( h < 0.0 )
		   h += 360.0f;
   }

   hsv[HUE] = h;
   hsv[VALUE] = cmax;
   hsv[SATURATION] = s;  
}

void Hsv::rgb2hsv(osg::Vec4& hsv, const osg::Vec4& rgb)
{
   // Let the Vec3 version do the work
   osg::Vec3 hsv3;
   osg::Vec3 rgb3(rgb[0], rgb[1], rgb[2]);
   rgb2hsv(hsv3,rgb3);

   // Copy to output (just pass alpha)
   hsv[HUE]        = hsv3[HUE];
   hsv[VALUE]      = hsv3[VALUE];
   hsv[SATURATION] = hsv3[SATURATION];  

   // Just pass alpha
   hsv[ALPHA] = rgb[ALPHA];
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Hsv
//------------------------------------------------------------------------------
Object* Hsv::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Hsv::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    using namespace std;

    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFormName() << endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "hue:        " << hue() << endl;

    indent(sout,i+j);
    sout << "saturation: " << saturation() << endl;

    indent(sout,i+j);
    sout << "value:      " << value() << endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << endl;
    }

    return sout;
}

} // End Basic namespace
} // End Eaagles namespace
