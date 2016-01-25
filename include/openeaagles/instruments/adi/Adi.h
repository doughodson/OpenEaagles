//------------------------------------------------------------------------------
// Class: Adi
//
// Description: Adi that knows how to translate and rotate its pitch and roll
// ladder according to aircraft pitch and roll.  You can either push data
// down by using the send function (see below) or you can get a pointer and
// use the member functions setPitch() and setRoll().
//
// Inputs for the send command:
//      UPDATE_INSTRUMENTS = updates pitch (this overrides the instrument function, because
//      we want to drive the ADI towards pitch, not have real time pitch, always in degrees)
//      UPDATE_VALUE = updates roll (degrees)
//      UPDATE_VALUE2 = updates roll (radians)
//      UPDATE_VALUE3 = updates max rate (degrees/second)
//------------------------------------------------------------------------------
#ifndef __oe_instruments_Adi_H__
#define __oe_instruments_Adi_H__

#include "openeaagles/instruments/Instrument.h"
#include "openeaagles/basic/units/Angles.h"

namespace oe {
namespace instruments {

class Adi : public Instrument {
    DECLARE_SUBCLASS(Adi,Instrument)

public:
    Adi();

    LCreal getPitchInches() const       { return scaledPitch; }
    LCreal getPitch() const             { return getPreScaleInstValue(); }
    LCreal getRollDeg() const           { return curPhi * static_cast<LCreal>(basic::Angle::R2DCC); }
    LCreal getRollRad() const           { return curPhi; }
    LCreal getMaxRate() const           { return maxRate; }

    virtual bool setRollRad(const LCreal newR);
    virtual bool setRollDeg(const LCreal newR);
    virtual bool setPitch(const LCreal newP);
    virtual bool setMaxRate(const LCreal newMR);

    void draw() override;

    bool event(const int event, basic::Object* const obj = nullptr) override;
    void updateData(const LCreal dt = 0) override;

protected:
    // slot functions
    virtual bool setSlotMaxRate(const basic::Angle* const newMR);
    virtual bool setSlotMaxRate(const basic::Number* const newMR);

private:
    // event function
    bool onUpdatePitchAdi(const basic::Number* const newP);
    bool onUpdateRollDegAdi(const basic::Number* const newR);
    bool onUpdateRollRadAdi(const basic::Number* const newR);
    bool onUpdateMaxRateAdi(const basic::Number* const newMR);

    LCreal pitch;       // actual pitch (degrees)
    LCreal scaledPitch; // our pitch value (inches)
    LCreal curTheta;    // our current pitch value (degrees)
    LCreal curPhi;      // our current roll value (radians)
    LCreal roll;        // our roll value (radians)
    LCreal maxRate;     // maximum mechanical rate at which the adi can move pitch or roll
};

}; // end instruments namespace
}; // end oe namespace

#endif
