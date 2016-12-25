
#include "openeaagles/instruments/buttons/Switch.hpp"
#include "openeaagles/base/Number.hpp"
#include <iostream>

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(Switch, "Switch")
EMPTY_SERIALIZER(Switch)
EMPTY_DELETEDATA(Switch)

BEGIN_SLOTTABLE(Switch)
   "selections",    // 1) How many selections are there?
   "currentState",  // 2) Which state do you want to start on?
END_SLOTTABLE(Switch)

BEGIN_SLOT_MAP(Switch)
   ON_SLOT(1, setSlotNumSelections, base::Number)
   ON_SLOT(2, setSlotCurrentState, base::Number)
END_SLOT_MAP()

Switch::Switch()
{
    STANDARD_CONSTRUCTOR()
    currentState = 1;
    numSelections = 1;
    currentStateSD.empty();
}

void Switch::copyData(const Switch& org, const bool)
{
    BaseClass::copyData(org);
    currentState = org.currentState;
    numSelections = org.numSelections;
    currentStateSD.empty();
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotNumSelections() - set the number of "stops"
//------------------------------------------------------------------------------
bool Switch::setSlotNumSelections(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) ok = setNumSelections(msg->getInt());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotCurrentState() - set the state of our switch
//------------------------------------------------------------------------------
bool Switch::setSlotCurrentState(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) ok = setCurrentState(msg->getInt());
    return ok;
}

// SET FUNCTIONS
//------------------------------------------------------------------------------
// setNumSelections() -
//------------------------------------------------------------------------------
bool Switch::setNumSelections(const int x)
{
    numSelections = x;
    return true;
}

//------------------------------------------------------------------------------
// setCurrentState() -
//------------------------------------------------------------------------------
bool Switch::setCurrentState(const int x)
{
    if (x <= numSelections) currentState = x;
    else currentState = numSelections;
    return true;
}

//------------------------------------------------------------------------------
// onSingleClick() detected when button is hit!
//------------------------------------------------------------------------------
bool Switch::onSingleClick()
{
    if (currentState < numSelections) currentState++;
    else currentState = 1;
    return BaseClass::onSingleClick();
}

void Switch::updateData(const double dt)
{
    BaseClass::updateData(dt);

    send("switch", SELECT, currentState, currentStateSD);
}

}
}
