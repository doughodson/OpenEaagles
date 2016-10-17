
#ifndef __oe_base_Transform_H__
#define __oe_base_Transform_H__

#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/osg/Matrix"

namespace oe {
namespace base {

class Angle;
class Number;

//------------------------------------------------------------------------------
// Class: Transform
// Base class:  Object -> Transform
// Factory name:Transform
// Slots:
//  x    <Number>    ! x transform (default: identity)
//  x    <Angle>    ! x transform (default: identity)
//  y    <Number>    ! y transform (default: identity)
//  y    <Angle>    ! y transform (default: identity)
//  z    <Number>    ! z transform (default: identity)
//  z    <Angle>    ! z transform (default: identity)
//  w    <Number>    ! w transform (default: identity)
//  w    <Angle>    ! w transform (default: identity)
//
// Description: Coordinate Transformations (e.g., translate, rotate, scale)
//
//
// Public methods: Base class public methods, plus ...
//
//      operator const osg::Matrix&()
//          Returns a copy of the transformation matrix
//
//      bool setComputeMatrix1(Angle* sc1obj)
//      bool setComputeMatrix1(Number* sc1obj)
//      ...
//      bool setComputeMatrix4(Number* sc4obj)
//          Set the n'th parameter to the transformation function
//          and create the transformation matrix.  Returns true if the matrix
//          is valid.  These functions must be called in order.  For example,
//          "x" must be set before "y".
//
//------------------------------------------------------------------------------
class Transform : public Object
{
    DECLARE_SUBCLASS(Transform, Object)

public:
    Transform();
    operator const osg::Matrix&()           { return m; }
    virtual bool setComputematrix1(const Angle* const sc1obj);
    virtual bool setComputematrix1(const Number* const sc1obj);
    virtual bool setComputematrix2(const Angle* const sc2obj);
    virtual bool setComputematrix2(const Number* const sc2obj);
    virtual bool setComputematrix3(const Angle* const sc3obj);
    virtual bool setComputematrix3(const Number* const sc3obj);
    virtual bool setComputematrix4(const Angle* const sc4obj);
    virtual bool setComputematrix4(const Number* const sc4obj);

protected:
    virtual void computeMatrix();
    osg::Matrix m;              // transformation matrix
    double      v[4];           // values
    int         nv;             // Number of values
};

//------------------------------------------------------------------------------
// Class: Translation
//
// Description: Coordinate Transformations (e.g., translate)
//
// Translation:
//    Two Parameters: Translate X by a factor of "x" and Y by a factor of "y".
//    Three Parameters: Translate X, Y and Z by factors of "x", "y" and "z" respectively.
//
// Factory names: Translation :
//    1. x  (Number)  x translation.
//    2. y  (Number)  y translation.
//    3. z  (Number)  z translation (optional).
//
//
//------------------------------------------------------------------------------
class Translation : public Transform
{
    DECLARE_SUBCLASS(Translation, Transform)
public:
    Translation();
protected:
   virtual void computeMatrix() override;
};

//------------------------------------------------------------------------------
// Class: Rotation
//
// Description: Coordinate Transformations (e.g., rotate)
//
// Rotation:
//    One parameter: Rotate "w" radians about the Z axis.
//    Four parameters: Rotate "w" radians about the vector (x,y,z).
// Slots:
//    1. x  (Number)  x component of vector (optional)
//    2. y  (Number)  y component of vector (optional)
//    3. z  (Number)  z component of vector (optional)
//    4. w  (Angle)   radians to rotate about vector
//
//
//------------------------------------------------------------------------------
class Rotation : public Transform
{
    DECLARE_SUBCLASS(Rotation, Transform)
public:
    Rotation();
protected:
   virtual void computeMatrix() override;
};

//------------------------------------------------------------------------------
// Class: Scale
//
// Description: Coordinate Transformations (e.g., scale)
//
// Scale:
//    One parameter: Scale both X and Y by a factor of "x".
//    Two parameters: Scale X by a factor of "x" and Y by a factor of "y".
//    Three Parameters: Scale X, Y and Z by factors of "x", "y" and "z" respectively.
// Slots:
//    1. x  (Number)  x scaling factor
//    2. y  (Number)  y scaling factor (optional)
//    3. z  (Number)  z scaling factor (optional)
//
//
//------------------------------------------------------------------------------
class Scale : public Transform
{
    DECLARE_SUBCLASS(Scale, Transform)
public:
    Scale();
protected:
   virtual void computeMatrix() override;
};

}
}

#endif
