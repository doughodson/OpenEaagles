//------------------------------------------------------------------------------
// Class: DialPointer
// Base class: BasicGL::Graphic -> AnalogDial -> DialPointer
// 
// Description: Pointer that will spin around a defined arc or circle at a defined
// scaled rate.  
// Input:  UPDATE_INSTRUMENTS (from instruments)
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_DialPointer_H__
#define __Eaagles_Instruments_DialPointer_H__

#include "openeaagles/instruments/dials/AnalogDial.h"

namespace Eaagles {
namespace Instruments {

class DialPointer : public AnalogDial {
    DECLARE_SUBCLASS(DialPointer,AnalogDial)
    
public:
    DialPointer();

    // BasicGL::Graphic interface
    virtual void draw();
};

}  // end of Instruments namespace
}  // end of Eaagles namespace

#endif
