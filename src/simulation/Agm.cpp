
#include "openeaagles/simulation/Agm.h"
#include "openeaagles/simulation/Track.h"

namespace Eaagles {
namespace Simulation {

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
        
    static Basic::String generic("AGM");
    setType(&generic);

   // Default guidance & dynamics for generic A/G missile
   // (derived models will override these values)
   setMaxTOF(180.0);
   setLethalRange(100.0f);
   setMaxBurstRng(500.0f);
   setSOBT(0.0f);
   setEOBT(180.0f);
   setTSG(1.0);
   setVpMin(500.0);
   setVpMax(800.0f);
   setVpMaxG(800.0f);
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
      if (los != 0) *los = tgtPos - p0;
      if (vel != 0) *vel = vel0;
      if (posx != 0) *posx = tgtPos;
   }
   else if (trk != 0) {
      //los = trk->getPosition();
      //vel = trk->getVelocity();
      const Player* tgt0 = trk->getTarget();
      osg::Vec3 p0 = getPosition();
      if (los != 0) *los = tgt0->getPosition() -  p0;
      if (vel != 0) *vel = tgt0->getVelocity();
      if (posx != 0) *posx = tgt0->getPosition();
   }
   else if (tgt != 0) {
      osg::Vec3 p0 = getPosition();
      if (los != 0) *los = tgt->getPosition() -  p0;
      if (vel != 0) *vel = tgt->getVelocity();
      if (posx != 0) *posx = tgt->getPosition();
   }
   else {
      // no guidance until we have a target
      return false;
   }

   return true;
}

} // End Simulation namespace
} // End Eaagles namespace
