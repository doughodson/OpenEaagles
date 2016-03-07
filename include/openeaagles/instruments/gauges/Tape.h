//------------------------------------------------------------------------------
// Class: Tape
// Base class: Instrument -> Tape
// Description: Dynamic class that renders only the portion of the tape
//              that is needed on the screen
//------------------------------------------------------------------------------
#ifndef __oe_instruments_Tape_H__
#define __oe_instruments_Tape_H__

#include "openeaagles/instruments/Instrument.h"

namespace oe {
namespace instruments {

class Tape : public Instrument
{
    DECLARE_SUBCLASS(Tape, Instrument)

public:
    Tape();

    void updateData(const double dt = 0.0) override;

    virtual bool setRange(const int x)                  { range = x; return true;     }
    virtual bool setHeight(const double x)              { height = x; return true;    }
    virtual bool setIncrement(const int x)              { increment = x; return true; }
    virtual bool setVertical(const bool x)              { vertical = x; return true;  }
    virtual bool setMaxNumber(const double x)           { maxNum = x; return true;    }
    virtual bool setMinNumber(const double x)           { minNum = x; return true;    }
    virtual bool setConvert(const bool x)               { convert = x; return true;   }

protected:
    bool setSlotRange(const base::Number* const x);
    bool setSlotHeight(const base::Number* const x);
    bool setSlotIncrement(const base::Number* const x);
    bool setSlotVertical(const base::Number* const x);
    bool setSlotMaxNum(const base::Number* const x);
    bool setSlotMinNum(const base::Number* const x);
    bool setSlotConvert(const base::Number* const x);

private:
    SendData transTapeGraphicSD;    // our translation of our tape graphic
    SendData transTapeGraphicVSD;   // vertical

    static const int MAX_NUMBERS = 25;
    int numberValsHunds[MAX_NUMBERS];
    int numberValsHundsVis[MAX_NUMBERS];
    int numberValsThous[MAX_NUMBERS];
    int numberValsThousVis[MAX_NUMBERS];
    SendData numberValsHundsSD[MAX_NUMBERS];
    SendData numberValsThousSD[MAX_NUMBERS];
    SendData numberValsHundsVisSD[MAX_NUMBERS];
    SendData numberValsThousVisSD[MAX_NUMBERS];
    int range;
    double height;
    int increment;
    bool vertical;
    double maxNum;
    double minNum;
    bool convert;
};

}
}

#endif
