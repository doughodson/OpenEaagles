//------------------------------------------------------------------------------
// Class: DialArcSegment
// Base class: graphics::Graphic -> AnalogDial -> DialArcSegment
//
// Description: An arc or circle that will be drawn according to the start
// angle and sweep angle set in AnalogDial.
// Input:  UPDATE_INSTRUMENTS (from Instrument)
//------------------------------------------------------------------------------
#ifndef __oe_instruments_DialArcSegment_H__
#define __oe_instruments_DialArcSegment_H__

#include "openeaagles/instruments/dials/AnalogDial.h"

namespace oe {
namespace instruments {

class DialArcSegment : public AnalogDial {
    DECLARE_SUBCLASS(DialArcSegment,AnalogDial)

public:
    DialArcSegment();

    // set functions
    virtual bool setIsDynamic(const bool newID);
    virtual bool setOuterRadius(const double x);
    virtual bool setFilled(const bool x);

    virtual void drawFunc() override;

    virtual void updateData(const double dt = 0.0) override;

protected:
    bool setSlotIsDynamic(const base::Number* const newD);
    bool setSlotOuterRadius(const base::Number* const x);
    bool setSlotFilled(const base::Number* const x);

    virtual bool onUpdateRadius(const base::Number* const x) override;

private:
    bool isDynamic;     // are we setting sweep angle based on value?
    double outerRadius;
    bool filled;
};

}
}

#endif
