//------------------------------------------------------------------------------
// Class: GaugeSlider
//------------------------------------------------------------------------------
#ifndef __oe_Instruments_GuageSlider_H__
#define __oe_Instruments_GuageSlider_H__

#include "openeaagles/instruments/gauges/AnalogGauge.h"

namespace oe {
namespace Instruments {

//------------------------------------------------------------------------------
// Class: GaugeSlider
//
// Description: Translates its graphical components vertically
// or horizontally along the gauge depending on the value given to it
//------------------------------------------------------------------------------
class GaugeSlider : public AnalogGauge
{
    DECLARE_SUBCLASS(GaugeSlider,AnalogGauge)

public:
    GaugeSlider();

    void draw() override;

private:
    LCreal sliderPos;       // direction, in inches, to move our slider (either Y or X-axis depending on the vertical flag)
};

}  // end Instruments namespace
}  // end oe namespace

#endif
