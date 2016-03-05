//------------------------------------------------------------------------------
// Class: LandingGear
//------------------------------------------------------------------------------
#ifndef __oe_instruments_LandingGear_H__
#define __oe_instruments_LandingGear_H__

#include "openeaagles/instruments/Instrument.h"

namespace oe {
namespace instruments {

//------------------------------------------------------------------------------
// Class: LandingGear
//
// Description: Generic Landing Gear readout
//
// Public member functions:
//      setGearDownValue() (units)
//      setGearUpValue()   (units)
//------------------------------------------------------------------------------
class LandingGear : public Instrument
{
    DECLARE_SUBCLASS(LandingGear,Instrument)

public:
    LandingGear();

    virtual bool setGearDownValue(const LCreal newDV);
    virtual bool setGearUpValue(const LCreal newUV);

    LCreal getGearUpValue() const   { return gearUV; }
    LCreal getGearDownValue() const { return gearDV; }
    int getGearState() const        { return gearState; }
    bool getInTransit() const       { return inTransit; }
    LCreal getGearPos() const       { return gearPos; }
    bool getHaveRotary() const      { return haveRotary; }

    void drawFunc() override;

    void updateData(const LCreal dt = 0.0) override;

protected:
    // slot functions
    bool setSlotGearDownValue(const base::Number* const newDV);
    bool setSlotGearUpValue(const base::Number* const newUV);

private:
    int gearState;      // is our gear down, up, or in transit?
    bool inTransit;     // are we going from one commanded position to another?
    LCreal gearPos;     // our gear position
    LCreal gearDV;      // our gear down indicator value
    LCreal gearUV;      // our gear up indicator value

    SendData gearSelSD; // which gear position we are setting
    bool haveRotary;    // do we have a rotary now that is our components (we won't draw if we do)
};

}  // end instruments namespace
}  // end oe namespace

#endif
