//------------------------------------------------------------------------------
// Class: AnalogGauge
//------------------------------------------------------------------------------
#ifndef	__Eaagles_Instruments_AnalogGuage_H__
#define __Eaagles_Instruments_AnalogGuage_H__

#include "openeaagles/instruments/Instrument.h"

namespace Eaagles {
namespace Instruments {

//------------------------------------------------------------------------------
// Class:	AnalogGauge
// Base class: BasicGL::Graphic -> Instrument -> AnalogGauge
// 
// Description: This will create a gauge that will
// draw vertically or horizontally depending on a flag.
// 
// Inputs:
//      UPDATE_INSTRUMENTS (from instrument)
//
// Public member functions:
//      setLeftBoundary() - where to draw the left side of the gauge
//      setRightBoundary() - where to draw the right side of the gauge
//      setIsOutlined() - if true, the guage isn't filled
//      setIsVertical() - if true, the gauge slides up and down
//------------------------------------------------------------------------------
class AnalogGauge : public Instrument {
    DECLARE_SUBCLASS(AnalogGauge,Instrument)
    
public:
    AnalogGauge();

    // set functions
    virtual bool setLeftBoundary(const LCreal newLB);
    virtual bool setRightBoundary(const LCreal newRB);
    virtual bool setIsOutlined(const bool newO);
    virtual bool setIsVertical(const bool newV);
    virtual bool setDrawMe(const bool x);
    
    // get functions
    LCreal getLeftBoundary() const  { return leftBoundary; }
    LCreal getRightBoundary() const { return rightBoundary; }
    bool isOutlined() const         { return outline; }
    bool isVertical() const         { return vertical; }
    bool isDrawing() const          { return drawMe; }
    
    // BasicGL::Graphic interface
    virtual void drawFunc();
    
protected: 
    // slot functions
    virtual bool setSlotLeftBoundary(const Basic::Number* const newLB);
    virtual bool setSlotRightBoundary(const Basic::Number* const newRB);
    virtual bool setSlotIsOutlined(const Basic::Number* const newO);
    virtual bool setSlotIsVertical(const Basic::Number* const newV);
       
private:
    LCreal gaugePos;                // direction, in inches, that we want to scale our gauge
    LCreal leftBoundary;            // our left boundary (in inches)
    LCreal rightBoundary;           // our right boundary (in inches)
    bool outline;                   // are we an outline or filled?
    bool vertical;                  // are we sliding vertically or horizontally? (True = vertical)    
    bool drawMe;                    // this is for derived classes that don't want us to draw
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
