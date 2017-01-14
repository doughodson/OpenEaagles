//------------------------------------------------------------------------------
// Classes: IrShape, IrSphere, IrBox
//------------------------------------------------------------------------------
#include "openeaagles/models/IrShapes.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/systems/IrSensor.hpp"
#include "openeaagles/models/IrSignature.hpp"
#include "openeaagles/models/IrQueryMsg.hpp"

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/osg/Vec3d"
#include "openeaagles/base/units/Areas.hpp"
#include "openeaagles/base/units/Distances.hpp"

#include <cmath>

namespace oe {
namespace models {

//==============================================================================
// Class: IrShape (Abstract)
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(IrShape, "IrShape")

BEGIN_SLOTTABLE(IrShape)
"area",
END_SLOTTABLE(IrShape)

BEGIN_SLOT_MAP(IrShape)
ON_SLOT(1,setSlotIrShapeArea,base::Number)
END_SLOT_MAP()

EMPTY_DELETEDATA(IrShape)
EMPTY_SERIALIZER(IrShape)

IrShape::IrShape()
{
   STANDARD_CONSTRUCTOR()
   area = 0.0;
}

void IrShape::copyData(const IrShape& org, const bool)
{
   BaseClass::copyData(org);
   area = org.area;
}

bool IrShape::setSlotIrShapeArea(const oe::base::Number* const num)
{
   double value = 0.0;
   const auto a = dynamic_cast<const base::Area*>(num);
   if (a != nullptr) {
      base::SquareMeters sm;
      value = static_cast<double>(sm.convert(*a));
   }
   else if (num != nullptr) {
      value = num->getReal();
   }
   area = value;
   return true;
}

double IrShape::getArea()
{
   return area;
}

double IrShape::getReflectorAreaInFieldOfView(const IrQueryMsg* const msg)
{
   double angleOffBoresight = msg->getAngleOffBoresight();

   double maxAngle = msg->getSendingSensor()->getIFOVTheta();

   if (angleOffBoresight > maxAngle) return 0;

   return getArea();
}


//==============================================================================
// Class: IrSphere
//==============================================================================
IMPLEMENT_SUBCLASS(IrSphere, "IrSphere")
EMPTY_SERIALIZER(IrSphere)

BEGIN_SLOTTABLE(IrSphere)
"radius",
END_SLOTTABLE(IrSphere)

BEGIN_SLOT_MAP(IrSphere)
ON_SLOT(1,setSlotIrSphereRadius,base::Number)
END_SLOT_MAP()

IrSphere::IrSphere()
{
   STANDARD_CONSTRUCTOR()
   radius = 10.0;
}

void IrSphere::copyData(const IrSphere& org, const bool)
{
   BaseClass::copyData(org);
   radius = org.radius;
}

void IrSphere::deleteData()
{
}

bool IrSphere::setSlotIrSphereRadius(const oe::base::Number* const s)
{
   double value = 0.0;

   const auto d = dynamic_cast<const base::Distance*>(s);
   if (d != nullptr) {
      base::Meters m;
      value = static_cast<double>(m.convert(*d));
   }
   else if (s != nullptr) {
      value = s->getReal();
   }
   radius = value;
   return true;
}

double IrSphere::getArea()
{
   //return (double) (radius * radius * 4.0 * PI);
   //Calculates the projected area of a sphere (projected area is a flat circle)
   return static_cast<double>(radius * radius * base::PI);
}


//==============================================================================
// Class: IrBox
//==============================================================================

IMPLEMENT_SUBCLASS(IrBox, "IrBox")
EMPTY_SERIALIZER(IrBox)

BEGIN_SLOTTABLE(IrBox)
"x",
"y",
"z",
END_SLOTTABLE(IrBox)

BEGIN_SLOT_MAP(IrBox)
ON_SLOT(1,setSlotIrBoxX,base::Number)
ON_SLOT(2,setSlotIrBoxY,base::Number)
ON_SLOT(3,setSlotIrBoxZ,base::Number)
END_SLOT_MAP()

IrBox::IrBox()
{
   STANDARD_CONSTRUCTOR()
   x = 10.0;
   y = 10.0;
   z = 10.0;
}

void IrBox::copyData(const IrBox& org, const bool)
{
   BaseClass::copyData(org);
   x = org.x;
   y = org.y;
   z = org.z;
}

void IrBox::deleteData()
{
}

bool IrBox::setSlotIrBoxX(const oe::base::Number* const s)
{
   double value = 0.0;

   const auto d = dynamic_cast<const base::Distance*>(s);
   if (d != nullptr) {
      base::Meters m;
      value = static_cast<double>(m.convert(*d));
   }
   else if (s != nullptr) {
      value = s->getReal();
   }
   x = value;
   return true;
}

bool IrBox::setSlotIrBoxY(const oe::base::Number* const s)
{
   double value = 0.0;

   const auto d = dynamic_cast<const base::Distance*>(s);
   if (d != nullptr) {
      base::Meters m;
      value = static_cast<double>(m.convert(*d));
   }
   else if (s != nullptr) {
       value = s->getReal();
   }
   y = value;
   return true;
}

bool IrBox::setSlotIrBoxZ(const oe::base::Number* const s)
{
   double value = 0.0;

   const auto d = dynamic_cast<const base::Distance*>(s);
   if (d != nullptr) {
      base::Meters m;
      value = static_cast<double>(m.convert(*d));
   }
   else if (s != nullptr) {
       value = s->getReal();
   }
   z = value;
   return true;
}

double IrBox::getArea()
{
   return static_cast<double>((x*y * 2) + (2*x*z) + (2*y*z));
}

double IrBox::getReflectorAreaInFieldOfView(const IrQueryMsg* const msg)
{
   double area = 0.0;
#if 1
   //retrieve the angle off of the gimbal boresight
   const double angleOffBoresight = msg->getAngleOffBoresight();

   //retrieve the planar angle of the IFOV of the IR sensor
   const double maxAngle = msg->getSendingSensor()->getIFOVTheta();

   //If the angle to the target is outside the IFOV, then return an area of 0.
   //This will cause the target to be discarded from further IR signature processing.
   if (angleOffBoresight > maxAngle){
      //std::cout << angleOffBoresight * base::Angle::R2DCC << " " << msg->getGimbalAzimuth() * base::Angle::R2DCC << " " << msg->getGimbalElevation() * base::Angle::R2DCC << " DETECTED" << std::endl;
      return area;
   }

   //std::cout << angleOffBoresight * base::Angle::R2DCC << " " << msg->getGimbalAzimuth() * base::Angle::R2DCC << " " << msg->getGimbalElevation() * base::Angle::R2DCC << " DETECTED" << std::endl;

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
   double angleOffBoresight = msg->getAngleOffBoresight();

   double maxAngle = msg->getSendingSensor()->getIFOVTheta();

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
   double frontToSensor = targetXVector * normalizedDirectionToSensor;
   if (frontToSensor < 0) frontToSensor = -frontToSensor;
      area += frontToSensor * x * z; // x, z are members of this object corresponding to
                                     // width and height.

   osg::Vec3 targetYVector;
   targetYVector[1] = normalizedTargetVelocityVector[1]; // y
   double sideToSensor = targetYVector * normalizedDirectionToSensor;
   if (sideToSensor < 0) sideToSensor = -sideToSensor;
      area += sideToSensor * y * z; // y,z are members of this object corresponding to
                                    // length and height.

   osg::Vec3 targetZVector;
   targetZVector[2] = normalizedTargetVelocityVector[2]; // y
   double topToSensor = targetZVector * normalizedDirectionToSensor;
   if (topToSensor < 0) topToSensor = -topToSensor;
      area += topToSensor * x * y; // x,y is a member of this object corresponding to
                                   // width and length.
   return area;
} // end else
#endif

}

}
}
