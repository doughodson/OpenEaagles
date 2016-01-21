//------------------------------------------------------------------------------
// Class: DialPointer
// Base class: BasicGL::Graphic -> AnalogDial -> DialPointer
//
// Description: Pointer that will spin around a defined arc or circle at a defined
// scaled rate.
// Input:  UPDATE_INSTRUMENTS (from instruments)
//------------------------------------------------------------------------------
#ifndef __oe_Instruments_DialPointer_H__
#define __oe_Instruments_DialPointer_H__

#include "openeaagles/instruments/dials/AnalogDial.h"

namespace oe {
namespace Instruments {

class DialPointer : public AnalogDial {
    DECLARE_SUBCLASS(DialPointer,AnalogDial)

public:
    DialPointer();

    void draw() override;
};

}  // end of Instruments namespace
}  // end of oe namespace

#endif
