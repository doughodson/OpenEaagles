//------------------------------------------------------------------------------
// Class: Instrument
// Base class: graphics::Graphic -> Instrument
//
// Description: Base class for all instruments, this class will receive a value
// via an UPDATE_INSTRUMENTS command, and from there will determine (if we have a
// scaling table) what our scaled table is.  It will then determine if we have a
// color rotary, and if we do, it will send the value to it to update itself...
//
// Inputs for send commands:
//      UPDATE_INSTRUMENT - updates the generic instruments value, and scales
//      it if there is a scaling table available.  The flag allowPassing will
//      determine if we pass the token down to our component instruments or not.
//------------------------------------------------------------------------------
#ifndef __oe_instruments_Instrument_H__
#define __oe_instruments_Instrument_H__

#include "openeaagles/graphics/Graphic.h"

namespace oe {
   namespace basic { class Table1; }

namespace instruments {

class Instrument : public graphics::Graphic {
    DECLARE_SUBCLASS(Instrument,graphics::Graphic)

public:
    Instrument();

    // get functions
    const basic::Table1* getScalingTable() const    { return myTable; }
    LCreal  getInstValue() const                    { return instVal; }
    LCreal  getPreScaleInstValue() const            { return preScaleInstVal; }
    bool isPassingAllowed() const                   { return allowPassing; }

    // set functions
    virtual bool setAllowValPass(const bool newVP);
    virtual bool setInstVal(const LCreal newPos);

    bool event(const int event, basic::Object* const obj = nullptr) override;
    void updateData(const LCreal dt = 0.0) override;

protected:
    // slot functions
    bool setSlotScalingTable(const basic::Table1* const newTable);
    bool setSlotInstVal(const basic::Number* const newVal);
    bool setSlotAllowValPass(const basic::Number* const newAVP);

    // event functions
    bool onUpdateInstVal(const basic::Number* const newPos);

private:
    // member variables
    const basic::Table1* myTable; // holds our scaling data
    LCreal instVal;                 // our instrument value
    LCreal preScaleInstVal;         // our pre-scaled instrument value (before linear interpolation)
    bool allowPassing;              // do we pass our instrument value down to our components?
};

}  // end instruments namespace
}  // end oe namespace

#endif
