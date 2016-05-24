
#include "openeaagles/simulation/Agm.h"
#include "openeaagles/simulation/Track.h"

namespace oe {
namespace simulation {

//==============================================================================
// Agm class -- Generic Air to Ground Missile
//==============================================================================

IMPLEMENT_SUBCLASS(Agm,"AgmMissile")
EMPTY_SLOTTABLE(Agm)
EMPTY_SERIALIZER(Agm)
EMPTY_COPYDATA(Agm)
EMPTY_DELETEDATA(Agm)

int Agm::getCategory() const               { return (MISSILE | GUIDED); }
const char* Agm::getDescription() const    { return "AGM"; }
const char* Agm::getNickname() const       { return "GenericAgm"; }

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Agm::Agm()
{
    STANDARD_CONSTRUCTOR()

    static base::String generic("AGM");
    setType(&generic);

   // Default guidance & dynamics for generic A/G missile
   // (derived models will override these values)
   setMaxTOF(180.0);
   setLethalRange(100.0);
   setMaxBurstRng(500.0);
   setSOBT(0.0);
   setEOBT(180.0);
   setTSG(1.0);
   setVpMin(500.0);
   setVpMax(800.0);
   setVpMaxG(800.0);
   setMaxG(5.0);
   setMaxAccel(50.0);
}

//------------------------------------------------------------------------------
// calculateVectors() --
//------------------------------------------------------------------------------
bool Agm::calculateVectors(const Player* const tgt, const Track* const trk, osg::Vec3* const los, osg::Vec3* const vel, osg::Vec3* const posx) const
{
   if (isTargetPositionValid()) {
      osg::Vec3 p0 = getPosition();
      osg::Vec3 tgtPos = getTargetPosition();
      osg::Vec3 vel0(0.0, 0.0, 0.0);     // zero velocity
      if (los != nullptr) *los = tgtPos - p0;
      if (vel != nullptr) *vel = vel0;
      if (posx != nullptr) *posx = tgtPos;
   }
   else if (trk != nullptr) {
      //los = trk->getPosition();
      //vel = trk->getVelocity();
      const Player* tgt0 = trk->getTarget();
      osg::Vec3 p0 = getPosition();
      if (los != nullptr) *los = tgt0->getPosition() -  p0;
      if (vel != nullptr) *vel = tgt0->getVelocity();
      if (posx != nullptr) *posx = tgt0->getPosition();
   }
   else if (tgt != nullptr) {
      osg::Vec3 p0 = getPosition();
      if (los != nullptr) *los = tgt->getPosition() -  p0;
      if (vel != nullptr) *vel = tgt->getVelocity();
      if (posx != nullptr) *posx = tgt->getPosition();
   }
   else {
      // no guidance until we have a target
      return false;
   }

   return true;
}

} // End simulation namespace
}
