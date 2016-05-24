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
#include "openeaagles/base/units/Angles.h"

namespace oe {
namespace instruments {

class Adi : public Instrument {
    DECLARE_SUBCLASS(Adi,Instrument)

public:
    Adi();

    double getPitchInches() const       { return scaledPitch; }
    double getPitch() const             { return getPreScaleInstValue(); }
    double getRollDeg() const           { return curPhi * static_cast<double>(base::Angle::R2DCC); }
    double getRollRad() const           { return curPhi; }
    double getMaxRate() const           { return maxRate; }

    virtual bool setRollRad(const double newR);
    virtual bool setRollDeg(const double newR);
    virtual bool setPitch(const double newP);
    virtual bool setMaxRate(const double newMR);

    virtual void draw() override;

    virtual bool event(const int event, base::Object* const obj = nullptr) override;
    virtual void updateData(const double dt = 0) override;

protected:
    // slot functions
    virtual bool setSlotMaxRate(const base::Angle* const newMR);
    virtual bool setSlotMaxRate(const base::Number* const newMR);

private:
    // event function
    bool onUpdatePitchAdi(const base::Number* const newP);
    bool onUpdateRollDegAdi(const base::Number* const newR);
    bool onUpdateRollRadAdi(const base::Number* const newR);
    bool onUpdateMaxRateAdi(const base::Number* const newMR);

    double pitch;       // actual pitch (degrees)
    double scaledPitch; // our pitch value (inches)
    double curTheta;    // our current pitch value (degrees)
    double curPhi;      // our current roll value (radians)
    double roll;        // our roll value (radians)
    double maxRate;     // maximum mechanical rate at which the adi can move pitch or roll
};

}
}

#endif
