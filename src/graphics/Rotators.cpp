#include "openeaagles/graphics/Rotators.h"
#include "openeaagles/basic/Number.h"

namespace oe {
namespace BasicGL {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Rotators, "Rotators")
EMPTY_SERIALIZER(Rotators)
EMPTY_DELETEDATA(Rotators)

// -----------------------------------------------------------------------------
// Event Handler (s)
// -----------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Rotators)
    ON_EVENT_OBJ(UPDATE_VALUE,  onXRotate,    basic::Angle)
    ON_EVENT_OBJ(UPDATE_VALUE,  onXRotate,    basic::Number)
    ON_EVENT_OBJ(UPDATE_VALUE2, onXRotateDeg, basic::Number)
    ON_EVENT_OBJ(UPDATE_VALUE3, onYRotate,    basic::Angle)
    ON_EVENT_OBJ(UPDATE_VALUE3, onYRotate,    basic::Number)
    ON_EVENT_OBJ(UPDATE_VALUE4, onYRotateDeg, basic::Number)
    ON_EVENT_OBJ(UPDATE_VALUE5, onZRotate,    basic::Angle)
    ON_EVENT_OBJ(UPDATE_VALUE5, onZRotate,    basic::Number)
    ON_EVENT_OBJ(UPDATE_VALUE6, onZRotateDeg, basic::Number)
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
bool Rotators::setXRotation(const LCreal angle)
{
    xRot = angle;
    return true;
}

//------------------------------------------------------------------------------
// setXRotationDeg() -- Sets the rotation angle (deg)
//------------------------------------------------------------------------------
bool Rotators::setXRotationDeg(const LCreal angle)
{
    xRot = angle * static_cast<LCreal>(basic::Angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setYRotation() -- Sets the Y rotation angle (rad)
//------------------------------------------------------------------------------
bool Rotators::setYRotation(const LCreal angle)
{
    yRot = angle;
    return true;
}

//------------------------------------------------------------------------------
// setYRotationDeg() -- Sets the Y rotation angle (deg)
//------------------------------------------------------------------------------
bool Rotators::setYRotationDeg(const LCreal angle)
{
    yRot = angle * static_cast<LCreal>(basic::Angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setZRotationDeg() -- Sets the Z rotation angle (deg)
//------------------------------------------------------------------------------
bool Rotators::setZRotationDeg(const LCreal angle)
{
    zRot = angle * static_cast<LCreal>(basic::Angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setZRotation() -- Sets the Z rotation angle (rad)
//------------------------------------------------------------------------------
bool Rotators::setZRotation(const LCreal angle)
{
    zRot = angle;
    return true;
}

//------------------------------------------------------------------------------
// setRotations() -- set all of our rotations
//------------------------------------------------------------------------------
bool Rotators::setRotations(const LCreal x, const LCreal y, const LCreal z)
{
    xRot = x;
    yRot = y;
    zRot = z;
    return true;
}

//------------------------------------------------------------------------------
// setRotationsDeg() -- set all of our rotations (deg)
//------------------------------------------------------------------------------
bool Rotators::setRotationsDeg(const LCreal x, const LCreal y, const LCreal z)
{
    xRot = x * static_cast<LCreal>(basic::Angle::D2RCC);
    yRot = y * static_cast<LCreal>(basic::Angle::D2RCC);
    zRot = z * static_cast<LCreal>(basic::Angle::D2RCC);
    return true;
}


//------------------------------------------------------------------------------
// onXRotate() -- updates the X rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onXRotate(const basic::Number* const rotation)
{
    if (rotation != nullptr) return setXRotation( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onXRotate() -- updates the X rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onXRotate(const basic::Angle* const rotation)
{
    if (rotation != nullptr)
        return setXRotation(static_cast<LCreal>(basic::Radians::convertStatic(*rotation)));
    return false;
}

//------------------------------------------------------------------------------
// onXRotateDeg() -- updates the X rotation value (deg)
//------------------------------------------------------------------------------
bool Rotators::onXRotateDeg(const basic::Number* const rotation)
{
    if (rotation != nullptr) return setXRotationDeg( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onYRotate() -- updates the Y rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onYRotate(const basic::Number* const rotation)
{
    if (rotation != nullptr) return setYRotation( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onYRotate() -- updates the Y rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onYRotate(const basic::Angle* const rotation)
{
    if (rotation != nullptr)
        return setYRotation(static_cast<LCreal>(basic::Radians::convertStatic(*rotation)));
    return false;
}

//------------------------------------------------------------------------------
// onYRotateDeg() -- updates the Y rotation value (deg)
//------------------------------------------------------------------------------
bool Rotators::onYRotateDeg(const basic::Number* const rotation)
{
    if (rotation != nullptr) return setYRotationDeg( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onZRotate() -- updates the Z rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onZRotate(const basic::Number* const rotation)
{
    if (rotation != nullptr) return setZRotation( rotation->getReal() );
    return false;
}

//------------------------------------------------------------------------------
// onZRotate() -- updates the Z rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onZRotate(const basic::Angle* const rotation)
{
    if (rotation != nullptr)
        return setZRotation(static_cast<LCreal>(basic::Radians::convertStatic(*rotation)));
    return false;
}

//------------------------------------------------------------------------------
// onZRotateDeg() -- updates the Z rotation value (deg)
//------------------------------------------------------------------------------
bool Rotators::onZRotateDeg(const basic::Number* const rotation)
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

} // End BasicGL namespace
} // End oe namespace


