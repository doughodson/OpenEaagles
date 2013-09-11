//==============================================================================
// IFF/ATC.Navaids PDUs (Portions of NetIO and Nib)
//==============================================================================

#include "openeaagles/dis/NetIO.h"
#include "openeaagles/dis/Nib.h"
#include "openeaagles/dis/pdu.h"

#include "openeaagles/simulation/Iff.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Network {
namespace Dis {

//------------------------------------------------------------------------------
// IFF Change/Option bits
//------------------------------------------------------------------------------
static const unsigned short CHANGE_INDICATOR     = 0x0001;
static const unsigned short ALTERNATE_MODE_4     = 0x0002;
static const unsigned short ALTERNATE_MODE_C     = 0x0004;

//------------------------------------------------------------------------------
// IffManager() -- (Output support) IFF manager
//------------------------------------------------------------------------------
bool Nib::IffManager(const LCreal curExecTime)
{
   NetIO* disIO = (NetIO*)(getNetIO());
   const Basic::Pair* pair = getPlayer()->getRadioByType(typeid(Simulation::Iff));

   // OK if the player has an IFF transponder and we're the correct version.
   bool ok = (disIO->getVersion() >= NetIO::VERSION_1278_1A) && (pair != 0);

   if (ok) {
      const Simulation::Iff* iffSystem = (const Simulation::Iff*) pair->object();

      if (isIffUpdateRequired(curExecTime, iffSystem)) {


         // Standard header stuff
         IffAtcNavaidsPDU pdu;
         pdu.header.protocolVersion = disIO->getVersion();
         pdu.header.exerciseIdentifier = disIO->getExerciseID();
         pdu.header.PDUType = NetIO::PDU_IFF_ATC_NAVAIDS;
         pdu.header.protocolFamily = NetIO::PDU_FAMILY_DIS_EMISSION_REG;
         pdu.header.timeStamp = disIO->timeStamp();
         pdu.header.length = sizeof(IffAtcNavaidsPDU);

         // Entity ID
         pdu.emittingEntityID.simulationID.siteIdentification = getSiteID();
         pdu.emittingEntityID.simulationID.applicationIdentification = getApplicationID();
         pdu.emittingEntityID.ID = getPlayerID();

         // Location (default is center of parent entity)
         pdu.location.component[0] = 0;
         pdu.location.component[1] = 0;
         pdu.location.component[2] = 0;

         // System ID (for now just use a standard Mark X11; upgrade to type from IFF later)
         pdu.systemID.systemType = 1;        // Mark X11
         pdu.systemID.systemName = 2;        // Mark X11
         pdu.systemID.systemMode = 0;        // Other
         pdu.systemID.options = iffOptions;  // Option bits

         // Functional operational data
         pdu.operationalData.systemStatus = iffFunOpData->systemStatus;
         pdu.operationalData.alternateParam4 = iffFunOpData->alternateParam4;
         pdu.operationalData.informationLayers = iffFunOpData->informationLayers;
         pdu.operationalData.modifier = iffFunOpData->modifier;
         pdu.operationalData.param1 = iffFunOpData->param1;
         pdu.operationalData.param2 = iffFunOpData->param2;
         pdu.operationalData.param3 = iffFunOpData->param3;
         pdu.operationalData.param4 = iffFunOpData->param4;
         pdu.operationalData.param5 = iffFunOpData->param5;
         pdu.operationalData.param6 = iffFunOpData->param6;

         // Event ID
         pdu.eventID.simulationID.siteIdentification = disIO->getSiteID();
         pdu.eventID.simulationID.applicationIdentification = disIO->getApplicationID();
         pdu.eventID.eventNumber = disIO->getNewIffEventID();

         //pdu.dumpData();
         if (Basic::NetHandler::isNotNetworkByteOrder()) pdu.swapBytes();
         ok = disIO->sendData((char*)&pdu,sizeof(pdu));

         iffLastExecTime = curExecTime;

      }

   }

   return ok;
}

//------------------------------------------------------------------------------
// isIffUpdateRequired() -- check to see if an update is required
//------------------------------------------------------------------------------
bool Nib::isIffUpdateRequired(const LCreal curExecTime, const Simulation::Iff* const iffSystem)
{

   // System status Bits
   static const unsigned short SYSTEM_STATUS_ON     = 0x0001;
   static const unsigned short SYSTEM_STATUS_NO_P1  = 0x0002;
   static const unsigned short SYSTEM_STATUS_NO_P2  = 0x0004;
   static const unsigned short SYSTEM_STATUS_NO_P3  = 0x0008;
   static const unsigned short SYSTEM_STATUS_NO_P4  = 0x0010;
   static const unsigned short SYSTEM_STATUS_NO_P5  = 0x0020;
   static const unsigned short SYSTEM_STATUS_NO_P6  = 0x0040;
   static const unsigned short SYSTEM_STATUS_FAILED = 0x0080;

   // Parameter bits
   static const unsigned short MODE_ON             = 0x2000;
   static const unsigned short MODE_C_ALT_NEG      = 0x0001;

   //Modifier bits
   static const unsigned short MODIFIER_EMERGENCY  = 0x0002;
   static const unsigned short MODIFIER_IDENT      = 0x0004;

   // Informational layer bits
   static const unsigned short LAYER_1_PRESENT     = 0x0002;
   static const unsigned short LAYER_2_PRESENT     = 0x0004;


   enum { NO, YES, UNSURE } result = UNSURE;    // Result of update check
   unsigned char options = 0;                   // New IFF PDU option bits

   // ---
   // Delta time since last message
   // ---
   LCreal drTime = curExecTime - iffLastExecTime;

   // ---
   // 1) First time?
   // ---
   if (iffFunOpData == 0) {
      options = CHANGE_INDICATOR;   // Initial report
      iffFunOpData = new FundamentalOpData();
      result = YES;
   }

   // ---
   // 2) Build the fundamental operational data and compare to the past values
   // ---
   if (options == CHANGE_INDICATOR || drTime >= 2.0f) {
      // not more than every 2 seconds or if this is the first time

      FundamentalOpData opData;

      // System Status
      opData.systemStatus = 0;
      if (iffSystem->getPowerSwitch() >= Simulation::Iff::PWR_ON) {
         opData.systemStatus = SYSTEM_STATUS_ON;
      }

      // Alternate parameter 4
      opData.alternateParam4 = 1;

      // Information Layers
      opData.informationLayers = LAYER_1_PRESENT;

      // Modifier
      opData.modifier = 0;
      if (iffSystem->getPowerSwitch() == Simulation::Iff::PWR_EMERGENCY) {
         opData.modifier = (opData.modifier | MODIFIER_EMERGENCY);
      }

      // Parameter 1 - Mode #1
      opData.param1 = iffSystem->getMode1();
      if (iffSystem->isEnabledMode1()) {
         opData.param1 = (opData.param1 | MODE_ON);
      }

      // Parameter 2 - Mode #2
      opData.param2 = iffSystem->getMode2();
      if (iffSystem->isEnabledMode2()) {
         opData.param2 = (opData.param2 | MODE_ON);
      }

      // Parameter 3 - Mode #3
      opData.param3 = iffSystem->getMode3a();
      if (iffSystem->isEnabledMode3a()) {
         opData.param3 = (opData.param3 | MODE_ON);
      }

      // Parameter 4 - Mode #4
      if (iffSystem->getWhichMode4()) {
         opData.param4 = iffSystem->getMode4A();
      }
      else {
         opData.param4 = iffSystem->getMode4B();
      }
      if (iffSystem->isEnabledMode4()) {
         opData.param4 = (opData.param4 | MODE_ON);
      }

      // Parameter 5 - Mode C
      opData.param5 = 0;
      LCreal alt100 = (LCreal) ((getPlayer()->getAltitudeFt() + 50.0) / 100.0);
      if (alt100 < 0) { alt100 = -alt100; opData.param5 = MODE_C_ALT_NEG; }
      unsigned short ialt = (unsigned short) alt100;
      opData.param5 = (opData.param5 | (ialt << 1));  // alt shifted left one bit
      if (iffSystem->isEnabledModeC()) {
         opData.param5 = (opData.param5 | MODE_ON);
      }

      // Parameter 6 -- Mode S (not supported)
      opData.param6 = 0;
      opData.systemStatus = (opData.systemStatus | SYSTEM_STATUS_NO_P6);

      // Now check if anything changed
      bool changed =
         (opData.param1 != iffFunOpData->param1) ||
         (opData.param2 != iffFunOpData->param2) ||
         (opData.param3 != iffFunOpData->param3) ||
         (opData.param4 != iffFunOpData->param4) ||
         (opData.param5 != iffFunOpData->param5) ||
         (opData.param6 != iffFunOpData->param6) ||
         (opData.systemStatus != iffFunOpData->systemStatus) ||
         (opData.alternateParam4 != iffFunOpData->alternateParam4) ||
         (opData.informationLayers != iffFunOpData->informationLayers) ||
         (opData.modifier != iffFunOpData->modifier);

      if (changed) {
         options = CHANGE_INDICATOR;
         result = YES;
      }

      // DMOC wants all options bits set (0x07)
      options = (CHANGE_INDICATOR | ALTERNATE_MODE_4 | ALTERNATE_MODE_C);

      // And don't forget to save our new values for output
      iffFunOpData->param1 = opData.param1;
      iffFunOpData->param2 = opData.param2;
      iffFunOpData->param3 = opData.param3;
      iffFunOpData->param4 = opData.param4;
      iffFunOpData->param5 = opData.param5;
      iffFunOpData->param6 = opData.param6;
      iffFunOpData->systemStatus = opData.systemStatus;
      iffFunOpData->alternateParam4 = opData.alternateParam4;
      iffFunOpData->informationLayers = opData.informationLayers;
      iffFunOpData->modifier = opData.modifier;
   }

   // ---
   // 3) At least every 10 seconds
   // ---
   if ( (result == UNSURE)) {
      if ( drTime >= 10.0f ) {
         result = YES;
      }
   }

   iffOptions = options;
   return (result == YES);
}

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace
