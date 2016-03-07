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
#ifndef __oe_instruments_AltitudeDial_H__
#define __oe_instruments_AltitudeDial_H__

#include "openeaagles/instruments/Instrument.h"

namespace oe {
namespace instruments {

class AltitudeDial : public Instrument
{
    DECLARE_SUBCLASS(AltitudeDial,Instrument)

public:
    AltitudeDial();

    void updateData(const double dt = 0) override;

private:
    bool figureRotation(const double thisAlt);

    SendData dialAltSD;                     // sending our altitude to our dial pointer
    SendData altHundredsSD;                 // our hundreds altitude
    SendData altOneThousandsSD;             // our one thousands altitude
    SendData altTenThousandsSD;             // our ten thousands altitude
};

}  // end instruments namespace
}  // end oe namespace

#endif
