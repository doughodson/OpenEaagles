//------------------------------------------------------------------------------
// ColorRotary
//------------------------------------------------------------------------------

#include "openeaagles/graphics/ColorRotary.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/osg/Vec4"

namespace oe {
namespace BasicGL {

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
    ON_SLOT(1, setSlotColors, basic::PairStream)
    ON_SLOT(2, setSlotValues, basic::PairStream)
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
bool ColorRotary::setSlotColors(basic::PairStream* const newStream)
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
bool ColorRotary::setSlotValues(const basic::PairStream* const newStream)
{
    bool ok = false;
    numVals = 0;
    if (newStream != nullptr) {
        basic::PairStream* a = newStream->clone();
        basic::List::Item* item = a->getFirstItem();
        while (item != nullptr) {
            basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
            if (pair != nullptr) {
                basic::Number* n = dynamic_cast<basic::Number*>(pair->object());
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
bool ColorRotary::determineColor(const LCreal value)
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
        basic::Pair* pair = myColors->getPosition(breakPoint);
        if (pair != nullptr) {
            basic::Color* listcolor = dynamic_cast<basic::Color*>(pair->object());
            if (listcolor != nullptr) {
               const osg::Vec4* vec = static_cast<const osg::Vec4*>(listcolor->getRGBA());
               color = *vec;
               ok = true;
            }
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for ColorRotary
//------------------------------------------------------------------------------
basic::Object* BasicGL::ColorRotary::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End BasicGL namespace
} // End oe namespace
