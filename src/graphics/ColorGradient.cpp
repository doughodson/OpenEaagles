
#include "openeaagles/graphics/ColorGradient.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"

namespace oe {
namespace graphics {

IMPLEMENT_SUBCLASS(ColorGradient,"ColorGradient")
EMPTY_SERIALIZER(ColorGradient)

BEGIN_SLOTTABLE(ColorGradient)
    "colors",      // set colors
END_SLOTTABLE(ColorGradient)

BEGIN_SLOT_MAP(ColorGradient)
    ON_SLOT(1, setSlotColors, basic::PairStream)
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
bool ColorGradient::setSlotColors(basic::PairStream* const newStream)
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

basic::Color* ColorGradient::getColorByIdx(const int idx)
{
    basic::Color* fCol = nullptr;

    if (myColors != nullptr) {
        basic::Pair* pair = myColors->getPosition(idx);
        if (pair != nullptr) {
            fCol = dynamic_cast<basic::Color*>(pair->object());
        }
    }
    return fCol;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for ColorGradient
//------------------------------------------------------------------------------
basic::Object* graphics::ColorGradient::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End graphics namespace
} // End oe namespace
