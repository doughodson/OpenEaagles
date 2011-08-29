//------------------------------------------------------------------------------
// Class:       Switch
// Base class:  Basic::Object -> Basic::Component -> BasicGL::Graphic -> Button -> Switch
// Description: Button that can have multiple formats, up to as many as the user
// wants.  
//------------------------------------------------------------------------------
#ifndef __OPENEAAGLES_INSTRUMENTS_SWITCH_H__
#define __OPENEAAGLES_INSTRUMENTS_SWITCH_H__

#include "openeaagles/instruments/buttons/Button.h"

namespace Eaagles {
namespace Instruments {

class Switch : public Button {
    DECLARE_SUBCLASS(Switch,Button)
    

public:
    Switch();
        
    // get functions
    int getCurrentState() const   { return currentState; }
        
    // set functions
    virtual bool setNumSelections(const int x); 
    virtual bool setCurrentState(const int x);

    // Button interface
    virtual bool onSingleClick();
    
    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0.000000);    
    
protected:
    bool setSlotNumSelections(const Basic::Number* const msg);
    bool setSlotCurrentState(const Basic::Number* const msg);    
    
private:    
    int currentState;   // which state are we currently in?
    int numSelections;  // number of selections to go through
    SendData currentStateSD;
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif // __OPENEAAGLES_INSTRUMENTS_SWITCH_H__
