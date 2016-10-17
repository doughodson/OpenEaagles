//------------------------------------------------------------------------------
// Class: GaugeSlider
//------------------------------------------------------------------------------
#ifndef __oe_instruments_GuageSlider_H__
#define __oe_instruments_GuageSlider_H__

#include "openeaagles/instruments/gauges/AnalogGauge.hpp"

namespace oe {
namespace instruments {

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

    virtual void draw() override;

private:
    double sliderPos;       // direction, in inches, to move our slider (either Y or X-axis depending on the vertical flag)
};

}
}

#endif
