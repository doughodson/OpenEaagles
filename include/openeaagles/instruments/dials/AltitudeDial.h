//------------------------------------------------------------------------------
// Class: AltitudeDial
//
// Description: A core complex altitude dial.
// Input: The instrument altitude (UPDATE_INSTRUMENTS send command)
// Output: Graphical tape representation of that altitude
//
// Slots:
//      1) hundreds - the hundreds graphical "tape"
//      2) thousands- the thousands "           "
//      3) tenthousands - the ten thousands graphical "tape"
//      4) hunScissors - gives us the x, width, y and height value of the scissors (has to be in that order)
//      5) thouScissors - same as hun
//      6) tenThouScissors - same as hun and thou
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_AltitudeDial_H__
#define __Eaagles_Instruments_AltitudeDial_H__

#include "openeaagles/instruments/Instrument.h"

namespace Eaagles {
namespace Instruments {

class AltitudeDial : public Instrument
{
    DECLARE_SUBCLASS(AltitudeDial,Instrument)

public:
    AltitudeDial();
    
    // Basic::Component interface
    void updateData(const LCreal dt = 0) override;

private:
    bool figureRotation(const LCreal thisAlt);
    
    SendData dialAltSD;                     // sending our altitude to our dial pointer
    SendData altHundredsSD;                 // our hundreds altitude
    SendData altOneThousandsSD;             // our one thousands altitude
    SendData altTenThousandsSD;             // our ten thousands altitude
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
