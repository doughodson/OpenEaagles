//------------------------------------------------------------------------------
// Color
//------------------------------------------------------------------------------

#include "openeaagles/basic/Color.h"

#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Color, "Color")
EMPTY_SERIALIZER(Color)
EMPTY_DELETEDATA(Color)

LCreal Color::defaultAlpha = 1.0f;

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Color::Color()
{
    STANDARD_CONSTRUCTOR()
    color[RED]   = 0.0f;    // default to black
    color[GREEN] = 0.0f;
    color[BLUE]  = 0.0f;
    color[ALPHA] = defaultAlpha;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Color::copyData(const Color& org, const bool)
{
   BaseClass::copyData(org);
   color = org.color; 
}

//------------------------------------------------------------------------------
// data access functions
//------------------------------------------------------------------------------
Color::operator const osg::Vec3*() const
{
    return (osg::Vec3*) &color;
}

Color::operator const osg::Vec4*() const
{
    return &color;
}

// Return the color vector of color array index PF_RED
LCreal Color::red() const
{
    return color[RED];
}

// Return the color vector of color array index PF_GREEN
LCreal Color::green() const
{
    return color[GREEN];
}

// Return the color vector of color array index PF_BLUE
LCreal Color::blue() const
{
    return color[BLUE];
}

// Return the color vector of color array index PF_ALPHA
LCreal Color::alpha() const
{
    return color[ALPHA];
}

// Get the address of the color vector array(3)
const osg::Vec3* Color::getRGB() const
{
    return (osg::Vec3*) &color;
}

// Get the address of the color vector array(4)
const osg::Vec4* Color::getRGBA() const
{
    return &color;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool Color::setRed(const LCreal value)
{
    bool ok = (value >= 0 && value <= 1);
    if (ok) color[Color::RED] = value;
    return ok;
}

bool Color::setGreen(const LCreal value)
{
    bool ok = (value >= 0 && value <= 1);
    if (ok) color[Color::GREEN] = value;
    return ok;
}

bool Color::setBlue(const LCreal value)
{
    bool ok = (value >= 0 && value <= 1);
    if (ok) color[Color::BLUE] = value;
    return ok;
}

bool Color::setAlpha(const LCreal value)
{
    bool ok = (value >= 0 && value <= 1);
    if (ok) color[Color::ALPHA] = value;
    return ok;
}

//------------------------------------------------------------------------------
// Comparison operators: == and !=
//------------------------------------------------------------------------------
bool operator==(const Color& c1, const Color& c2)
{
    return (c1.getRGBA() == c2.getRGBA());
}

bool operator!=(const Color& c1, const Color& c2)
{
    return (c1.getRGBA() != c2.getRGBA());
}

//------------------------------------------------------------------------------
// getDefaultAlpha() -- returns the default alpha value
//------------------------------------------------------------------------------
LCreal Color::getDefaultAlpha()
{
   return defaultAlpha;
}

//------------------------------------------------------------------------------
// setDefaultAlpha()
//------------------------------------------------------------------------------
void Color::setDefaultAlpha(const LCreal alpha)
{
    defaultAlpha = alpha;
}

} // End Basic namespace
} // End Eaagles namespace
