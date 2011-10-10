//------------------------------------------------------------------------------
// Class: ColorGradient
// Description:  List of colors that will return a given color based on the idx.
// This will be used by graphics to create a per-vertex color, which makes a 
// gradient.  
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_ColorGradient_H__
#define __Eaagles_BasicGL_ColorGradient_H__

#include "openeaagles/basic/Color.h"

namespace Eaagles {

namespace Basic { class PairStream; }

namespace BasicGL {

class ColorGradient : public Basic::Color {
	DECLARE_SUBCLASS(ColorGradient,Basic::Color)

public:
    ColorGradient();
    
	Basic::Color* getColorByIdx(const int idx);

protected:
	bool setSlotColors(Basic::PairStream* const newStream);    
    
private:
    static const int MAX_VALUES = 50;
	Basic::PairStream* myColors;             
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif
