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
#ifndef __oe_instruments_AnalogDial_H__
#define __oe_instruments_AnalogDial_H__

#include "openeaagles/instruments/Instrument.hpp"

namespace oe {
namespace instruments {

class AnalogDial : public Instrument
{
    DECLARE_SUBCLASS(AnalogDial,Instrument)

public:
    AnalogDial();

    virtual bool setOriginAngle(const double na);
    virtual bool setSweepAngle(const double newSweepAngle);
    virtual bool setRadius(const double newR);
    virtual bool setMobile(const bool newM);
    virtual bool setSlices(const int x);

    // here are the get functions
    double getStartAngle() const    { return originAngle; }
    double getSweepAngle() const    { return sweepAngle; }
    bool getMobile() const          { return isMobile; }
    double getRadius() const        { return radius; }
    int getSlices() const           { return slices; }

    virtual void drawFunc() override;

    virtual bool event(const int event, base::Object* const obj = nullptr) override;

protected:
    // slot functions
    bool setSlotOriginAngle(const base::Number* const newAngle);
    bool setSlotMobile(const base::Number* const newM);
    bool setSlotSweepAngle(const base::Number* const newSweepAngle);
    bool setSlotRadius(const base::Number* const newR);
    bool setSlotSlices(const base::Number* const x);
    // event function
    virtual bool onUpdateRadius(const base::Number* const x);

private:
    double originAngle;             // angle we start drawing ticks from (degrees, default is 0)
    double positionAngle;           // our position (if we are being rotated)
    double  sweepAngle;             // how far around the circle we sweep
    double   radius;                // radius of our background
    bool    isMobile;               // are we moving around on the dial, or just sending the value down (to our components)
    int slices;                     // number of slices to use while drawing
};

}
}

#endif
