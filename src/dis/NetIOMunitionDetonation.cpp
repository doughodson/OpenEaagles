//==============================================================================
// Munition Detonation PDUs (Portions of NetIO and Nib)
//==============================================================================

#include "openeaagles/dis/NetIO.h"
#include "openeaagles/dis/Nib.h"
#include "openeaagles/dis/pdu.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Network {
namespace Dis {

//------------------------------------------------------------------------------
// processDetonationPDU() callback -- 
//------------------------------------------------------------------------------
void NetIO::processDetonationPDU(const DetonationPDU* const pdu)
{
    
   // Get the Firing Player's ID
   unsigned short fPlayerId = pdu->firingEntityID.ID;
   unsigned short fSiteId = pdu->firingEntityID.simulationID.siteIdentification;
   unsigned short fApplicationId = pdu->firingEntityID.simulationID.applicationIdentification;

   // Ignore our own PDUs
   if (fSiteId == getSiteID() && fApplicationId == getApplicationID()) return;

   // Get the Munition Player's ID
   unsigned short mPlayerId = pdu->munitionID.ID;
   unsigned short mSiteId = pdu->munitionID.simulationID.siteIdentification;
   unsigned short mApplicationId = pdu->munitionID.simulationID.applicationIdentification;

   // Get the Target Player's ID
   unsigned short tPlayerId = pdu->targetEntityID.ID;
   unsigned short tSiteId = pdu->targetEntityID.simulationID.siteIdentification;
   unsigned short tApplicationId = pdu->targetEntityID.simulationID.applicationIdentification;
    
   // ---
   // 1) Find the target player
   // ---
   Simulation::Player* tPlayer = 0;
   if (tPlayerId != 0 && tSiteId != 0 && tApplicationId != 0) {
      Simulation::Nib* tNib = findDisNib(tPlayerId, tSiteId, tApplicationId, OUTPUT_NIB);
      if (tNib != 0) {
         tPlayer = tNib->getPlayer();
      }
   }
   //std::cout << "Net kill(2) tPlayer = " << tPlayer << std::endl;
  
   // ---
   // 2) Find the firing player and munitions (networked) IPlayers
   // ---
   Simulation::Player* fPlayer = 0;
   if (fPlayerId != 0 && fSiteId != 0 && fApplicationId != 0) {
      Simulation::Nib* fNib = findDisNib(fPlayerId, fSiteId, fApplicationId, INPUT_NIB);
      if (fNib != 0) {
         fPlayer = fNib->getPlayer();
      }
      else {
         SPtr<Basic::PairStream> players( getSimulation()->getPlayers() );
         fPlayer = getSimulation()->findPlayer(fPlayerId);
      }
   }

   Simulation::Nib* mNib = 0;
   if (mPlayerId != 0 && mSiteId != 0 && mApplicationId != 0) {
      mNib = findDisNib(mPlayerId, mSiteId, mApplicationId, INPUT_NIB);
   }
                
    //std::cout << "Net kill(3) fNib = " << fNib << ", mNib = " << mNib << std::endl;
    
   // ---
   // 3) Update the data of the munition's NIB and player 
   // ---
   Simulation::Weapon* mPlayer = 0;
   if (mNib != 0) {

      // ---
      // a) Set the munition's NIB to the location of the detonation 
      // ---

      // Get the geocentric position, velocity and acceleration from the PDU
      osg::Vec3d geocPos;
      geocPos[Basic::Nav::IX] = pdu->location.X_coord;
      geocPos[Basic::Nav::IY] = pdu->location.Y_coord;
      geocPos[Basic::Nav::IZ] = pdu->location.Z_coord;

      osg::Vec3d geocVel;
      geocVel[Basic::Nav::IX] = pdu->velocity.component[0];
      geocVel[Basic::Nav::IY] = pdu->velocity.component[1];
      geocVel[Basic::Nav::IZ] = pdu->velocity.component[2];

      osg::Vec3d geocAcc(0,0,0);
      osg::Vec3d geocAngles(0,0,0);
      osg::Vec3d arates(0,0,0);

      // (re)initialize the dead reckoning function
      mNib->resetDeadReckoning(
         Simulation::Nib::STATIC_DRM,
         geocPos,
         geocVel,
         geocAcc,
         geocAngles,
         arates);

      // Set the NIB's mode to DETONATED
      mNib->setMode(Simulation::Player::DETONATED);

      // Find the munition player and set it's mode, location and target position
      mPlayer = dynamic_cast<Simulation::Weapon*>(mNib->getPlayer());
      if (mPlayer != 0) {

         // Munition's mode
         mPlayer->setMode(Simulation::Player::DETONATED);

         // munition's position, velocity and acceleration at the time of the detonation
         mPlayer->setGeocPosition(geocPos);
         mPlayer->setGeocVelocity(geocVel);
         mPlayer->setGeocAcceleration(geocAcc);

         // detonation results
         mPlayer->setDetonationResults(Simulation::Weapon::Detonation(pdu->detonationResult));

         // Munition's target player and the location of detonation relative to target
         mPlayer->setTargetPlayer(tPlayer,false);
         LCreal x = pdu->locationInEntityCoordinates.component[0];
         LCreal y = pdu->locationInEntityCoordinates.component[1];
         LCreal z = pdu->locationInEntityCoordinates.component[2];
         osg::Vec3 loc(x,y,z);
         mPlayer->setDetonationLocation(loc);

         // Munition's launcher
         if (mPlayer->getLaunchVehicle() == 0 && fPlayer != 0) {
            mPlayer->setLaunchVehicle(fPlayer);
         }
      }
   }

   // ---
   // 4) Check all local players for the effects of the detonation
   // ---
   if (mPlayer != 0) {
      mPlayer->checkDetonationEffect();
   }
}


//------------------------------------------------------------------------------
// munitionDetonationMsgFactory() -- (Output) Munition detonation message factory
//------------------------------------------------------------------------------
bool Nib::munitionDetonationMsgFactory(const LCreal)
{
   // Dummy weapon?
   const Simulation::Weapon* ww = dynamic_cast<const Simulation::Weapon*>( getPlayer() );
   if (ww != 0) {
      if (ww->isDummy()) return true;
   }

    bool ok = true;
    //std::cout << "NetIO::munitionDetonationMsgFactory() HERE!!" << std::endl;

    // Get our NetIO
    NetIO* disIO = (NetIO*)(getNetIO());
    
    // If our NIB's player just detonated, then it must be a weapon!
    Simulation::Weapon* mPlayer = dynamic_cast<Simulation::Weapon*>(getPlayer());
    if (mPlayer == 0) return false;

    // Ok, we have the weapon, now get the firing and target players
    Simulation::Player* tPlayer = mPlayer->getTargetPlayer();
    Simulation::Player* fPlayer = mPlayer->getLaunchVehicle();
    if (fPlayer == 0) return false;

    // ---
    // PDU header 
    // ---
    DetonationPDU pdu;
    pdu.header.protocolVersion = disIO->getVersion();
    pdu.header.PDUType = NetIO::PDU_DETONATION;
    pdu.header.protocolFamily = NetIO::PDU_FAMILY_WARFARE;
    pdu.header.length = sizeof(DetonationPDU);
    pdu.header.exerciseIdentifier = disIO->getExerciseID();
    pdu.header.timeStamp = disIO->timeStamp();
    pdu.header.status = 0;
    pdu.header.padding = 0;

    // ---
    // Set the PDU data with the firing (launcher) player's id
    // ---
    pdu.firingEntityID.ID = fPlayer->getID();
    pdu.firingEntityID.simulationID.siteIdentification = getSiteID();
    pdu.firingEntityID.simulationID.applicationIdentification = getApplicationID();
    
    // ---
    // Set the PDU data with the munition's ID
    // ---
    pdu.munitionID.ID = mPlayer->getID();
    pdu.munitionID.simulationID.siteIdentification = getSiteID();
    pdu.munitionID.simulationID.applicationIdentification = getApplicationID();
    
    // ---
    // Set the PDU data with the target's ID
    // ---
    {
         bool tOk = false;
         if (tPlayer != 0) {
            pdu.targetEntityID.ID = tPlayer->getID();
            if (tPlayer->isLocalPlayer()) {
               // Local player, use our site/app/exerc IDs
               pdu.targetEntityID.simulationID.siteIdentification = getSiteID();
               pdu.targetEntityID.simulationID.applicationIdentification = getApplicationID();
               tOk = true;
            }
            else {
               // Networked player, use it's NIB's IDs
               const Nib* fNIB = dynamic_cast<const Nib*>( tPlayer->getNib() );
               if (fNIB != 0) {
                  pdu.targetEntityID.simulationID.siteIdentification = fNIB->getSiteID();
                  pdu.targetEntityID.simulationID.applicationIdentification = fNIB->getApplicationID();
                  tOk = true;
               }
            }
         }
         if (!tOk) {
            pdu.targetEntityID.ID = 0;
            pdu.targetEntityID.simulationID.siteIdentification = 0;
            pdu.targetEntityID.simulationID.applicationIdentification = 0;
         }
    }
    
    // ---
    // Event ID
    // ---
    pdu.eventID.simulationID.siteIdentification = getSiteID();
    pdu.eventID.simulationID.applicationIdentification = getApplicationID();
    pdu.eventID.eventNumber = mPlayer->getReleaseEventID();

    // ---
    // Location & Velociy
    // ---

    // World Coordinates
    osg::Vec3d geocPos = mPlayer->getGeocPosition();
    pdu.location.X_coord = geocPos[Basic::Nav::IX];
    pdu.location.Y_coord = geocPos[Basic::Nav::IY];
    pdu.location.Z_coord = geocPos[Basic::Nav::IZ];

    // Velocity
    osg::Vec3d geocVel = mPlayer->getGeocVelocity();
    pdu.velocity.component[0] = (float)geocVel[Basic::Nav::IX];
    pdu.velocity.component[1] = (float)geocVel[Basic::Nav::IY];
    pdu.velocity.component[2] = (float)geocVel[Basic::Nav::IZ];

    // ---
    // Burst
    // ---
    pdu.burst.munition.kind         = getEntityKind();
    pdu.burst.munition.domain       = getEntityDomain();
    pdu.burst.munition.country      = getEntityCountry();
    pdu.burst.munition.category     = getEntityCategory();
    pdu.burst.munition.subcategory  = getEntitySubcategory();
    pdu.burst.munition.specific     = getEntitySpecific();
    pdu.burst.munition.extra        = getEntityExtra();
    pdu.burst.warhead = 0;
    pdu.burst.fuse = 0;;
    pdu.burst.quantity = 1;
    pdu.burst.rate = 0;
    
    // ---
    // Location 
    // ---
    osg::Vec3 lpos = mPlayer->getDetonationLocation();
    pdu.locationInEntityCoordinates.component[0] = (float) lpos[0];
    pdu.locationInEntityCoordinates.component[1] = (float) lpos[1];
    pdu.locationInEntityCoordinates.component[2] = (float) lpos[2];

    // ---
    // Results 
    // ---
    pdu.detonationResult = (unsigned char)( mPlayer->getDetonationResults() );
    pdu.numberOfArticulationParameters = 0;

    //std::cout << "NetIO::munitionDetonationMsgFactory() results: " << int(pdu.detonationResult) << std::endl;
    //pdu.dumpData();

    // ---
    // Send the PDU
    // ---
    if (Basic::NetHandler::isNotNetworkByteOrder()) pdu.swapBytes();
    ok = disIO->sendData((char*)&pdu,sizeof(pdu));

    // Set the detonation message sent flag so that we don't do this again.
    setDetonationMessageSent(true);

    return ok;
}

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace
