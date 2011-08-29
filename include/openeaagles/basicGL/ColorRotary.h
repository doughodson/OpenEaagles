//------------------------------------------------------------------------------
// Class:  ColorRotary
//------------------------------------------------------------------------------
#ifndef __ColorRotary_H_89EC9924_C7E8_4d4b_B9A0_C6F9AE60197D
#define __ColorRotary_H_89EC9924_C7E8_4d4b_B9A0_C6F9AE60197D

#include "openeaagles/basic/Color.h"

namespace Eaagles {
	namespace Basic { class PairStream; }
namespace BasicGL {


//------------------------------------------------------------------------------
// Class:  ColorRotary
// Date:   01/29/04
// Author: Lee Sines
// Base class:  Object -> Color -> ColorRotary
//
// Description: Takes a list of colors and breakpoints, and changes colors based
// on the value given to it in comparison with the breakpoints
//
// Form name: ColorRotary
// Slots:
//      breakcolors: <PairStream>     // sets colors
//      breakpoints: <PairStream>     // sets our breakpoints
//
// Example:
//  ( Graphic
//      color: ( ColorRotary
//                  colors: {
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
class ColorRotary : public Basic::Color {
	DECLARE_SUBCLASS(ColorRotary,Basic::Color)

public:
    ColorRotary();
    
    // called to determine a color based on a given value
    virtual bool determineColor(const LCreal value);

protected:
	bool setSlotColors(Basic::PairStream* const newStream);    
	bool setSlotValues(const Basic::PairStream* const newStream);
    
private:
    static const int MAX_VALUES = 50;
	Basic::PairStream* myColors;             // our colors (rgba value)
    LCreal        myValues[MAX_VALUES]; // our values 
    int           numVals;              // number of values
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif      /* __ColorRotary_H_89EC9924_C7E8_4d4b_B9A0_C6F9AE60197D */
