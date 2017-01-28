
#include "openeaagles/base/Color.hpp"

#include "openeaagles/base/osg/Vec3d"
#include "openeaagles/base/osg/Vec4d"

#include "openeaagles/base/Number.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Color, "Color")
EMPTY_SERIALIZER(Color)
EMPTY_DELETEDATA(Color)

double Color::defaultAlpha = 1.0f;

Color::Color()
{
    STANDARD_CONSTRUCTOR()
    color[RED]   = 0.0f;    // default to black
    color[GREEN] = 0.0f;
    color[BLUE]  = 0.0f;
    color[ALPHA] = defaultAlpha;
}

void Color::copyData(const Color& org, const bool)
{
   BaseClass::copyData(org);
   color = org.color; 
}

//------------------------------------------------------------------------------
// data access functions
//------------------------------------------------------------------------------
Color::operator const Vec3d*() const
{
    // Note: Color is a Vec4, which is just a four element array [ r g b a ], and
    // we're reinterpreting it as a Vec3, which is a three element array [ r g b ].
    return reinterpret_cast<const Vec3d*>( &color );
}

Color::operator const Vec4d*() const
{
    return &color;
}

// Return the color vector of color array index PF_RED
double Color::red() const
{
    return color[RED];
}

// Return the color vector of color array index PF_GREEN
double Color::green() const
{
    return color[GREEN];
}

// Return the color vector of color array index PF_BLUE
double Color::blue() const
{
    return color[BLUE];
}

// Return the color vector of color array index PF_ALPHA
double Color::alpha() const
{
    return color[ALPHA];
}

// Get the address of the color vector array(3)
const Vec3d* Color::getRGB() const
{
    // Note: Color is a Vec4, which is just a four element array [ r g b a ], and
    // we're reinterpreting it as a Vec3, which is a three element array [ r g b ].
    return reinterpret_cast<const Vec3d*>( &color );
}

// Get the address of the color vector array(4)
const Vec4d* Color::getRGBA() const
{
    return &color;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool Color::setRed(const double value)
{
    bool ok = (value >= 0 && value <= 1);
    if (ok) color[Color::RED] = value;
    return ok;
}

bool Color::setGreen(const double value)
{
    bool ok = (value >= 0 && value <= 1);
    if (ok) color[Color::GREEN] = value;
    return ok;
}

bool Color::setBlue(const double value)
{
    bool ok = (value >= 0 && value <= 1);
    if (ok) color[Color::BLUE] = value;
    return ok;
}

bool Color::setAlpha(const double value)
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
double Color::getDefaultAlpha()
{
   return defaultAlpha;
}

//------------------------------------------------------------------------------
// setDefaultAlpha()
//------------------------------------------------------------------------------
void Color::setDefaultAlpha(const double alpha)
{
    defaultAlpha = alpha;
}

}
}
