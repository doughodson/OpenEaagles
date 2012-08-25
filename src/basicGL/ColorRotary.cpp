//------------------------------------------------------------------------------
// ColorRotary
//------------------------------------------------------------------------------

#include "openeaagles/basicGL/ColorRotary.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/osg/Vec4"

namespace Eaagles {
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
    ON_SLOT(1, setSlotColors, Basic::PairStream)
    ON_SLOT(2, setSlotValues, Basic::PairStream)
END_SLOT_MAP()

ColorRotary::ColorRotary()
{
   STANDARD_CONSTRUCTOR()
   // default gives us no colors, but just makes us black
   color[Color::RED] = 0;
   color[Color::GREEN] = 0;
   color[Color::BLUE] = 0;
   color[Color::ALPHA] = getDefaultAlpha();
   
   myColors = 0;
   numVals = 0;
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ColorRotary::copyData(const ColorRotary& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) {
        myColors = 0;
    }    
    
    if (org.numVals > 0) {
        for (int i = 0; i < org.numVals; i++) {
            myValues[i] = org.myValues[i];
        }   
    }
    if (org.myColors != 0) {
        setSlotColors(org.myColors);
    }
    else setSlotColors(0);
    numVals = org.numVals;
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void ColorRotary::deleteData()
{
    if (myColors != 0) myColors->unref();
    myColors = 0;
}

// SLOT FUNCTIONS


// SET FUNCTIONS
// set our slot colors via a pairstream
bool ColorRotary::setSlotColors(Basic::PairStream* const newStream)
{
    bool ok = false;
    if (newStream != 0) {
        myColors = newStream;
        myColors->ref();
        ok = true;
    }
    return ok;
}
// set our slot values via a pairstream
bool ColorRotary::setSlotValues(const Basic::PairStream* const newStream)
{
    bool ok = false;
    numVals = 0;
    if (newStream != 0) {
        Basic::PairStream* a = (Basic::PairStream*) newStream->clone();
        Basic::List::Item* item = a->getFirstItem();
        while (item != 0) {
            Basic::Pair* pair = (Basic::Pair*) item->getValue();
            if (pair != 0) {
                Basic::Number* n = dynamic_cast<Basic::Number*>(pair->object());
                if (n != 0) {
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
    int i = 0;
    // do an endpoint check while we are at it
    if (value >= myValues[numVals-1]) breakPoint = numVals;
    while (!ok && i < numVals) {
        if (value >= myValues[i] && value < myValues[i +1]){
            breakPoint = (i + 1);
            ok = true;
        } 
        else i++;
    }

    // now set the proper color (using the breakpoint index) 
    if (myColors != 0) {
        Basic::Pair* pair = myColors->getPosition(breakPoint);
        if (pair != 0) {
            Basic::Color* listcolor = dynamic_cast<Basic::Color*>(pair->object());
            if (listcolor != 0) {
               osg::Vec4* vec = (osg::Vec4*) listcolor->getRGBA();
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
Basic::Object* BasicGL::ColorRotary::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End BasicGL namespace
} // End Eaagles namespace
