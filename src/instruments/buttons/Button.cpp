#include "openeaagles/instruments/buttons/Button.hpp"
#include "openeaagles/base/Number.hpp"
#include "openeaagles/graphics/Display.hpp"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(Button, "Button")
EMPTY_SERIALIZER(Button)
EMPTY_DELETEDATA(Button)

BEGIN_SLOTTABLE(Button)
    "eventId",        // Which event we will send for each button (A, B, C events)
END_SLOTTABLE(Button)

BEGIN_SLOT_MAP(Button)
    ON_SLOT(1, setSlotEventId, base::Number)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Button)
    ON_EVENT(ON_SINGLE_CLICK, onSingleClick)
    ON_EVENT(ON_CANCEL, onCancel)
END_EVENT_HANDLER()

Button::Button()
{
    STANDARD_CONSTRUCTOR()
    eventId = -1;
}

void Button::copyData(const Button& org, const bool)
{
    BaseClass::copyData(org);
    eventId = org.eventId;
}

//------------------------------------------------------------------------------
// setSlotEventId() - sets our slot event Id
//------------------------------------------------------------------------------
bool Button::setSlotEventId(const base::Number* const newEvent)
{
    bool ok = false;
    if (newEvent != nullptr) {
        int a = newEvent->getInt();
        ok = setEventId(a);
    }
    return ok;
}

//------------------------------------------------------------------------------
// onSingleClick() - tells us we have been clicked, and we can override this
// to make it do whatever we want.
//------------------------------------------------------------------------------
bool Button::onSingleClick()
{
    // when I am clicked, I will send an event to my container, we find out what
    // event Id we have, and send that eventId
    bool ok = false;
    graphics::Display* myDisplay = (graphics::Display*)findContainerByType(typeid(graphics::Display));

    if (myDisplay != nullptr) {
        myDisplay->buttonEvent(getEventId());
        ok = true;
    }

    return ok;
}

//------------------------------------------------------------------------------
// onCancel() - this is where we cancel button pushes
//------------------------------------------------------------------------------
bool Button::onCancel()
{
    // do nothing here, but our pushbuttons and switches will!
    return true;
}

}
}
