//------------------------------------------------------------------------------
// Class: AnalogDial
//
// Description:  Used as a parent class for other dials, it will draw a background
// that can be visible or not, depending on a flag.  It can also rotate according
// to a scale, or it can be fixed.  This is a generic intelligent background
// that will also pass along instrument values down to its components.
//
// Inputs:
//      UPDATE_INSTRUMENTS -> (from instrument), sets our rotation angle
//      UPDATE_VALUE -> setRadius (inches)
//------------------------------------------------------------------------------
#ifndef __oe_Instruments_AnalogDial_H__
#define __oe_Instruments_AnalogDial_H__

#include "openeaagles/instruments/Instrument.h"

namespace oe {
namespace Instruments {

class AnalogDial : public Instrument
{
    DECLARE_SUBCLASS(AnalogDial,Instrument)

public:
    AnalogDial();

    virtual bool setOriginAngle(const LCreal na);
    virtual bool setSweepAngle(const LCreal newSweepAngle);
    virtual bool setRadius(const LCreal newR);
    virtual bool setMobile(const bool newM);
    virtual bool setSlices(const int x);

    // here are the get functions
    LCreal getStartAngle() const    { return originAngle; }
    LCreal getSweepAngle() const    { return sweepAngle; }
    bool getMobile() const          { return isMobile; }
    LCreal getRadius() const        { return radius; }
    int getSlices() const           { return slices; }

    void drawFunc() override;

    bool event(const int event, Basic::Object* const obj = nullptr) override;

protected:
    // slot functions
    bool setSlotOriginAngle(const Basic::Number* const newAngle);
    bool setSlotMobile(const Basic::Number* const newM);
    bool setSlotSweepAngle(const Basic::Number* const newSweepAngle);
    bool setSlotRadius(const Basic::Number* const newR);
    bool setSlotSlices(const Basic::Number* const x);
    // event function
    virtual bool onUpdateRadius(const Basic::Number* const x);

private:
    LCreal originAngle;             // angle we start drawing ticks from (degrees, default is 0)
    LCreal positionAngle;           // our position (if we are being rotated)
    LCreal  sweepAngle;             // how far around the circle we sweep
    LCreal   radius;                // radius of our background
    bool    isMobile;               // are we moving around on the dial, or just sending the value down (to our components)
    int slices;                     // number of slices to use while drawing
};

}  // end Instruments namespace
}  // end oe namespace

#endif
