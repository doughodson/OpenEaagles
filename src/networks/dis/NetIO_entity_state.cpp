//------------------------------------------------------------------------------
// Class: NetIO
// Description: Portions of class defined to support entity state PDUs
//------------------------------------------------------------------------------

#include "openeaagles/networks/dis/NetIO.hpp"
#include "openeaagles/networks/dis/Nib.hpp"
#include "openeaagles/networks/dis/Ntm.hpp"
#include "openeaagles/networks/dis/pdu.hpp"

#include "openeaagles/models/players/AirVehicle.hpp"
#include "openeaagles/models/players/GroundVehicle.hpp"
#include "openeaagles/models/players/LifeForms.hpp"
#include "openeaagles/models/players/Missile.hpp"
#include "openeaagles/models/systems/StoresMgr.hpp"
#include "openeaagles/models/Simulation.hpp"

#include "openeaagles/simulation/Station.hpp"

#include "openeaagles/base/Nav.hpp"
#include "openeaagles/base/NetHandler.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/util/str_utils.hpp"

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace dis {

//------------------------------------------------------------------------------
// processEntityStatePDU() callback --
//------------------------------------------------------------------------------
void NetIO::processEntityStatePDU(const EntityStatePDU* const pdu)
{
    // Get PDU IDs
    unsigned short playerId = pdu->entityID.ID;
    unsigned short site = pdu->entityID.simulationID.siteIdentification;
    unsigned short app = pdu->entityID.simulationID.applicationIdentification;

    // ---
    // Make sure it's not one of ours
    // ---

    // Reject PDUs with our application and site IDs
    if (site == getSiteID() &&  app == getApplicationID()) return;

    // Search test (reject PDUs from players on our output list)
    interop::Nib* testNib = findDisNib(playerId, site, app, OUTPUT_NIB);
    if (testNib != nullptr) return;

    // ---
    // Find the Network Interface Block
    // ---
    Nib* nib = static_cast<Nib*>( findDisNib(playerId, site, app, INPUT_NIB) );

    // ---
    // When we don't have a NIB, create one
    // ---
    if (nib == nullptr) {
        nib = static_cast<Nib*>( createNewInputNib() );
        if (nib != nullptr) {
            nib->setPlayerID(playerId);
            if (pdu->entityMarking.characterSet == 1) {
               char name[12];
               base::utStrcpy(name, 12, reinterpret_cast<const char*>(pdu->entityMarking.marking));
               nib->setPlayerName(name);
            }
            else
               nib->setPlayerName("DIS PLAYER");

            // Set the site id, app id and fed name
            {
               nib->setSiteID(site);
               nib->setApplicationID(app);
               char cbuff[32];
               makeFederateName(cbuff, 32, site, app);
               const auto fname = new base::String(cbuff);
               nib->setFederateName(fname);
               fname->unref();
            }

            nib->setDeadReckoning( interop::Nib::DeadReckoning( pdu->deadReckoningAlgorithm ) );

            nib->setEntityType(
               pdu->entityType.kind,
               pdu->entityType.domain,
               pdu->entityType.country,
               pdu->entityType.category,
               pdu->entityType.subcategory,
               pdu->entityType.specific,
               pdu->entityType.extra);

            // Side: When mapping Force ID to Player Side ...
            if (pdu->forceID == FRIENDLY_FORCE) {
                // Friendly's are blue, ...
                nib->setSide(models::Player::BLUE);
            }
            else if (pdu->forceID == OPPOSING_FORCE) {
                // opposing side is red, ...
                nib->setSide(models::Player::RED);
            }
            else if (pdu->forceID == NEUTRAL_FORCE) {
                // Neutrals are white, ...
                nib->setSide(models::Player::WHITE);
            }
            else  {
                // and everyone else is gray.
                nib->setSide(models::Player::GRAY);
            }

            addNib2InputList(nib);
            nib->unref();
        }
    }

    // ---
    // When we have a NIB, transfer our packet data to it.
    // ---
    if (nib != nullptr) {
       nib->entityStatePdu2Nib(pdu);
    }
}

}
}
