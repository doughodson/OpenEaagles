//------------------------------------------------------------------------------
// Class:       Knob
// Base class:  basic::Object -> basic::Component -> graphics::Graphic -> Rotators -> Knob
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
namespace basic { class Table1; }
namespace instruments {

class Knob : public Button{
    DECLARE_SUBCLASS(Knob,Button)

public:
    Knob();

    LCreal const getValue() { return value; }

    bool onSingleClick() override;

    virtual bool setEndless(const bool x)           { endless = x; return true; }
    virtual bool setEndlessStart(const LCreal x)    { endlessStart = x; return true; }
    virtual bool setEndlessLimit(const LCreal x)    { endlessLimit = x; return true; }

    void drawFunc() override;
    void draw() override;

    void updateData(const LCreal dt = 0.0) override;
    bool event(const int event, basic::Object* const obj = nullptr) override;

protected:
    // Sets the Event ID to newEvent
    bool setSlotValueTable(basic::Table1* const x);
    bool setSlotEndless(const basic::Number* const x);
    bool setSlotEndlessStart(const basic::Number* const x);
    bool setSlotEndlessLimit(const basic::Number* const x);

    // event functions
    bool onMotion();

private:
    void computeRotation();     // translate our x,y to rotation from 0 values
    basic::Table1* table;     // holds our values for rotation and value
    LCreal value;               // value we currently have
    int startX;                 // starting mouse x position (pixels) of the center of our graphic
    int startY;                 // starting mouse y position (pixels) of the center of our graphic
    LCreal angle;               // current angle
    LCreal lastAngle;           // our last angle
    int degsRotation;           // how much to rotate
    LCreal findStartTimer;
    bool start;
    LCreal running;
    LCreal startAngle;
    bool endless;               // does our knob keep spinning?
    LCreal endlessStart;        // start value of the endless knob (default to 0)
    LCreal endlessLimit;        // end value of the endless knob (default to 360)
};

}  // end instruments namespace
}  // end oe namespace

#endif
