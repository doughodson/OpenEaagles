//------------------------------------------------------------------------------
// Class:       RotarySwitch
// Base class:  Basic::Object -> Basic::Component -> BasicGL::Graphic -> Button -> RotarySwitch
// Description: Generic multi-position selector switch that knows when it is left or right 
//                mouse clicked and moves to next or previous switch position.
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_RotarySwitch_H__
#define __Eaagles_Instruments_RotarySwitch_H__

#include "openeaagles/instruments/buttons/Button.h"

namespace Eaagles {
namespace Instruments {

class RotarySwitch : public Button {
    DECLARE_SUBCLASS(RotarySwitch,Button)

public:
   RotarySwitch();

   // Get current switch position
   int getCurrentPosition() const { return currentPosition; }; 

   // Button class interface
   virtual bool onSingleClick();

   // Basic::Component class interface
   virtual void updateData(const LCreal dt = 0.00000f);
    
protected: 
    bool setSlotAngles(const Basic::PairStream* const stream);
    bool setSlotStartPosition(const Basic::Number* const x);

private:  
    static const int MAX_ANGLES = 500;

    int currentPosition;    //current switch position
    LCreal angles[MAX_ANGLES];
    int numAngs;
    int startPosition;
    SendData angleSD;       // angle to send to our rotator
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
