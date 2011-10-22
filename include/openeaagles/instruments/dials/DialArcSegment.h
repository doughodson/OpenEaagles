//------------------------------------------------------------------------------
// Class:	DialArcSegment
// Base class: BasicGL::Graphic -> AnalogDial -> DialArcSegment
//
// Description: An arc or circle that will be drawn according to the start
// angle and sweep angle set in AnalogDial.  
// Input:  UPDATE_INSTRUMENTS (from Instrument) 
//------------------------------------------------------------------------------
#ifndef	__Eaagles_Instruments_DialArcSegment_H__
#define __Eaagles_Instruments_DialArcSegment_H__

#include "openeaagles/instruments/dials/AnalogDial.h"

namespace Eaagles {
namespace Instruments {

class DialArcSegment : public AnalogDial {
    DECLARE_SUBCLASS(DialArcSegment,AnalogDial)
    
public:
    DialArcSegment();
    
    // set functions
    virtual bool setIsDynamic(const bool newID);
    virtual bool setOuterRadius(const LCreal x);
    virtual bool setFilled(const bool x);

    // BasicGL::Graphic interface
    virtual void drawFunc();

    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0.0000000);
    
protected:
    bool setSlotIsDynamic(const Basic::Number* const newD);
    bool setSlotOuterRadius(const Basic::Number* const x);
    bool setSlotFilled(const Basic::Number* const x);

    // event function
    virtual bool onUpdateRadius(const Basic::Number* const x);

private:
    bool isDynamic;     // are we setting sweep angle based on value?
    LCreal outerRadius;
    bool filled;
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
