
#include "openeaagles/models/players/SpaceVehicle.hpp"
#include "openeaagles/models/dynamics/SpaceDynamicsModel.hpp"

#include "openeaagles/base/units/Distances.hpp"

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(SpaceVehicle,"SpaceVehicle")
EMPTY_SLOTTABLE(SpaceVehicle)
EMPTY_SERIALIZER(SpaceVehicle)
EMPTY_COPYDATA(SpaceVehicle)
EMPTY_DELETEDATA(SpaceVehicle)

SpaceVehicle::SpaceVehicle()
{
   STANDARD_CONSTRUCTOR()

   static base::String generic("GenericSpaceVehicle");
   setType(&generic);
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int SpaceVehicle::getMajorType() const
{
   return SPACE_VEHICLE;
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------

double SpaceVehicle::getFuelWt() const
{
   double value = 0.0;
   if (getDynamicsModel() != nullptr) {
      value = getDynamicsModel()->getFuelWt();
   }
   return value;
}

double SpaceVehicle::getFuelWtMax() const
{
   double value = 0.0;
   if (getDynamicsModel() != nullptr) {
      value = getDynamicsModel()->getFuelWtMax();
   }
   return value;
}

double SpaceVehicle::getGrossWeight() const
{
   double value = 0.0;
   if (getDynamicsModel() != nullptr) {
      value = getDynamicsModel()->getGrossWeight();
   }
   return value;
}

//------------------------------------------------------------------------------
// setControlStickYawInput(yaw) -- Yaw inputs: normalized
//          yaw:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceVehicle::setControlStickYawInput(const double yaw)
{
   bool ok = false;
   auto model = dynamic_cast<SpaceDynamicsModel*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setControlStickYawInput(yaw);
   }
   return ok;
}

//------------------------------------------------------------------------------
// setTranslateXInput(transx) -- Translate X inputs: normalized
//          transx:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceVehicle::setTranslateXInput(const double transx)
{
   bool ok = false;
   auto model = dynamic_cast<SpaceDynamicsModel*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setTranslateXInput(transx);
   }
   return ok;
}

//------------------------------------------------------------------------------
// setTranslateYInput(transy) -- Translate Y inputs: normalized
//          transy:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceVehicle::setTranslateYInput(const double transy)
{
   bool ok = false;
   auto model = dynamic_cast<SpaceDynamicsModel*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setTranslateYInput(transy);
   }
   return ok;
}

//------------------------------------------------------------------------------
// setTranslateZInput(transz) -- Translate Z inputs: normalized
//          transz:  -1.0 -> maz left;  0.0 -> center;  1.0 -> maz right
//------------------------------------------------------------------------------
bool SpaceVehicle::setTranslateZInput(const double transz)
{
   bool ok = false;
   auto model = dynamic_cast<SpaceDynamicsModel*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setTranslateZInput(transz);
   }
   return ok;
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int SpaceVehicle::getNumberOfEngines() const
{
   int  n = 0;
   if (getDynamicsModel() != nullptr) {
      n = getDynamicsModel()->getNumberOfEngines();
   }
   return n;
}

int SpaceVehicle::getEngThrust(double* const data, const int max) const
{
   int  n = 0;
   if (getDynamicsModel() != nullptr) {
      n = getDynamicsModel()->getEngThrust(data, max);
   }
   return n;
}


//==============================================================================
// class BoosterSpaceVehicle
//==============================================================================
IMPLEMENT_SUBCLASS(BoosterSpaceVehicle,"BoosterSpaceVehicle")
EMPTY_SLOTTABLE(BoosterSpaceVehicle)
EMPTY_SERIALIZER(BoosterSpaceVehicle)
EMPTY_COPYDATA(BoosterSpaceVehicle)
EMPTY_DELETEDATA(BoosterSpaceVehicle)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
BoosterSpaceVehicle::BoosterSpaceVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericBoosterSpaceVehicle");
    setType(&generic);
}


//==============================================================================
// class MannedSpaceVehicle
//==============================================================================
IMPLEMENT_SUBCLASS(MannedSpaceVehicle,"MannedSpaceVehicle")
EMPTY_SLOTTABLE(MannedSpaceVehicle)
EMPTY_SERIALIZER(MannedSpaceVehicle)
EMPTY_COPYDATA(MannedSpaceVehicle)
EMPTY_DELETEDATA(MannedSpaceVehicle)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
MannedSpaceVehicle::MannedSpaceVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericMannedSpaceVehicle");
    setType(&generic);
}


//==============================================================================
// class UnmannedSpaceVehicle
//==============================================================================
IMPLEMENT_SUBCLASS(UnmannedSpaceVehicle,"UnmannedSpaceVehicle")
EMPTY_SLOTTABLE(UnmannedSpaceVehicle)
EMPTY_SERIALIZER(UnmannedSpaceVehicle)
EMPTY_COPYDATA(UnmannedSpaceVehicle)
EMPTY_DELETEDATA(UnmannedSpaceVehicle)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
UnmannedSpaceVehicle::UnmannedSpaceVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericUnmannedSpaceVehicle");
    setType(&generic);
}

void UnmannedSpaceVehicle::dynamics(const double dt)
{
   BaseClass::dynamics(dt);
   //osg::Vec3d p = getGeocPosition();
   //osg::Vec3d v = getGeocVelocity();
   //std::cout << "USV::Dyn(" << this << ")";
   //std::cout << " lla=[ " << getLatitude() << ", " << getLongitude() << ", " << (getAltitudeAglM() * base::Distance::M2NM)  << " ]";
   //std::cout << " pos=[ " << p[0] << ", " << p[1] << ", " << p[2] << " ]";
   //std::cout << " vec=[ " << v[0] << ", " << v[1] << ", " << v[2] << " ]";
   //std::cout << std::endl;
}

}
}
