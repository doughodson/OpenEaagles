
#include "openeaagles/networks/dis/Nib.hpp"
#include "openeaagles/networks/dis/Ntm.hpp"
#include "openeaagles/networks/dis/EmissionPduHandler.hpp"
#include "openeaagles/networks/dis/pdu.hpp"

#include "openeaagles/models/players/AirVehicle.hpp"
#include "openeaagles/models/systems/Jammer.hpp"
#include "openeaagles/models/systems/Radar.hpp"
#include "openeaagles/models/WorldModel.hpp"

#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {

namespace dis {

IMPLEMENT_PARTIAL_SUBCLASS(Nib, "DisNib")
EMPTY_SLOTTABLE(Nib)
EMPTY_SERIALIZER(Nib)

Nib::Nib(const interop::NetIO::IoType ioType) : interop::Nib(ioType)
{
   iffFunOpData = nullptr;
   iffOptions = 0;
   iffLastExecTime = 0;
   iffEventId = 0;

   disKind = 0;
   disDomain = 0;
   disCountry = 0;
   disCategory = 0;
   disSubcategory = 0;
   disSpecific = 0;
   disExtra = 0;

   siteID = 0;
   appID = 0;

   for (unsigned int i = 0; i < MAX_AMSL; i++) {
      apartMslTypes[i] = nullptr;
   }

   numEmissionSystems = 0;
   for (unsigned int i = 0; i < MAX_EM_SYSTEMS; i++) {
      emitterSysHandler[i] = nullptr;
   }
   emissionSystemsIndex = 0;
   timeOffset = 0.0;
}


Nib::Nib(const Nib& org) : interop::Nib(org.getIoType())
{
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

Nib::~Nib()
{
   STANDARD_DESTRUCTOR()
}

Nib& Nib::operator=(const Nib& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

Nib* Nib::clone() const
{
    return new Nib(*this);
}

void Nib::copyData(const Nib& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      iffFunOpData = nullptr;
      for (unsigned int i = 0; i < MAX_AMSL; i++) {
         apartMslTypes[i] = nullptr;
      }
      for (unsigned int i = 0; i < MAX_EM_SYSTEMS; i++) {
         emitterSysHandler[i] = nullptr;
      }
   }

   // Clear (not copy) the attached missile types
   for (unsigned int i = 0; i < MAX_AMSL; i++) {
      if (apartMslTypes[i] != nullptr) {
         apartMslTypes[i]->unref();;
         apartMslTypes[i] = nullptr;
      }
   }

   // Clear (not copy) the emitter system handlers
   for (unsigned int i = 0; i < MAX_EM_SYSTEMS; i++) {
      if (emitterSysHandler[i] != nullptr) {
         delete emitterSysHandler[i];
         emitterSysHandler[i] = nullptr;
      }
   }
   numEmissionSystems = 0;
   emissionSystemsIndex = 0;

   // copy the rest of the data
   disKind = org.disKind;
   disDomain = org.disDomain;
   disCountry = org.disCountry;
   disCategory = org.disCategory;
   disSubcategory = org.disSubcategory;
   disSpecific = org.disSpecific;
   disExtra = org.disExtra;

   siteID = org.siteID;
   appID = org.appID;

   if (iffFunOpData != nullptr) {
      delete iffFunOpData;
      iffFunOpData = nullptr;
   }
   iffOptions = 0;
   iffLastExecTime = 0;
   iffEventId = 0;
   timeOffset = 0.0;
}

void Nib::deleteData()
{
   if (iffFunOpData != nullptr) {
      delete iffFunOpData;
      iffFunOpData = nullptr;
   }

   for (unsigned int i = 0; i < MAX_AMSL; i++) {
      if (apartMslTypes[i] != nullptr) {
         apartMslTypes[i]->unref();
         apartMslTypes[i] = nullptr;
      }
   }

   for (unsigned int i = 0; i < MAX_EM_SYSTEMS; i++) {
      if (emitterSysHandler[i] != nullptr) {
         delete emitterSysHandler[i];
         emitterSysHandler[i] = nullptr;
      }
   }
   numEmissionSystems = 0;
   emissionSystemsIndex = 0;
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool Nib::shutdownNotification()
{
   for (unsigned int i = 0; i < MAX_AMSL; i++) {
      if (apartMslTypes[i] != nullptr) {
         apartMslTypes[i]->unref();
         apartMslTypes[i] = nullptr;
      }
   }

   for (unsigned int i = 0; i < MAX_EM_SYSTEMS; i++) {
      if (emitterSysHandler[i] != nullptr) {
         emitterSysHandler[i]->unref();
         emitterSysHandler[i] = nullptr;
      }
   }
   numEmissionSystems = 0;
   emissionSystemsIndex = 0;

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// set functions
//------------------------------------------------------------------------------

void Nib::setSiteID(const unsigned short v)
{
    siteID = v;
}

void Nib::setApplicationID(const unsigned short v)
{
    appID = v;
}

//------------------------------------------------------------------------------
// networkOutputManagers() --  derived networkOutputManagers()
//------------------------------------------------------------------------------
bool Nib::networkOutputManagers(const double curExecTime)
{
    // Manager IFF updates
    IffManager(curExecTime);

    // Manager emitter beam updates
    emitterBeamsManager(curExecTime);

    return true;
}

//------------------------------------------------------------------------------
// updateTheIPlayer() -- (Input support)
//   Called by our processInputList() to its time to update the
//   networked player.
//------------------------------------------------------------------------------
void Nib::updateTheIPlayer()
{
    models::Player* p = getPlayer();

   // ---
   // If we haven't tried to created the IPlayer yet ...
   // ---
   if (p == nullptr && isEntityTypeUnchecked()) {
      // create the player
      p = getNetIO()->createIPlayer(this);
   }

   // ---
   // Update the player's data from our object's attributes
   // ---
   if (p != nullptr) {
      // This transfers player data from our basic NIB to the player.
      nib2PlayerState();
      // ... transfer additional data in the future ....

      // ---
      // check all emission handlers for timeout
      // ---
      NetIO* const disIO = static_cast<NetIO*>(getNetIO());
      if (disIO->getVersion() >= NetIO::VERSION_7) {
         const double curExecTime = disIO->getSimulation()->getExecTimeSec();
         for (unsigned char i = 0; i < numEmissionSystems; i++) {
            double drTime = curExecTime - emitterSysHandler[i]->getEmPduExecTime();
            if ( drTime >= (disIO->getHbtPduEe() * disIO->getHbtTimeoutMplier()) ) {
               emitterSysHandler[i]->setTimedOut();
            }
         }
      }

    }
}


//------------------------------------------------------------------------------
// Set (DIS) Entity type enumerations
//------------------------------------------------------------------------------
bool Nib::setEntityType(
       const unsigned char k,       // Kind
       const unsigned char d,       // Domain
       const unsigned short cc,     // Country code
       const unsigned char c,       // Category
       const unsigned char sc,      // Subcategory
       const unsigned char sp,      // Specific
       const unsigned char e        // Extra
    )
{
   // Some simple validity checks
   bool ok = (k < NetIO::NUM_ENTITY_KINDS) || (d < NetIO::MAX_ENTITY_DOMAINS);

   if (ok) {
      disKind = k;
      disDomain = d;
      disCountry = cc;
      disCategory = c;
      disSubcategory = sc;
      disSpecific = sp;
      disExtra = e;
   }

   return ok;
}

//------------------------------------------------------------------------------
// (Input) processElectromagneticEmissionPDU()
//------------------------------------------------------------------------------
bool Nib::processElectromagneticEmissionPDU(const ElectromagneticEmissionPDU* const pdu)
{
   bool ok = false;

   // ---
   // Loop through the Emission Systems in the PDU
   // ---
   for (unsigned char is = 0; is < pdu->numberOfSystems && is < MAX_EM_SYSTEMS; is++) {
      const EmissionSystem* es = pdu->getEmissionSystem(is);
      if (es != nullptr) {

         unsigned char idNum = es->emitterSystem.emitterIdentificationNumber;
         EmissionPduHandler* handler = nullptr;

         // ---
         // Does a handler for this system already exist in our list?
         // ---
         for (unsigned char i = 0; i < numEmissionSystems && handler == nullptr; i++) {
            if (idNum == emitterSysHandler[i]->getEmitterIdNumber()) {
               // Yes it does!
               handler = emitterSysHandler[i];
            }
         }

         // ---
         // Create a handler if one doesn't already exist
         // ---
         if (handler == nullptr && numEmissionSystems < MAX_EM_SYSTEMS) {

            // First, try to find an Emission PDU handler for this type system.
            // If we find one, then clone it for our use.
            NetIO* const disIO = static_cast<NetIO*>(getNetIO());
            const EmissionPduHandler* tmp = disIO->findEmissionPduHandler(es);
            if (tmp != nullptr) {
               handler = tmp->clone();
            }

            if (handler != nullptr) {
               // Handler found!
               handler->setEmitterIdNumber( idNum );
               handler->setEmitterFunction( es->emitterSystem.function );
               emitterSysHandler[numEmissionSystems] = handler;
               numEmissionSystems++;
            }
            else {
               // Handler wasn't found?
               std::cerr << "dis::Nib::processElectromagneticEmissionPDU() warning: ";
               std::cerr << "EmissionPduHandler not found for emitterName: " << es->emitterSystem.emitterName;
               std::cerr << std::endl;
            }

         }

         // ---
         // Ok, pass the location of the EmissionSystem (and beam data) to the handler
         // ---
         if (handler != nullptr) {
            ok = handler->updateIncoming(pdu, es, this);
         }

      }
   } // End: for all systems

   return ok;
}

//------------------------------------------------------------------------------
// emitterBeamsManager()
//    -- (Output) Manages the emitter beam for this NIB(Player)
//------------------------------------------------------------------------------
bool Nib::emitterBeamsManager(const double curExecTime)
{
   // ---
   // First, find all of our player's RfSensor systems and setup their handlers
   // ---
   if ( numEmissionSystems == 0 ) {

      // Check for the single-beam RADAR
      {
         // (DPG -- #### only a simple, single-beam Radar)
         const base::Pair * pair = getPlayer()->getSensorByType(typeid(models::Radar));
         if (pair != nullptr) {
            models::RfSensor* rs = (models::RfSensor*) pair->object();

            // When we have a R/F sensor, create a handler for it
            EmissionPduHandler* handler = nullptr;

            // First, try to find an Emission PDU handler for this type system.
            // If we find one, then clone it for our use.
            NetIO* const disIO = static_cast<NetIO*>(getNetIO());
            const EmissionPduHandler* tmp = disIO->findEmissionPduHandler(rs);
            if (tmp != nullptr) {
               handler = tmp->clone();
            }

            // Handler wasn't found? Then just create a simple, default emission handler
            if (handler == nullptr) {
               handler = new EmissionPduHandler();
            }

            handler->setSensor( rs );
            handler->setEmitterIdNumber( numEmissionSystems + 1 );
            emitterSysHandler[numEmissionSystems] = handler;
            numEmissionSystems++;
         }
      } // end radar check

      // Check for a Jammer
      {
         const base::Pair * pair = getPlayer()->getSensorByType(typeid(models::Jammer));
         if (pair != nullptr) {
            models::RfSensor* js = (models::RfSensor*) pair->object();

            bool singleBeam = true;
            base::PairStream* subcomponents = js->getComponents();
            if (subcomponents != nullptr) {

               // Check for multi-beam jammer (each beam is a subcomponent Jammer)
               base::List::Item* item = subcomponents->getFirstItem();
               while (item != nullptr && numEmissionSystems < MAX_EM_SYSTEMS) {

                  const auto pair = static_cast<base::Pair*>( item->getValue() );
                  const auto jam = dynamic_cast<models::Jammer*>( pair->object() );
                  if (jam != nullptr) {
                     singleBeam = false;

                     // When we have a R/F sensor, create a handler for it
                     EmissionPduHandler* handler = nullptr;

                     // First, try to find an Emission PDU handler for this type system.
                     // If we find one, then clone it for our use.
                     const auto disIO = static_cast<NetIO*>(getNetIO());
                     const auto tmp = disIO->findEmissionPduHandler(jam);
                     if (tmp != nullptr) {
                        handler = tmp->clone();
                     }

                     // Handler wasn't found? Then just create a simple, default handler
                     if (handler == nullptr) {
                        handler = new EmissionPduHandler();
                        handler->setEmitterFunction(EmissionPduHandler::ESF_JAMMING);
                     }

                     handler->setSensor( jam );
                     handler->setEmitterIdNumber( numEmissionSystems + 1 );
                     emitterSysHandler[numEmissionSystems] = handler;
                     numEmissionSystems++;

                  }
                  item = item->getNext();
               }

               subcomponents->unref();
               subcomponents = nullptr;
            }

            // Single beam jammer
            if (singleBeam && numEmissionSystems < MAX_EM_SYSTEMS) {

               // When we have a R/F sensor, create a handler for it
               EmissionPduHandler* handler = nullptr;

               // First, try to find an Emission PDU handler for this type system.
               // If we find one, then clone it for our use.
               NetIO* const disIO = static_cast<NetIO*>(getNetIO());
               const EmissionPduHandler* tmp = disIO->findEmissionPduHandler(js);
               if (tmp != nullptr) {
                  handler = tmp->clone();
               }

               // Handler wasn't found? Then just create a simple, default jammer handler
               if (handler == nullptr) {
                  handler = new EmissionPduHandler();
                  handler->setEmitterFunction(EmissionPduHandler::ESF_JAMMING);
               }

               handler->setSensor( js );
               handler->setEmitterIdNumber( numEmissionSystems + 1 );
               emitterSysHandler[numEmissionSystems] = handler;
               numEmissionSystems++;

            }

         }

      }  // end jammer check

   }  // end (numEmissionSystems == 0)

   // ---
   // Have the handlers check their electromagnetic emission systems
   // and generate the PDUs as needed.
   // ---
   for (emissionSystemsIndex = 0; emissionSystemsIndex < numEmissionSystems; emissionSystemsIndex++) {
      if (emitterSysHandler[emissionSystemsIndex] != nullptr) {
         emitterSysHandler[emissionSystemsIndex]->updateOutgoing(curExecTime, this);
      }
   }

   return true;
}

}
}
