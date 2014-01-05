//------------------------------------------------------------------------------
// Classes: IrShape, IrSphere, IrBox
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_IrShape_H__
#define __Eaagles_Simulation_IrShape_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {

namespace Basic { class Number; }

namespace Simulation {

class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrShape
// Description: Base class for infrared shape
// Factory name: IrShape
//------------------------------------------------------------------------------
class IrShape : public Basic::Object
{
   DECLARE_SUBCLASS(IrShape,Basic::Object)
public:
   IrShape();

   virtual LCreal getArea();            // returns the effective area as a number in square meters
   virtual LCreal getReflectorAreaInFieldOfView(const IrQueryMsg* const msg);
   virtual bool setSlotIrShapeArea(const Basic::Number* const s);
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

   virtual LCreal getArea();
   LCreal getRadius()          { return radius; }

   virtual bool setSlotIrSphereRadius(const Basic::Number* const s);

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

   virtual LCreal getArea();
   virtual LCreal getReflectorAreaInFieldOfView(const IrQueryMsg* const msg);

   virtual bool setSlotIrBoxX(const Basic::Number* const s);
   virtual bool setSlotIrBoxY(const Basic::Number* const s);
   virtual bool setSlotIrBoxZ(const Basic::Number* const s);

private:

   LCreal x,y,z;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
