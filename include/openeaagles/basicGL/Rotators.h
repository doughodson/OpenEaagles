//------------------------------------------------------------------------------
// Class: Rotators
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_Rotators_H__
#define __Eaagles_BasicGL_Rotators_H__

#include "Graphic.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace BasicGL {

//------------------------------------------------------------------------------
// Class: Rotators
//
// Description: A simple graphic that rotates, and can accept either degrees
// or radians, about 3 axes.
//
// Factory name: Rotators
//  Events messages:
//      UPDATE_VALUE - rotates around the X axis - radians (Angle or Number)     
//      UPDATE_VALUE2 - rotates around the X axis - degrees (Number only)
//      UPDATE_VALUE3 - rotates around the Y axis - radians (Angle or Number)
//      UPDATE_VALUE4 - rotates around the Y axis - degrees (Number only)
//      UPDATE_VALUE5 - rotates around the Z axis - radians (Angle or Number)
//      UPDATE_VALUE6 - rotates around the Z axis - degrees (Number only)
//
// Note: Since we convert statically to radians any time we pass an Angle as 
// an event argument, there is no need to have an event function that converts
// an Angle to degrees.  You can pass a 'Degrees' object as an event argument using
// UPDATE_VALUE, and it will automatically convert to radians.  That is why there
// is no onXRotateDeg with an Angle argument.
//------------------------------------------------------------------------------
class Rotators : public Graphic {
    DECLARE_SUBCLASS(Rotators,Graphic)
public:
    Rotators();

    // Get functions
    LCreal getXRotation()    const  { return xRot; }
    LCreal getXRotationDeg() const  { return xRot * static_cast<LCreal>(Basic::Angle::R2DCC); }  
    LCreal getYRotation()    const  { return yRot; }
    LCreal getYRotationDeg() const  { return yRot * static_cast<LCreal>(Basic::Angle::R2DCC); }  
    LCreal getZRotation()    const  { return zRot; }
    LCreal getZRotationDeg() const  { return zRot * static_cast<LCreal>(Basic::Angle::R2DCC); }  
    // get all angles
    void getRotationAngles(LCreal& x, LCreal& y, LCreal& z);

    // Set functions
    bool setXRotation(const LCreal angle);                  
    bool setXRotationDeg(const LCreal angle);  
    bool setYRotation(const LCreal angle);                  
    bool setYRotationDeg(const LCreal angle);  
    bool setZRotation(const LCreal angle);                  
    bool setZRotationDeg(const LCreal angle);  
    bool setRotations(const LCreal x, const LCreal y = 0, const LCreal z = 0);
    bool setRotationsDeg(const LCreal x, const LCreal y = 0, const LCreal z = 0);

    // Graphic interface
    void draw() override;

    // Component interface
    bool event(const int event, Basic::Object* const obj = 0) override;  

private:
    // Event callbacks
    bool onXRotate(const Basic::Angle* const rotation);
    bool onXRotate(const Basic::Number* const rotation);
    bool onXRotateDeg(const Basic::Number* const rotation);
    bool onYRotate(const Basic::Angle* const rotation);
    bool onYRotate(const Basic::Number* const rotation);
    bool onYRotateDeg(const Basic::Number* const rotation);
    bool onZRotate(const Basic::Angle* const rotation);
    bool onZRotate(const Basic::Number* const rotation);
    bool onZRotateDeg(const Basic::Number* const rotation);

    LCreal xRot;  // x axis rotation
    LCreal yRot;  // y axis rotation
    LCreal zRot;  // z axis rotation 
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif
