//------------------------------------------------------------------------------
// Class: CompassRose
// Base Class: Basic::Object -> BasicGL::Graphic -> CompassRose
//
// Description: Generic compass used as a direction indicator.  It will 
// rotate about a heading either by someone telling it through a member function.
// Inputs:
//      UPDATE_VALUE  -> rotation angle (deg)
//      UPDATE_VALUE2 -> rotation angle (rad)
//      UPDATE_VALUE3 -> centered radius
//      UPDATE_VALUE4 -> decentered radius
//      UPDATE_VALUE5 -> displacement
//      UPDATE_VALUE6 -> centered or not
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_CompassRose_H__
#define __Eaagles_Instruments_CompassRose_H__

#include "openeaagles/basicGL/Graphic.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace Instruments {

class CompassRose : public BasicGL::Graphic
{
    DECLARE_SUBCLASS(CompassRose, BasicGL::Graphic)
        
public:
    CompassRose();
    
    // Set functions
    virtual bool setRotationDeg(const LCreal newR);
    virtual bool setRotationRad(const LCreal newR);
    virtual bool setCenteredRadius(const LCreal newR);
    virtual bool setDeCenteredRadius(const LCreal newDR);
    virtual bool setDisplacement(const LCreal newD);
    virtual bool setCentered(const bool newC);
    
    // Get function
    LCreal getRotationDeg() const      { return rot * (LCreal) Basic::Angle::R2DCC; }
    LCreal getRotationRad() const      { return rot; }
    LCreal getCenteredRadius() const   { return cenRadius; }
    LCreal getDeCenteredRadius() const { return decRadius; }
    bool isCentered() const            { return centered; }
    LCreal getDisplacement() const     { return displacement; }
    
    // BasicGL::Graphic interface
    virtual void draw();
        
    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0.000000);
    virtual bool event(const int event, Basic::Object* const obj = 0);
    
protected:
    // slot functions
    bool setSlotCenteredRadius(const Basic::Number* const newR);
    bool setSlotDeCenteredRadius(const Basic::Number* const newR);
    bool setSlotDisplacement(const Basic::Number* const newD);

    // event functions
    bool onUpdateRotDeg(const Basic::Number* const x);
    bool onUpdateRadius(const Basic::Number* const x);
    bool onUpdateCenRad(const Basic::Number* const x);
    bool onUpdateDecRadius(const Basic::Number* const x);
    bool onUpdateDisplacement(const Basic::Number* const x);
    bool onUpdateCentered(const Basic::Number* const x);
    
private:
    LCreal rot;         // rotation angle (rads)
    LCreal cenRadius;   // our centered radius (inches)
    LCreal decRadius;   // our decentered radius (inches)
    bool centered;      // centered flag
    LCreal displacement;// how far to translate when we go decentered (inches)
    
    SendData lTicksSD;
    SendData sTicksSD;
    SendData dialSD;
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
