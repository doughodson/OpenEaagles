//------------------------------------------------------------------------------
// Hls
//------------------------------------------------------------------------------
#include "openeaagles/basic/Hls.h"
#include "openeaagles/basic/Float.h"

namespace oe {
namespace basic {

IMPLEMENT_SUBCLASS(Hls,"hls")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Hls)
    "hue",         // 1: ... Hue component, range(0.0 to 360.0)
    "lightness",   // 2: ... Lightness component, range(0.0 to 1.0)
    "saturation",  // 3: ... Saturation component, range(0.0 to 1.0)
END_SLOTTABLE(Hls)

// Map slot table to handles
BEGIN_SLOT_MAP(Hls)
    ON_SLOT(1,setHue,Number)
    ON_SLOT(2,setLightness,Number)
    ON_SLOT(3,setSaturation,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Hls::Hls(const LCreal h, const LCreal l, const LCreal s)
{
   STANDARD_CONSTRUCTOR()
   hls[HUE]        = h;     // set the values
   hls[LIGHTNESS]  = l;
   hls[SATURATION] = s;
   hls2rgb(color,hls);   // set the rgb values
}

Hls::Hls()
{
   STANDARD_CONSTRUCTOR()
   hls[HUE]        = 0.0f;  // default to black
   hls[LIGHTNESS]  = 0.0f;
   hls[SATURATION] = 0.0f;
   hls2rgb(color,hls);   // set the rgb values
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Hls::copyData(const Hls& org, const bool)
{
   BaseClass::copyData(org);
   hls = org.hls;
}

EMPTY_DELETEDATA(Hls)


//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------
LCreal Hls::hue() const
{
    return hls[HUE];
}

LCreal Hls::saturation() const
{
    return hls[SATURATION];
}

LCreal Hls::lightness() const
{
    return hls[LIGHTNESS];
}

void Hls::getHLS(osg::Vec3& hhh) const
{
    hhh.set(hls[HUE],hls[SATURATION],hls[LIGHTNESS]);
}

//------------------------------------------------------------------------------
// setHue() -- set the hue value
//------------------------------------------------------------------------------
bool Hls::setHue(Number* const msg)
{
    if (msg == nullptr) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 360);
    if (ok) { hls[HUE] = value; hls2rgb(color,hls); }
    else std::cerr << "Hls::setHue: invalid entry(" << value << "), valid range: 0 to 360" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setSaturation() -- set the saturation value
//------------------------------------------------------------------------------
bool Hls::setSaturation(Number* const msg)
{
    if (msg == nullptr) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { hls[SATURATION] = value; hls2rgb(color,hls); }
    else std::cerr << "Hls::setSaturation: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setLightness() -- set the lightness value
//------------------------------------------------------------------------------
bool Hls::setLightness(Number* const msg)
{
    if (msg == nullptr) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { hls[LIGHTNESS] = value; hls2rgb(color,hls); }
    else std::cerr << "Hls::setLightness: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Hls
//------------------------------------------------------------------------------
Object* Hls::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// value() -- utility function used in hls2rgb
//------------------------------------------------------------------------------
LCreal Hls::value(LCreal n1, LCreal n2, LCreal hue)
{
    if (hue > 360.0f) hue -= 360.0f;
    if (hue < 0.0f) hue += 360.0f;

    if (hue < 60.0)
        return(n1+(n2-n1)*(hue/60.0f));
    else if (hue < 180.0f)
        return(n2);
    else if (hue < 240.0f)
        return(n1+(n2-n1)*((240.0f-hue)/60.0f));
    else
        return(n1);
}

//------------------------------------------------------------------------------
// hls2rgb() -- converts a Hue, Saturation, Lightness (HLS) color to a
//              Red, Green, Blue (RGB) value.
//------------------------------------------------------------------------------
void Hls::hls2rgb(osg::Vec4& rgb, const osg::Vec3& hls)
{
    LCreal m1, m2;

    if (hls[LIGHTNESS] <= 0.5)
        m2 = hls[LIGHTNESS] * (1.0f + hls[SATURATION]);
    else
        m2 = hls[SATURATION] + hls[LIGHTNESS] * (1.0f - hls[SATURATION]);
    m1 = 2.0f * hls[LIGHTNESS] - m2;

    if (hls[SATURATION] == 0.0) {
        rgb[RED] = hls[LIGHTNESS];
        rgb[GREEN] = hls[LIGHTNESS];
        rgb[BLUE] = hls[LIGHTNESS];
    }
    else {
        rgb[RED]   = value( m1, m2, hls[HUE] + 120.0f );
        rgb[GREEN] = value( m1, m2, hls[HUE] );
        rgb[BLUE]  = value( m1, m2, hls[HUE] - 120.0f );
    }

    rgb[ALPHA] = defaultAlpha;
}

//------------------------------------------------------------------------------
// rgb2hls() -- converts a Red, Green, Blue (RGB) color to a
//              Hue, Saturation, Lightness (HLS) value.
//------------------------------------------------------------------------------
void Hls::rgb2hls(osg::Vec3& hls, const osg::Vec4& rgb)
{
    LCreal rc, gc, bc;
    LCreal maxcol, mincol, cdelta;

    maxcol = lcMax( rgb[RED], lcMax(rgb[GREEN],rgb[BLUE]));
    mincol = lcMin( rgb[RED], lcMin(rgb[GREEN],rgb[BLUE]));
    hls[LIGHTNESS] = (mincol + maxcol) / 2.0f;

    if (maxcol == mincol) {
        hls[SATURATION] = 0.0f;
        hls[HUE] = 0.0f;
    } else {
        cdelta = maxcol - mincol;
        rc = ( maxcol - rgb[RED] ) / cdelta;
        gc = ( maxcol - rgb[GREEN] ) / cdelta;
        bc = ( maxcol - rgb[BLUE] ) / cdelta;

        if (hls[LIGHTNESS] <= 0.5)
            hls[SATURATION] = cdelta / (maxcol + mincol);
        else
            hls[SATURATION] = cdelta / (2.0f - cdelta);

        if ( rgb[RED] == maxcol)
            hls[HUE] = bc - gc;
        else if ( rgb[GREEN] == maxcol )
            hls[HUE] = 2.0f + rc - bc;
        else
            hls[HUE] = 4.0f + gc - rc;
        hls[HUE] *= 60.0f;
        if (hls[HUE] < 0.0) hls[HUE] += 360.0f;
   }
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Hls::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "hue:        " << hue() << std::endl;

    indent(sout,i+j);
    sout << "lightness:  " << lightness() << std::endl;

    indent(sout,i+j);
    sout << "saturation: " << saturation() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End basic namespace
} // End oe namespace
