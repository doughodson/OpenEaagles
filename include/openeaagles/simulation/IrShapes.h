//------------------------------------------------------------------------------
// Classes: IrShape, IrSphere, IrBox
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_IrShape_H__
#define __oe_Simulation_IrShape_H__

#include "openeaagles/basic/Object.h"

namespace oe {

namespace basic { class Number; }

namespace Simulation {

class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrShape
// Description: Base class for infrared shape
// Factory name: IrShape
//------------------------------------------------------------------------------
class IrShape : public basic::Object
{
   DECLARE_SUBCLASS(IrShape,basic::Object)
public:
   IrShape();

   virtual LCreal getArea();            // returns the effective area as a number in square meters
   virtual LCreal getReflectorAreaInFieldOfView(const IrQueryMsg* const msg);
   virtual bool setSlotIrShapeArea(const basic::Number* const s);
private:
   LCreal area;
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

   LCreal getArea() override;

   LCreal getRadius()              { return radius; }

   virtual bool setSlotIrSphereRadius(const basic::Number* const s);

private:

   LCreal radius;
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

   LCreal getArea() override;
   LCreal getReflectorAreaInFieldOfView(const IrQueryMsg* const msg) override;

   virtual bool setSlotIrBoxX(const basic::Number* const s);
   virtual bool setSlotIrBoxY(const basic::Number* const s);
   virtual bool setSlotIrBoxZ(const basic::Number* const s);

private:

   LCreal x,y,z;
};

} // End Simulation namespace
} // End oe namespace

#endif
