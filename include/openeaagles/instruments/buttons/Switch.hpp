
#ifndef __oe_instruments_Switch_H__
#define __oe_instruments_Switch_H__

#include "openeaagles/instruments/buttons/Button.hpp"

namespace oe {
namespace instruments {

//------------------------------------------------------------------------------
// Class: Switch
// Description: Button that can have multiple formats, up to as many as the user
// wants.
//------------------------------------------------------------------------------
class Switch : public Button
{
    DECLARE_SUBCLASS(Switch,Button)

public:
    Switch();

    // get functions
    int getCurrentState() const   { return currentState; }

    // set functions
    virtual bool setNumSelections(const int x);
    virtual bool setCurrentState(const int x);

    virtual bool onSingleClick() override;

    virtual void updateData(const double dt = 0.0) override;

protected:
    bool setSlotNumSelections(const base::Number* const msg);
    bool setSlotCurrentState(const base::Number* const msg);

private:
    int currentState {1};   // which state are we currently in?
    int numSelections {1};  // number of selections to go through
    SendData currentStateSD;
};

}
}

#endif
