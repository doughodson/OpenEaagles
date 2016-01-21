//------------------------------------------------------------------------------
// Rgb
//------------------------------------------------------------------------------
#include "openeaagles/basic/Rgb.h"
#include "openeaagles/basic/Float.h"

namespace oe {
namespace Basic {

IMPLEMENT_SUBCLASS(Rgb,"rgb")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Rgb)
    "red",      // 1: red component,   range(0.0f .. 1.0f)
    "green",    // 2: green component, range(0.0f .. 1.0f)
    "blue",     // 3: blue component,  range(0.0f .. 1.0f)
END_SLOTTABLE(Rgb)

// Map slot table to handles 
BEGIN_SLOT_MAP(Rgb)
    ON_SLOT(1,setSlotRed,Number)
    ON_SLOT(2,setSlotGreen,Number)
    ON_SLOT(3,setSlotBlue,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Rgb::Rgb(const LCreal r, const LCreal g, const LCreal b)
{
   STANDARD_CONSTRUCTOR()
   color[Color::RED]   = r;
   color[Color::GREEN] = g;
   color[Color::BLUE]  = b;
   color[Color::ALPHA] = getDefaultAlpha();
}

Rgb::Rgb()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Rgb::copyData(const Rgb& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void Rgb::deleteData()
{
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Rgb::setSlotRed(Number* const msg)
{
    LCreal value = msg->getReal();
    bool ok = setRed( value );
    if (!ok) std::cerr << "Rgb::setRed: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

bool Rgb::setSlotGreen(Number* const msg)
{
    LCreal value = msg->getReal();
    bool ok = setGreen( value );
    if (!ok) std::cerr << "Rgb::setGreen: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

bool Rgb::setSlotBlue(Number* const msg)
{
    LCreal value = msg->getReal();
    bool ok = setBlue( value );
    if (!ok) std::cerr << "Rgb::setBlue: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

bool Rgb::setSlotAlpha(Number* const msg)
{
    LCreal value = msg->getReal();
    bool ok = setAlpha( value );
    if (!ok) std::cerr << "Rgb::setAlpha: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for Rgb
//------------------------------------------------------------------------------
Object* Rgb::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Rgb::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "red:   " << red() << std::endl;

    indent(sout,i+j);
    sout << "green: " << green() << std::endl;

    indent(sout,i+j);
    sout << "blue:  " << blue() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Basic namespace
} // End oe namespace
