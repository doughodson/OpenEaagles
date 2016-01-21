//------------------------------------------------------------------------------
// Cmy
//------------------------------------------------------------------------------

#include "openeaagles/basic/Cmy.h"

#include "openeaagles/basic/Float.h"

namespace oe {
namespace basic {

IMPLEMENT_SUBCLASS(Cmy,"cmy")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Cmy)
    "cyan",     // 1: ... Cyan component, range(0.0 to 1.0)
    "magenta",  // 2: ... Magenta component, range(0.0 to 1.0)
    "yellow",   // 3: ... Yellow component, range(0.0 to 1.0)
END_SLOTTABLE(Cmy)

// Map slot table to handles 
BEGIN_SLOT_MAP(Cmy)
    ON_SLOT(1,setCyan,Number)
    ON_SLOT(2,setMagenta,Number)
    ON_SLOT(3,setYellow,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Cmy::Cmy(const LCreal c, const LCreal m, const LCreal y)
{
   STANDARD_CONSTRUCTOR()
   cmy[CYAN]    = c;     // set the values
   cmy[MAGENTA] = m;
   cmy[YELLOW]  = y;
   cmy2rgb(color,cmy);   // set the rgb values
}

Cmy::Cmy()
{
   STANDARD_CONSTRUCTOR()
   cmy[CYAN]    = 0.0f;  // default to black
   cmy[MAGENTA] = 0.0f;
   cmy[YELLOW]  = 0.0f;
   cmy2rgb(color,cmy);   // set the rgb values
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Cmy::copyData(const Cmy& org, const bool)
{
   BaseClass::copyData(org);
   cmy = org.cmy;
}

EMPTY_DELETEDATA(Cmy)


//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------
LCreal Cmy::cyan() const
{
    return cmy[CYAN];
}

LCreal Cmy::magenta() const
{
    return cmy[MAGENTA];
}

LCreal Cmy::yellow() const
{
    return cmy[YELLOW];
}

void Cmy::getCMY(osg::Vec3& hhh) const
{
    hhh.set(cmy[CYAN],cmy[MAGENTA],cmy[YELLOW]);
}

//------------------------------------------------------------------------------
// setCyan() -- set the cyan value
//------------------------------------------------------------------------------
bool Cmy::setCyan(Number* const msg)
{
    if (msg == nullptr) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { cmy[CYAN] = value; cmy2rgb(color,cmy); }
    else std::cerr << "Cmy::setCyan: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setMagenta() -- set the magenta value
//------------------------------------------------------------------------------
bool Cmy::setMagenta(Number* const msg)
{
    if (msg == nullptr) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { cmy[MAGENTA] = value; cmy2rgb(color,cmy); }
    else std::cerr << "Cmy::setMagenta: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// setYellow() -- set the yellow value
//------------------------------------------------------------------------------
bool Cmy::setYellow(Number* const msg)
{
    if (msg == nullptr) return false;
    LCreal value = msg->getReal();
    bool ok = (value >= 0 && value <= 1);
    if (ok) { cmy[YELLOW] = value; cmy2rgb(color,cmy); }
    else std::cerr << "Cmy::setYellow: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Cmy
//------------------------------------------------------------------------------
Object* Cmy::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// cmy2rgb() -- converts a Cyan, Magenta, Yellow (CMY) color to a
//              Red, Green, Blue (RGB) value.
//
// This code is based on '/usr/people/4Dgifts/iristools/libgutil/colormod.c'
//------------------------------------------------------------------------------
void Cmy::cmy2rgb(osg::Vec4& rgb, const osg::Vec3& cmy)
{
    rgb[RED]   = 1 - cmy[CYAN];
    rgb[GREEN] = 1 - cmy[MAGENTA];
    rgb[BLUE]  = 1 - cmy[YELLOW];
    rgb[ALPHA] = defaultAlpha;
}

//------------------------------------------------------------------------------
// rgb2cmy -- converts a Red, Green, Blue (RGB) color to a
//              Cyan, Magenta, Yellow (CMY) value.
//
// This code is based on '/usr/people/4Dgifts/iristools/libgutil/colormod.c'
//------------------------------------------------------------------------------
void Cmy::rgb2cmy(osg::Vec3& cmy, const osg::Vec4& rgb)
{
    cmy[CYAN]    = 1 - rgb[RED];
    cmy[MAGENTA] = 1 - rgb[GREEN];
    cmy[YELLOW]  = 1 - rgb[BLUE];
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Cmy::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "cyan:    " << cyan() << std::endl;

    indent(sout,i+j);
    sout << "magenta: " << magenta() << std::endl;

    indent(sout,i+j);
    sout << "yellow:  " << yellow() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End basic namespace
} // End oe namespace
