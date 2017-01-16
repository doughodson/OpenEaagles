
#ifndef __oe_graphics_Rotators_H__
#define __oe_graphics_Rotators_H__

#include "Graphic.hpp"

#include "openeaagles/base/units/unit_utils.hpp"

namespace oe {
namespace base { class Angle; }
namespace graphics {

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
class Rotators : public Graphic
{
    DECLARE_SUBCLASS(Rotators, Graphic)

public: 
    Rotators();

    // Get functions
    double getXRotation()    const  { return xRot; }
    double getXRotationDeg() const  { return xRot * static_cast<double>(base::angle::R2DCC); }
    double getYRotation()    const  { return yRot; }
    double getYRotationDeg() const  { return yRot * static_cast<double>(base::angle::R2DCC); }
    double getZRotation()    const  { return zRot; }
    double getZRotationDeg() const  { return zRot * static_cast<double>(base::angle::R2DCC); }
    // get all angles
    void getRotationAngles(double& x, double& y, double& z);

    // Set functions
    bool setXRotation(const double angle);
    bool setXRotationDeg(const double angle);
    bool setYRotation(const double angle);
    bool setYRotationDeg(const double angle);
    bool setZRotation(const double angle);
    bool setZRotationDeg(const double angle);
    bool setRotations(const double x, const double y = 0, const double z = 0);
    bool setRotationsDeg(const double x, const double y = 0, const double z = 0);

    virtual void draw() override;
    virtual bool event(const int event, base::Object* const obj = nullptr) override;

private:
    // Event callbacks
    bool onXRotate(const base::Angle* const rotation);
    bool onXRotate(const base::Number* const rotation);
    bool onXRotateDeg(const base::Number* const rotation);
    bool onYRotate(const base::Angle* const rotation);
    bool onYRotate(const base::Number* const rotation);
    bool onYRotateDeg(const base::Number* const rotation);
    bool onZRotate(const base::Angle* const rotation);
    bool onZRotate(const base::Number* const rotation);
    bool onZRotateDeg(const base::Number* const rotation);

    double xRot;  // x axis rotation
    double yRot;  // y axis rotation
    double zRot;  // z axis rotation
};

}
}

#endif
