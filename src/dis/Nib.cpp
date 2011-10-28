
#include "openeaagles/dis/Nib.h"
#include "openeaagles/dis/Ntm.h"
#include "openeaagles/dis/EmissionPduHandler.h"
#include "openeaagles/dis/pdu.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Jammer.h"
#include "openeaagles/simulation/Radar.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Network {
namespace Dis {

//==============================================================================
// Class: Nib
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(Nib,"DisNib")
EMPTY_SLOTTABLE(Nib)
EMPTY_SERIALIZER(Nib)

Nib::Nib(const Simulation::NetIO::IoType ioType) : Simulation::Nib(ioType)
{
   iffFunOpData = 0;
   iffOptions = 0;
   iffLastExecTime = 0;

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
      apartMslTypes[i] = 0;
   }

   numEmissionSystems = 0;
   for (unsigned int i = 0; i < MAX_EM_SYSTEMS; i++) {
      emitterSysHandler[i] = 0;
   }
   emissionSystemsIndex = 0;
}


Nib::Nib(const Nib& org) : Simulation::Nib(org.getIoType())
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

Basic::Object* Nib::clone() const
{
    return new Nib(*this);
}

void Nib::copyData(const Nib& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      iffFunOpData = 0;
      for (unsigned int i = 0; i < MAX_AMSL; i++) {
         apartMslTypes[i] = 0;
      }
      for (unsigned int i = 0; i < MAX_EM_SYSTEMS; i++) {
         emitterSysHandler[i] = 0;
      }
   }

   // Clear (not copy) the attached missile types
   for (unsigned int i = 0; i < MAX_AMSL; i++) {
      if (apartMslTypes[i] != 0) {
         apartMslTypes[i]->unref();;
         apartMslTypes[i] = 0;
      }
   }

   // Clear (not copy) the emitter system handlers
   for (unsigned int i = 0; i < MAX_EM_SYSTEMS; i++) {
      if (emitterSysHandler[i] != 0) {
         delete emitterSysHandler[i];
         emitterSysHandler[i] = 0;
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

   if (iffFunOpData != 0) {
      delete iffFunOpData;
      iffFunOpData = 0;
   }
   iffOptions = 0;
   iffLastExecTime = 0;
}

void Nib::deleteData()
{
   if (iffFunOpData != 0) {
      delete iffFunOpData;
      iffFunOpData = 0;
   }

   for (unsigned int i = 0; i < MAX_AMSL; i++) {
      if (apartMslTypes[i] != 0) {
         apartMslTypes[i]->unref();
         apartMslTypes[i] = 0;
      }
   }

   for (unsigned int i = 0; i < MAX_EM_SYSTEMS; i++) {
      if (emitterSysHandler[i] != 0) {
         delete emitterSysHandler[i];
         emitterSysHandler[i] = 0;
      }
   }
   numEmissionSystems = 0;
   emissionSystemsIndex = 0;
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
bool Nib::networkOutputManagers(const LCreal curExecTime)
{
    // Manager IFF updates
    IffManager(curExecTime);

    // Manager emitter beam updates
    emitterBeamsManager(curExecTime);

    return true;
}

//------------------------------------------------------------------------------
// updateTheIPlayer() -- (Input support)
//   Called by our processInputList() to it's time to update the EAAGLES's
//   networked player.
//------------------------------------------------------------------------------
void Nib::updateTheIPlayer()
{
    Simulation::Player* p = getPlayer();

   // ---
   // If we haven't tried to created the IPlayer yet ...
   // ---
   if (p == 0 && isEntityTypeUnchecked()) {
      // create the player
      p = getNetIO()->createIPlayer(this);
   }

   // ---
   // Update the player's data from our object's attributes
   // ---
   if (p != 0) {
      // This transfers player data from our basic NIB to the player.
      nib2PlayerState();
      // ... transfer additional data in the future ....

#ifdef DISV7
      // ---
      // check all emission handlers for timeout
      // ---
      NetIO* const disIO = (NetIO*)(getNetIO());
      double curExecTime = disIO->getSimulation()->getExecTimeSec();
      for (unsigned char i = 0; i < numEmissionSystems; i++) {
         LCreal drTime = curExecTime - emitterSysHandler[i]->getEmPduExecTime();
         if ( drTime >= (disIO->get_HBT_PDU_EE() * disIO->get_HBT_TIMEOUT_MPLIER()) ) {
            emitterSysHandler[i]->setTimedOut();
         }
      }
#endif

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
// (Input) processElectromagneticEmissionPDU() -- 
//------------------------------------------------------------------------------
bool Nib::processElectromagneticEmissionPDU(const ElectromagneticEmissionPDU* const pdu)
{
   bool ok = false;

   // ---
   // Loop through the Emission Systems in the PDU
   // ---
   for (unsigned char is = 0; is < pdu->numberOfSystems && is < MAX_EM_SYSTEMS; is++) {
      const EmissionSystem* es = pdu->getEmissionSystem(is);
      if (es != 0) {

         unsigned char idNum = es->emitterSystem.emitterIdentificationNumber;
         EmissionPduHandler* handler = 0;

         // ---
         // Does a handler for this system already exist in our list?
         // ---
         for (unsigned char i = 0; i < numEmissionSystems && handler == 0; i++) {
            if (idNum == emitterSysHandler[i]->getEmitterIdNumber()) {
               // Yes it does!
               handler = emitterSysHandler[i];
            }
         }
         
         // ---
         // Create a handler if one doesn't already exist
         // ---
         if (handler == 0 && numEmissionSystems < MAX_EM_SYSTEMS) {

            // First, try to find an Emission PDU handler for this type system.
            // If we find one, then clone it for our use.
            NetIO* const disIO = (NetIO*)(getNetIO());
            const EmissionPduHandler* tmp = disIO->findEmissionPduHandler(es);
            if (tmp != 0) {
               handler = (EmissionPduHandler*) tmp->clone();
            }

            if (handler != 0) {
               // Handler found!
               handler->setEmitterIdNumber( idNum );
               handler->setEmitterFunction( es->emitterSystem.function );
               emitterSysHandler[numEmissionSystems] = handler;
               numEmissionSystems++;
            }
            else {
               // Handler wasn't found? 
               std::cerr << "Dis::Nib::processElectromagneticEmissionPDU() warning: ";
               std::cerr << "EmissionPduHandler not found for emitterName: " << es->emitterSystem.emitterName;
               std::cerr << std::endl;
            }

         }
         
         // ---
         // Ok, pass the location of the EmissionSystem (and beam data) to the handler
         // ---
         if (handler != 0) {
            ok = handler->updateIncoming(es, this);
         }

      } 
   } // End: for all systems

   return ok;
}

//------------------------------------------------------------------------------
// emitterBeamsManager() 
//    -- (Output) Manages the emitter beam for this NIB(Player)
//------------------------------------------------------------------------------
bool Nib::emitterBeamsManager(const LCreal curExecTime)
{
   // ---   
   // First, find all of our player's RfSensor systems and setup their handlers
   // ---
   if ( numEmissionSystems == 0 ) {

      // Check for the single-beam RADAR
      {
         // (DPG -- #### only a simple, single-beam Radar)
         const Basic::Pair * pair = getPlayer()->getSensorByType(typeid(Simulation::Radar));
         if (pair != 0) {
            Simulation::RfSensor* rs = (Simulation::RfSensor*) pair->object();

            // When we have a R/F sensor, create a handler for it
            EmissionPduHandler* handler = 0;

            // First, try to find an Emission PDU handler for this type system.
            // If we find one, then clone it for our use.
            NetIO* const disIO = (NetIO*)(getNetIO());
            const EmissionPduHandler* tmp = disIO->findEmissionPduHandler(rs);
            if (tmp != 0) {
               handler = (EmissionPduHandler*) tmp->clone();
            }

            // Handler wasn't found? Then just create a simple, default emission handler
            if (handler == 0) {
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
         const Basic::Pair * pair = getPlayer()->getSensorByType(typeid(Simulation::Jammer));
         if (pair != 0) {
            Simulation::RfSensor* js = (Simulation::RfSensor*) pair->object();

            bool singleBeam = true;
            Basic::PairStream* subcomponents = js->getComponents();
            if (subcomponents != 0) {

               // Check for multi-beam jammer (each beam is a subcomponent Jammer)
               Basic::List::Item* item = subcomponents->getFirstItem();
               while (item != 0 && numEmissionSystems < MAX_EM_SYSTEMS) {

                  Basic::Pair* pair = static_cast<Basic::Pair*>( item->getValue() );
                  Simulation::Jammer* jam = dynamic_cast<Simulation::Jammer*>( pair->object() );
                  if (jam != 0) {
                     singleBeam = false;

                     // When we have a R/F sensor, create a handler for it
                     EmissionPduHandler* handler = 0;

                     // First, try to find an Emission PDU handler for this type system.
                     // If we find one, then clone it for our use.
                     NetIO* const disIO = (NetIO*)(getNetIO());
                     const EmissionPduHandler* tmp = disIO->findEmissionPduHandler(jam);
                     if (tmp != 0) {
                        handler = (EmissionPduHandler*) tmp->clone();
                     }

                     // Handler wasn't found? Then just create a simple, default handler
                     if (handler == 0) {
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
               subcomponents = 0;
            }

            // Single beam jammer
            if (singleBeam && numEmissionSystems < MAX_EM_SYSTEMS) {

               // When we have a R/F sensor, create a handler for it
               EmissionPduHandler* handler = 0;

               // First, try to find an Emission PDU handler for this type system.
               // If we find one, then clone it for our use.
               NetIO* const disIO = (NetIO*)(getNetIO());
               const EmissionPduHandler* tmp = disIO->findEmissionPduHandler(js);
               if (tmp != 0) {
                  handler = (EmissionPduHandler*) tmp->clone();
               }

               // Handler wasn't found? Then just create a simple, default jammer handler
               if (handler == 0) {
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
      if (emitterSysHandler[emissionSystemsIndex] != 0) {
         emitterSysHandler[emissionSystemsIndex]->updateOutgoing(curExecTime, this);
      }
   }

   return true;
}

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace
