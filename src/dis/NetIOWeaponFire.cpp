//==============================================================================
// Weapon Fire PDUs (Portions of NetIO and Nib)
//==============================================================================

#include "openeaagles/dis/NetIO.h"
#include "openeaagles/dis/Nib.h"
#include "openeaagles/dis/pdu.h"

#include "openeaagles/simulation/AirVehicle.h"
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
// processFirePDU() callback --
//------------------------------------------------------------------------------
void NetIO::processFirePDU(const FirePDU* const pdu)
{
    // Get the Firing Player's ID
    unsigned short fPlayerId = pdu->firingEntityID.ID;
    unsigned short fSiteId = pdu->firingEntityID.simulationID.siteIdentification;
    unsigned short fApplicationId = pdu->firingEntityID.simulationID.applicationIdentification;

    //std::cout << "NetIO::processFirePDU() fired";
    //std::cout << "(" << pdu->firingEntityID.ID;
    //std::cout << "," << pdu->firingEntityID.simulationID.applicationIdentification ;
    //std::cout << "," << pdu->firingEntityID.simulationID.siteIdentification;
    //std::cout << ")" << std::endl;

    // Ignore our own PDUs
    if (fSiteId == getSiteID() && fApplicationId == getApplicationID()) return;

    //pdu->dumpData();

    // Get the Munition Player's ID
    unsigned short mPlayerId = pdu->munitionID.ID;
    unsigned short mSiteId = pdu->munitionID.simulationID.siteIdentification;
    unsigned short mApplicationId = pdu->munitionID.simulationID.applicationIdentification;

    // Get the Target Player's ID
    unsigned short tPlayerId = pdu->targetEntityID.ID;
    unsigned short tSiteId = pdu->targetEntityID.simulationID.siteIdentification;
    unsigned short tApplicationId = pdu->targetEntityID.simulationID.applicationIdentification;

    // ---
    // 1) Find the target (local) player
    // ---
    Simulation::Player* tPlayer = 0;
    if (tSiteId == getSiteID() && tApplicationId == getApplicationID()) {
        // Must be local
        SPtr<Basic::PairStream> players( getSimulation()->getPlayers() );
        tPlayer = getSimulation()->findPlayer(tPlayerId);
    }
    //std::cout << "Net Fire(2) tPlayer = " << tPlayer << std::endl;

    // ---
    // 2) Find the firing player and munitions (networked) IPlayers
    // ---
    Simulation::Nib* fNib = 0;
    Simulation::Nib* mNib = 0;
    if (fSiteId != getSiteID() || fApplicationId != getApplicationID()) {
        // Must be networked players
        fNib = findDisNib(fPlayerId, fSiteId, fApplicationId, INPUT_NIB);
        mNib = findDisNib(mPlayerId, mSiteId, mApplicationId, INPUT_NIB);
    }
    //std::cout << "Net Fire(3) fNib = " << fNib << ", mNib = " << mNib << std::endl;

    // --- Nothing really needs to be done.
}

//------------------------------------------------------------------------------
// weaponFireMsgFactory() -- (Output support) Weapon fire message factory
//------------------------------------------------------------------------------
bool Nib::weaponFireMsgFactory(const LCreal)
{
    bool ok = true;
    //std::cout << "NetIO::weaponFireMsgFactory() HERE!!" << std::endl;

    // Get our NetIO
    NetIO* disIO = (NetIO*)(getNetIO());
    //Simulation* sim = disIO->getSimulation();

    // Set the NIB mode so that we don't do this again.
    setMode(Simulation::Player::ACTIVE);

    // Our NIB's player is a weapon that just became active
    Simulation::Weapon* mPlayer = (Simulation::Weapon*)(getPlayer());

    // Ok, we have the weapon, now get the firing and target players
    Simulation::Player* tPlayer = mPlayer->getTargetPlayer();
    Simulation::Player* fPlayer = mPlayer->getLaunchVehicle();
    if (fPlayer == 0) return false;

    // ---
    // PDU header
    // ---
    FirePDU pdu;
    pdu.header.protocolVersion = disIO->getVersion();
    pdu.header.exerciseIdentifier = disIO->getExerciseID();
    pdu.header.PDUType = NetIO::PDU_FIRE;
    pdu.header.protocolFamily = NetIO::PDU_FAMILY_WARFARE;
    pdu.header.timeStamp = disIO->timeStamp();
    pdu.header.length = sizeof(FirePDU);
    pdu.header.status = 0;
    pdu.header.padding = 0;

    // ---
    // Set the PDU data with the firing (launcher) player's id
    // ---
    pdu.firingEntityID.ID = fPlayer->getID();
    pdu.firingEntityID.simulationID.siteIdentification = disIO->getSiteID();
    pdu.firingEntityID.simulationID.applicationIdentification = disIO->getApplicationID();

    // ---
    // Set the PDU data with the munition's ID
    // ---
    pdu.munitionID.ID = mPlayer->getID();
    pdu.munitionID.simulationID.siteIdentification = disIO->getSiteID();
    pdu.munitionID.simulationID.applicationIdentification = disIO->getApplicationID();

    // ---
    // Set the PDU data with the target's ID
    // ---
    {
      bool tOk = false;
      if (tPlayer != 0) {
         pdu.targetEntityID.ID = tPlayer->getID();
         if (tPlayer->isLocalPlayer()) {
            // Local player, use our site/app/exercise IDs
            pdu.targetEntityID.simulationID.siteIdentification = disIO->getSiteID();
            pdu.targetEntityID.simulationID.applicationIdentification = disIO->getApplicationID();
            tOk = true;
         }
         else {
            const Nib* fNIB = dynamic_cast<const Nib*>( tPlayer->getNib() );
            if (fNIB != 0) {
               // Networked player, use it's NIB's IDs
               pdu.targetEntityID.simulationID.siteIdentification = fNIB->getSiteID();
               pdu.targetEntityID.simulationID.applicationIdentification = fNIB->getApplicationID();
               tOk = true;
            }
         }
      }
      if (!tOk) {
         // Networked player, use it's NIB's IDs
         pdu.targetEntityID.ID = 0;
         pdu.targetEntityID.simulationID.siteIdentification = 0;
         pdu.targetEntityID.simulationID.applicationIdentification = 0;
      }
    }

    // ---
    // Event ID
    // ---
    pdu.eventID.simulationID.siteIdentification = disIO->getSiteID();
    pdu.eventID.simulationID.applicationIdentification = disIO->getApplicationID();
    pdu.eventID.eventNumber = mPlayer->getReleaseEventID();

    // ---
    // Location & Velocity
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
    // Range and fire index
    // ---
    pdu.fireMissionIndex = 0;
    pdu.range = 0.0;

    //pdu.dumpData();
    //std::cout << "NetIO::weaponFireMsgFactory() fired:";
    //std::cout << "(" << pdu.firingEntityID.ID;
    //std::cout << "," << pdu.firingEntityID.simulationID.applicationIdentification ;
    //std::cout << "," << pdu.firingEntityID.simulationID.siteIdentification;
    //std::cout << ")   munition:";
    //std::cout << "(" << pdu.munitionID.ID;
    //std::cout << "," << pdu.munitionID.simulationID.applicationIdentification ;
    //std::cout << "," << pdu.munitionID.simulationID.siteIdentification;
    //std::cout << ")" << std::endl;

    if (Basic::NetHandler::isNotNetworkByteOrder()) pdu.swapBytes();
    ok = disIO->sendData((char*)&pdu,sizeof(pdu));

    return ok;
}

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace
