#include "openeaagles/instruments/buttons/Switch.h"
#include "openeaagles/base/Number.h"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(Switch,"Switch")
EMPTY_SERIALIZER(Switch)

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Switch)
   "selections",    // 1) How many selections are there?
   "currentState",  // 2) Which state do you want to start on?
END_SLOTTABLE(Switch)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Switch)
   ON_SLOT(1, setSlotNumSelections, basic::Number)
   ON_SLOT(2, setSlotCurrentState, basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Switch::Switch()
{
    STANDARD_CONSTRUCTOR()
    currentState = 1;
    numSelections = 1;
    currentStateSD.empty();
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Switch::copyData(const Switch& org, const bool)
{
    BaseClass::copyData(org);
    currentState = org.currentState;
    numSelections = org.numSelections;
    currentStateSD.empty();
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Switch)

// SLOT functions
//------------------------------------------------------------------------------
// setSlotNumSelections() - set the number of "stops"
//------------------------------------------------------------------------------
bool Switch::setSlotNumSelections(const basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) ok = setNumSelections(msg->getInt());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotCurrentState() - set the state of our switch
//------------------------------------------------------------------------------
bool Switch::setSlotCurrentState(const basic::Number* const msg)
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

//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void Switch::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);

    send("switch", SELECT, currentState, currentStateSD);
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Switch
//------------------------------------------------------------------------------
basic::Object* Switch::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end instruments namespace
}  // end oe namespace
