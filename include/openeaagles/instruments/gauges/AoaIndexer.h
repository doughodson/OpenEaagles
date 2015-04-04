//------------------------------------------------------------------------------
// Class: Instrument -> AoAIndexer
//
// Description: Generic AOA gauge that, given limits, will indicate over, under,
// and in-range aoa values.
//
// Inputs:
//      UPDATE_INSTRUMENTS token = actual aoa
//      All the rest are slots (red min, max, yellow min, max, and green min, max)
//      with:
//      red -> over limit area
//      green -> in limit area
//      yellow -> under limit area
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_AoAIndexer_H__
#define __Eaagles_Instruments_AoAIndexer_H__

#include "openeaagles/instruments/Instrument.h"

namespace Eaagles {
namespace Instruments {

class AoAIndexer : public Instrument
{
    DECLARE_SUBCLASS(AoAIndexer,Instrument)

public:

    AoAIndexer();

    // set functions
    virtual bool setAoaRedMin(const LCreal a);
    virtual bool setAoaRedMax(const LCreal b);
    virtual bool setAoaYellowMin(const LCreal a);
    virtual bool setAoaYellowMax(const LCreal b);
    virtual bool setAoaGreenMin(const LCreal a);
    virtual bool setAoaGreenMax(const LCreal b);

    // get functions
    LCreal getAoaRedMin() const     { return aoaRedMin; }
    LCreal getAoaRedMax() const     { return aoaRedMax; }
    LCreal getAoaYellowMin() const  { return aoaYellowMin; }
    LCreal getAoaYellowMax() const  { return aoaYellowMax; }
    LCreal getAoaGreenMin() const   { return aoaGreenMin; }
    LCreal getAoaGreenMax() const   { return aoaGreenMax; }
    int    getState() const         { return aoaState; }

    void drawFunc() override;

    void updateData(const LCreal dt = 0) override;

protected:

    // slot functions
    virtual bool setSlotAoaRedMin(const Basic::Number* const newRMin);
    virtual bool setSlotAoaRedMax(const Basic::Number* const newRMax);
    virtual bool setSlotAoaYellowMin(const Basic::Number* const newYMin);
    virtual bool setSlotAoaYellowMax(const Basic::Number* const newYMax);
    virtual bool setSlotAoaGreenMax(const Basic::Number* const newGMax);
    virtual bool setSlotAoaGreenMin(const Basic::Number* const newGMin);

private:

//  --  -> aoaMax
//
//  --  -> aoaZero
//
//  --  -> aoaMin

    LCreal aoaRedMin;       // min value of where the red portion of our aoa lights up
    LCreal aoaRedMax;       // max value of where the red portion of our aoa lights up
    LCreal aoaYellowMin;    // min value of where the yellow portion of our aoa lights up
    LCreal aoaYellowMax;    // max value of where the yellow portion of our aoa lights up
    LCreal aoaGreenMin;     // min value of where the green portion of our aoa lights up
    LCreal aoaGreenMax;     // max value of where the green portion of our aoa lights up

    GLuint displayList;     // our display list name
    bool isDlist;           // do we have a display list?
    int    aoaState;        // shows which state we are in
    SendData selectSD;      // which graphics are we selecting? (If we are using a rotary)
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
