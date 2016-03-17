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
   namespace base { class Table1; }

namespace instruments {

class Instrument : public graphics::Graphic {
    DECLARE_SUBCLASS(Instrument, graphics::Graphic)

public:
    Instrument();

    // get functions
    const base::Table1* getScalingTable() const    { return myTable; }
    double  getInstValue() const                   { return instVal; }
    double  getPreScaleInstValue() const           { return preScaleInstVal; }
    bool isPassingAllowed() const                  { return allowPassing; }

    // set functions
    virtual bool setAllowValPass(const bool newVP);
    virtual bool setInstVal(const double newPos);

    bool event(const int event, base::Object* const obj = nullptr) override;
    void updateData(const double dt = 0.0) override;

protected:
    // slot functions
    bool setSlotScalingTable(const base::Table1* const newTable);
    bool setSlotInstVal(const base::Number* const newVal);
    bool setSlotAllowValPass(const base::Number* const newAVP);

    // event functions
    bool onUpdateInstVal(const base::Number* const newPos);

private:
    // member variables
    const base::Table1* myTable;    // holds our scaling data
    double instVal;                 // our instrument value
    double preScaleInstVal;         // our pre-scaled instrument value (before linear interpolation)
    bool allowPassing;              // do we pass our instrument value down to our components?
};

}
}

#endif
