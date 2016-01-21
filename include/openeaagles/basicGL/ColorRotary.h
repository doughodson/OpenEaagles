//------------------------------------------------------------------------------
// Class: ColorRotary
//------------------------------------------------------------------------------
#ifndef __oe_BasicGL_ColorRotary_H__
#define __oe_BasicGL_ColorRotary_H__

#include "openeaagles/basic/Color.h"

namespace oe {

namespace basic { class PairStream; }

namespace BasicGL {


//------------------------------------------------------------------------------
// Class: ColorRotary
//
// Base class: Object -> Color -> ColorRotary
//
// Description: Takes a list of colors and breakpoints, and changes colors based
// on the value given to it in comparison with the breakpoints
//
// Factory name: ColorRotary
// Slots:
//      breakcolors: <PairStream>     // sets colors
//      breakpoints: <PairStream>     // sets our breakpoints
//
// Example:
//  ( Graphic
//      color: ( ColorRotary
//                  breakcolors: {
//                      (rgb 1 0 1)
//                      (hsv 0 1 1)
//                  }
//                  breakpoints: {
//                      0
//                      50
//                  }
//  This will remain rgb 1 0 1 until the value hits 50 or more, in which
//  it switches to the next color
//
//  bool ColorRotary::determineColor(const LCreal value)
//      determineColors() - Take our value, and look for a corresponding color
//      and breakpoint
//
//  bool ColorRotary::setSlotColors(PairStream* const newStream)
//       Set our slot colors via a pairstream
//
//  bool ColorRotary::setSlotValues(const PairStream* const newStream)
//       Set our slot values via a pairstream
//
//------------------------------------------------------------------------------
class ColorRotary : public basic::Color
{
    DECLARE_SUBCLASS(ColorRotary,basic::Color)

public:
    ColorRotary();

    // called to determine a color based on a given value
    virtual bool determineColor(const LCreal value);

protected:
    bool setSlotColors(basic::PairStream* const newStream);
    bool setSlotValues(const basic::PairStream* const newStream);

private:
    static const unsigned int MAX_VALUES = 50;
    basic::PairStream* myColors;              // our colors (rgba value)
    LCreal myValues[MAX_VALUES];              // our values
    unsigned int numVals;                    // number of values
};

} // End BasicGL namespace
} // End oe namespace

#endif
