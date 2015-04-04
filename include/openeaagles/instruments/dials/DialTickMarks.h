//------------------------------------------------------------------------------
// Class: DialTickMarks
// Base class: BasicGL::Graphic -> AnalogDial -> DialTickMarks
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
#ifndef __Eaagles_Instruments_DialTickMarks_H__
#define __Eaagles_Instruments_DialTickMarks_H__

#include "openeaagles/instruments/dials/AnalogDial.h"

namespace Eaagles {
namespace Instruments {

class DialTickMarks : public AnalogDial
{
    DECLARE_SUBCLASS(DialTickMarks,AnalogDial)

public:
    DialTickMarks();

    // get functions
    LCreal getLength() const                    { return length; }
    int  getQuantity() const                    { return quantity; }
    BasicGL::Graphic* getTickGraphic() const    { return myGraphic;  }

    // set functions
    virtual bool setLength(const LCreal newLength);
    virtual bool setQuantity(const int newQ);
    virtual bool setTickGraphic(const BasicGL::Graphic* const newGraphic);

    void drawFunc() override;

protected:
    // slot functions
    bool setSlotLength(const Basic::Number* const newLength);
    bool setSlotQuantity(const Basic::Number* const newQ);

private:
    LCreal      length;             // tick mark length (if not a graphic)
    int         quantity;           // how many tick marks will we have?
    BasicGL::Graphic*  myGraphic; // our graphic (if we choose to use on for a tick mark)
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
