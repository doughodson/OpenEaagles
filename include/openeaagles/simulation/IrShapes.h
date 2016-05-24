//------------------------------------------------------------------------------
// Classes: IrShape, IrSphere, IrBox
//------------------------------------------------------------------------------
#ifndef __oe_simulation_IrShape_H__
#define __oe_simulation_IrShape_H__

#include "openeaagles/base/Object.h"

namespace oe {

namespace base { class Number; }

namespace simulation {

class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrShape
// Description: Base class for infrared shape
// Factory name: IrShape
//------------------------------------------------------------------------------
class IrShape : public base::Object
{
   DECLARE_SUBCLASS(IrShape,base::Object)
public:
   IrShape();

   virtual double getArea();            // returns the effective area as a number in square meters
   virtual double getReflectorAreaInFieldOfView(const IrQueryMsg* const msg);
   virtual bool setSlotIrShapeArea(const base::Number* const s);
private:
   double area;
};

//------------------------------------------------------------------------------
// Class: IrSphere
//
// Factory name: IrSphere
// Slots:
//   radius    <Number>   ! Radius of sphere (default is 10)
//------------------------------------------------------------------------------
class IrSphere : public IrShape
{
   DECLARE_SUBCLASS(IrSphere,IrShape)

public:

   IrSphere();

   double getArea() override;

   double getRadius()              { return radius; }

   virtual bool setSlotIrSphereRadius(const base::Number* const s);

private:

   double radius;
};

//------------------------------------------------------------------------------
// Class: IrBox
//
// Factory name: IrBox
// Slots:
//   x        < Number>         ! x dimension  (default is 10)
//   y        < Number>         ! y dimension
//   z        < Number>         ! z dimension
//------------------------------------------------------------------------------
class IrBox : public IrShape
{
   DECLARE_SUBCLASS(IrBox,IrShape)

public:

   IrBox();

   double getArea() override;
   double getReflectorAreaInFieldOfView(const IrQueryMsg* const msg) override;

   virtual bool setSlotIrBoxX(const base::Number* const s);
   virtual bool setSlotIrBoxY(const base::Number* const s);
   virtual bool setSlotIrBoxZ(const base::Number* const s);

private:

   double x,y,z;
};

}
}

#endif
