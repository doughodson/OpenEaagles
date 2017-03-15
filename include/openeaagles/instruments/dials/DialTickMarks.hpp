
#ifndef __oe_instruments_DialTickMarks_H__
#define __oe_instruments_DialTickMarks_H__

#include "openeaagles/instruments/dials/AnalogDial.hpp"

namespace oe {
namespace instruments {

//------------------------------------------------------------------------------
// Class: DialTickMarks
//
// Description: Handles tick marks for an analog dial, or any other general
// application.
//
// Input: UPDATE_INSTRUMENTS (from instruments)
// Public member functions:
//      setLength() - set the length of each tick mark (inches)
//      setQuantity() - # of tick marks
//      setTickGraphic() - if you want to use a complex graphic as a tick mark
//      this will override the setLength() function and will use a graphic to
//      draw.
//------------------------------------------------------------------------------
class DialTickMarks : public AnalogDial
{
    DECLARE_SUBCLASS(DialTickMarks,AnalogDial)

public:
    DialTickMarks();

    double getLength() const                    { return length; }
    int  getQuantity() const                    { return quantity; }
    graphics::Graphic* getTickGraphic() const    { return myGraphic;  }

    virtual bool setLength(const double newLength);
    virtual bool setQuantity(const int newQ);
    virtual bool setTickGraphic(const graphics::Graphic* const newGraphic);

    virtual void drawFunc() override;

protected:
    // slot functions
    bool setSlotLength(const base::Number* const newLength);
    bool setSlotQuantity(const base::Number* const newQ);

private:
    double length {1.0};    // tick mark length (if not a graphic)
    int quantity {1};       // how many tick marks will we have?
    graphics::Graphic* myGraphic {};  // our graphic (if we choose to use on for a tick mark)
};

}
}

#endif
