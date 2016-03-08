//==============================================================================
// Classes: dis::NetIO and dis::NtmInputNode
//==============================================================================

#include "openeaagles/networks/dis/NetIO.h"
#include "openeaagles/networks/dis/Nib.h"
#include "openeaagles/networks/dis/Ntm.h"
#include "openeaagles/networks/dis/EmissionPduHandler.h"
#include "openeaagles/networks/dis/pdu.h"

#include "openeaagles/simulation/Radar.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/base/List.h"
#include "openeaagles/base/NetHandler.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Distances.h"
#include "openeaagles/base/units/Times.h"
#include "openeaagles/base/util/string_utils.h"

#include <cstdlib>
#include <cstring>
#include <cctype>

namespace oe {

namespace dis {

//==============================================================================
// Class: dis::NtmInputNode
// Description: DIS incoming NTM class
//==============================================================================

class NtmInputNode : public simulation::NetIO::NtmInputNode {
   DECLARE_SUBCLASS(NtmInputNode,simulation::NetIO::NtmInputNode)

public:
   enum { ROOT_LVL, KIND_LVL, DOMAIN_LVL, COUNTRYCODE_LVL,
         CATEGORY_LVL, SUBCATEGORY_LVL, SPECIFIC_LVL, EXTRA_LVL };

public:
   NtmInputNode(const unsigned int level, const unsigned int code, const Ntm* ntm = nullptr);

   virtual const Ntm* findNtmByTypeCodes(
         const unsigned char  kind,
         const unsigned char  domain,
         const unsigned short countryCode,
         const unsigned char  category,
         const unsigned char  subcategory = 0,
         const unsigned char  specific = 0,
         const unsigned char  extra = 0
      ) const;

   // NetIO::NtmOutputNode class functions
   virtual const simulation::Ntm* findNetworkTypeMapper(const simulation::Nib* const nib) const;
   virtual bool add2OurLists(simulation::Ntm* const ntm);
   virtual void print(std::ostream& sout, const int icnt) const;

private:
   unsigned int level;        // Level
   unsigned int code;         // Code for this level
   const Ntm* ourNtm;         // Our default NTM
   base::List* subnodeList;  // List of NtmInputNode nodes below this level
};


//==============================================================================
// Class: dis::NetIO
//==============================================================================
IMPLEMENT_SUBCLASS(NetIO,"DisNetIO")

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
static const double HRT_BEAT_MPLIER       = 2.5;                                 //  Multiplier
static const double HRT_BEAT_TIMER        = 5;                                   //  seconds
static const double DRA_POS_THRST_DFLT    = 3.0;                                 //  meters
static const double DRA_ORIENT_THRST_DFLT = static_cast<double>(3.0 * base::PI/180.0); //  radians

// DISv7 default heartbeats
static const double HBT_PDU_EE          = 10;                           //  seconds
static const double HBT_PDU_IFF         = 10;                           //  seconds
static const double HBT_PDU_RECEIVER    = 60;                           //  seconds
static const double HBT_PDU_TRANSMITTER = 2;                            //  seconds
static const double HBT_TIMEOUT_MPLIER  = 2.4;                          //  Multiplier

// DISv7 default thresholds
static const double EE_AZ_THRSH = static_cast<double>(1.0 * base::PI/180.0);  //  radians
static const double EE_EL_THRSH = static_cast<double>(1.0 * base::PI/180.0);  //  radians

static const double EE_ERP_THRSH  = static_cast<double>(1.0);           //  dB
static const double EE_FREQ_THRSH = static_cast<double>(1.0);           //  Hz
static const double EE_FRNG_THRSH = static_cast<double>(1.0);           //  Hz
static const double EE_PRF_THRSH  = static_cast<double>(1.0);           //  Hz
static const double EE_PW_THRSH   = static_cast<double>(1e-6);          //  seconds
//static const unsigned int EE_HIGH_DENSITY_THRSH = 10;               //  no units


//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(NetIO)
   "netInput",             //  1) Network input handler
   "netOutput",            //  2) Network output handler
   "version",              //  3) DIS version number [ 0 .. 6 ] (IST-CF-03-01, May 5, 2003)
   "maxTimeDR",            //  4: Max DR time                                   (base::Time)
   "maxPositionError",     //  5: Max DR position error                         (base::Distance)
   "maxOrientationError",  //  6: Max DR angular error                          (base::Angle)
   "maxAge",               //  7: Max age (without update) of networked players (base::Time)
   "maxEntityRange",       //  8: Max entity range of networked players         (base::Distance)
   "emissionPduHandlers",  //  9: List of Electromagnetic-Emission PDU handlers (EmissionPduHandler)
   "siteID",               // 10: Site Identification
   "applicationID",        // 11: Application Identification
   "exerciseID",           // 12: Exercise Identification
END_SLOTTABLE(NetIO)

// Map slot table to handles
BEGIN_SLOT_MAP(NetIO)
   ON_SLOT(1, setSlotNetInput,         base::NetHandler)
   ON_SLOT(2, setSlotNetOutput,        base::NetHandler)
   ON_SLOT(3, setSlotVersion,          base::Number)

   ON_SLOT(4, setSlotMaxTimeDR,        base::Time)
   ON_SLOT(4, setSlotMaxTimeDR,        base::PairStream)

   ON_SLOT(5, setSlotMaxPositionErr,   base::Distance)
   ON_SLOT(5, setSlotMaxPositionErr,   base::PairStream)

   ON_SLOT(6, setSlotMaxOrientationErr, base::Angle)
   ON_SLOT(6, setSlotMaxOrientationErr, base::PairStream)

   ON_SLOT(7, setSlotMaxAge,           base::Time)
   ON_SLOT(7, setSlotMaxAge,           base::PairStream)

   ON_SLOT(8, setSlotMaxEntityRange,   base::Distance)
   ON_SLOT(8, setSlotMaxEntityRange,   base::PairStream)

   ON_SLOT(9, setSlotEmissionPduHandlers, base::PairStream)

   ON_SLOT(10, setSlotSiteID,             base::Number)
   ON_SLOT(11, setSlotApplicationID,      base::Number)
   ON_SLOT(12, setSlotExerciseID,         base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
NetIO::NetIO() : netInput(nullptr), netOutput(nullptr)
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void NetIO::initData()
{
   // DIS parameters
   setVersion(VERSION_1278_1A);

   siteID = 1;
   appID = 1;
   exerciseID = 1;

   // First the defaults
   setMaxTimeDR(HRT_BEAT_TIMER, 255, 255);                   //  (seconds)
   setMaxPositionErr(DRA_POS_THRST_DFLT, 255, 255);          //  (meters)
   setMaxOrientationErr(DRA_ORIENT_THRST_DFLT, 255, 255);    //  (radians)
   setMaxAge(HRT_BEAT_MPLIER*HRT_BEAT_TIMER, 255, 255);      //  (seconds)
   setMaxEntityRange(static_cast<double>(0), 255, 255);      // no range filtering

   // Clear emission PDU handle table
   for (unsigned int i = 0; i < MAX_EMISSION_HANDLERS; i++) {
      emissionHandlers[i] = nullptr;
   }
   nEmissionHandlers = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void NetIO::copyData(const NetIO& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   version = org.version;

   siteID = org.siteID;
   appID = org.appID;
   exerciseID = org.exerciseID;

   clearEmissionPduHandlers();
   for (unsigned int i = 0; i < org.nEmissionHandlers; i++) {
      const EmissionPduHandler* const tmp = org.emissionHandlers[i]->clone();
      addEmissionPduHandler(tmp);
      tmp->unref();
   }

   for (unsigned char i = 0; i < NUM_ENTITY_KINDS; i++) {
      for (unsigned char j = 0; j < MAX_ENTITY_DOMAINS; j++) {
         maxEntityRange[i][j] = org.maxEntityRange[i][j];
         maxEntityRange2[i][j] = org.maxEntityRange2[i][j];
         maxTimeDR[i][j] = org.maxTimeDR[i][j];
         maxPositionErr[i][j] = org.maxPositionErr[i][j];
         maxOrientationErr[i][j] = org.maxOrientationErr[i][j];
         maxAge[i][j] = org.maxAge[i][j];
      }
   }

   // We need to init this ourselves, so ...
   netInput = nullptr;
   netOutput = nullptr;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void NetIO::deleteData()
{
    clearEmissionPduHandlers();
    netInput = nullptr;
    netOutput = nullptr;
}

//------------------------------------------------------------------------------
// setVersion() -- Set the DIS version number
//------------------------------------------------------------------------------
bool NetIO::setVersion(const unsigned char v)
{
   version = v;
   return true;
}

//------------------------------------------------------------------------------
// initNetwork() -- Init the network
//------------------------------------------------------------------------------
bool NetIO::initNetwork()
{
    bool ok = true;

    // Initialize network input handler
    if (netInput != nullptr) {
        if (netInput->initNetwork(true)) std::cout << "netInput Initialize OK" << std::endl;
    }
    else {
        std::cerr << "NetIO::initNetwork(): failure to find the network input handler (see slot 'netInput')" << std::endl;
        ok = false;
    }

    // Initialize network output handler
    if (netOutput != nullptr) {
        if (netOutput->initNetwork(true)) std::cout << "netOutput Initialize OK" << std::endl;
    }
    else {
        std::cerr << "NetIO::initNetwork(): failure to find the network output handler (see slot 'netOutput')" << std::endl;
        ok = false;
    }

    return ok;
}

//------------------------------------------------------------------------------
// netInputHander() -- Network input handler
//------------------------------------------------------------------------------
void NetIO::netInputHander()
{
   // Read PDUs
   unsigned int j0 = 0;
   while ( (j0 < MAX_PDUs) && (recvData(reinterpret_cast<char*>(&inputBuffer[j0]), MAX_PDU_SIZE) > 0) ) {
      j0++;
   }

   while (j0 > 0) {

      // Process incoming PDUs
      unsigned int j1 = 0;
      while (j1 < j0) {
         PDUHeader* header = reinterpret_cast<PDUHeader*>(&inputBuffer[j1++][0]);

         if (isInputEnabled()) {

            // Notes: the header's bytes are still in network order, but since the
            // data we're using are all type 'char' then we're saving time by not
            // doing an initial byte swap of the header.

            if (getExerciseID() == 0 || (getExerciseID() == header->exerciseIdentifier)) {
               // When we're interested in this exercise ...
               switch (header->PDUType) {

                  case PDU_ENTITY_STATE: {
                     //std::cout << "Entity State PDU." << std::endl;
                     EntityStatePDU* pPdu = reinterpret_cast<EntityStatePDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->entityID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->entityID.simulationID.applicationIdentification) {
                           processEntityStatePDU(pPdu);
                     }
                  }
                  break;

                  case PDU_FIRE: {
                     FirePDU* pPdu = reinterpret_cast<FirePDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->firingEntityID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->firingEntityID.simulationID.applicationIdentification) {
                           processFirePDU(pPdu);
                     }
                  }
                  break;

                  case PDU_DETONATION: {
                     DetonationPDU* pPdu = reinterpret_cast<DetonationPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->firingEntityID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->firingEntityID.simulationID.applicationIdentification) {
                           processDetonationPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_SIGNAL: {
                     SignalPDU* pPdu = reinterpret_cast<SignalPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->radioRefID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->radioRefID.simulationID.applicationIdentification) {
                           processSignalPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_TRANSMITTER: {
                     TransmitterPDU* pPdu = reinterpret_cast<TransmitterPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->radioRefID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->radioRefID.simulationID.applicationIdentification) {
                           processTransmitterPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_ELECTROMAGNETIC_EMISSION: {
                     ElectromagneticEmissionPDU* pPdu = reinterpret_cast<ElectromagneticEmissionPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->emittingEntityID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->emittingEntityID.simulationID.applicationIdentification) {
                           processElectromagneticEmissionPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_DATA_QUERY: {
                     DataQueryPDU* pPdu = reinterpret_cast<DataQueryPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->originatingID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->originatingID.simulationID.applicationIdentification) {
                           processDataQueryPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_DATA: {
                     DataPDU* pPdu = reinterpret_cast<DataPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->originatingID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->originatingID.simulationID.applicationIdentification) {
                           processDataPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_COMMENT: {
                     CommentPDU* pPdu = reinterpret_cast<CommentPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->originatingID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->originatingID.simulationID.applicationIdentification) {
                           processCommentPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_START_RESUME: {
                     StartPDU* pPdu = reinterpret_cast<StartPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->originatingID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->originatingID.simulationID.applicationIdentification) {
                           processStartPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_STOP_FREEZE: {
                     StopPDU* pPdu = reinterpret_cast<StopPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->originatingID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->originatingID.simulationID.applicationIdentification) {
                           processStopPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_ACKNOWLEDGE: {
                     AcknowledgePDU* pPdu = reinterpret_cast<AcknowledgePDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->originatingID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->originatingID.simulationID.applicationIdentification) {
                           processAcknowledgePDU(pPdu);
                     }
                  }
                  break;

                  case PDU_ACTION_REQUEST: {
                     ActionRequestPDU* pPdu = reinterpret_cast<ActionRequestPDU*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->originatingID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->originatingID.simulationID.applicationIdentification) {
                           processActionRequestPDU(pPdu);
                     }
                  }
                  break;

                  case PDU_ACTION_REQUEST_R: {
                     ActionRequestPDU_R* pPdu = reinterpret_cast<ActionRequestPDU_R*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->originatingID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->originatingID.simulationID.applicationIdentification) {
                           processActionRequestPDU_R(pPdu);
                     }
                  }
                  break;

                  case PDU_ACTION_RESPONSE_R: {
                     ActionResponsePDU_R* pPdu = reinterpret_cast<ActionResponsePDU_R*>(header);
                     if (base::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                     if (getSiteID() != pPdu->originatingID.simulationID.siteIdentification ||
                        getApplicationID() != pPdu->originatingID.simulationID.applicationIdentification) {
                           processActionResponsePDU_R(pPdu);
                     }
                  }
                  break;

                  default: {
                     // Note: users will need to do their own byte swapping and checks
                     processUserPDU(header);
                  }
                  break;

               } // PDU switch

            } // if correct exercise
         }  // Inputs enabled

      }  // processing PDUs

      // Read more PDUs
      j0 = 0;
      while ( (j0 < MAX_PDUs) && (recvData(reinterpret_cast<char*>(&inputBuffer[j0]), MAX_PDU_SIZE) > 0) ) {
         j0++;
      }
   }

}

//------------------------------------------------------------------------------
// processInputList() -- Update players/systems from the Input-list
//------------------------------------------------------------------------------
void NetIO::processInputList()
{
   for (unsigned int idx = 0; idx < getInputListSize(); idx++) {
      Nib* nib = static_cast<Nib*>(getInputNib(idx));
      if (nib != nullptr) nib->updateTheIPlayer();
   }

//   std::cout << "n = " << getInputListSize();      // #DPG#
//   base::PairStream* p = getSimulation()->getPlayers();
//   if (p != 0) {
//      std::cout << ";  np = " << p->entries();
//      p->unref();
//   }
//   std::cout << std::endl;
}

//------------------------------------------------------------------------------
// processSignalPDU() callback --
//------------------------------------------------------------------------------
bool NetIO::processSignalPDU(const SignalPDU* const)
{
   return true;
}

//------------------------------------------------------------------------------
// processTransmitterPDU() callback --
//------------------------------------------------------------------------------
bool NetIO::processTransmitterPDU(const TransmitterPDU* const)
{
   return true;
}

//------------------------------------------------------------------------------
// processDataQueryPDU() callback --
//------------------------------------------------------------------------------
bool NetIO::processDataQueryPDU(const DataQueryPDU* const)
{
   return true;
}

//------------------------------------------------------------------------------
// processDataPDU() callback --
//------------------------------------------------------------------------------
bool NetIO::processDataPDU(const DataPDU* const)
{
   return true;
}

//------------------------------------------------------------------------------
// processCommentPDU() callback --
//------------------------------------------------------------------------------
bool NetIO::processCommentPDU(const CommentPDU* const)
{
   return true;
}

//------------------------------------------------------------------------------
// processStartPDU() callback --
//------------------------------------------------------------------------------
bool NetIO::processStartPDU(const oe::dis::StartPDU* const)
{
    return true;
}

//------------------------------------------------------------------------------
// processStopPDU() callback --
//------------------------------------------------------------------------------
bool NetIO::processStopPDU(const oe::dis::StopPDU* const)
{
     return true;
}

//------------------------------------------------------------------------------
// processAcknowledgePDU() callback --
//------------------------------------------------------------------------------
bool NetIO::processAcknowledgePDU(const oe::dis::AcknowledgePDU* const)
{
    return true;
}

//------------------------------------------------------------------------------
// processActionRequestPDU() callback --
//------------------------------------------------------------------------------
bool NetIO::processActionRequestPDU(const oe::dis::ActionRequestPDU* const)
{
    return true;
}

//------------------------------------------------------------------------------
// processActionRequestPDU_R() callback --
//------------------------------------------------------------------------------
bool NetIO::processActionRequestPDU_R(const oe::dis::ActionRequestPDU_R* const)
{
    return true;
}

//------------------------------------------------------------------------------
// processActionResponsePDU_R() callback --
//------------------------------------------------------------------------------
bool NetIO::processActionResponsePDU_R(const oe::dis::ActionResponsePDU_R* const)
{
    return true;
}

//------------------------------------------------------------------------------
// User defined function to process unknown PDUs (PDU bytes are still in network order)
//------------------------------------------------------------------------------
bool NetIO::processUserPDU(const PDUHeader* const)
{
   return true;
}

//------------------------------------------------------------------------------
// nibFactory() -- Create a new Nib
//------------------------------------------------------------------------------
simulation::Nib* NetIO::nibFactory(const simulation::NetIO::IoType ioType)
{
   return new Nib(ioType);
}


//------------------------------------------------------------------------------
// Create a new NIBs
//------------------------------------------------------------------------------

simulation::Nib* NetIO::createNewOutputNib(simulation::Player* const player)
{
   Nib* nib = static_cast<Nib*>(nibFactory(OUTPUT_NIB));
   if (nib != nullptr) {
      nib->setNetIO(this);
      nib->setPlayer(player);
      nib->setPlayerID(player->getID());
      nib->setSide(player->getSide());

      // Default DR: World, No rotation, 2nd order linear
      nib->setDeadReckoning(Nib::FVW_DRM);

      bool ok = true;
      const base::String* fName = getFederateName();
      unsigned short site = getSiteID();
      unsigned short app  = getApplicationID();
      if (player->isNetworkedPlayer()) {
         simulation::Nib* pNib = player->getNib();
         fName = pNib->getFederateName();
         // Mapping another federate name to DIS site and application IDs.
         // Currently using parseFederateName(), but really should have a
         // more robust federate name to site/app ID map list.
         if (fName != nullptr) ok = parseFederateName(&site, &app, *fName);
         else ok = false;
      }
      nib->setFederateName(fName);
      nib->setSiteID( site );
      nib->setApplicationID( app );

      if (ok) {
         // Maps the player type to an output entity type.
         // Note: isEntityTypeValid() will return false if there
         // isn't a type mapper (Ntb) assigned to this Nib.
         nib->setOutputPlayerType(player);

         // Inform the player
         player->setOutgoingNib(nib, getNetworkID());
      }
   }
   return nib;
}

//------------------------------------------------------------------------------
// findDisNib() -- find the NIB that matches ALL IDs.
//------------------------------------------------------------------------------
Nib* NetIO::findDisNib(const unsigned short playerID, const unsigned short site, const unsigned short app, const IoType ioType)
{
   Nib* nib = nullptr;
   char cbuff[32];
   bool ok = makeFederateName(cbuff, 32, site, app);
   if (ok) {
      base::String fname(cbuff);
      nib = dynamic_cast<Nib*>( findNib(playerID, &fname, ioType) );
   }
   return nib;
}


//------------------------------------------------------------------------------
// processElectromagneticEmissionPDU() callback --
//------------------------------------------------------------------------------
void NetIO::processElectromagneticEmissionPDU(const ElectromagneticEmissionPDU* const pdu)
{
    // ---
    // Emitting Player's ID checks
    // ---
    unsigned short ePlayerId = pdu->emittingEntityID.ID;
    unsigned short eSiteId = pdu->emittingEntityID.simulationID.siteIdentification;
    unsigned short eApplicationId = pdu->emittingEntityID.simulationID.applicationIdentification;

    // Ignore our own PDUs
    if (eSiteId == getSiteID() && eApplicationId == getApplicationID()) return;

    // Or PDUs with no systems
    if (pdu->numberOfSystems == 0) return;

    // Or if we have no emission handlers
    if (nEmissionHandlers == 0) return;

    // Find the NIB and IPlayer for the emitting player (they must exist or we're too early)
    Nib* eNib = findDisNib(ePlayerId, eSiteId, eApplicationId, INPUT_NIB);
    if (eNib == nullptr) return;

    // Pass on to the IPlayer's NIB for processing
    eNib->processElectromagneticEmissionPDU(pdu);
}

//------------------------------------------------------------------------------
// recvData() -- receive data packet
//------------------------------------------------------------------------------
int NetIO::recvData(char* const packet, const int maxSize)
{
   int result = 0;
   if (netInput != nullptr) {
      result = netInput->recvData(packet, maxSize);
   }
   return result;
}

//------------------------------------------------------------------------------
// sendData() -- send data packet
//------------------------------------------------------------------------------
bool NetIO::sendData(const char* const packet, const int size)
{
   bool result = 0;
   if (netOutput != nullptr) {
      result = netOutput->sendData( packet, size );
   }
   return result;
}

//------------------------------------------------------------------------------
// makeTimeStamp() -- makes a DIS time stamp
//------------------------------------------------------------------------------
unsigned int NetIO::timeStamp()
{
   unsigned int ts = 0;
   if (getTimeline() == UTC) {
      ts = makeTimeStamp( static_cast<double>(getSimulation()->getSysTimeOfDay()), true );
   }
   else {
      ts = makeTimeStamp( static_cast<double>(getSimulation()->getExecTimeSec()), false );
   }
   return ts;
}

//------------------------------------------------------------------------------
// makeTimeStamp() -- makes a DIS time stamp
//------------------------------------------------------------------------------
unsigned int NetIO::makeTimeStamp(const double ctime, const bool absolute)
{
    // compute seconds in this hour
    const int hours = static_cast<int>(ctime / 3600.0);
    double secondsThisHour = (ctime - static_cast<double>(hours*3600));

    // 31 MSBs are for the 3600 seconds in this hour
    unsigned int ts = static_cast<unsigned int>((secondsThisHour/3600.0) * 0x7fffffff);
    ts = (ts << 1);                    // shift to 31 MSBs
    if (absolute) ts = (ts | 0x01);    // Sets LSB if using absolute time

    return ts;
}


//------------------------------------------------------------------------------
// Generate a federate name from the site and application numbers:
//  "SnnAmm" -- where nn and mm are the site and app numbers.
//------------------------------------------------------------------------------
bool NetIO::makeFederateName(char* const fedName, const unsigned int len, const unsigned short site, const unsigned short app)
{
   static const int p10[5] = { 10000, 1000, 100, 10, 1 };

   bool ok = false;
   if (fedName != nullptr && len > 0 && site > 0 && app > 0) {
      char cbuff[64];
      unsigned int idx = 0;

      // First the site number: Snnn
      cbuff[idx++] = 'S';
      {
         int tmp = site;
         bool digits = false;
         for (unsigned int i = 0; i < 5; i++) {
            int rr = tmp/p10[i];
            if (rr > 0 || digits) {
               cbuff[idx++] = '0' + char(rr);
               digits = true;
            }
            tmp -= (rr * p10[i]);
         }
      }

      // then the application number: Annn
      cbuff[idx++] = 'A';
      {
         int tmp = app;
         bool digits = false;
         for (unsigned int i = 0; i < 5; i++) {
            int rr = tmp/p10[i];
            if (rr > 0 || digits) {
               cbuff[idx++] = '0' + char(rr);
               digits = true;
            }
            tmp -= (rr * p10[i]);
         }
      }

      cbuff[idx++] = 0;

      if (idx <= len) {
         base::lcStrcpy(fedName,len,cbuff);
         ok = true;
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// Parse federate name for the site and application numbers
//  (We're expecting "SnnAmm" where nn and mm are the site and app numbers.)
//------------------------------------------------------------------------------
bool NetIO::parseFederateName(unsigned short* const site, unsigned short* const app, const char* const fedName)
{
   bool ok = false;
   if (site != nullptr && app != nullptr && fedName != nullptr) {
      unsigned short tSite = 0;
      unsigned short tApp = 0;
      unsigned int idx = 0;

      // First check and convert site number
      ok = ( std::toupper(fedName[idx++]) == 'S' );
      if (ok) {
         unsigned int tmp = 0;
         unsigned int cnt = 0;
         while (cnt < 5 && std::isdigit(fedName[idx])) {
            tmp *= 10;
            tmp += static_cast<int>(fedName[idx++] - '0');
            cnt++;
         }
         ok = (tmp > 0) && (tmp <= 0xFFFF);
         if (ok) tSite = static_cast<unsigned short>(tmp);
      }

      // Next check and convert application number
      ok = ( std::toupper(fedName[idx++]) == 'A' );
      if (ok) {
         unsigned int tmp = 0;
         unsigned int cnt = 0;
         while (cnt < 5 && std::isdigit(fedName[idx])) {
            tmp *= 10;
            tmp += static_cast<int>(fedName[idx++] - '0');
            cnt++;
         }
         ok = (tmp > 0) && (tmp <= 0xFFFF);
         if (ok) tApp = static_cast<unsigned short>(tmp);
      }

      // If all is well, send the values back to the user.
      if (ok) {
         *site = tSite;
         *app  = tApp;
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// Generate a federation name from the exercise numbers:
//  "Ennn" -- where nnn is the exercise number, which must be greater than zero
//------------------------------------------------------------------------------
bool NetIO::makeFederationName(char* const fedName, const unsigned int len, const unsigned short exercise)
{
   static const int p10[5] = { 10000, 1000, 100, 10, 1 };

   bool ok = false;
   if (fedName != nullptr && len > 0 && exercise > 0) {
      char cbuff[64];
      unsigned int idx = 0;

      cbuff[idx++] = 'E';
      {
         int tmp = exercise;
         bool digits = false;
         for (unsigned int i = 0; i < 5; i++) {
            int rr = tmp/p10[i];
            if (rr > 0 || digits) {
               cbuff[idx++] = '0' + char(rr);
               digits = true;
            }
            tmp -= (rr * p10[i]);
         }
      }

      cbuff[idx++] = 0;

      if (idx <= len) {
         base::lcStrcpy(fedName,len,cbuff);
         ok = true;
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// Parse federation name for the exercise number
//  (We're expecting "Ennn" where nnn is the exercise.)
//------------------------------------------------------------------------------
bool NetIO::parseFederationName(unsigned short* const exercise, const char* const fedName)
{
   bool ok = false;
   if (exercise != nullptr && fedName != nullptr) {
      unsigned short tExercise = 0;
      unsigned int idx = 0;

      ok = ( std::toupper(fedName[idx++]) == 'E' );
      if (ok) {
         unsigned int tmp = 0;
         unsigned int cnt = 0;
         while (cnt < 5 && std::isdigit(fedName[idx])) {
            tmp *= 10;
            tmp += static_cast<int>(fedName[idx++] - '0');
            cnt++;
         }
         ok = (tmp > 0) && (tmp <= 0xFFFF);
         if (ok) tExercise = static_cast<unsigned short>(tmp);
      }

      // If all is well, send the value back to the user.
      if (ok) {
         *exercise = tExercise;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Finds the Ntm by DIS entity type codes
//------------------------------------------------------------------------------
const dis::Ntm* NetIO::findNtmByTypeCodes(
         const unsigned char  kind,
         const unsigned char  domain,
         const unsigned short countryCode,
         const unsigned char  category,
         const unsigned char  subcategory,
         const unsigned char  specific,
         const unsigned char  extra
      ) const
{
   const dis::Ntm* result = nullptr;

   const dis::NtmInputNode* disRoot = dynamic_cast<const dis::NtmInputNode*>( getRootNtmInputNode() );
   if (disRoot != nullptr) {

      result = disRoot->findNtmByTypeCodes(kind, domain, countryCode, category, subcategory, specific, extra);

   }
   return result;
}

//------------------------------------------------------------------------------
// Data access (get) routines
//------------------------------------------------------------------------------

double NetIO::getMaxEntityRange(const simulation::Nib* const nib) const
{
   double value = 0;
   if (nib != nullptr) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != nullptr) {
         const unsigned char k = disNib->getEntityKind();
         const unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxEntityRange[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxEntityRange(nib);
      }
   }
   return value;
}

double NetIO::getMaxEntityRangeSquared(const simulation::Nib* const nib) const
{
   double value = 0;
   if (nib != nullptr) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != nullptr) {
         const unsigned char k = disNib->getEntityKind();
         const unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxEntityRange2[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxEntityRangeSquared(nib);
      }
   }
   return value;
}

double NetIO::getMaxTimeDR(const simulation::Nib* const nib) const
{
   double value = 0;
   if (nib != nullptr) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != nullptr) {
         const unsigned char k = disNib->getEntityKind();
         const unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxTimeDR[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxTimeDR(nib);
      }
   }
   return value;
}

double NetIO::getMaxPositionErr(const simulation::Nib* const nib) const
{
   double value = 0;
   if (nib != nullptr) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != nullptr) {
         const unsigned char k = disNib->getEntityKind();
         const unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxPositionErr[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxPositionErr(nib);
      }
   }
   return value;
}

double NetIO::getMaxOrientationErr(const simulation::Nib* const nib) const
{
   double value = 0;
   if (nib != nullptr) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != nullptr) {
         const unsigned char k = disNib->getEntityKind();
         const unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxOrientationErr[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxOrientationErr(nib);
      }
   }
   return value;
}

double NetIO::getMaxAge(const simulation::Nib* const nib) const
{
   double value = 0;
   if (nib != nullptr) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != nullptr) {
         const unsigned char k = disNib->getEntityKind();
         const unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxAge[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxAge(nib);
      }
   }
   return value;
}

//------------------------------------------------------------------------------
// Data set routines
//------------------------------------------------------------------------------

// Defines our NetIO federate name based on the site and application numbers
void NetIO::defineFederateName()
{
   if (siteID > 0 && appID > 0) {
      char cbuff[32];
      bool ok = makeFederateName(cbuff, 32, siteID, appID);
      if (ok) {
         base::String* p = new base::String(cbuff);
         setFederateName(p);
         p->unref();
      }
   }
   else {
      setFederateName(nullptr);
   }
}

// Defines our NetIO federation name based on the exercise number
void NetIO::defineFederationName()
{
   if (exerciseID > 0) {
      char cbuff[32];
      bool ok = makeFederationName(cbuff, 32, exerciseID);
      if (ok) {
         base::String* p = new base::String(cbuff);
         setFederationName(p);
         p->unref();
      }
   }
   else {
      setFederationName(nullptr);
   }
}

// Sets the network's site ID
bool NetIO::setSiteID(const unsigned short v)
{
    siteID = v;
    defineFederateName();
    return true;
}

// Sets the network's application ID
bool NetIO::setApplicationID(const unsigned short v)
{
    appID = v;
    defineFederateName();
    return true;
}

// Sets the network's exercise ID
bool NetIO::setExerciseID(const unsigned char v)
{
    exerciseID = v;
    defineFederationName();
    return true;
}

// setMaxEntityRange() -- Sets max entity range (meters)
bool NetIO::setMaxEntityRange(const double v, const unsigned char kind, const unsigned char domain)
{
   // default loop limits (just in case we're doing all)
   unsigned char imin = 0;
   unsigned char imax = NUM_ENTITY_KINDS;
   unsigned char jmin = 0;
   unsigned char jmax = MAX_ENTITY_DOMAINS;

   // Clamp i to kind (if valid)
   if (kind < NUM_ENTITY_KINDS) {
      imin = kind;
      imax = kind+1;
   }

   // Clamp j to domain (if valid)
   if (domain < MAX_ENTITY_DOMAINS) {
      jmin = domain;
      jmax = domain+1;
   }

   // Fill as needed
   for (unsigned char i = imin; i < imax; i++) {
      for (unsigned char j = jmin; j < jmax; j++) {
         maxEntityRange[i][j] = v;
         maxEntityRange2[i][j] = (v*v);
      }
   }
   return true;
}

// setMaxTimeDR() -- Sets max DR Time (seconds)
bool NetIO::setMaxTimeDR(const double v, const unsigned char kind, const unsigned char domain)
{
   // default loop limits (just in case we're doing all)
   unsigned char imin = 0;
   unsigned char imax = NUM_ENTITY_KINDS;
   unsigned char jmin = 0;
   unsigned char jmax = MAX_ENTITY_DOMAINS;

   // Clamp i to kind (if valid)
   if (kind < NUM_ENTITY_KINDS) {
      imin = kind;
      imax = kind+1;
   }

   // Clamp j to domain (if valid)
   if (domain < MAX_ENTITY_DOMAINS) {
      jmin = domain;
      jmax = domain+1;
   }

   // Fill as needed
   for (unsigned char i = imin; i < imax; i++) {
      for (unsigned char j = jmin; j < jmax; j++) {
         maxTimeDR[i][j] = v;
      }
   }
   return true;
}

// setMaxPositionErr() -- Sets max position error (meters)
bool NetIO::setMaxPositionErr(const double v, const unsigned char kind, const unsigned char domain)
{
   // default loop limits (just in case we're doing all)
   unsigned char imin = 0;
   unsigned char imax = NUM_ENTITY_KINDS;
   unsigned char jmin = 0;
   unsigned char jmax = MAX_ENTITY_DOMAINS;

   // Clamp i to kind (if valid)
   if (kind < NUM_ENTITY_KINDS) {
      imin = kind;
      imax = kind+1;
   }

   // Clamp j to domain (if valid)
   if (domain < MAX_ENTITY_DOMAINS) {
      jmin = domain;
      jmax = domain+1;
   }

   // Fill as needed
   for (unsigned char i = imin; i < imax; i++) {
      for (unsigned char j = jmin; j < jmax; j++) {
         maxPositionErr[i][j] = v;
      }
   }
   return true;
}

// setMaxOrientationErr() -- Sets max orientation error (radians)
bool NetIO::setMaxOrientationErr(const double v, const unsigned char kind, const unsigned char domain)
{
   // default loop limits (just in case we're doing all)
   unsigned char imin = 0;
   unsigned char imax = NUM_ENTITY_KINDS;
   unsigned char jmin = 0;
   unsigned char jmax = MAX_ENTITY_DOMAINS;

   // Clamp i to kind (if valid)
   if (kind < NUM_ENTITY_KINDS) {
      imin = kind;
      imax = kind+1;
   }

   // Clamp j to domain (if valid)
   if (domain < MAX_ENTITY_DOMAINS) {
      jmin = domain;
      jmax = domain+1;
   }

   // Fill as needed
   for (unsigned char i = imin; i < imax; i++) {
      for (unsigned char j = jmin; j < jmax; j++) {
         maxOrientationErr[i][j] = v;
      }
   }
   return true;
}

// Sets max age (seconds)
bool NetIO::setMaxAge(const double v, const unsigned char kind, const unsigned char domain)
{
   // default loop limits (just in case we're doing all)
   unsigned char imin = 0;
   unsigned char imax = NUM_ENTITY_KINDS;
   unsigned char jmin = 0;
   unsigned char jmax = MAX_ENTITY_DOMAINS;

   // Clamp i to kind (if valid)
   if (kind < NUM_ENTITY_KINDS) {
      imin = kind;
      imax = kind+1;
   }

   // Clamp j to domain (if valid)
   if (domain < MAX_ENTITY_DOMAINS) {
      jmin = domain;
      jmax = domain+1;
   }

   // Fill as needed
   for (unsigned char i = imin; i < imax; i++) {
      for (unsigned char j = jmin; j < jmax; j++) {
         maxAge[i][j] = v;
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Set DR parameters
//------------------------------------------------------------------------------

// Sets max entity range for this entity kind/domain
bool NetIO::setMaxEntityRange(const base::Distance* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != nullptr) {
        base::Meters ref;
        double meters = ref.convert(*p);
        ok = setMaxEntityRange(meters, kind, domain);
    }
    return ok;
}

// Sets max DR time for this entity kind/domain
bool NetIO::setMaxTimeDR(const base::Time* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != nullptr) {
        base::Seconds ref;
        double sec = ref.convert(*p);
        ok = setMaxTimeDR(sec, kind, domain);
    }
    return ok;
}

// Sets max position error for this entity kind/domain
bool NetIO::setMaxPositionErr(const base::Distance* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != nullptr) {
        base::Meters ref;
        double meters = ref.convert(*p);
        ok = setMaxPositionErr(meters, kind, domain);
    }
    return ok;
}

// Sets max orientation error for this entity kind/domain
bool NetIO::setMaxOrientationErr(const base::Angle* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != nullptr) {
        base::Radians ref;
        double radians = static_cast<double>(ref.convert(*p));
        ok = setMaxOrientationErr(radians, kind, domain);
    }
    return ok;
}

// Sets max age (without update) of a networked player of this entity kind/domain
bool NetIO::setMaxAge(const base::Time* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != nullptr) {
        base::Seconds ref;
        double sec = ref.convert(*p);
        ok = setMaxAge(sec, kind, domain);
    }
    return ok;
}

// DISv7 additions
double NetIO::getHbtPduEe() const         { return HBT_PDU_EE; }
double NetIO::getHbtTimeoutMplier() const { return HBT_TIMEOUT_MPLIER; }
double NetIO::getEeAzThrsh() const        { return EE_AZ_THRSH; }
double NetIO::getEeElThrsh() const        { return EE_EL_THRSH; }

double NetIO::getEeErpThrsh() const       { return EE_ERP_THRSH; }
double NetIO::getEeFreqThrsh() const      { return EE_FREQ_THRSH; }
double NetIO::getEeFrngThrsh() const      { return EE_FRNG_THRSH; }
double NetIO::getEePrfThrsh() const       { return EE_PRF_THRSH; }
double NetIO::getEePwThrsh() const        { return EE_PW_THRSH; }

//unsigned int NetIO::get_EE_HIGH_DENSITY_THRSH(void) const { return EE_HIGH_DENSITY_THRSH; }


// Adds an item to the emission PDU handler table
void NetIO::addEmissionPduHandler(const EmissionPduHandler* const item)
{
   if (nEmissionHandlers < MAX_EMISSION_HANDLERS) {
      item->ref();
      emissionHandlers[nEmissionHandlers] = item;
      nEmissionHandlers++;
   }
}

// Clears the emission PDU handler table
void NetIO::clearEmissionPduHandlers()
{
   // Clear our emission PDU handler table --
   // Just in case someone is scanning the table, we clear the entries
   // and decrement 'nEmissionHandlers' before the object is unref()'ed
   while (nEmissionHandlers > 0) {
      nEmissionHandlers--;
      emissionHandlers[nEmissionHandlers]->unref();
      emissionHandlers[nEmissionHandlers] = nullptr;
   }
}


//------------------------------------------------------------------------------
// Find Emission PDU handlers
//------------------------------------------------------------------------------

// By RfSensor data
const EmissionPduHandler* NetIO::findEmissionPduHandler(const simulation::RfSensor* const msg)
{
   const EmissionPduHandler* handler = nullptr;
   if (msg != nullptr && nEmissionHandlers > 0) {
      // Try to find one with a matching R/F sensor ...
      for (unsigned int i = 0; i < nEmissionHandlers && handler == nullptr; i++) {
         if (emissionHandlers[i]->isMatchingRfSystemType(msg)) {
            handler = emissionHandlers[i];
         }
      }
      // If not found, try to find a default outgoing handler
      for (unsigned int i = 0; i < nEmissionHandlers && handler == nullptr; i++) {
         if (emissionHandlers[i]->isDefaultOutgoingHandler()) {
            handler = emissionHandlers[i];
         }
      }
   }
   return handler;
}

// By Emission System PDU data
const EmissionPduHandler* NetIO::findEmissionPduHandler(const EmissionSystem* const msg)
{
   const EmissionPduHandler* handler = nullptr;
   if (msg != nullptr && nEmissionHandlers > 0) {
      // Try to find one with a matching emitter name
      for (unsigned int i = 0; i < nEmissionHandlers && handler == nullptr; i++) {
         if (emissionHandlers[i]->isMatchingRfSystemType(msg)) {
            handler = emissionHandlers[i];
         }
      }
      // If not found, try to find a default incoming handler
      for (unsigned int i = 0; i < nEmissionHandlers && handler == nullptr; i++) {
         if (emissionHandlers[i]->isDefaultIncomingHandler()) {
            handler = emissionHandlers[i];
         }
      }
   }
   return handler;
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

// Sets our federate name (override NetIO)
bool NetIO::setSlotFederateName(const base::String* const)
{
   std::cerr << "NetIO::setSlotFederateName() -- With NetIO, use the siteID and applicationID slots to create the federate name" << std::endl;
   return false;
}

// Sets our federation name (override NetIO)
bool NetIO::setSlotFederationName(const base::String* const)
{
   std::cerr << "NetIO::setSlotFederationName() -- With NetIO, use the exerciseID slot to create the federation name" << std::endl;
   return false;
}

// Set Network Input Handler
bool NetIO::setSlotNetInput(base::NetHandler* const msg)
{
    netInput = msg;
    return true;
}

// Net Network Output Handler
bool NetIO::setSlotNetOutput(base::NetHandler* const msg)
{
    netOutput = msg;
    return true;
}

// DIS version number [ 0 .. 6 ] (IST-CF-03-01, May 5, 2003)
bool NetIO::setSlotVersion(const base::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        int v = num->getInt();
        if (v >= 0 && v < VERSION_MAX) {
            setVersion(static_cast<unsigned char>(v));
            ok = true;
        }
        else {
            std::cerr << "NetIO::setSlotVersion() Invalid DIS version number: range: [ 0 ... " << (VERSION_MAX-1) << " ]" << std::endl;
        }
    }
    return ok;
}

// Sets the maximum range for pairs of entities by kind/domain
bool NetIO::setSlotMaxEntityRange(const base::PairStream* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const base::List::Item* item = msg->getFirstItem();
      while (item != nullptr) {

            // get the slot and object from the pair
            const base::Pair* p = static_cast<const base::Pair*>(item->getValue());
            const char* const slotname = *p->slot();
            const base::Distance* pp = dynamic_cast<const base::Distance*>( p->object() );

            if (pp != nullptr) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everything is valid, so let setMaxEntityRange() handle it
                  ok = setMaxEntityRange(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxEntityRange(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxEntityRange(): slot: " << slotname << " is not a valid base::Distance!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}

// Sets the maximum range for all entity types
bool NetIO::setSlotMaxEntityRange(const base::Distance* const msg)
{
   return setMaxEntityRange(msg, 255, 255);
}

// Sets max DR times for pairs of entities by kind/domain
bool NetIO::setSlotMaxTimeDR(const base::PairStream* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const base::List::Item* item = msg->getFirstItem();
      while (item != nullptr) {

            // get the slot and object from the pair
            const base::Pair* p = static_cast<const base::Pair*>(item->getValue());
            const char* const slotname = *p->slot();
            const base::Time* pp = dynamic_cast<const base::Time*>( p->object() );

            if (pp != nullptr) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everything is valid, so let setMaxTimeDR() handle it
                  ok = setMaxTimeDR(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxTimeDR(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxTimeDR(): slot: " << slotname << " is not a valid base::Time!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}


// Sets max DR times for all entity types
bool NetIO::setSlotMaxTimeDR(const base::Time* const msg)
{
   return setMaxTimeDR(msg, 255, 255);
}


// Sets max position errors for pairs of entities by kind/domain
bool NetIO::setSlotMaxPositionErr(const base::PairStream* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const base::List::Item* item = msg->getFirstItem();
      while (item != nullptr) {

            // get the slot and object from the pair
            const base::Pair* p = static_cast<const base::Pair*>(item->getValue());
            const char* const slotname = *p->slot();
            const base::Distance* pp = dynamic_cast<const base::Distance*>( p->object() );

            if (pp != nullptr) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everything is valid, so let setMaxPositionErr() handle it
                  ok = setMaxPositionErr(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxPositionErr(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxPositionErr(): slot: " << slotname << " is not a valid base::Distance!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}

// Sets max position errors for all entity types
bool NetIO::setSlotMaxPositionErr(const base::Distance* const msg)
{
   return setMaxPositionErr(msg, 255, 255);
}

// Sets max orientation errors for pairs of entities by kind/domain
bool NetIO::setSlotMaxOrientationErr(const base::PairStream* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const base::List::Item* item = msg->getFirstItem();
      while (item != nullptr) {

            // get the slot and object from the pair
            const base::Pair* p = static_cast<const base::Pair*>(item->getValue());
            const char* const slotname = *p->slot();
            const base::Angle* pp = dynamic_cast<const base::Angle*>( p->object() );

            if (pp != nullptr) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everything is valid, so let setMaxOrientationErr() handle it
                  ok = setMaxOrientationErr(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxOrientationErr(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxOrientationErr(): slot: " << slotname << " is not a valid base::Angle!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}

// Sets max orientation errors for all entity types
bool NetIO::setSlotMaxOrientationErr(const base::Angle* const msg)
{
   return setMaxOrientationErr(msg, 255, 255);
}


// Sets max ages (without update) of for pairs of networked entities by kind/domain
bool NetIO::setSlotMaxAge(const base::PairStream* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const base::List::Item* item = msg->getFirstItem();
      while (item != nullptr) {

            // get the slot and object from the pair
            const base::Pair* p = static_cast<const base::Pair*>(item->getValue());
            const char* const slotname = *p->slot();
            const base::Time* pp = dynamic_cast<const base::Time*>( p->object() );

            if (pp != nullptr) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everything is valid, so let setMaxAge() handle it
                  ok = setMaxAge(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxAge(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxAge(): slot: " << slotname << " is not a valid base::Time!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}

// Sets max ages (without update) of all entity types
bool NetIO::setSlotMaxAge(const base::Time* const msg)
{
   return setMaxAge(msg, 255, 255);
}

// Sets the list of Electromagnetic Emission PDU handlers
bool NetIO::setSlotEmissionPduHandlers(base::PairStream* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       // First clear the old table
       clearEmissionPduHandlers();

       // Now scan the pair stream and put all Ntm objects into the table.
       base::List::Item* item = msg->getFirstItem();
       while (item != nullptr && nEmissionHandlers < MAX_EMISSION_HANDLERS) {
          base::Pair* pair = static_cast<base::Pair*>(item->getValue());
          EmissionPduHandler* handler = dynamic_cast<EmissionPduHandler*>( pair->object() );
          if (handler != nullptr) {
             // We have an Ntm object, so put it in the table
             addEmissionPduHandler(handler);
          }
          item = item->getNext();
       }
       ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// slot2KB() -- converts a slotname string to kind and domain numbers.
//  Valid formats: "Kn" and "KnDnn"
//  Where "Kn" is the entity kind number [0 .. 9],
//   and  "Dnn" is an optional entity domain number [0 .. 11]
//  Examples: k3  K1D2  k2d5
//  Returns true if a valid kind, with optional domain, was found
//------------------------------------------------------------------------------
bool NetIO::slot2KD(const char* const slotname, unsigned char* const kind, unsigned char* const domain)
{
   bool ok = false;
   if (slotname != nullptr && kind != nullptr) {
      size_t len = std::strlen(slotname);
      bool haveTheK = (len > 0 && (slotname[0] == 'K' || slotname[0] == 'k') );
      bool haveTheD = (len > 2 && (slotname[2] == 'D' || slotname[2] == 'd') );
      if (haveTheK && std::isdigit(slotname[1]) && (len == 2 || haveTheD)) {
         // Ok, we have a single digit (kind)
         unsigned char k = static_cast<unsigned char>(slotname[1] - '0');
         if (k < NUM_ENTITY_KINDS) {
            // We have a valid kind value!
            if (haveTheD) {
               // We have a single digit (kind) followed by a 'D',
               // so we should have a domain value ...
               bool isNum = true;
               for (int i = 3; isNum && slotname[i] != '\0'; i++) {
                  if ( !std::isdigit(slotname[i]) ) isNum = false;
               }
               if (isNum) {
                  int d = std::atoi(&slotname[3]);
                  if (d >= 0 && d < MAX_ENTITY_DOMAINS) {
                     // At this point we have both kind and domain numbers
                     *kind = k;
                     *domain = static_cast<unsigned char>(d);
                     ok = true;
                  }
               }
            }
            else {
               // We have only the kind number, but that's OK
               *kind = k;
               ok = true;
            }
         }
      }
   }
   return ok;
}

// Set Site ID
bool NetIO::setSlotSiteID(const base::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        int v = num->getInt();
        if (v >= 0 && v <= 65535) {
            ok = setSiteID(static_cast<unsigned short>(v));
        }
        else {
            std::cerr << "NetIO::setSlotSiteID(): invalid number(" << v << "); valid range:[0 ... 65535]" << std::endl;
        }
    }
    return ok;
}

// Set Application ID
bool NetIO::setSlotApplicationID(const base::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        int v = num->getInt();
        if (v >= 0 && v <= 65535) {
            ok = setApplicationID(static_cast<unsigned short>(v));
        }
        else {
            std::cerr << "NetIO::setSlotApplicationID(): invalid number(" << v << "); valid range:[0 ... 65535]" << std::endl;
        }
    }
    return ok;
}

// Set Exercise ID
bool NetIO::setSlotExerciseID(const base::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        int v = num->getInt();
        if (v >= 0 && v <= 255) {
            ok = setExerciseID(static_cast<unsigned char>(v));
        }
        else {
            std::cerr << "NetIO::setSlotExerciseID(): invalid number(" << v << "); valid range:[0 ... 255]" << std::endl;
        }
    }
    return ok;
}
//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* NetIO::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& NetIO::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }


    // Network Input Handler
    if (netInput != nullptr) {
        indent(sout,i+j);
        sout << "netInput: ";
        netInput->serialize(sout,(i+j+4),true);
    }

    // Network Output Handler
    if (netOutput != nullptr) {
        indent(sout,i+j);
        sout << "netOutput: ";
        netOutput->serialize(sout,(i+j+4),true);
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
// Test quick lookup of incoming entity types
//------------------------------------------------------------------------------
void NetIO::testInputEntityTypes(const unsigned int n)
{
   const NtmInputNode* root = getRootNtmInputNode();
   const unsigned int maxTypes = getNumInputEntityTypes();
   if (n > 0 && root != nullptr && maxTypes > 0) {
      for (unsigned int i = 0; i < n; i++) {
         int r = std::rand();
         double nr = (static_cast<double>(r) / static_cast<double>(RAND_MAX));
         int idx = base::nint(nr * (maxTypes - 1));
         const Ntm* origNtm = static_cast<const Ntm*>(getInputEntityType(idx));
         std::cout << "i= " << i;
         std::cout << "; idx= " << idx;
         std::cout << "; origNtm= " << origNtm;
         if (origNtm != nullptr) {

            std::cout << "; [ ";
            std::cout << static_cast<int>(origNtm->getEntityKind()) << ",";
            std::cout << static_cast<int>(origNtm->getEntityDomain()) << ",";
            std::cout << static_cast<int>(origNtm->getEntityCountry()) << ",";
            std::cout << static_cast<int>(origNtm->getEntityCategory()) << ",";
            std::cout << static_cast<int>(origNtm->getEntitySubcategory()) << ",";
            std::cout << static_cast<int>(origNtm->getEntitySpecific()) << ",";
            std::cout << static_cast<int>(origNtm->getEntityExtra()) << ")";
            std::cout << " ]";

            const Ntm* foundNtm = findNtmByTypeCodes(
                  origNtm->getEntityKind(),
                  origNtm->getEntityDomain(),
                  origNtm->getEntityCountry(),
                  origNtm->getEntityCategory(),
                  origNtm->getEntitySubcategory(),
                  origNtm->getEntitySpecific(),
                  origNtm->getEntityExtra()
               );

            std::cout << "; foundNtm= " << foundNtm;
            if (foundNtm != nullptr) {
               const simulation::Player* foundP = origNtm->getTemplatePlayer();
               std::cout << "; form: " << foundP->getFactoryName();
               base::safe_ptr<const base::String> foundType( static_cast<const base::String*>( foundP->getType() ) );
               if (foundType != nullptr) std::cout << "; type: " << *foundType;
            }
            if (origNtm == foundNtm) {
               std::cout << "; Match!!";
            }
            else {
               std::cout << "; NO match!!";
            }
         }

         std::cout << std::endl;
      }
   }
}

//------------------------------------------------------------------------------
// Test quick lookup of outgoing entity types -- this routine is used to
// test the quick lookup tree.  We do 'n' quick lookups by doing a random
// draw of a Ntm from the main NTM list, getting and cloning the template
// player, optionally modifying the type string, and doing a lookup.
//------------------------------------------------------------------------------
void NetIO::testOutputEntityTypes(const unsigned int n)
{
   const NtmOutputNode* root = getRootNtmOutputNode();
   const unsigned int maxTypes = getNumOutputEntityTypes();
   if (n > 0 && root != nullptr && maxTypes > 0) {
      for (unsigned int i = 0; i < n; i++) {
         int r = std::rand();
         double nr = static_cast<double>(r) / static_cast<double>(RAND_MAX);
         int idx = base::nint(nr * (maxTypes - 1));
         const Ntm* origNtm = static_cast<const Ntm*>(getOutputEntityTypes(idx));
         std::cout << "i= " << i;
         std::cout << "; idx= " << idx;
         std::cout << "; origNtm= " << origNtm;
         if (origNtm != nullptr) {

            const simulation::Player* origP = origNtm->getTemplatePlayer();
            simulation::Player* origP1 = origP->clone();

            std::cout << "; form: " << origP->getFactoryName();
            base::safe_ptr<base::String> origType( (base::String*) origP->getType() );
            if (origType != nullptr) {

               char cbuff[64];
               base::lcStrcpy(cbuff, 64, origType->getString());

#if 0 /* optionally increment the last character to look for generic matches */
               size_t ll = std::strlen(cbuff);
               if (ll > 1) {
                  cbuff[ll-1]++;
               }
#endif

               base::String* newType = new base::String(cbuff);
               origP1->setType(newType);

               base::String* origType1 = const_cast<base::String*>(static_cast<const base::String*>(origP1->getType()));
               std::cout << "; type1: " << *origType1;
            }

            const Ntm* foundNtm = static_cast<const Ntm*>(root->findNetworkTypeMapper(origP1));
            std::cout << "; foundNtm= " << foundNtm;
            if (foundNtm != nullptr) {
               std::cout << "; [ ";
               std::cout << static_cast<int>(foundNtm->getEntityKind()) << ",";
               std::cout << static_cast<int>(foundNtm->getEntityDomain()) << ",";
               std::cout << static_cast<int>(foundNtm->getEntityCountry()) << ",";
               std::cout << static_cast<int>(foundNtm->getEntityCategory()) << ",";
               std::cout << static_cast<int>(foundNtm->getEntitySubcategory()) << ",";
               std::cout << static_cast<int>(foundNtm->getEntitySpecific()) << ",";
               std::cout << static_cast<int>(foundNtm->getEntityExtra()) << ")";
               std::cout << " ]";
            }
            if (origNtm == foundNtm) {
               std::cout << "; Match!!";
            }
            else {
               std::cout << "; NO match!!";
            }
         }
         std::cout << std::endl;
      }
   }
}

//==============================================================================
// Class: dis::NtmInputNode
// Description: DIS incoming NTM node
//==============================================================================

IMPLEMENT_SUBCLASS(NtmInputNode,"NtmInputNode")
EMPTY_SLOTTABLE(NtmInputNode)
EMPTY_SERIALIZER(NtmInputNode)

//------------------------------------------------------------------------------
// root incoming NTM node factory
//------------------------------------------------------------------------------
simulation::NetIO::NtmInputNode* NetIO::rootNtmInputNodeFactory() const
{
   return new dis::NtmInputNode(dis::NtmInputNode::ROOT_LVL,0); // root level
}

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
NtmInputNode::NtmInputNode(const unsigned int l, const unsigned int c, const Ntm* ntm)
   : level(l), code(c), ourNtm(nullptr), subnodeList(nullptr)
{
   STANDARD_CONSTRUCTOR()

   if (ntm != nullptr) {
      ourNtm = ntm;
      ourNtm->ref();
   }
   subnodeList = new base::List();
}

void NtmInputNode::copyData(const NtmInputNode& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      ourNtm = nullptr;
      subnodeList = nullptr;
   }

   level = org.level;
   code = org.code;

   if (ourNtm != nullptr) {
      ourNtm->unref();
      ourNtm = nullptr;
   }
   if (org.ourNtm != nullptr) {
      ourNtm = org.ourNtm->clone();
   }

   if (subnodeList != nullptr) {
      subnodeList->unref();
      subnodeList = nullptr;
   }
   if (org.subnodeList != nullptr) {
      subnodeList = org.subnodeList->clone();
   }
}

void NtmInputNode::deleteData()
{
   if (ourNtm != nullptr) {
      ourNtm->unref();
      ourNtm = nullptr;
   }

   if (subnodeList != nullptr) {
      subnodeList->unref();
      subnodeList = nullptr;
   }
}

//------------------------------------------------------------------------------
// Find the NTM based on the incoming entity type codes in the NIB
//------------------------------------------------------------------------------
const simulation::Ntm* NtmInputNode::findNetworkTypeMapper(const simulation::Nib* const nib) const
{
   const simulation::Ntm* result = nullptr;

   const dis::Nib* disNib = dynamic_cast<const dis::Nib*>( nib );
   if (disNib != nullptr) {
      result = findNtmByTypeCodes(
            disNib->getEntityKind(),
            disNib->getEntityDomain(),
            disNib->getEntityCountry(),
            disNib->getEntityCategory(),
            disNib->getEntitySubcategory(),
            disNib->getEntitySpecific(),
            disNib->getEntityExtra()
         );
   }
   return result;
}

//------------------------------------------------------------------------------
// Find the NTM based on the incoming entity type codes in the NIB
//------------------------------------------------------------------------------
const Ntm* NtmInputNode::findNtmByTypeCodes(
      const unsigned char  kind,
      const unsigned char  domain,
      const unsigned short countryCode,
      const unsigned char  category,
      const unsigned char  subcategory,
      const unsigned char  specific,
      const unsigned char  extra
   ) const
{
   const Ntm* result = nullptr;

   {
      // Yes we have the proper type of NIB ...

      // Make sure that the NIB's code for this level matches our code
      bool match = true;
      switch (level) {
         case ROOT_LVL :         match = true;                   break; // the 'root' node always matches
         case KIND_LVL :         match = (code == kind);         break;
         case DOMAIN_LVL :       match = (code == domain);       break;
         case COUNTRYCODE_LVL :  match = (code == countryCode);  break;
         case CATEGORY_LVL :     match = (code == category);     break;
         case SUBCATEGORY_LVL :  match = (code == subcategory);  break;
         case SPECIFIC_LVL :     match = (code == specific);     break;
         case EXTRA_LVL :        match = (code == extra);        break;
      }

      if (match) {

         // First, if we're not the last 'extra' level then search
         // our subnodes to see if they can find a match
         if (level < EXTRA_LVL) {
            const base::List::Item* item = subnodeList->getFirstItem();
            while (item != nullptr && result == nullptr) {
               const NtmInputNode* subnode = static_cast<const NtmInputNode*>(item->getValue());
               result = subnode->findNtmByTypeCodes(kind, domain, countryCode, category, subcategory, specific, extra);
               item = item->getNext();
            }
         }

         // Second, we can use our NTM object, but only if we're at the category
         // level or higher (i.e., we must have match at the kind, domain, country
         // code and category levels)
         if (result == nullptr && level >= CATEGORY_LVL) {
            result = ourNtm;
         }
      }

   }

   return result;
}


//------------------------------------------------------------------------------
// Add the NTM to our sublist of nodes.
//------------------------------------------------------------------------------
bool NtmInputNode::add2OurLists(simulation::Ntm* const ntm)
{
   bool ok = false;

   // Make sure we have the correct kind of NTM ...
   dis::Ntm* disNtm = dynamic_cast<dis::Ntm*>( ntm );
   if (disNtm != nullptr) {

      // Make sure that the NTM's code for this level matches our code
      unsigned int currLevelCode = 0;
      unsigned int nextLevelCode = 0;
      switch (level) {
         case ROOT_LVL : {
            currLevelCode = 0;
            nextLevelCode = disNtm->getEntityKind();
            break;
          }
         case KIND_LVL : {
            currLevelCode = disNtm->getEntityKind();
            nextLevelCode = disNtm->getEntityDomain();
            break;
          }
         case DOMAIN_LVL : {
            currLevelCode = disNtm->getEntityDomain();
            nextLevelCode = disNtm->getEntityCountry();
            break;
          }
         case COUNTRYCODE_LVL : {
            currLevelCode = disNtm->getEntityCountry();
            nextLevelCode = disNtm->getEntityCategory();
            break;
          }
         case CATEGORY_LVL : {
            currLevelCode = disNtm->getEntityCategory();
            nextLevelCode = disNtm->getEntitySubcategory();
            break;
          }
         case SUBCATEGORY_LVL : {
            currLevelCode = disNtm->getEntitySubcategory();
            nextLevelCode = disNtm->getEntitySpecific();
            break;
          }
         case SPECIFIC_LVL : {
            currLevelCode = disNtm->getEntitySpecific();
            nextLevelCode = disNtm->getEntityExtra();
            break;
          }
         case EXTRA_LVL : {
            currLevelCode = disNtm->getEntityExtra();
            nextLevelCode = 0;
            break;
          }
      }

      // Does our code match the NIB's entity type code for this level?
      // And the 'root' node always matches.
      bool match = (code == currLevelCode) || (level == ROOT_LVL);

      if (match) {
         bool err = false;

         // Case #1; if we're at the 'category' level or above, and all remaining codes are
         // zero, then this becomes a wild card terminal node.
         {
            bool wild = (level >= CATEGORY_LVL);

            if (wild && level < EXTRA_LVL)        wild = (disNtm->getEntityExtra() == 0);
            if (wild && level < SPECIFIC_LVL)     wild = (disNtm->getEntitySpecific() == 0);
            if (wild && level < SUBCATEGORY_LVL)  wild = (disNtm->getEntitySubcategory() == 0);

            if (wild) {
               // wild card terminal node
               if (ourNtm == nullptr) {
                  ourNtm = disNtm;
                  ourNtm->ref();
                  ok = true;
               }
               else if (isMessageEnabled(MSG_WARNING)) {
                  std::cerr << "Warning: duplicate incoming NTM(";
                  std::cerr << static_cast<int>(disNtm->getEntityKind()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntityDomain()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntityCountry()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntityCategory()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntitySubcategory()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntitySpecific()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntityExtra()) << ")";
                  std::cerr << ", second ignored" << std::endl;
                  err = true;
               }
            }
         }

         // Case #2; if we're at the 'specific' level, then create a terminal node
         // for the Ntm.  The wild card case was handle in case #1.
         if (!ok && !err && level == SPECIFIC_LVL) {

            // make sure the terminal node doesn't already exist.
            bool alreadyExists = false;
            const base::List::Item* item = subnodeList->getFirstItem();
            while (item != nullptr && !alreadyExists) {
               //NtmInputNode* subnode = (NtmInputNode*) item->getValue();
               const NtmInputNode* csubnode = static_cast<const NtmInputNode*>(item->getValue());
               NtmInputNode* subnode = const_cast<NtmInputNode*>(csubnode);
               alreadyExists = (nextLevelCode == subnode->code);
               item = item->getNext();
            }

            if (!alreadyExists) {
               NtmInputNode* newNode = new NtmInputNode( (level+1), nextLevelCode, disNtm );
               subnodeList->put(newNode);
               newNode->unref();   // ref()'d when put into the subnodelist
               ok = true;
            }
            else if (isMessageEnabled(MSG_WARNING)) {
               std::cerr << "Warning: duplicate incoming NTM(";
               std::cerr << static_cast<int>(disNtm->getEntityKind()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntityDomain()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntityCountry()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntityCategory()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntitySubcategory()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntitySpecific()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntityExtra()) << ")";
               std::cerr << ", second ignored" << std::endl;
               err = true;
            }
         }

         // Case #3; if we're at a level less than the 'specific' level, so try
         // to add the NTM to one of our existing subnodes.
         if (!ok && !err && level < SPECIFIC_LVL) {
            const base::List::Item* item = subnodeList->getFirstItem();
            while (item != nullptr && !ok) {
               //NtmInputNode* subnode = (NtmInputNode*) item->getValue();
               const NtmInputNode* csubnode = static_cast<const NtmInputNode*>(item->getValue());
               NtmInputNode* subnode = const_cast<NtmInputNode*>(csubnode);
               if (nextLevelCode == subnode->code) {
                  ok = subnode->add2OurLists(disNtm);
               }
               item = item->getNext();
            }
         }

         // Case #4; We didn't create a terminal node, and the NTM was added to
         // one of our existing subnodes, then create a new subnode for it.
         if (!ok && !err) {
            // Create a new node and add the NTM
            NtmInputNode* newNode = new NtmInputNode( (level+1), nextLevelCode );
            subnodeList->put(newNode);
            ok = newNode->add2OurLists(disNtm);
            newNode->unref();   // ref()'d when put into the subnodelist
         }
      }

   }

   return ok;
}

//------------------------------------------------------------------------------
// print our data and our subnodes
//------------------------------------------------------------------------------
void NtmInputNode::print(std::ostream& sout, const int icnt) const
{
   // Print our node's form name
   indent(sout,icnt);
   sout << "( NtmInputNode: level=" << level << ", code=" << code;
   sout << std::endl;

   // Print our Ntm object
   if (ourNtm != nullptr) {
      ourNtm->serialize(sout, icnt+4);
   }

   // Print our subnodes
   {
      const base::List::Item* item = subnodeList->getFirstItem();
      while (item != nullptr) {
         const NtmInputNode* subnode = static_cast<const NtmInputNode*>(item->getValue());
         subnode->print(sout,icnt+4);
         item = item->getNext();
      }
   }

   indent(sout,icnt);
   sout << ")" << std::endl;
}

}
}
