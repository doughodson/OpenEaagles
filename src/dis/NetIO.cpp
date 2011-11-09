//==============================================================================
// Classes: Dis::NetIO and Dis::NtmInputNode
//==============================================================================

#include "openeaagles/dis/NetIO.h"
#include "openeaagles/dis/Nib.h"
#include "openeaagles/dis/Ntm.h"
#include "openeaagles/dis/EmissionPduHandler.h"
#include "openeaagles/dis/pdu.h"

#include "openeaagles/simulation/Radar.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"

namespace Eaagles {
namespace Network {
namespace Dis {

//==============================================================================
// Class: Dis::NtmInputNode
// Description: DIS incoming NTM class
//==============================================================================

class NtmInputNode : public Simulation::NetIO::NtmInputNode {
   DECLARE_SUBCLASS(NtmInputNode,Simulation::NetIO::NtmInputNode)

public:
   enum { ROOT_LVL, KIND_LVL, DOMAIN_LVL, COUNTRYCODE_LVL,
         CATEGORY_LVL, SUBCATEGORY_LVL, SPECIFIC_LVL, EXTRA_LVL };

public:
   NtmInputNode(const unsigned int level, const unsigned int code, const Ntm* ntm = 0);

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
   virtual const Simulation::Ntm* findNetworkTypeMapper(const Simulation::Nib* const nib) const;
   virtual bool add2OurLists(Simulation::Ntm* const ntm);
   virtual void print(std::ostream& sout, const int icnt) const;

private:
   unsigned int level;        // Level
   unsigned int code;         // Code for this level
   const Ntm* ourNtm;         // Our default NTM
   Basic::List* subnodeList;  // List of NtmInputNode nodes below this level
};


//==============================================================================
// Class: Dis::NetIO
//==============================================================================
IMPLEMENT_SUBCLASS(NetIO,"DisNetIO")

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
const LCreal HRT_BEAT_MPLIER = 2.5;                            //  Multiplier
const LCreal HRT_BEAT_TIMER = 5;                               //  seconds 
const LCreal DRA_POS_THRST_DFLT = 3.0;                         //  meters
const LCreal DRA_ORIENT_THRST_DFLT = (LCreal)(3.0 * PI/180.0); //  radians

// DISv7 default heartbeats   // DRAC-FAB
const LCreal HBT_PDU_EE = 10;                                  //  seconds 
const LCreal HBT_PDU_IFF = 10;                                 //  seconds 
const LCreal HBT_PDU_RECEIVER = 60;                            //  seconds 
const LCreal HBT_PDU_TRANSMITTER = 2;                          //  seconds 
const LCreal HBT_TIMEOUT_MPLIER = 2.4;                         //  Multiplier

// DISv7 default thresholds
const LCreal EE_AZ_THRSH = (LCreal)(1.0 * PI/180.0);            //  radians
const LCreal EE_EL_THRSH = (LCreal)(1.0 * PI/180.0);            //  radians

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(NetIO)
   "netInput",             //  1) Network input handler
   "netOutput",            //  2) Network output handler
   "version",              //  3) DIS version number [ 0 .. 6 ] (IST-CF-03-01, May 5, 2003)
   "maxTimeDR",            //  4: Max DR time                            (Basic::Time)
   "maxPositionError",     //  5: Max DR position error                  (Basic::Distance)
   "maxOrientationError",  //  6: Max DR anglular error                 (Basic::Angle)
   "maxAge",               //  7: Max age (without update) of networked players (Basic::Time)
   "maxEntityRange",       //  8: Max enity range of networked players (Basic::Distance)
   "emissionPduHandlers",  //  9: List of Electromagnetic-Emission PDU handlers (EmissionPduHandler)
   "siteID",               // 10: Site Identification    
   "applicationID",        // 11: Application Identification
   "exerciseID",           // 12: Exercise Identification
END_SLOTTABLE(NetIO)

// Map slot table to handles 
BEGIN_SLOT_MAP(NetIO)
   ON_SLOT(1, setSlotNetInput,         Basic::NetHandler)
   ON_SLOT(2, setSlotNetOutput,        Basic::NetHandler)
   ON_SLOT(3, setSlotVersion,          Basic::Number)

   ON_SLOT(4, setSlotMaxTimeDR,        Basic::Time)
   ON_SLOT(4, setSlotMaxTimeDR,        Basic::PairStream)

   ON_SLOT(5, setSlotMaxPositionErr,   Basic::Distance)
   ON_SLOT(5, setSlotMaxPositionErr,   Basic::PairStream)

   ON_SLOT(6, setSlotMaxOrientationErr, Basic::Angle)
   ON_SLOT(6, setSlotMaxOrientationErr, Basic::PairStream)

   ON_SLOT(7, setSlotMaxAge,           Basic::Time)
   ON_SLOT(7, setSlotMaxAge,           Basic::PairStream)

   ON_SLOT(8, setSlotMaxEntityRange,   Basic::Distance)
   ON_SLOT(8, setSlotMaxEntityRange,   Basic::PairStream)

   ON_SLOT(9, setSlotEmissionPduHandlers, Basic::PairStream)

   ON_SLOT(10, setSlotSiteID,             Basic::Number)
   ON_SLOT(11, setSlotApplicationID,      Basic::Number)
   ON_SLOT(12, setSlotExerciseID,         Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
NetIO::NetIO() : netInput(0), netOutput(0)
{
   STANDARD_CONSTRUCTOR()

   // DIS parameters
   setVersion(VERSION_1278_1A);

   // First the defaults
   setMaxTimeDR(HRT_BEAT_TIMER, 255, 255);                   //  (seconds)
   setMaxPositionErr(DRA_POS_THRST_DFLT, 255, 255);          //  (meters)
   setMaxOrientationErr(DRA_ORIENT_THRST_DFLT, 255, 255);    //  (radians)
   setMaxAge(HRT_BEAT_MPLIER*HRT_BEAT_TIMER, 255, 255);      //  (seconds)
   setMaxEntityRange(LCreal(0), 255, 255); // no range filtering
   
   // Clear emission PDU handle table
   for (unsigned int i = 0; i < MAX_EMISSION_HANDLERS; i++) {
      emissionHandlers[i] = 0;
   }    
   nEmissionHandlers = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void NetIO::copyData(const NetIO& org, const bool cc)
{
   BaseClass::copyData(org);
   
   if (cc) {
      for (unsigned int i = 0; i < MAX_EMISSION_HANDLERS; i++) {
         emissionHandlers[i] = 0;
      }    
      nEmissionHandlers = 0;
   }

   clearEmissionPduHandlers();
   for (unsigned int i = 0; i < org.nEmissionHandlers; i++) {
      const EmissionPduHandler* const tmp = 
         (const EmissionPduHandler*) org.emissionHandlers[i]->clone();
      addEmissionPduHandler(tmp);
      tmp->unref();
   }

   version = org.version;
    
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
   netInput = 0;
   netOutput = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void NetIO::deleteData()
{
    clearEmissionPduHandlers();
    netInput = 0;
    netOutput = 0;
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
    if (netInput != 0) {
        if (netInput->initNetwork(true)) std::cout << "netInput Initialize OK" << std::endl;
    }
    else {
        std::cerr << "NetIO::initNetwork(): failure to find the network input handler (see slot 'netInput')" << std::endl;
        ok = false;
    }
    
    // Initialize network output handler
    if (netOutput != 0) {
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
   // Packets
   static const unsigned int MAX_PDUs = 500;
   unsigned int packet[MAX_PDUs][MAX_PDU_SIZE/4];

   unsigned int j0 = 0;

   // Read PDUs
   while ( (j0 < MAX_PDUs) && (recvData((char*)&packet[j0], MAX_PDU_SIZE) > 0) ) {
      j0++;
   }

   while (j0 > 0) {

      // Process incoming PDUs
      unsigned int j1 = 0;
      while (j1 < j0) {
         PDUHeader* header = (PDUHeader*) &packet[j1++][0];

         if (isInputEnabled()) {

            // Notes: the header's bytes are still in network order, but since the
            // data we're using are all type 'char' then we're saving time by not
            // doing an initial byte swap of the header.

            if (getExerciseID() == 0 || (getExerciseID() == header->exerciseIdentifier)) {
               // When we're interested in this exercies ... 
               switch (header->PDUType) {

               case PDU_ENTITY_STATE: {
                  //std::cout << "Entity State PDU." << std::endl;
                  EntityStatePDU* pPdu = (EntityStatePDU *) header;
                  if (Basic::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                  if (getSiteID() != pPdu->entityID.simulationID.siteIdentification || 
                     getApplicationID() != pPdu->entityID.simulationID.applicationIdentification) {
                        processEntityStatePDU(pPdu);
                  }
                                      }
                                      break;

               case PDU_FIRE: {
                  FirePDU* pPdu = (FirePDU *) header;
                  if (Basic::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                  if (getSiteID() != pPdu->firingEntityID.simulationID.siteIdentification ||
                     getApplicationID() != pPdu->firingEntityID.simulationID.applicationIdentification) {
                        processFirePDU(pPdu);
                  }
                              }
                              break;

               case PDU_DETONATION: {
                  DetonationPDU* pPdu = (DetonationPDU *) header;
                  if (Basic::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                  if (getSiteID() != pPdu->firingEntityID.simulationID.siteIdentification ||
                     getApplicationID() != pPdu->firingEntityID.simulationID.applicationIdentification) {
                        processDetonationPDU(pPdu);
                  }
                                    }
                                    break;

               case PDU_SIGNAL: {
                  SignalPDU* pPdu = (SignalPDU *) header;
                  if (Basic::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                  if (getSiteID() != pPdu->emittingEntityID.simulationID.siteIdentification ||
                     getApplicationID() != pPdu->emittingEntityID.simulationID.applicationIdentification) {
                        processSignalPDU(pPdu);
                  }
                                }
                                break;

               case PDU_TRANSMITTER: {
                  TransmitterPDU* pPdu = (TransmitterPDU *) header;
                  if (Basic::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                  if (getSiteID() != pPdu->emittingEntityID.simulationID.siteIdentification ||
                     getApplicationID() != pPdu->emittingEntityID.simulationID.applicationIdentification) {
                        processTransmitterPDU(pPdu);
                  }
                                     }
                                     break;

               case PDU_ELECTROMAGNETIC_EMISSION: {
                  ElectromagneticEmissionPDU* pPdu = (ElectromagneticEmissionPDU *) header;
                  if (Basic::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                  if (getSiteID() != pPdu->emittingEntityID.simulationID.siteIdentification ||
                     getApplicationID() != pPdu->emittingEntityID.simulationID.applicationIdentification) {
                        processElectromagneticEmissionPDU(pPdu);
                  }
                                                  }
                                                  break;

               case PDU_DATA_QUERY: {
                  DataQueryPDU* pPdu = (DataQueryPDU *) header;
                  if (Basic::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                  if (getSiteID() != pPdu->emittingEntityID.simulationID.siteIdentification ||
                     getApplicationID() != pPdu->emittingEntityID.simulationID.applicationIdentification) {
                        processDataQueryPDU(pPdu);
                  }
                                    }
                                    break;

               case PDU_DATA: {
                  DataPDU* pPdu = (DataPDU *) header;
                  if (Basic::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                  if (getSiteID() != pPdu->emittingEntityID.simulationID.siteIdentification ||
                     getApplicationID() != pPdu->emittingEntityID.simulationID.applicationIdentification) {
                        processDataPDU(pPdu);
                  }
                              }
                              break;

               case PDU_COMMENT: {
                  CommentPDU* pPdu = (CommentPDU *) header;
                  if (Basic::NetHandler::isNotNetworkByteOrder()) pPdu->swapBytes();
                  if (getSiteID() != pPdu->emittingEntityID.simulationID.siteIdentification ||
                     getApplicationID() != pPdu->emittingEntityID.simulationID.applicationIdentification) {
                        processCommentPDU(pPdu);
                  }
                                 }
                                 break;

               default: {
                  // Note: users will need to do their own byte swapping and checks
                  processUserPDU(header);
                        }
                        break;

               } // PDU switch

            } // if correct exersize
         }  // Inputs enabled

      }  // processing PDUs

      // Read more PDUs
      j0 = 0;
      while ( (j0 < MAX_PDUs) && (recvData((char*)&packet[j0], MAX_PDU_SIZE) > 0) ) {
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
      Nib* nib = (Nib*)(getInputNib(idx));
      if (nib != 0) nib->updateTheIPlayer();
   }

//   std::cout << "n = " << getInputListSize();      // #DPG#
//   Basic::PairStream* p = getSimulation()->getPlayers();
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
// User defined function to process unknown PDUs (PDU bytes are still in network order)
//------------------------------------------------------------------------------
bool NetIO::processUserPDU(const PDUHeader* const)
{
   return true;
}

//------------------------------------------------------------------------------
// nibFactory() -- Create a new Nib
//------------------------------------------------------------------------------
Simulation::Nib* NetIO::nibFactory(const Simulation::NetIO::IoType ioType)
{
   return new Nib(ioType);
}


//------------------------------------------------------------------------------
// Create a new NIBs
//------------------------------------------------------------------------------

Simulation::Nib* NetIO::createNewOutputNib(Simulation::Player* const player)
{
   Nib* nib = (Nib*) nibFactory(OUTPUT_NIB);
   if (nib != 0) {
      nib->setNetIO(this);
      nib->setPlayer(player);
      nib->setPlayerID(player->getID());
      nib->setSide(player->getSide());

      // Default DR: World, No rotation, 2nd order linear
      nib->setDeadReckoning(Nib::FVW_DRM);

      bool ok = true;
      const Basic::String* fName = getFederateName();
      unsigned short site = getSiteID();
      unsigned short app  = getApplicationID();
      if (player->isNetworkedPlayer()) {
         Simulation::Nib* pNib = player->getNib();
         fName = pNib->getFederateName();
         // Mapping another federate name to DIS site and application IDs.
         // Currently using parseFederateName(), but really should have a
         // more robust federate name to site/app ID map list.
         if (fName != 0) ok = parseFederateName(&site, &app, *fName);
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
      }
   }
   return nib;
}

//------------------------------------------------------------------------------
// findDisNib() -- find the NIB that matches ALL IDs.
//------------------------------------------------------------------------------
Nib* NetIO::findDisNib(const unsigned short playerID, const unsigned short site, const unsigned short app, const IoType ioType)
{
   Nib* nib = 0;
   char cbuff[32];
   bool ok = makeFederateName(cbuff, 32, site, app);
   if (ok) {
      Basic::String fname(cbuff);
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

    // Find the NIB and IPlayer for the emitting player (they must exist or we're too early)
    Nib* eNib = findDisNib(ePlayerId, eSiteId, eApplicationId, INPUT_NIB);
    if (eNib == 0) return;

    // Pass on to the IPlayer's NIB for processing
    eNib->processElectromagneticEmissionPDU(pdu);
}

//------------------------------------------------------------------------------
// recvData() -- recieve data packet
//------------------------------------------------------------------------------
int NetIO::recvData(char* const packet, const int maxSize)
{
   int result = 0;
   if (netInput != 0) {
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
   if (netOutput != 0) {
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
      ts = makeTimeStamp( (LCreal) getSimulation()->getSysTimeOfDay(), true );
   }
   else {
      ts = makeTimeStamp( (LCreal) getSimulation()->getExecTimeSec(), false );
   }
   return ts;
}

//------------------------------------------------------------------------------
// makeTimeStamp() -- makes a DIS time stamp
//------------------------------------------------------------------------------
unsigned int NetIO::makeTimeStamp(const LCreal ctime, const bool absolute)
{
    // compute seconds in this hour
    int hours = int(ctime / 3600.0);
    LCreal secondsThisHour = (ctime - LCreal(hours*3600));
    
    // 31 MSBs are for the 3600 seconds in this hour
    unsigned int ts = (unsigned int)( (secondsThisHour/3600.0) * 0x7fffffff );
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
   if (fedName != 0 && len > 0 && site > 0 && app > 0) {
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
         lcStrcpy(fedName,len,cbuff);
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
   if (site != 0 && app != 0 && fedName != 0) {
      unsigned short tSite = 0;
      unsigned short tApp = 0;
      unsigned int idx = 0;
      
      // First check and convert site number
      ok = ( toupper(fedName[idx++]) == 'S' );
      if (ok) {
         unsigned int tmp = 0;
         unsigned int cnt = 0;
         while (cnt < 5 && isdigit(fedName[idx])) {
            tmp *= 10;
            tmp += int(fedName[idx++] - '0');
            cnt++;
         }
         ok = (tmp > 0) && (tmp <= 0xFFFF);
         if (ok) tSite = (unsigned short) tmp;
      }
      
      // Next check and convert application number
      ok = ( toupper(fedName[idx++]) == 'A' );
      if (ok) {
         unsigned int tmp = 0;
         unsigned int cnt = 0;
         while (cnt < 5 && isdigit(fedName[idx])) {
            tmp *= 10;
            tmp += int(fedName[idx++] - '0');
            cnt++;
         }
         ok = (tmp > 0) && (tmp <= 0xFFFF);
         if (ok) tApp = (unsigned short) tmp;
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
   if (fedName != 0 && len > 0 && exercise > 0) {
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
         lcStrcpy(fedName,len,cbuff);
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
   if (exercise != 0 && fedName != 0) {
      unsigned short tExercise = 0;
      unsigned int idx = 0;
      
      ok = ( toupper(fedName[idx++]) == 'E' );
      if (ok) {
         unsigned int tmp = 0;
         unsigned int cnt = 0;
         while (cnt < 5 && isdigit(fedName[idx])) {
            tmp *= 10;
            tmp += int(fedName[idx++] - '0');
            cnt++;
         }
         ok = (tmp > 0) && (tmp <= 0xFFFF);
         if (ok) tExercise = (unsigned short) tmp;
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
const Dis::Ntm* NetIO::findNtmByTypeCodes(
         const unsigned char  kind,
         const unsigned char  domain,
         const unsigned short countryCode,
         const unsigned char  category,
         const unsigned char  subcategory,
         const unsigned char  specific,
         const unsigned char  extra
      ) const
{
   const Dis::Ntm* result = 0;

   const Dis::NtmInputNode* disRoot = dynamic_cast<const Dis::NtmInputNode*>( getRootNtmInputNode() );
   if (disRoot != 0) {

      result = disRoot->findNtmByTypeCodes(kind, domain, countryCode, category, subcategory, specific, extra);

   }
   return result;
}

//------------------------------------------------------------------------------
// Data access (get) routines
//------------------------------------------------------------------------------

LCreal NetIO::getMaxEntityRange(const Simulation::Nib* const nib) const
{
   LCreal value = 0;
   if (nib != 0) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != 0) {
         unsigned char k = disNib->getEntityKind();
         unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxEntityRange[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxEntityRange(nib);
      }
   }
   return value;
}

LCreal NetIO::getMaxEntityRangeSquared(const Simulation::Nib* const nib) const
{
   LCreal value = 0;
   if (nib != 0) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != 0) {
         unsigned char k = disNib->getEntityKind();
         unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxEntityRange2[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxEntityRangeSquared(nib);
      }
   }
   return value;
}

LCreal NetIO::getMaxTimeDR(const Simulation::Nib* const nib) const
{
   LCreal value = 0;
   if (nib != 0) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != 0) {
         unsigned char k = disNib->getEntityKind();
         unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxTimeDR[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxTimeDR(nib);
      }
   }
   return value;
}

LCreal NetIO::getMaxPositionErr(const Simulation::Nib* const nib) const
{
   LCreal value = 0;
   if (nib != 0) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != 0) {
         unsigned char k = disNib->getEntityKind();
         unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxPositionErr[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxPositionErr(nib);
      }
   }
   return value;
}

LCreal NetIO::getMaxOrientationErr(const Simulation::Nib* const nib) const
{
   LCreal value = 0;
   if (nib != 0) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != 0) {
         unsigned char k = disNib->getEntityKind();
         unsigned char d = disNib->getEntityDomain();
         value = (k < NUM_ENTITY_KINDS && d < MAX_ENTITY_DOMAINS) ? maxOrientationErr[k][d] : 0;
      }
      else {
         value = BaseClass::getMaxOrientationErr(nib);
      }
   }
   return value;
}

LCreal NetIO::getMaxAge(const Simulation::Nib* const nib) const
{
   LCreal value = 0;
   if (nib != 0) {
      const Nib* disNib = dynamic_cast<const Nib*>(nib);
      if (disNib != 0) {
         unsigned char k = disNib->getEntityKind();
         unsigned char d = disNib->getEntityDomain();
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
         Basic::String* p = new Basic::String(cbuff);
         setFederateName(p);
         p->unref();
      }
   }
   else {
      setFederateName(0);
   }
}

// Defines our NetIO federation name based on the exercise number
void NetIO::defineFederationName()
{
   if (exerciseID > 0) {
      char cbuff[32];
      bool ok = makeFederationName(cbuff, 32, exerciseID);
      if (ok) {
         Basic::String* p = new Basic::String(cbuff);
         setFederationName(p);
         p->unref();
      }
   }
   else {
      setFederationName(0);
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
bool NetIO::setMaxEntityRange(const LCreal v, const unsigned char kind, const unsigned char domain)
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
bool NetIO::setMaxTimeDR(const LCreal v, const unsigned char kind, const unsigned char domain)
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
bool NetIO::setMaxPositionErr(const LCreal v, const unsigned char kind, const unsigned char domain)
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

// setMaxOrientationErr() -- Sets max orientatio error (radians)
bool NetIO::setMaxOrientationErr(const LCreal v, const unsigned char kind, const unsigned char domain)
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
bool NetIO::setMaxAge(const LCreal v, const unsigned char kind, const unsigned char domain)
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
bool NetIO::setMaxEntityRange(const Basic::Distance* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != 0) {
        Basic::Meters ref;
        LCreal meters = ref.convert(*p);
        ok = setMaxEntityRange(meters, kind, domain);
    }
    return ok;
}

// Sets max DR time for this entity kind/domain
bool NetIO::setMaxTimeDR(const Basic::Time* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != 0) {
        Basic::Seconds ref;
        LCreal sec = ref.convert(*p);
        ok = setMaxTimeDR(sec, kind, domain);
    }
    return ok;
}

// Sets max position error for this entity kind/domain
bool NetIO::setMaxPositionErr(const Basic::Distance* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != 0) {
        Basic::Meters ref;
        LCreal meters = ref.convert(*p);
        ok = setMaxPositionErr(meters, kind, domain);
    }
    return ok;
}

// Sets max orientation error for this entity kind/domain
bool NetIO::setMaxOrientationErr(const Basic::Angle* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != 0) {
        Basic::Radians ref;
        LCreal radians = (LCreal)ref.convert(*p);
        ok = setMaxOrientationErr(radians, kind, domain);
    }
    return ok;
}

// Sets max age (without update) of a networked player of this entity kind/domain
bool NetIO::setMaxAge(const Basic::Time* const p, const unsigned char kind, const unsigned char domain)
{
    bool ok = false;
    if (p != 0) {
        Basic::Seconds ref;
        LCreal sec = ref.convert(*p);
        ok = setMaxAge(sec, kind, domain);
    }
    return ok;
}

// DISv7 additions
LCreal NetIO::getHbtPduEe() const         { return HBT_PDU_EE; }
LCreal NetIO::getHbtTimeoutMplier() const { return HBT_TIMEOUT_MPLIER; }
LCreal NetIO::getEeAzThrsh() const        { return EE_AZ_THRSH; }
LCreal NetIO::getEeElThrsh() const        { return EE_EL_THRSH; }


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
      emissionHandlers[nEmissionHandlers] = 0;
   }
}


//------------------------------------------------------------------------------
// Find Emission PDU handlers
//------------------------------------------------------------------------------

// By RfSensor data
const EmissionPduHandler* NetIO::findEmissionPduHandler(const Simulation::RfSensor* const msg)
{
   const EmissionPduHandler* handler = 0;
   if (msg != 0 && nEmissionHandlers > 0) {
      // Try to find one with a matching R/F sensor ...
      for (unsigned int i = 0; i < nEmissionHandlers && handler == 0; i++) {
         if (emissionHandlers[i]->isMatchingRfSystemType(msg)) {
            handler = emissionHandlers[i];
         }
      }
      // If not found, try to find a default outgoing handler
      for (unsigned int i = 0; i < nEmissionHandlers && handler == 0; i++) {
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
   const EmissionPduHandler* handler = 0;
   if (msg != 0 && nEmissionHandlers > 0) {
      // Try to find one with a matching emitter name
      for (unsigned int i = 0; i < nEmissionHandlers && handler == 0; i++) {
         if (emissionHandlers[i]->isMatchingRfSystemType(msg)) {
            handler = emissionHandlers[i];
         }
      }
      // If not found, try to find a default incoming handler
      for (unsigned int i = 0; i < nEmissionHandlers && handler == 0; i++) {
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
bool NetIO::setSlotFederateName(const Basic::String* const)
{
   std::cerr << "NetIO::setSlotFederateName() -- With NetIO, use the siteID and applicationID slots to create the federate name" << std::endl;
   return false;
}

// Sets our federation name (override NetIO)
bool NetIO::setSlotFederationName(const Basic::String* const)
{
   std::cerr << "NetIO::setSlotFederationName() -- With NetIO, use the exerciseID slot to create the federation name" << std::endl;
   return false;
}

// Set Network Input Handler
bool NetIO::setSlotNetInput(Basic::NetHandler* const msg)
{
    netInput = msg;
    return true;
}

// Net Network Output Handler
bool NetIO::setSlotNetOutput(Basic::NetHandler* const msg)
{
    netOutput = msg;
    return true;
}

// DIS version number [ 0 .. 6 ] (IST-CF-03-01, May 5, 2003)
bool NetIO::setSlotVersion(const Basic::Number* const num)
{
    bool ok = false;
    if (num != 0) {
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
bool NetIO::setSlotMaxEntityRange(const Basic::PairStream* const msg)
{
   bool ok = false;
   if (msg != 0) {
      const Basic::List::Item* item = msg->getFirstItem();
      while (item != 0) {

            // get the slot and object from the pair
            const Basic::Pair* p = (Basic::Pair*) item->getValue();
            const char* const slotname = *p->slot();
            const Basic::Distance* pp = dynamic_cast<const Basic::Distance*>( p->object() );

            if (pp != 0) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everythings valid, so let setMaxEntityRange() handle it
                  ok = setMaxEntityRange(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxEntityRange(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxEntityRange(): slot: " << slotname << " is not a valid Basic::Distance!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}

// Sets the maximum range for all entity types
bool NetIO::setSlotMaxEntityRange(const Basic::Distance* const msg)
{
   return setMaxEntityRange(msg, 255, 255);
}

// Sets max DR times for pairs of entities by kind/domain
bool NetIO::setSlotMaxTimeDR(const Basic::PairStream* const msg)
{
   bool ok = false;
   if (msg != 0) {
      const Basic::List::Item* item = msg->getFirstItem();
      while (item != 0) {

            // get the slot and object from the pair
            const Basic::Pair* p = (Basic::Pair*) item->getValue();
            const char* const slotname = *p->slot();
            const Basic::Time* pp = dynamic_cast<const Basic::Time*>( p->object() );

            if (pp != 0) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everythings valid, so let setMaxTimeDR() handle it
                  ok = setMaxTimeDR(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxTimeDR(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxTimeDR(): slot: " << slotname << " is not a valid Basic::Time!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}


// Sets max DR times for all entity types
bool NetIO::setSlotMaxTimeDR(const Basic::Time* const msg)
{
   return setMaxTimeDR(msg, 255, 255);
}


// Sets max position errors for pairs of entities by kind/domain
bool NetIO::setSlotMaxPositionErr(const Basic::PairStream* const msg)
{
   bool ok = false;
   if (msg != 0) {
      const Basic::List::Item* item = msg->getFirstItem();
      while (item != 0) {

            // get the slot and object from the pair
            const Basic::Pair* p = (Basic::Pair*) item->getValue();
            const char* const slotname = *p->slot();
            const Basic::Distance* pp = dynamic_cast<const Basic::Distance*>( p->object() );

            if (pp != 0) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everythings valid, so let setMaxPositionErr() handle it
                  ok = setMaxPositionErr(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxPositionErr(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxPositionErr(): slot: " << slotname << " is not a valid Basic::Distance!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}

// Sets max position errors for all entity types
bool NetIO::setSlotMaxPositionErr(const Basic::Distance* const msg)
{
   return setMaxPositionErr(msg, 255, 255);
}

// Sets max orientation errors for pairs of entities by kind/domain
bool NetIO::setSlotMaxOrientationErr(const Basic::PairStream* const msg)
{
   bool ok = false;
   if (msg != 0) {
      const Basic::List::Item* item = msg->getFirstItem();
      while (item != 0) {

            // get the slot and object from the pair
            const Basic::Pair* p = (Basic::Pair*) item->getValue();
            const char* const slotname = *p->slot();
            const Basic::Angle* pp = dynamic_cast<const Basic::Angle*>( p->object() );

            if (pp != 0) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everythings valid, so let setMaxOrientationErr() handle it
                  ok = setMaxOrientationErr(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxOrientationErr(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxOrientationErr(): slot: " << slotname << " is not a valid Basic::Angle!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}

// Sets max orientation errors for all entity types
bool NetIO::setSlotMaxOrientationErr(const Basic::Angle* const msg)
{
   return setMaxOrientationErr(msg, 255, 255);
}


// Sets max ages (without update) of for pairs of networked entities by kind/domain
bool NetIO::setSlotMaxAge(const Basic::PairStream* const msg)
{
   bool ok = false;
   if (msg != 0) {
      const Basic::List::Item* item = msg->getFirstItem();
      while (item != 0) {

            // get the slot and object from the pair
            const Basic::Pair* p = (Basic::Pair*) item->getValue();
            const char* const slotname = *p->slot();
            const Basic::Time* pp = dynamic_cast<const Basic::Time*>( p->object() );

            if (pp != 0) {
               // Ok, we have a valid object,
               //  now can we get valid 'kind' and 'domain' numbers from the slot name?
               unsigned char kind = 255;
               unsigned char domain = 255;
               bool isNum = slot2KD(slotname, &kind, &domain);
               if (isNum) {
                  // Everythings valid, so let setMaxAge() handle it
                  ok = setMaxAge(pp, kind, domain);
               }
               else {
                  std::cerr << "NetIO::setSlotMaxAge(): slot: " << slotname << " is not a valid." << std::endl;
               }
            }
            else {
               std::cerr << "NetIO::setSlotMaxAge(): slot: " << slotname << " is not a valid Basic::Time!" << std::endl;
            }

            item = item->getNext();
      }
   }
   return ok;
}

// Sets max ages (without update) of all entity types
bool NetIO::setSlotMaxAge(const Basic::Time* const msg)
{
   return setMaxAge(msg, 255, 255);
}
  
// Sets the list of Electromagnetic Emission PDU handlers
bool NetIO::setSlotEmissionPduHandlers(Basic::PairStream* const msg)
{
    bool ok = false;
    if (msg != 0) {
       // First clear the old table
       clearEmissionPduHandlers();

       // Now scan the pair stream and put all Ntm objects into the table.
       Basic::List::Item* item = msg->getFirstItem();
       while (item != 0 && nEmissionHandlers < MAX_EMISSION_HANDLERS) {
          Basic::Pair* pair = (Basic::Pair*) (item->getValue());
          EmissionPduHandler* handler = dynamic_cast<EmissionPduHandler*>( pair->object() );
          if (handler != 0) {
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
   if (slotname != 0 && kind != 0) {
      size_t len = strlen(slotname);
      bool haveTheK = (len > 0 && (slotname[0] == 'K' || slotname[0] == 'k') );
      bool haveTheD = (len > 2 && (slotname[2] == 'D' || slotname[2] == 'd') );
      if (haveTheK && isdigit(slotname[1]) && (len == 2 || haveTheD)) {
         // Ok, we have a single digit (kind)
         unsigned char k = (unsigned char) (slotname[1] - '0');
         if (k < NUM_ENTITY_KINDS) {
            // We have a valid kind value!
            if (haveTheD) {
               // We have a single digit (kind) followed by a 'D',
               // so we should have a domain value ...
               bool isNum = true;
               for (int i = 3; isNum && slotname[i] != '\0'; i++) {
                  if ( !isdigit(slotname[i]) ) isNum = false;
               }
               if (isNum) {
                  int d = atoi(&slotname[3]);
                  if (d >= 0 && d < MAX_ENTITY_DOMAINS) {
                     // At this point we have both kind and domain numbers
                     *kind = k;
                     *domain = (unsigned char) d;
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
bool NetIO::setSlotSiteID(const Basic::Number* const num)
{
    bool ok = false;
    if (num != 0) {
        int v = num->getInt();
        if (v >= 0 && v <= 65535) {
            ok = setSiteID((unsigned short)(v));
        }
        else {
            std::cerr << "NetIO::setSlotSiteID(): invalid number(" << v << "); valid range:[0 ... 65535]" << std::endl;
        }
    }
    return ok;
}

// Set Application ID
bool NetIO::setSlotApplicationID(const Basic::Number* const num)
{
    bool ok = false;
    if (num != 0) {
        int v = num->getInt();
        if (v >= 0 && v <= 65535) {
            ok = setApplicationID((unsigned short)(v));
        }
        else {
            std::cerr << "NetIO::setSlotApplicationID(): invalid number(" << v << "); valid range:[0 ... 65535]" << std::endl;
        }
    }
    return ok;
}

// Set Exercise ID
bool NetIO::setSlotExerciseID(const Basic::Number* const num)
{
    bool ok = false;
    if (num != 0) {
        int v = num->getInt();
        if (v >= 0 && v <= 255) {
            ok = setExerciseID((unsigned char)(v));
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
Basic::Object* NetIO::getSlotByIndex(const int si)
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
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }


    // Network Input Handler
    if (netInput != 0) {
        indent(sout,i+j);
        sout << "netInput: ";
        netInput->serialize(sout,(i+j+4),true);
    }

    // Network Output Handler
    if (netOutput != 0) {
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
   if (n > 0 && root != 0 && maxTypes > 0) {
      for (unsigned int i = 0; i < n; i++) {
         int r = rand();
         LCreal nr = (LCreal(r) / LCreal(RAND_MAX));
         int idx = nint(nr * (maxTypes - 1));
         const Ntm* origNtm = (Ntm*) getInputEntityType(idx);
         std::cout << "i= " << i;
         std::cout << "; idx= " << idx;
         std::cout << "; origNtm= " << origNtm;
         if (origNtm != 0) {

            std::cout << "; [ ";
            std::cout << int(origNtm->getEntityKind()) << ",";
            std::cout << int(origNtm->getEntityDomain()) << ",";
            std::cout << int(origNtm->getEntityCountry()) << ",";
            std::cout << int(origNtm->getEntityCategory()) << ",";
            std::cout << int(origNtm->getEntitySubcategory()) << ",";
            std::cout << int(origNtm->getEntitySpecific()) << ",";
            std::cout << int(origNtm->getEntityExtra()) << ")";
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
            if (foundNtm != 0) {
               const Simulation::Player* foundP = origNtm->getTemplatePlayer();
               std::cout << "; form: " << foundP->getFormName();
               SPtr<Basic::String> foundType( (Basic::String*) foundP->getType() );
               if (foundType != 0) std::cout << "; type: " << *foundType;
            }
            if (origNtm == foundNtm) {
               std::cout << "; Match!!";
            }
            else {
               std::cout << "; NO mach!!";
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
   if (n > 0 && root != 0 && maxTypes > 0) {
      for (unsigned int i = 0; i < n; i++) {
         int r = rand();
         LCreal nr = (LCreal(r) / LCreal(RAND_MAX));
         int idx = nint(nr * (maxTypes - 1));
         const Ntm* origNtm = (Ntm*) getOutputEntityTypes(idx);
         std::cout << "i= " << i;
         std::cout << "; idx= " << idx;
         std::cout << "; origNtm= " << origNtm;
         if (origNtm != 0) {

            const Simulation::Player* origP = origNtm->getTemplatePlayer();
            Simulation::Player* origP1 = (Simulation::Player*) origP->clone();

            std::cout << "; form: " << origP->getFormName();
            SPtr<Basic::String> origType( (Basic::String*) origP->getType() );
            if (origType != 0) {

               char cbuff[64];
               lcStrcpy(cbuff, 64, origType->getString());

#if 0 /* optionally increment the last character to look for generic matches */
               size_t ll = strlen(cbuff);
               if (ll > 1) {
                  cbuff[ll-1]++;
               }
#endif

               Basic::String* newType = new Basic::String(cbuff);
               origP1->setType(newType);

               Basic::String* origType1 = (Basic::String*) origP1->getType();
               std::cout << "; type1: " << *origType1;
            }

            const Ntm* foundNtm = (const Ntm*) root->findNetworkTypeMapper(origP1);
            std::cout << "; foundNtm= " << foundNtm;
            if (foundNtm != 0) {
               std::cout << "; [ ";
               std::cout << int(foundNtm->getEntityKind()) << ",";
               std::cout << int(foundNtm->getEntityDomain()) << ",";
               std::cout << int(foundNtm->getEntityCountry()) << ",";
               std::cout << int(foundNtm->getEntityCategory()) << ",";
               std::cout << int(foundNtm->getEntitySubcategory()) << ",";
               std::cout << int(foundNtm->getEntitySpecific()) << ",";
               std::cout << int(foundNtm->getEntityExtra()) << ")";
               std::cout << " ]";
            }
            if (origNtm == foundNtm) {
               std::cout << "; Match!!";
            }
            else {
               std::cout << "; NO mach!!";
            }
         }
         std::cout << std::endl;
      }
   }
}

//==============================================================================
// Class: Dis::NtmInputNode
// Description: DIS incoming NTM node
//==============================================================================

IMPLEMENT_SUBCLASS(NtmInputNode,"NtmInputNode")
EMPTY_SLOTTABLE(NtmInputNode)
EMPTY_SERIALIZER(NtmInputNode)

//------------------------------------------------------------------------------
// root incoming NTM node factory
//------------------------------------------------------------------------------
Simulation::NetIO::NtmInputNode* NetIO::rootNtmInputNodeFactory() const
{
   return new Dis::NtmInputNode(Dis::NtmInputNode::ROOT_LVL,0); // root level
}

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
NtmInputNode::NtmInputNode(const unsigned int l, const unsigned int c, const Ntm* ntm)
   : level(l), code(c), ourNtm(0), subnodeList(0)
{
   STANDARD_CONSTRUCTOR()

   if (ntm != 0) {
      ourNtm = ntm;
      ourNtm->ref();
   }
   subnodeList = new Basic::List();
}

void NtmInputNode::copyData(const NtmInputNode& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      ourNtm = 0;
      subnodeList = 0;
   }

   level = org.level;
   code = org.code;

   if (ourNtm != 0) {
      ourNtm->unref();
      ourNtm = 0;
   }
   if (org.ourNtm != 0) {
      ourNtm = (Ntm*) org.ourNtm->clone();
   }

   if (subnodeList != 0) {
      subnodeList->unref();
      subnodeList = 0;
   }
   if (org.subnodeList != 0) {
      subnodeList = (Basic::List*) org.subnodeList->clone();
   }
}

void NtmInputNode::deleteData()
{
   if (ourNtm != 0) {
      ourNtm->unref();
      ourNtm = 0;
   }

   if (subnodeList != 0) {
      subnodeList->unref();
      subnodeList = 0;
   }
}

//------------------------------------------------------------------------------
// Find the NTM based on the incoming entity type codes in the NIB
//------------------------------------------------------------------------------
const Simulation::Ntm* NtmInputNode::findNetworkTypeMapper(const Simulation::Nib* const nib) const
{
   const Simulation::Ntm* result = 0;

   const Dis::Nib* disNib = dynamic_cast<const Dis::Nib*>( nib );
   if (disNib != 0) {
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
   const Ntm* result = 0;

   {
      // Yes we have the proper type of NIB ...

      // Make sure that the NIB's code for this level matches our code
      bool match = true;
      switch (level) {
         case ROOT_LVL :         match = true; break; // the 'root' node always matches
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
            const Basic::List::Item* item = subnodeList->getFirstItem();
            while (item != 0 && result == 0) {
               const NtmInputNode* subnode = (const NtmInputNode*) item->getValue();
               result = subnode->findNtmByTypeCodes(kind, domain, countryCode, category, subcategory, specific, extra);
               item = item->getNext();
            }
         }

         // Second, we can use our NTM object, but only if we're at the category
         // level or higher (i.e., we must have match at the kind, domain, country
         // code and category levels)
         if (result == 0 && level >= CATEGORY_LVL) {
            result = ourNtm;
         }
      }

   }

   return result;
}


//------------------------------------------------------------------------------
// Add the NTM to our sublist of nodes.
//------------------------------------------------------------------------------
bool NtmInputNode::add2OurLists(Simulation::Ntm* const ntm)
{
   bool ok = false;

   // Make sure we have the correct kind of NTM ...
   Dis::Ntm* disNtm = dynamic_cast<Dis::Ntm*>( ntm );
   if (disNtm != 0) {

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
               if (ourNtm == 0) {
                  ourNtm = disNtm;
                  ourNtm->ref();
                  ok = true;
               }
               else if (isMessageEnabled(MSG_WARNING)) {
                  std::cerr << "Warning: duplicate incoming NTM(";
                  std::cerr << int(disNtm->getEntityKind()) << ",";
                  std::cerr << int(disNtm->getEntityDomain()) << ",";
                  std::cerr << int(disNtm->getEntityCountry()) << ",";
                  std::cerr << int(disNtm->getEntityCategory()) << ",";
                  std::cerr << int(disNtm->getEntitySubcategory()) << ",";
                  std::cerr << int(disNtm->getEntitySpecific()) << ",";
                  std::cerr << int(disNtm->getEntityExtra()) << ")";
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
            const Basic::List::Item* item = subnodeList->getFirstItem();
            while (item != 0 && !alreadyExists) {
               NtmInputNode* subnode = (NtmInputNode*) item->getValue();
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
               std::cerr << int(disNtm->getEntityKind()) << ",";
               std::cerr << int(disNtm->getEntityDomain()) << ",";
               std::cerr << int(disNtm->getEntityCountry()) << ",";
               std::cerr << int(disNtm->getEntityCategory()) << ",";
               std::cerr << int(disNtm->getEntitySubcategory()) << ",";
               std::cerr << int(disNtm->getEntitySpecific()) << ",";
               std::cerr << int(disNtm->getEntityExtra()) << ")";
               std::cerr << ", second ignored" << std::endl;
               err = true;
            }
         }

         // Case #3; if we're at a level less than the 'specific' level, so try
         // to add the NTM to one of our existing subnodes.
         if (!ok && !err && level < SPECIFIC_LVL) {
            const Basic::List::Item* item = subnodeList->getFirstItem();
            while (item != 0 && !ok) {
               NtmInputNode* subnode = (NtmInputNode*) item->getValue();
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
   if (ourNtm != 0) {
      ourNtm->serialize(sout, icnt+4);
   }

   // Print our subnodes
   {
      const Basic::List::Item* item = subnodeList->getFirstItem();
      while (item != 0) {
         const NtmInputNode* subnode = (const NtmInputNode*) item->getValue();
         subnode->print(sout,icnt+4);
         item = item->getNext();
      }
   }

   indent(sout,icnt);
   sout << ")" << std::endl;
}

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace
