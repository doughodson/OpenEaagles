//------------------------------------------------------------------------------
// Class: Nib
// Description: Portions of class defined to support weapon fire PDUs
//------------------------------------------------------------------------------

#include "openeaagles/networks/dis/NetIO.hpp"
#include "openeaagles/networks/dis/Nib.hpp"
#include "openeaagles/networks/dis/pdu.hpp"

#include "openeaagles/simulation/AirVehicle.hpp"
#include "openeaagles/simulation/Simulation.hpp"
#include "openeaagles/simulation/Weapon.hpp"
#include "openeaagles/base/Nav.hpp"
#include "openeaagles/base/NetHandler.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {

namespace dis {

//------------------------------------------------------------------------------
// weaponFireMsgFactory() -- (Output support) Weapon fire message factory
//------------------------------------------------------------------------------
bool Nib::weaponFireMsgFactory(const double)
{
    bool ok = true;
    //std::cout << "NetIO::weaponFireMsgFactory() HERE!!" << std::endl;

    // Get our NetIO
    NetIO* disIO = static_cast<NetIO*>(getNetIO());
    //Simulation* sim = disIO->getSimulation();

    // Set the NIB mode so that we don't do this again.
    setMode(simulation::Player::ACTIVE);

    // Our NIB's player is a weapon that just became active
    simulation::Weapon* mPlayer = static_cast<simulation::Weapon*>(getPlayer());

    // Ok, we have the weapon, now get the firing and target players
    simulation::Player* tPlayer = mPlayer->getTargetPlayer();
    simulation::Player* fPlayer = mPlayer->getLaunchVehicle();
    if (fPlayer == nullptr) return false;

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
      if (tPlayer != nullptr) {
         pdu.targetEntityID.ID = tPlayer->getID();
         if (tPlayer->isLocalPlayer()) {
            // Local player, use our site/app/exercise IDs
            pdu.targetEntityID.simulationID.siteIdentification = disIO->getSiteID();
            pdu.targetEntityID.simulationID.applicationIdentification = disIO->getApplicationID();
            tOk = true;
         }
         else {
            const Nib* fNIB = dynamic_cast<const Nib*>( tPlayer->getNib() );
            if (fNIB != nullptr) {
               // Networked player, use its NIB's IDs
               pdu.targetEntityID.simulationID.siteIdentification = fNIB->getSiteID();
               pdu.targetEntityID.simulationID.applicationIdentification = fNIB->getApplicationID();
               tOk = true;
            }
         }
      }
      if (!tOk) {
         // Networked player, use its NIB's IDs
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
    pdu.location.X_coord = geocPos[base::Nav::IX];
    pdu.location.Y_coord = geocPos[base::Nav::IY];
    pdu.location.Z_coord = geocPos[base::Nav::IZ];

    // Velocity
    osg::Vec3d geocVel = mPlayer->getGeocVelocity();
    pdu.velocity.component[0] = static_cast<float>(geocVel[base::Nav::IX]);
    pdu.velocity.component[1] = static_cast<float>(geocVel[base::Nav::IY]);
    pdu.velocity.component[2] = static_cast<float>(geocVel[base::Nav::IZ]);

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

    if (base::NetHandler::isNotNetworkByteOrder()) pdu.swapBytes();
    ok = disIO->sendData(reinterpret_cast<char*>(&pdu),sizeof(pdu));

    return ok;
}

}
}
