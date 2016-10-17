//------------------------------------------------------------------------------
// Class: DialPointer
// Base class: graphics::Graphic -> AnalogDial -> DialPointer
//
// Description: Pointer that will spin around a defined arc or circle at a defined
// scaled rate.
// Input:  UPDATE_INSTRUMENTS (from instruments)
//------------------------------------------------------------------------------
#ifndef __oe_instruments_DialPointer_H__
#define __oe_instruments_DialPointer_H__

#include "openeaagles/instruments/dials/AnalogDial.hpp"

namespace oe {
namespace instruments {

class DialPointer : public AnalogDial {
    DECLARE_SUBCLASS(DialPointer,AnalogDial)

public:
    DialPointer();

    virtual void draw() override;
};

}
}

#endif
