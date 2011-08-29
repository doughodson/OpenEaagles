//------------------------------------------------------------------------------
// Class: Instrument
// Base class: BasicGL::Graphic -> Instrument
// 
// Description: Baseclass for all instruments, this class will receive a value
// via an UPDATE_INSTRUMENTS command, and from there will determine (if we have a
// scaling table) what our scaled table is.  It will then determine if we have a
// color rotary, and if we do, it will send the value to it to update itself...
//
// Inputs for send commands:
//      UPDATE_INSTRUMENT - updates the generic instruments value, and scales
//      it if there is a scaling table available.  The flag allowPassing will
//      determine if we pass the token down to our component instruments or not.
//------------------------------------------------------------------------------
#ifndef	__OPENEAAGLES_INSTRUMENTS_INSTRUMENT_H__
#define __OPENEAAGLES_INSTRUMENTS_INSTRUMENT_H__

#include "openeaagles/basicGL/Graphic.h"

namespace Eaagles {
   namespace Basic { class Table1; }

namespace Instruments {

class Instrument : public BasicGL::Graphic {
    DECLARE_SUBCLASS(Instrument,BasicGL::Graphic)
    
public:
    Instrument();

    // get functions
    const Basic::Table1* getScalingTable() const { return myTable; } 
    LCreal  getInstValue() const          { return instVal; } 
    LCreal  getPreScaleInstValue() const  { return preScaleInstVal; }
    bool isPassingAllowed() const         { return allowPassing; }

    // set functions
    virtual bool setAllowValPass(const bool newVP); 
    virtual bool setInstVal(const LCreal newPos);
    
    // Basic::Component interface
    virtual bool event(const int event, Basic::Object* const obj = 0);  
    virtual void updateData(const LCreal dt = 0.000000);

protected:
    // slot functions
    bool setSlotScalingTable(const Basic::Table1* const newTable);
    bool setSlotInstVal(const Basic::Number* const newVal);
    bool setSlotAllowValPass(const Basic::Number* const newAVP);
    
    // event functions
    bool onUpdateInstVal(const Basic::Number* const newPos);
    
private:
    // member variables
    const Basic::Table1* myTable; // holds our scaling data
    LCreal instVal;                 // our instrument value
    LCreal preScaleInstVal;         // our pre-scaled instrument value (before linear interpolation)
    bool allowPassing;              // do we pass our instrument value down to our components?
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif	// __OPENEAAGLES_INSTRUMENTS_INSTRUMENT_H__

