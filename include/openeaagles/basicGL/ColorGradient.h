//------------------------------------------------------------------------------
// Class: ColorGradient
// Description:  List of colors that will return a given color based on the idx.
// This will be used by graphics to create a per-vertex color, which makes a
// gradient.
//------------------------------------------------------------------------------
#ifndef __oe_BasicGL_ColorGradient_H__
#define __oe_BasicGL_ColorGradient_H__

#include "openeaagles/basic/Color.h"

namespace oe {

namespace basic { class PairStream; }

namespace BasicGL {

class ColorGradient : public basic::Color {
    DECLARE_SUBCLASS(ColorGradient,basic::Color)

public:
    ColorGradient();

    basic::Color* getColorByIdx(const int idx);

protected:
    bool setSlotColors(basic::PairStream* const newStream);

private:
    static const int MAX_VALUES = 50;
    basic::PairStream* myColors;
};

} // End BasicGL namespace
} // End oe namespace

#endif
