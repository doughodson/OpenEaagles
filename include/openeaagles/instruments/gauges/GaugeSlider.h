//------------------------------------------------------------------------------
// Class: GaugeSlider
//------------------------------------------------------------------------------
#ifndef	__Eaagles_Instruments_GuageSlider_H__
#define __Eaagles_Instruments_GuageSlider_H__

#include "openeaagles/instruments/gauges/AnalogGauge.h"

namespace Eaagles {
namespace Instruments {

//------------------------------------------------------------------------------
// Class:	GaugeSlider
// 
// Description: Translates it's graphical components vertically
// or horizontally along the gauge depending on the value given to it
//------------------------------------------------------------------------------
class GaugeSlider : public AnalogGauge {
    DECLARE_SUBCLASS(GaugeSlider,AnalogGauge)
    
public:
    GaugeSlider();

    // BasicGL::Graphic interface
    virtual void draw();
                
private:
    LCreal sliderPos;               // direction, in inches, to move our slider (either Y or X-axis depending on the vertical flag)
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
