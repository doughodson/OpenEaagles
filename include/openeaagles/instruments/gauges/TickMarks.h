//------------------------------------------------------------------------------
// Class: TickMarks
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_TickMarks_H__
#define __Eaagles_Instruments_TickMarks_H__

#include "openeaagles/instruments/gauges/AnalogGauge.h"

namespace Eaagles {
namespace Instruments {

//------------------------------------------------------------------------------
// Class: TickMarks
//
// Description: Uses tick marks in a linear fashion, either horizontally or
// vertically, and draws them accordingly.  This is very similar to dial
// tick marks, only it doesn't rotate and doesn't draw around the circle.
//------------------------------------------------------------------------------
class TickMarks : public AnalogGauge
{
    DECLARE_SUBCLASS(TickMarks,AnalogGauge)

public:
    TickMarks();

    // set functions
    virtual bool setTickMarkLength(const LCreal newLength);
    virtual bool setQuantity(const int newQ);
    virtual bool setGaugeLength(const LCreal newL);
    virtual bool setFlip(const bool x);
    virtual bool setTickGraphic(const BasicGL::Graphic* const newGraphic);

    // get functions
    LCreal getTickMarkLength() const { return lengthTM; }
    int  getQuantity() const         { return quantity; }

    void drawFunc() override;

protected:
    // slot functions
    bool setSlotTickMarkLength(const Basic::Number* const newLength);
    bool setSlotQuantity(const Basic::Number* const newQ);
    bool setSlotGaugeLength(const Basic::Number* const newL);
    bool setSlotFlip(const Basic::Number* const x);

private:
    LCreal lengthTM;       // tick mark length (if not a graphic)
    int    quantity;       // how many tick marks will we have?
    LCreal gaugeLength;    // length we are spanning our tick marks over
    bool   flip;           // our flip variable
    BasicGL::Graphic* myGraphic; // our graphic (if we choose to use one for a tick mark)
};

}  // end of Instruments namespace
}  // end of Eaagles namespace

#endif
