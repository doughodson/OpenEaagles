#include "openeaagles/instruments/gauges/GaugeSlider.h"

namespace Eaagles {
namespace Instruments {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(GaugeSlider, "GaugeSlider")
EMPTY_SERIALIZER(GaugeSlider)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
GaugeSlider::GaugeSlider()
{
    STANDARD_CONSTRUCTOR()
    sliderPos = 0;
    setDrawMe(false);
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void GaugeSlider::copyData(const GaugeSlider& org, const bool)
{
    BaseClass::copyData(org);
    sliderPos = org.sliderPos;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(GaugeSlider)


//------------------------------------------------------------------------------
// draw() -- draws the object(s)
//------------------------------------------------------------------------------
void GaugeSlider::draw()
{
    sliderPos = getInstValue();
    bool vertical = isVertical();
    lcSaveMatrix();
    if (vertical) lcTranslate(0, sliderPos);
    else lcTranslate(sliderPos, 0);

        BaseClass::draw();

    lcRestoreMatrix();
}

}  // end Instruments namespace
}  // end Eaagles namespace
