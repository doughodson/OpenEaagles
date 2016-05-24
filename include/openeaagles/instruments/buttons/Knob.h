//------------------------------------------------------------------------------
// Class:       Knob
// Base class:  base::Object -> base::Component -> graphics::Graphic -> Rotators -> Knob
// Description: Basic knob that will rotate from a starting fixed angle to a
// given finish angle, and you can specify the values returned from those angles based on
// start and finish value (Table1)
//
// knobTable:
//  ( Table1
//      x: [ 0 360 ]    // degrees of rotation
//   data: [ 1 1000 ]   // values to return at that specific rotation
//  )
//  startAngle: 20      // angle from which to start rotation
//------------------------------------------------------------------------------
#ifndef __oe_instruments_Knob_H__
#define __oe_instruments_Knob_H__

#include "openeaagles/instruments/buttons/Button.h"

namespace oe {
namespace base { class Table1; }
namespace instruments {

class Knob : public Button{
    DECLARE_SUBCLASS(Knob,Button)

public:
    Knob();

    double const getValue() { return value; }

    virtual bool onSingleClick() override;

    virtual bool setEndless(const bool x)           { endless = x; return true; }
    virtual bool setEndlessStart(const double x)    { endlessStart = x; return true; }
    virtual bool setEndlessLimit(const double x)    { endlessLimit = x; return true; }

    virtual void drawFunc() override;
    virtual void draw() override;

    virtual void updateData(const double dt = 0.0) override;
    virtual bool event(const int event, base::Object* const obj = nullptr) override;

protected:
    // Sets the Event ID to newEvent
    bool setSlotValueTable(base::Table1* const x);
    bool setSlotEndless(const base::Number* const x);
    bool setSlotEndlessStart(const base::Number* const x);
    bool setSlotEndlessLimit(const base::Number* const x);

    // event functions
    bool onMotion();

private:
    void computeRotation();     // translate our x,y to rotation from 0 values
    base::Table1* table;     // holds our values for rotation and value
    double value;               // value we currently have
    int startX;                 // starting mouse x position (pixels) of the center of our graphic
    int startY;                 // starting mouse y position (pixels) of the center of our graphic
    double angle;               // current angle
    double lastAngle;           // our last angle
    int degsRotation;           // how much to rotate
    double findStartTimer;
    bool start;
    double running;
    double startAngle;
    bool endless;               // does our knob keep spinning?
    double endlessStart;        // start value of the endless knob (default to 0)
    double endlessLimit;        // end value of the endless knob (default to 360)
};

}
}

#endif
