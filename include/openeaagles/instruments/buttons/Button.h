//------------------------------------------------------------------------------
// Class:       Button
// Base class:  base::Object -> base::Component -> graphics::Graphic -> Button
// Description: Generic button that knows when it is clicked
//------------------------------------------------------------------------------
#ifndef __oe_instruments_Button_H__
#define __oe_instruments_Button_H__

#include "openeaagles/graphics/Graphic.h"

namespace oe {
namespace instruments {

class Button : public graphics::Graphic {
    DECLARE_SUBCLASS(Button,graphics::Graphic)

public:
    Button();

    // Returns the event ID.
    virtual int getEventId() const               { return eventId; }

    // Sets the Event ID to newID.
    virtual bool setEventId(const int newId)     { eventId = newId; return true; };

    // Right now, this function merely detects when the button has been
    // clicked, but it can be overridden to do anything.
    virtual bool onSingleClick();

    // Cancel
    virtual bool onCancel();

    bool event(const int event, base::Object* const obj = nullptr) override;

protected:
    // Sets the Event ID to newEvent
    virtual bool setSlotEventId(const base::Number* const newEvent);

private:
    int eventId;        // holds our event number
};

}  // end instruments namespace
}  // end oe namespace

#endif
