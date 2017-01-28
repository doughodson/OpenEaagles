
#include "openeaagles/graphics/ColorRotary.hpp"

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/Identifier.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/osg/Vec4d"

namespace oe {
namespace graphics {

IMPLEMENT_SUBCLASS(ColorRotary,"ColorRotary")
EMPTY_SERIALIZER(ColorRotary)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(ColorRotary)
    "breakcolors",      // set colors
    "breakpoints",      // set our breakpoints
END_SLOTTABLE(ColorRotary)

//------------------------------------------------------------------------------
// Map the slots for this class type
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(ColorRotary)
    ON_SLOT(1, setSlotColors, base::PairStream)
    ON_SLOT(2, setSlotValues, base::PairStream)
END_SLOT_MAP()

ColorRotary::ColorRotary()
{
    STANDARD_CONSTRUCTOR()
    // default gives us no colors, but just makes us black
    color[Color::RED] = 0;
    color[Color::GREEN] = 0;
    color[Color::BLUE] = 0;
    color[Color::ALPHA] = getDefaultAlpha();

    // inits
    myColors = nullptr;
    numVals = 0;
    for (unsigned int i=0; i<MAX_VALUES; i++) {
        myValues[i] = 0;
    }
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ColorRotary::copyData(const ColorRotary& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) {
        myColors = nullptr;
    }

    if (org.numVals > 0) {
        for (unsigned int i = 0; i < org.numVals; i++) {
            myValues[i] = org.myValues[i];
        }
    }
    if (org.myColors != nullptr) {
        setSlotColors(org.myColors);
    }
    else setSlotColors(nullptr);
    numVals = org.numVals;
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void ColorRotary::deleteData()
{
    if (myColors != nullptr) myColors->unref();
    myColors = nullptr;
}

//------------------------------------------------------------------------------
// SLOT FUNCTIONS
//------------------------------------------------------------------------------

// set our slot colors via a PairStream
bool ColorRotary::setSlotColors(base::PairStream* const newStream)
{
    bool ok = false;
    if (newStream != nullptr) {
        myColors = newStream;
        myColors->ref();
        ok = true;
    }
    return ok;
}

// set our slot values via a pairstream
bool ColorRotary::setSlotValues(const base::PairStream* const newStream)
{
    bool ok = false;
    numVals = 0;
    if (newStream != nullptr) {
        base::PairStream* a = newStream->clone();
        base::List::Item* item = a->getFirstItem();
        while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            if (pair != nullptr) {
                const auto n = dynamic_cast<base::Number*>(pair->object());
                if (n != nullptr) {
                    myValues[numVals] = n->getReal();
                    numVals++;
                }
            }
            item = item->getNext();
        }
        ok = true;
        a->unref();
    }
    return ok;
}

//------------------------------------------------------------------------------
// determineColors() - take our value, and look for a corresponding color
// and breakpoint
//------------------------------------------------------------------------------
bool ColorRotary::determineColor(const double value)
{
    bool ok = false;
    int breakPoint = 0;

    // find out where we are in the break table
    unsigned int i = 0;
    // do an endpoint check while we are at it
    if (value >= myValues[numVals-1]) breakPoint = numVals;
    while (!ok && i < numVals) {
        if (value >= myValues[i] && value < myValues[i+1]){
            breakPoint = (i + 1);
            ok = true;
        }
        else i++;
    }

    // now set the proper color (using the breakpoint index)
    if (myColors != nullptr) {
        base::Pair* pair = myColors->getPosition(breakPoint);
        if (pair != nullptr) {
            const auto listcolor = dynamic_cast<base::Color*>(pair->object());
            if (listcolor != nullptr) {
               const auto vec = static_cast<const base::Vec4d*>(listcolor->getRGBA());
               color = *vec;
               ok = true;
            }
        }
    }
    return ok;
}

}
}
