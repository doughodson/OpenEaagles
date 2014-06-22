//------------------------------------------------------------------------------
// Class: BearingPointer
// Base Class: Basic::Object -> BasicGL::Graphic -> CompassRose -> BearingPointer
//
// Description: This is a bearing pointer, which works in conjunction with 
// a compass rose to determine bearing, however, it is independent of CompassRose,
// and can be used as a stand-alone item.  You can also add a head and tail
// graphic to the pointer, to customize it.  It defaults to a standard 0.5 inch
// wide bearing pointer with a line for a tail.  
//
// Inputs:
//      UPDATE_VALUE 1 - 6 (from Compass rose) -> handles centered, radius, and such
//      UPDATE_VALUE7 - sets bearing (radians or Basic::Angle)
//      UPDATE_VALUE8 - sets bearing (degrees)
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_BearingPointer_H__
#define __Eaagles_Instruments_BearingPointer_H__

#include "openeaagles/instruments/maps/CompassRose.h"

namespace Eaagles {
namespace Instruments {

class BearingPointer : public CompassRose
{
    DECLARE_SUBCLASS(BearingPointer, CompassRose)
        
public:
    BearingPointer();
    
    // get function    
    LCreal getBearingRad() const { return bearing; } // radians
    LCreal getBearingDeg() const { return bearing * static_cast<LCreal>(Basic::Angle::R2DCC); }    // degrees
    BasicGL::Graphic* getHeadGraphic() const { return head; }
    BasicGL::Graphic* getTailGraphic() const { return tail; }
    
    // Set functions
    bool setBearingRad(const LCreal newB);             // radians
    bool setBearingDeg(const LCreal newB);             // degrees
    
    // BasicGL::Graphic interface
    virtual void drawFunc();
    virtual void draw();
    
    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0);
    virtual bool event(const int key, Basic::Object* const obj = 0);

protected:
    // slot functions
    virtual bool setSlotHeadGraphic(const BasicGL::Graphic* const newH);
    virtual bool setSlotTailGraphic(const BasicGL::Graphic* const newT);

private:
    // event handlers
    bool onUpdateRadBearingPointer(const Basic::Angle* const msg);
    bool onUpdateRadBearingPointer(const Basic::Number* const msg);
    bool onUpdateDegBearingPointer(const Basic::Number* const msg);
    
    LCreal bearing;     // used for bearing pointer, or if anyone else needs it for calculations (radians)
    LCreal myRotation;  // how much we are going to rotate the compass
    LCreal myRadius;    // our radius (comes from dial radius)
    BasicGL::Graphic* head;    // our head graphic (if we have one)
    BasicGL::Graphic* tail;    // our tail graphic (if we have one)
};

}  // end of Instruments namespace
}  // end of Eaagles namespace

#endif
