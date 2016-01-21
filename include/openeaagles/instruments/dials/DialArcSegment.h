//------------------------------------------------------------------------------
// Class: DialArcSegment
// Base class: BasicGL::Graphic -> AnalogDial -> DialArcSegment
//
// Description: An arc or circle that will be drawn according to the start
// angle and sweep angle set in AnalogDial.
// Input:  UPDATE_INSTRUMENTS (from Instrument)
//------------------------------------------------------------------------------
#ifndef __oe_Instruments_DialArcSegment_H__
#define __oe_Instruments_DialArcSegment_H__

#include "openeaagles/instruments/dials/AnalogDial.h"

namespace oe {
namespace Instruments {

class DialArcSegment : public AnalogDial {
    DECLARE_SUBCLASS(DialArcSegment,AnalogDial)

public:
    DialArcSegment();

    // set functions
    virtual bool setIsDynamic(const bool newID);
    virtual bool setOuterRadius(const LCreal x);
    virtual bool setFilled(const bool x);

    void drawFunc() override;

    void updateData(const LCreal dt = 0.0) override;

protected:
    bool setSlotIsDynamic(const basic::Number* const newD);
    bool setSlotOuterRadius(const basic::Number* const x);
    bool setSlotFilled(const basic::Number* const x);

    bool onUpdateRadius(const basic::Number* const x) override;

private:
    bool isDynamic;     // are we setting sweep angle based on value?
    LCreal outerRadius;
    bool filled;
};

}  // end Instruments namespace
}  // end oe namespace

#endif
