//------------------------------------------------------------------------------
// Class: NetIO
// Description: Portions of class defined to support weapon fire PDUs
//------------------------------------------------------------------------------

#include "openeaagles/networks/dis/NetIO.hpp"
#include "openeaagles/networks/dis/Nib.hpp"
#include "openeaagles/networks/dis/pdu.hpp"

#include "openeaagles/models/players/AirVehicle.hpp"
#include "openeaagles/models/players/AbstractWeapon.hpp"
#include "openeaagles/models/Simulation.hpp"

#include "openeaagles/base/Nav.hpp"
#include "openeaagles/base/NetHandler.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {

namespace dis {

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
    models::Player* tPlayer = nullptr;
    if (tSiteId == getSiteID() && tApplicationId == getApplicationID()) {
        // Must be local
        base::safe_ptr<base::PairStream> players( getSimulation()->getPlayers() );
        tPlayer = dynamic_cast<models::Player*>(getSimulation()->findPlayer(tPlayerId));  // added DDH
    }
    //std::cout << "Net Fire(2) tPlayer = " << tPlayer << std::endl;

    // ---
    // 2) Find the firing player and munitions (networked) IPlayers
    // ---
    interop::Nib* fNib = nullptr;
    interop::Nib* mNib = nullptr;
    if (fSiteId != getSiteID() || fApplicationId != getApplicationID()) {
        // Must be networked players
        fNib = findDisNib(fPlayerId, fSiteId, fApplicationId, INPUT_NIB);
        mNib = findDisNib(mPlayerId, mSiteId, mApplicationId, INPUT_NIB);
    }
    //std::cout << "Net Fire(3) fNib = " << fNib << ", mNib = " << mNib << std::endl;

    // --- Nothing really needs to be done.
}

}
}
