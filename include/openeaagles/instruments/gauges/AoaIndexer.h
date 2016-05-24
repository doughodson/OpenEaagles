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
#ifndef __oe_instruments_AoAIndexer_H__
#define __oe_instruments_AoAIndexer_H__

#include "openeaagles/instruments/Instrument.h"

namespace oe {
namespace instruments {

class AoAIndexer : public Instrument
{
    DECLARE_SUBCLASS(AoAIndexer,Instrument)

public:
    AoAIndexer();

    virtual bool setAoaRedMin(const double a);
    virtual bool setAoaRedMax(const double b);
    virtual bool setAoaYellowMin(const double a);
    virtual bool setAoaYellowMax(const double b);
    virtual bool setAoaGreenMin(const double a);
    virtual bool setAoaGreenMax(const double b);

    double getAoaRedMin() const     { return aoaRedMin; }
    double getAoaRedMax() const     { return aoaRedMax; }
    double getAoaYellowMin() const  { return aoaYellowMin; }
    double getAoaYellowMax() const  { return aoaYellowMax; }
    double getAoaGreenMin() const   { return aoaGreenMin; }
    double getAoaGreenMax() const   { return aoaGreenMax; }
    int    getState() const         { return aoaState; }

    virtual void drawFunc() override;

    virtual void updateData(const double dt = 0) override;

protected:

    // slot functions
    virtual bool setSlotAoaRedMin(const base::Number* const newRMin);
    virtual bool setSlotAoaRedMax(const base::Number* const newRMax);
    virtual bool setSlotAoaYellowMin(const base::Number* const newYMin);
    virtual bool setSlotAoaYellowMax(const base::Number* const newYMax);
    virtual bool setSlotAoaGreenMax(const base::Number* const newGMax);
    virtual bool setSlotAoaGreenMin(const base::Number* const newGMin);

private:

//  --  -> aoaMax
//
//  --  -> aoaZero
//
//  --  -> aoaMin

    double aoaRedMin;       // min value of where the red portion of our aoa lights up
    double aoaRedMax;       // max value of where the red portion of our aoa lights up
    double aoaYellowMin;    // min value of where the yellow portion of our aoa lights up
    double aoaYellowMax;    // max value of where the yellow portion of our aoa lights up
    double aoaGreenMin;     // min value of where the green portion of our aoa lights up
    double aoaGreenMax;     // max value of where the green portion of our aoa lights up

    GLuint displayList;     // our display list name
    bool isDlist;           // do we have a display list?
    int aoaState;           // shows which state we are in
    SendData selectSD;      // which graphics are we selecting? (If we are using a rotary)
};

}
}

#endif
