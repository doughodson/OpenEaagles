//------------------------------------------------------------------------------
// Classes: IrShape, IrSphere, IrBox
//------------------------------------------------------------------------------
#include "openeaagles/simulation/IrShapes.h"
#include "openeaagles/simulation/IrSignature.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/IrSensor.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/units/Areas.h"
#include "openeaagles/basic/units/Distances.h"

#include <cmath>

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: IrShape (Abstract)
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(IrShape,"IrShape")

BEGIN_SLOTTABLE(IrShape)
"area",
END_SLOTTABLE(IrShape)

BEGIN_SLOT_MAP(IrShape)
ON_SLOT(1,setSlotIrShapeArea,Basic::Number)
END_SLOT_MAP()

EMPTY_DELETEDATA(IrShape)
EMPTY_SERIALIZER(IrShape)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IrShape::IrShape()
{
   STANDARD_CONSTRUCTOR()
   area = 0.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IrShape::copyData(const IrShape& org, const bool)
{
   BaseClass::copyData(org);
   area = org.area;
}

Basic::Object* IrShape::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

bool IrShape::setSlotIrShapeArea(const Eaagles::Basic::Number* const num)
{
   LCreal value = 0.0;
   const Basic::Area* a = dynamic_cast<const Basic::Area*>(num);
   if (a != nullptr) {
      Basic::SquareMeters sm;
      value = static_cast<LCreal>(sm.convert(*a));
   }
   else if (num != nullptr) {
      value = num->getReal();
   }
   area = value;
   return true;
}

LCreal IrShape::getArea()
{
   return area;
}

LCreal IrShape::getReflectorAreaInFieldOfView(const IrQueryMsg* const msg)
{
   LCreal angleOffBoresight = msg->getAngleOffBoresight();

   LCreal maxAngle = msg->getSendingSensor()->getIFOVTheta();

   if (angleOffBoresight > maxAngle) return 0;

   return getArea();
}


//==============================================================================
// Class: IrSphere
//==============================================================================
IMPLEMENT_SUBCLASS(IrSphere,"IrSphere")
EMPTY_SERIALIZER(IrSphere)

BEGIN_SLOTTABLE(IrSphere)
"radius",
END_SLOTTABLE(IrSphere)

BEGIN_SLOT_MAP(IrSphere)
ON_SLOT(1,setSlotIrSphereRadius,Basic::Number)
END_SLOT_MAP()
//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IrSphere::IrSphere()
{
   STANDARD_CONSTRUCTOR()
   radius = 10.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IrSphere::copyData(const IrSphere& org, const bool)
{
   BaseClass::copyData(org);
   radius = org.radius;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void IrSphere::deleteData()
{
}

Basic::Object* IrSphere::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

bool IrSphere::setSlotIrSphereRadius(const Eaagles::Basic::Number* const s)
{
   LCreal value = 0.0;

   const Basic::Distance* d = dynamic_cast<const Basic::Distance*>(s);
   if (d != nullptr) {
      Basic::Meters m;
      value = static_cast<LCreal>(m.convert(*d));
   }
   else if (s != nullptr) {
      value = s->getReal();
   }
   radius = value;
   return true;
}

LCreal IrSphere::getArea()
{
   //return (LCreal) (radius * radius * 4.0 * PI);
   //Calculates the projected area of a sphere (projected area is a flat circle)
   return static_cast<LCreal>(radius * radius * PI);
}


//==============================================================================
// Class: IrBox
//==============================================================================

IMPLEMENT_SUBCLASS(IrBox,"IrBox")
EMPTY_SERIALIZER(IrBox)

BEGIN_SLOTTABLE(IrBox)
"x",
"y",
"z",
END_SLOTTABLE(IrBox)

BEGIN_SLOT_MAP(IrBox)
ON_SLOT(1,setSlotIrBoxX,Basic::Number)
ON_SLOT(2,setSlotIrBoxY,Basic::Number)
ON_SLOT(3,setSlotIrBoxZ,Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IrBox::IrBox()
{
   STANDARD_CONSTRUCTOR()
   x = 10.0;
   y = 10.0;
   z = 10.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IrBox::copyData(const IrBox& org, const bool)
{
   BaseClass::copyData(org);
   x = org.x;
   y = org.y;
   z = org.z;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void IrBox::deleteData()
{
}

Basic::Object* IrBox::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

bool IrBox::setSlotIrBoxX(const Eaagles::Basic::Number* const s)
{
   LCreal value = 0.0;

   const Basic::Distance* d = dynamic_cast<const Basic::Distance*>(s);
   if (d != nullptr) {
      Basic::Meters m;
      value = static_cast<LCreal>(m.convert(*d));
   }
   else if (s != nullptr) {
      value = s->getReal();
   }
   x = value;
   return true;
}

bool IrBox::setSlotIrBoxY(const Eaagles::Basic::Number* const s)
{
   LCreal value = 0.0;

   const Basic::Distance* d = dynamic_cast<const Basic::Distance*>(s);
   if (d != nullptr) {
      Basic::Meters m;
      value = static_cast<LCreal>(m.convert(*d));
   }
   else if (s != nullptr) {
       value = s->getReal();
   }
   y = value;
   return true;
}

bool IrBox::setSlotIrBoxZ(const Eaagles::Basic::Number* const s)
{
   LCreal value = 0.0;

   const Basic::Distance* d = dynamic_cast<const Basic::Distance*>(s);
   if (d != nullptr) {
      Basic::Meters m;
      value = static_cast<LCreal>(m.convert(*d));
   }
   else if (s != nullptr) {
       value = s->getReal();
   }
   z = value;
   return true;
}

LCreal IrBox::getArea()
{
   return static_cast<LCreal>((x*y * 2) + (2*x*z) + (2*y*z));
}

LCreal IrBox::getReflectorAreaInFieldOfView(const IrQueryMsg* const msg)
{
   LCreal area = 0.0;
#if 1
   //retrieve the angle off of the gimbal boresight
   const LCreal angleOffBoresight = msg->getAngleOffBoresight();

   //retrieve the planar angle of the IFOV of the IR sensor
   const LCreal maxAngle = msg->getSendingSensor()->getIFOVTheta();

   //If the angle to the target is outside the IFOV, then return an area of 0.
   //This will cause the target to be discarded from further IR signature processing.
   if (angleOffBoresight > maxAngle){
      //std::cout << angleOffBoresight * Basic::Angle::R2DCC << " " << msg->getGimbalAzimuth() * Basic::Angle::R2DCC << " " << msg->getGimbalElevation() * Basic::Angle::R2DCC << " DETECTED" << std::endl;
      return area;
   }

   //std::cout << angleOffBoresight * Basic::Angle::R2DCC << " " << msg->getGimbalAzimuth() * Basic::Angle::R2DCC << " " << msg->getGimbalElevation() * Basic::Angle::R2DCC << " DETECTED" << std::endl;

   //Calculate the projected area of the box.
   const osg::Vec3d targetAoiVector = msg->getAoiVector();

   // front/back of the box to the sensor
   // x, z are members of this object corresponding to width and height.
   area += std::fabs(targetAoiVector[0]) * x * z;

   // side/side of the box to the sensor
   // y,z are members of this object corresponding to length and height.
   area += std::fabs(targetAoiVector[1]) * y * z;

   // top/bottom of the box to the sensor.
   // x,y is a member of this object corresponding to width and length.
   area += std::fabs(targetAoiVector[2]) * x * y;

   return area;

#else
   LCreal angleOffBoresight = msg->getAngleOffBoresight();

   LCreal maxAngle = msg->getSendingSensor()->getIFOVTheta();

   if (angleOffBoresight > maxAngle)
      return area;
   else {

   const Player* sensorPlatform = msg->getOwnship();
   const Player* target = msg->getTarget();
   osg::Vec3 targetVelocityVector = target->getVelocity();
   osg::Vec3 targetPosition = target->getPosition();
   osg::Vec3 sensorPosition = sensorPlatform->getPosition();
   osg::Vec3 directionToSensor =  sensorPosition - targetPosition;
   osg::Vec3 normalizedDirectionToSensor = directionToSensor;
   normalizedDirectionToSensor.normalize();
   osg::Vec3 normalizedTargetVelocityVector = targetVelocityVector;
   normalizedTargetVelocityVector.normalize();
   osg::Vec3 targetXVector;
   targetXVector[0] = normalizedTargetVelocityVector[0]; // x
   LCreal frontToSensor = targetXVector * normalizedDirectionToSensor;
   if (frontToSensor < 0) frontToSensor = -frontToSensor;
      area += frontToSensor * x * z; // x, z are members of this object corresponding to
                                     // width and height.

   osg::Vec3 targetYVector;
   targetYVector[1] = normalizedTargetVelocityVector[1]; // y
   LCreal sideToSensor = targetYVector * normalizedDirectionToSensor;
   if (sideToSensor < 0) sideToSensor = -sideToSensor;
      area += sideToSensor * y * z; // y,z are members of this object corresponding to
                                    // length and height.

   osg::Vec3 targetZVector;
   targetZVector[2] = normalizedTargetVelocityVector[2]; // y
   LCreal topToSensor = targetZVector * normalizedDirectionToSensor;
   if (topToSensor < 0) topToSensor = -topToSensor;
      area += topToSensor * x * y; // x,y is a member of this object corresponding to
                                   // width and length.
   return area;
} // end else
#endif


} // end of method

} // End Simulation namespace
} // End Eaagles namespace
