//------------------------------------------------------------------------------
// Class:       Switch
// Base class:  Basic::Object -> Basic::Component -> BasicGL::Graphic -> Button -> Switch
// Description: Button that can have multiple formats, up to as many as the user
// wants.
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_Switch_H__
#define __Eaagles_Instruments_Switch_H__

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

    bool onSingleClick() override;

    void updateData(const LCreal dt = 0.0) override;

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

#endif
