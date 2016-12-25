
#include "openeaagles/graphics/ColorGradient.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"

namespace oe {
namespace graphics {

IMPLEMENT_SUBCLASS(ColorGradient,"ColorGradient")
EMPTY_SERIALIZER(ColorGradient)

BEGIN_SLOTTABLE(ColorGradient)
    "colors",      // set colors
END_SLOTTABLE(ColorGradient)

BEGIN_SLOT_MAP(ColorGradient)
    ON_SLOT(1, setSlotColors, base::PairStream)
END_SLOT_MAP()

ColorGradient::ColorGradient()
{
   STANDARD_CONSTRUCTOR()
   // default gives us no colors, but just makes us black
   color[Color::RED] = 0;
   color[Color::GREEN] = 0;
   color[Color::BLUE] = 0;
   color[Color::ALPHA] = getDefaultAlpha();

   myColors = nullptr;
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ColorGradient::copyData(const ColorGradient& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) myColors = nullptr;

    setSlotColors(org.myColors);
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void ColorGradient::deleteData()
{
    if (myColors != nullptr) {
        myColors->unref();
        myColors = nullptr;
    }
}

// set our slot colors via a pairstream
bool ColorGradient::setSlotColors(base::PairStream* const newStream)
{
    if (myColors != nullptr) {
        myColors->unref();
        myColors = nullptr;
    }
    if (newStream != nullptr) {
        myColors = newStream;
        myColors->ref();
    }
    return true;
}

base::Color* ColorGradient::getColorByIdx(const int idx)
{
    base::Color* fCol = nullptr;

    if (myColors != nullptr) {
        base::Pair* pair = myColors->getPosition(idx);
        if (pair != nullptr) {
            fCol = dynamic_cast<base::Color*>(pair->object());
        }
    }
    return fCol;
}

}
}
