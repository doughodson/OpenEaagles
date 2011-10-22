//------------------------------------------------------------------------------
// Class: LandingGear
//------------------------------------------------------------------------------
#ifndef	__Eaagles_Instruments_LandingGear_H__
#define __Eaagles_Instruments_LandingGear_H__

#include "openeaagles/instruments/Instrument.h"

namespace Eaagles {
namespace Instruments {

//------------------------------------------------------------------------------
// Class: LandingGear
// 
// Description: Generic Landing Gear readout
//
// Public member functions: 
//      setGearDownValue() (units)
//      setGearUpValue()   (units)
//------------------------------------------------------------------------------
class LandingGear : public Instrument {
    DECLARE_SUBCLASS(LandingGear,Instrument)

public:
    LandingGear();

    // set functions
    virtual bool setGearDownValue(const LCreal newDV);
    virtual bool setGearUpValue(const LCreal newUV);
    
    // get functions
    LCreal getGearUpValue() const   { return gearUV; }
    LCreal getGearDownValue() const { return gearDV; }    
    int getGearState() const        { return gearState; }
    bool getInTransit() const       { return inTransit; }
    LCreal getGearPos() const       { return gearPos; }
    bool getHaveRotary() const      { return haveRotary; }
    
    // BasicGL::Graphic interface
    virtual void drawFunc(); 
    
    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0.0f);
    
protected:
    // slot functions
    bool setSlotGearDownValue(const Basic::Number* const newDV);
    bool setSlotGearUpValue(const Basic::Number* const newUV);
    
private:
    int gearState;      // is our gear down, up, or in transit?
    bool inTransit;     // are we going from one commanded position to another?
    LCreal gearPos;     // our gear position
    LCreal gearDV;      // our gear down indicator value
    LCreal gearUV;      // our gear up indicator value
    
    SendData gearSelSD; // which gear position we are setting
    bool haveRotary;    // do we have a rotary now that is our components (we won't draw if we do)
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
