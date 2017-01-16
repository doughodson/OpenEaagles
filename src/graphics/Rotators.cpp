
#include "openeaagles/graphics/Rotators.hpp"

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/units/Angles.hpp"

namespace oe {
namespace graphics {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Rotators, "Rotators")
EMPTY_SERIALIZER(Rotators)
EMPTY_DELETEDATA(Rotators)

// -----------------------------------------------------------------------------
// Event Handler (s)
// -----------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Rotators)
    ON_EVENT_OBJ(UPDATE_VALUE,  onXRotate,    base::Angle)
    ON_EVENT_OBJ(UPDATE_VALUE,  onXRotate,    base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE2, onXRotateDeg, base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE3, onYRotate,    base::Angle)
    ON_EVENT_OBJ(UPDATE_VALUE3, onYRotate,    base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE4, onYRotateDeg, base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE5, onZRotate,    base::Angle)
    ON_EVENT_OBJ(UPDATE_VALUE5, onZRotate,    base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE6, onZRotateDeg, base::Number)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Rotators::Rotators()
{
    STANDARD_CONSTRUCTOR()

    xRot = -1;
    yRot = -1;
    zRot = -1;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Rotators::copyData(const Rotators& org, const bool)
{
    BaseClass::copyData(org);

    xRot = org.xRot;
    yRot = org.yRot;
    zRot = org.zRot;
}

//------------------------------------------------------------------------------
// setXRotation() -- Sets the X rotation angle (rad)
//------------------------------------------------------------------------------
bool Rotators::setXRotation(const double angle)
{
    xRot = angle;
    return true;
}

//------------------------------------------------------------------------------
// setXRotationDeg() -- Sets the rotation angle (deg)
//------------------------------------------------------------------------------
bool Rotators::setXRotationDeg(const double angle)
{
    xRot = angle * static_cast<double>(base::angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setYRotation() -- Sets the Y rotation angle (rad)
//------------------------------------------------------------------------------
bool Rotators::setYRotation(const double angle)
{
    yRot = angle;
    return true;
}

//------------------------------------------------------------------------------
// setYRotationDeg() -- Sets the Y rotation angle (deg)
//------------------------------------------------------------------------------
bool Rotators::setYRotationDeg(const double angle)
{
    yRot = angle * static_cast<double>(base::angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setZRotationDeg() -- Sets the Z rotation angle (deg)
//------------------------------------------------------------------------------
bool Rotators::setZRotationDeg(const double angle)
{
    zRot = angle * static_cast<double>(base::angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setZRotation() -- Sets the Z rotation angle (rad)
//------------------------------------------------------------------------------
bool Rotators::setZRotation(const double angle)
{
    zRot = angle;
    return true;
}

//------------------------------------------------------------------------------
// setRotations() -- set all of our rotations
//------------------------------------------------------------------------------
bool Rotators::setRotations(const double x, const double y, const double z)
{
    xRot = x;
    yRot = y;
    zRot = z;
    return true;
}

//------------------------------------------------------------------------------
// setRotationsDeg() -- set all of our rotations (deg)
//------------------------------------------------------------------------------
bool Rotators::setRotationsDeg(const double x, const double y, const double z)
{
    xRot = x * static_cast<double>(base::angle::D2RCC);
    yRot = y * static_cast<double>(base::angle::D2RCC);
    zRot = z * static_cast<double>(base::angle::D2RCC);
    return true;
}


//------------------------------------------------------------------------------
// onXRotate() -- updates the X rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onXRotate(const base::Number* const rotation)
{
    if (rotation != nullptr) return setXRotation( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onXRotate() -- updates the X rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onXRotate(const base::Angle* const rotation)
{
    if (rotation != nullptr)
        return setXRotation(static_cast<double>(base::Radians::convertStatic(*rotation)));
    return false;
}

//------------------------------------------------------------------------------
// onXRotateDeg() -- updates the X rotation value (deg)
//------------------------------------------------------------------------------
bool Rotators::onXRotateDeg(const base::Number* const rotation)
{
    if (rotation != nullptr) return setXRotationDeg( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onYRotate() -- updates the Y rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onYRotate(const base::Number* const rotation)
{
    if (rotation != nullptr) return setYRotation( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onYRotate() -- updates the Y rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onYRotate(const base::Angle* const rotation)
{
    if (rotation != nullptr)
        return setYRotation(static_cast<double>(base::Radians::convertStatic(*rotation)));
    return false;
}

//------------------------------------------------------------------------------
// onYRotateDeg() -- updates the Y rotation value (deg)
//------------------------------------------------------------------------------
bool Rotators::onYRotateDeg(const base::Number* const rotation)
{
    if (rotation != nullptr) return setYRotationDeg( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onZRotate() -- updates the Z rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onZRotate(const base::Number* const rotation)
{
    if (rotation != nullptr) return setZRotation( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onZRotate() -- updates the Z rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onZRotate(const base::Angle* const rotation)
{
    if (rotation != nullptr)
        return setZRotation(static_cast<double>(base::Radians::convertStatic(*rotation)));
    return false;
}

//------------------------------------------------------------------------------
// onZRotateDeg() -- updates the Z rotation value (deg)
//------------------------------------------------------------------------------
bool Rotators::onZRotateDeg(const base::Number* const rotation)
{
    if (rotation != nullptr) return setZRotationDeg( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void Rotators::draw()
{
    lcSaveMatrix();
    if (xRot != -1) lcRotate(1, 0, 0, xRot);
    if (yRot != -1) lcRotate(0, 1, 0, yRot);
    if (zRot != -1) lcRotate(0, 0, 1, zRot);
    Graphic::draw();
    lcRestoreMatrix();
}

}
}


