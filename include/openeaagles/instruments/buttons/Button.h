//------------------------------------------------------------------------------
// Class:       Button
// Base class:  Basic::Object -> Basic::Component -> BasicGL::Graphic -> Button
// Description: Generic button that knows when it is clicked
//------------------------------------------------------------------------------
#ifndef __oe_Instruments_Button_H__
#define __oe_Instruments_Button_H__

#include "openeaagles/basicGL/Graphic.h"

namespace oe {
namespace Instruments {

class Button : public BasicGL::Graphic {
    DECLARE_SUBCLASS(Button,BasicGL::Graphic)

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

    bool event(const int event, Basic::Object* const obj = nullptr) override;

protected:
    // Sets the Event ID to newEvent
    virtual bool setSlotEventId(const Basic::Number* const newEvent);

private:
    int eventId;        // holds our event number
};

}  // end Instruments namespace
}  // end oe namespace

#endif
