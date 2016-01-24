//------------------------------------------------------------------------------
// Class: Nib
// Description: Portions of class defined to support munition detonation PDUs
//------------------------------------------------------------------------------

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

namespace oe {
namespace network {
namespace dis {

//------------------------------------------------------------------------------
// munitionDetonationMsgFactory() -- (Output) Munition detonation message factory
//------------------------------------------------------------------------------
bool Nib::munitionDetonationMsgFactory(const LCreal)
{
   // Dummy weapon?
   const Simulation::Weapon* ww = dynamic_cast<const Simulation::Weapon*>( getPlayer() );
   if (ww != nullptr) {
      if (ww->isDummy()) return true;
   }

    bool ok = true;
    //std::cout << "NetIO::munitionDetonationMsgFactory() HERE!!" << std::endl;

    // Get our NetIO
    NetIO* disIO = static_cast<NetIO*>(getNetIO());

    // If our NIB's player just detonated, then it must be a weapon!
    Simulation::Weapon* mPlayer = dynamic_cast<Simulation::Weapon*>(getPlayer());
    if (mPlayer == nullptr) return false;

    // Ok, we have the weapon, now get the firing and target players
    Simulation::Player* tPlayer = mPlayer->getTargetPlayer();
    Simulation::Player* fPlayer = mPlayer->getLaunchVehicle();
    if (fPlayer == nullptr) return false;

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
         if (tPlayer != nullptr) {
            pdu.targetEntityID.ID = tPlayer->getID();
            if (tPlayer->isLocalPlayer()) {
               // Local player, use our site/app/exerc IDs
               pdu.targetEntityID.simulationID.siteIdentification = getSiteID();
               pdu.targetEntityID.simulationID.applicationIdentification = getApplicationID();
               tOk = true;
            }
            else {
               // Networked player, use its NIB's IDs
               const Nib* fNIB = dynamic_cast<const Nib*>( tPlayer->getNib() );
               if (fNIB != nullptr) {
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
    // Location & Velocity
    // ---

    // World Coordinates
    osg::Vec3d geocPos = mPlayer->getGeocPosition();
    pdu.location.X_coord = geocPos[basic::Nav::IX];
    pdu.location.Y_coord = geocPos[basic::Nav::IY];
    pdu.location.Z_coord = geocPos[basic::Nav::IZ];

    // Velocity
    osg::Vec3d geocVel = mPlayer->getGeocVelocity();
    pdu.velocity.component[0] = static_cast<float>(geocVel[basic::Nav::IX]);
    pdu.velocity.component[1] = static_cast<float>(geocVel[basic::Nav::IY]);
    pdu.velocity.component[2] = static_cast<float>(geocVel[basic::Nav::IZ]);

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
    pdu.locationInEntityCoordinates.component[0] = static_cast<float>(lpos[0]);
    pdu.locationInEntityCoordinates.component[1] = static_cast<float>(lpos[1]);
    pdu.locationInEntityCoordinates.component[2] = static_cast<float>(lpos[2]);

    // ---
    // Results
    // ---
    pdu.detonationResult = static_cast<unsigned char>( mPlayer->getDetonationResults() );
    pdu.numberOfArticulationParameters = 0;

    //std::cout << "NetIO::munitionDetonationMsgFactory() results: " << int(pdu.detonationResult) << std::endl;
    //pdu.dumpData();

    // ---
    // Send the PDU
    // ---
    if (basic::NetHandler::isNotNetworkByteOrder()) pdu.swapBytes();
    ok = disIO->sendData(reinterpret_cast<char*>(&pdu), sizeof(pdu));

    // Set the detonation message sent flag so that we don't do this again.
    setDetonationMessageSent(true);

    return ok;
}

} // End dis namespace
} // End Network namespace
} // End oe namespace
