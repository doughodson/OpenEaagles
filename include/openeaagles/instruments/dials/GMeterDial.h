//------------------------------------------------------------------------------
// Class:	BasicGL::Graphic -> AnalogDial -> GMeterDial
//
// Description: G meter, which simply keeps track of min and max and actual G's
//
// Inputs: 
//      UPDATE_INSTRUMENTS (from instruments) -> g's
//      RESET_EVENT -> resets min and max g's
//------------------------------------------------------------------------------
#ifndef	__OPENEAAGLES_INSTRUMENTS_GMETERDIAL_H__
#define __OPENEAAGLES_INSTRUMENTS_GMETERDIAL_H__

#include "openeaagles/instruments/dials/AnalogDial.h"

namespace Eaagles {
namespace Instruments {

class GMeterDial : public AnalogDial {
    DECLARE_SUBCLASS(GMeterDial,AnalogDial)

public:
    GMeterDial();
    
    // BasicGL::Graphic interface
    virtual void drawFunc(); 
    
    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0.000000f);
    virtual void reset();
    
private:
    LCreal rotation;    // our rotation angle (of the gauge arrow)
    LCreal maxG;        // max g value
    LCreal minG;        // min g value
    
    SendData gsSD;      // actual gs
    SendData minGSD;    // min gs
    SendData maxGSD;    // max gs
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif	// __OPENEAAGLES_INSTRUMENTS_GMETERDIAL_H__
