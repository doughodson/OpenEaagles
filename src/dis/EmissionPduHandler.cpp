//------------------------------------------------------------------------------
// Electromagnetic Emission PDU (Portions of NetIO and Nib)
//------------------------------------------------------------------------------

#include "openeaagles/dis/EmissionPduHandler.h"
#include "openeaagles/dis/Nib.h"
#include "openeaagles/dis/pdu.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/simulation/Jammer.h"
#include "openeaagles/simulation/Radar.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/functors/Functions.h"
#include "openeaagles/basic/functors/Tables.h"
#include "openeaagles/basic/Decibel.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Number.h"

#include <cmath>
#include <cstring>

namespace oe {
namespace network {
namespace dis {

//==============================================================================
// EmissionPduHandler class
//==============================================================================
IMPLEMENT_SUBCLASS(EmissionPduHandler,"EmissionPduHandler")
EMPTY_SERIALIZER(EmissionPduHandler)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(EmissionPduHandler)
   "emitterName",       // 1) DIS emitter name (see enums)
   "emitterFunction",   // 2) DIS emitter function code (see enums)
   "sensor",            // 3) Template sensor model
   "antenna",           // 4) Template antenna model
   "defaultIn",         // 5) This is the default handler for incoming PDUs
   "defaultOut",        // 6) This is the default handler for outgoing PDUs
END_SLOTTABLE(EmissionPduHandler)

// Map slot table to handles
BEGIN_SLOT_MAP(EmissionPduHandler)
    ON_SLOT(1, setSlotEmitterName,     basic::Number )
    ON_SLOT(2, setSlotEmitterFunction, basic::Number )
    ON_SLOT(3, setSlotSensorTemplate,  simulation::RfSensor )
    ON_SLOT(4, setSlotAntennaTemplate, simulation::Antenna )
    ON_SLOT(5, setSlotDefaultIn,       basic::Number )
    ON_SLOT(6, setSlotDefaultOut,      basic::Number )
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
EmissionPduHandler::EmissionPduHandler()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void EmissionPduHandler::initData()
{
   noTemplatesFound = false;
   defaultIn = false;
   defaultOut = false;

   emitterIdNumber = 0;
   emitterName = 0;
   emitterFunction = ESF_FIRE_CONTROL; // Default

   emPduExecTime = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void EmissionPduHandler::copyData(const EmissionPduHandler& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // Copy handler info
   setEmitterName( org.emitterName );
   setEmitterFunction( org.emitterFunction );
   noTemplatesFound = org.noTemplatesFound;
   defaultIn = org.defaultIn;
   defaultOut = org.defaultOut;

   // But clear out the rest (so we can start over as a new handler)
   emPduExecTime = 0;

   setSensor(nullptr);

   setSensorModel(nullptr);
   if (org.getSensorModel() != nullptr) {
      simulation::RfSensor* tmp = org.getSensorModel()->clone();
      setSensorModel(tmp);
      tmp->unref();
   }

   setAntennaModel(nullptr);
   if (org.getAntennaModel() != nullptr) {
      simulation::Antenna* tmp = org.getAntennaModel()->clone();
      setAntennaModel(tmp);
      tmp->unref();
   }

   // ---
   // copy the saved (N-1) data
   // ---
   setSavedEmissionSystemData( *( org.getSavedEmissionSystemData() ) );

   for (unsigned int j = 0; j < MAX_EM_BEAMS; j++) {

      setSavedEmitterBeamData( j,  *( org.getSavedEmitterBeamData(j) ) );

      for (int k = 0; k < MAX_TARGETS_IN_TJ_FIELD; k++) {
         setSavedTrackJamTargetData(j, k, *( org.getSavedTrackJamTargetData(j,k) ) );
      }
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void EmissionPduHandler::deleteData()
{
   if (sensor != nullptr) { sensor->event(oe::basic::Component::SHUTDOWN_EVENT); }
   setSensor(nullptr);

   if (sensorModel != nullptr) { sensorModel->event(oe::basic::Component::SHUTDOWN_EVENT); }
   setSensorModel(nullptr);

   if (antennaModel != nullptr) { antennaModel->event(oe::basic::Component::SHUTDOWN_EVENT); }
   setAntennaModel(nullptr);
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Unique emitter ID number
bool EmissionPduHandler::setEmitterIdNumber(const unsigned char num)
{
   emitterIdNumber = num;
   return true;
}

// Emitter function code
bool EmissionPduHandler::setEmitterFunction(const unsigned char num)
{
   emitterFunction = num;
   return true;
}

// Sets our R/F emitter system
bool EmissionPduHandler::setSensor(simulation::RfSensor* const msg)
{
   sensor = msg;
   return true;
}

// Sets our template sensor model
bool EmissionPduHandler::setSensorModel(simulation::RfSensor* const msg)
{
   sensorModel = msg;
   return true;
}

// Sets our template antenna model
bool EmissionPduHandler::setAntennaModel(simulation::Antenna* const msg)
{
   antennaModel = msg;
   return true;
}

// PDU exec time (last send)
void EmissionPduHandler::setEmPduExecTime(const LCreal v)
{
    emPduExecTime = v;
}

// Sets the our DIS Emitter Name
bool EmissionPduHandler::setEmitterName(const unsigned short newEmitterName)
{
   emitterName = newEmitterName;
   return true;
}

bool EmissionPduHandler::setDefaultIn(const bool flg)
{
   defaultIn = flg;
   return true;
}

bool EmissionPduHandler::setDefaultOut(const bool flg)
{
   defaultOut = flg;
   return true;
}

// Saved EmissionSystem data
bool EmissionPduHandler::setSavedEmissionSystemData(const EmissionSystem& newES)
{
   emissionSystemN1 = newES;
   return true;
}

// Saved EmitterBeamData
bool EmissionPduHandler::setSavedEmitterBeamData(const unsigned int idx, const EmitterBeamData& newEBD)
{
   bool ok = false;
   if (idx < MAX_EM_BEAMS) {
      emitterBeamDataN1[idx] = newEBD;
      ok = true;
   }
   return ok;
}

// Save track/jam target data
bool EmissionPduHandler::setSavedTrackJamTargetData(const unsigned int ibeam, const unsigned int ifield, const TrackJamTargets& newTJT)
{
{
   bool ok = false;
   if (ibeam < MAX_EM_BEAMS && ifield < MAX_TARGETS_IN_TJ_FIELD) {
      tjTargetsN1[ibeam][ifield] = newTJT;
      ok = true;
   }
   return ok;
}
}

bool EmissionPduHandler::setTemplatesFound(const bool newTF)
{
   noTemplatesFound = !newTF;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// Sets the our DIS Emitter Name
bool EmissionPduHandler::setSlotEmitterName(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int i = msg->getInt();
      if (i >= 0 && i <= 0xffff) {
         ok = setEmitterName( static_cast<unsigned short>(i) );
      }
   }
   return ok;
}

// Sets our DIS Emitter Function
bool EmissionPduHandler::setSlotEmitterFunction(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int i = msg->getInt();
      if (i >= 0 && i <= 0xff) {
         ok = setEmitterFunction( static_cast<unsigned char>(i) );
      }
   }
   return ok;
}

// Sets our template sensor model
bool EmissionPduHandler::setSlotSensorTemplate(simulation::RfSensor* const msg)
{
   return setSensorModel(msg);
}

// Sets our template antenna model
bool EmissionPduHandler::setSlotAntennaTemplate(simulation::Antenna* const msg)
{
   return setAntennaModel(msg);
}

bool EmissionPduHandler::setSlotDefaultIn(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setDefaultIn( msg->getBoolean() );
   }
   return ok;
}

bool EmissionPduHandler::setSlotDefaultOut(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setDefaultOut( msg->getBoolean() );
   }
   return ok;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// Returns true if RfSensor data matches our parameters
bool EmissionPduHandler::isMatchingRfSystemType(const simulation::RfSensor* const p) const
{
   bool match = false;
   if (p != nullptr && sensorModel != nullptr) {
      match = (std::strcmp(sensorModel->getTypeId(),p->getTypeId()) == 0);
   }
   return match;
}

// True if EmissionSystem PDU data matches our parameters.
bool EmissionPduHandler::isMatchingRfSystemType(const EmissionSystem* const p) const
{
   bool match = false;
   if (p != nullptr) {
      // All we need to match is the DIS "emitter name"
      match = (emitterName == p->emitterSystem.emitterName);
   }
   return match;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
basic::Object* EmissionPduHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// setTimedOut() -- incoming player has not sent EE PDU recently
//------------------------------------------------------------------------------
void EmissionPduHandler::setTimedOut()
{
   simulation::RfSensor* rfSys = getSensor();
   if (rfSys != nullptr) {
      rfSys->setTransmitterEnableFlag(false);
      rfSys->setReceiverEnabledFlag(false);
   }
   return;
}

//------------------------------------------------------------------------------
// updateIncoming() -- Process an emission system from incoming PDUs
//------------------------------------------------------------------------------
bool EmissionPduHandler::updateIncoming(const ElectromagneticEmissionPDU* const pdu, const EmissionSystem* const es, Nib* const nib)
{
   simulation::Player* player = nib->getPlayer();
   if (player == nullptr || noTemplatesFound) return false;

   // ---
   // Default sensor: process only one beam)
   // ---
   if (es->numberOfBeams > 0) {
      const EmitterBeamData* bd = es->getEmitterBeamData(0);

      // ---
      // Use our template models to create the RfSensor and Antenna
      // ---
      if (getSensor() == nullptr && !noTemplatesFound) {

         simulation::RfSensor* rp = getSensorModel();
         simulation::Antenna*  ap = getAntennaModel();

         // If we have both the RF system and antenna models ...
         if (rp != nullptr && ap != nullptr) {
            rp->setAntenna(ap); // The Radar owns the antenna

            // Reset
            rp->reset();
            ap->reset();

            // Give the antenna list to the IPlayer
            {
               // First get the (top level) container gimbal
               simulation::Gimbal* gimbal = player->getGimbal();
               if (gimbal == nullptr) {
                  // Create the container gimbal!
                  gimbal = new simulation::Gimbal();
                  basic::Pair* pair = new basic::Pair("gimbal", gimbal);
                  gimbal->unref();  // pair owns it
                  player->addComponent(pair);
                  pair->unref();    // player owns it
               }
               // Add this antenna to the container gimbal
               ap->container(gimbal);
               basic::Pair* pair = new basic::Pair("antenna", ap);
               ap->unref();   // pair owns it
               gimbal->addComponent(pair);
               pair->unref(); // top level gimbal owns it
            }

            // Give the sensor list to the IPlayer
            {
               // First get the (top level) sensor manager
               simulation::RfSensor* sm = player->getSensor();
               if (sm == nullptr) {
                  // Create the sensor manager
                  sm = new simulation::SensorMgr();
                  basic::Pair* pair = new basic::Pair("sensorMgr", sm);
                  sm->unref();   // pair owns it
                  player->addComponent(pair);
                  pair->unref(); // player owns it
               }
               // Add this system to the sensor manager
               rp->container(sm);
               basic::Pair* pair = new basic::Pair("sensor", rp);
               rp->unref();   // pair owns it
               sm->addComponent(pair);
               pair->unref(); // sensor manager owns it
            }

            setSensor(rp);
         }

         // If we do NOT have both the RF system and antenna models ...
         else {
            noTemplatesFound = true;

            std::cerr << "EmissionPduHandler::updateIncoming(): warning -- no template ";
            if (rp == nullptr) std::cerr << "R/F model";
            if (rp == nullptr && ap == nullptr) std::cerr << " or ";
            if (ap == nullptr) std::cerr << "antenna model";
            std::cerr << " found for emitterName: " << es->emitterSystem.emitterName;
            std::cerr << std::endl;

         }
      }

      // ---
      // Update the IPlayer's sensor/antenna structures with the PDU data
      // ---
      simulation::RfSensor* rfSys = getSensor();
      if (rfSys != nullptr && !noTemplatesFound) {
         simulation::Antenna* antenna = rfSys->getAntenna();

         // reset the timeout clock for this Iplayer's emissions
         setEmPduExecTime(player->getSimulation()->getExecTimeSec());

         rfSys->setFrequency( bd->parameterData.frequency );
         rfSys->setBandwidth( bd->parameterData.frequencyRange );

         // DPG ### Setting peak power to the effected radiated power from the PDU,
         // so our transmitter loss and antenna gain should both be set to 0 dB (real 1.0).
         basic::Decibel db( bd->parameterData.effectiveRadiatedPower  );  // dBm (dB milliwatts)
         rfSys->setPeakPower( db.getReal() / 1000.0f );

         rfSys->setPRF( bd->parameterData.pulseRepetitiveFrequency );
         rfSys->setPulseWidth( bd->parameterData.pulseWidth / 1000000.0f );

         if ( bd->beamData.beamAzimuthCenter == 0 &&
            (bd->beamData.beamAzimuthSweep == 0 || bd->beamData.beamAzimuthSweep >= PI)
            ) {
               // circular scan
               antenna->setRefAzimuth( 0 );
               antenna->setRefElevation( bd->beamData.beamElevationCenter );
               antenna->setScanMode( simulation::ScanGimbal::CIRCULAR_SCAN );
               antenna->setCmdRate( (24.0f * static_cast<LCreal>(basic::Angle::D2RCC)), 0 );  // default rates
         }
         else {
            // Standard search volume parameters
            antenna->setRefAzimuth( bd->beamData.beamAzimuthCenter );
            antenna->setRefElevation( bd->beamData.beamElevationCenter );
            // note that beamElevationSweep corresponds to scanHeight; setSearchVolume is expecting el component to be scanHeight+.5*barspacing
            antenna->setSearchVolume( bd->beamData.beamAzimuthSweep * 2.0f, bd->beamData.beamElevationSweep * 2.0f);
         }

         // IPlayer's transmit (when they're active) but don't need to receive
         if (pdu->header.protocolVersion >= NetIO::VERSION_7) {
            rfSys->setTransmitterEnableFlag((bd->beamStatus == BS_ACTIVE));
         }
         else {
            rfSys->setTransmitterEnableFlag(true);
         }
         rfSys->setReceiverEnabledFlag(false);
      }
   }


   // No beam data -- turn off the transmitter and receiver
   else {
      simulation::RfSensor* rfSys = getSensor();
      if (rfSys != nullptr) {
         rfSys->setTransmitterEnableFlag(false);
         rfSys->setReceiverEnabledFlag(false);
      }
   }

   return true;
}

//------------------------------------------------------------------------------
// updateOutgoing() -- Check to see if a PDU needs to be sent; returns true if a PDU was sent
//------------------------------------------------------------------------------
bool EmissionPduHandler::updateOutgoing(const LCreal curExecTime, Nib* const nib)
{
   bool pduSent = false;

   bool stateChg = false;
   if (nib != nullptr) {
      NetIO* const disIO = static_cast<NetIO*>(nib->getNetIO());
      if (disIO != nullptr && isUpdateRequired(curExecTime, &stateChg, nib)) {

         // Out going Electromagnetic Emission PDU is just a buffer to be filled
         unsigned int packet[NetIO::MAX_PDU_SIZE / 4];
         ElectromagneticEmissionPDU* pdu = reinterpret_cast<ElectromagneticEmissionPDU*>(&packet[0]);

         // Standard header stuff
         pdu->header.protocolVersion = disIO->getVersion();
         pdu->header.exerciseIdentifier = disIO->getExerciseID();
         pdu->header.PDUType = NetIO::PDU_ELECTROMAGNETIC_EMISSION;
         pdu->header.protocolFamily = NetIO::PDU_FAMILY_DIS_EMISSION_REG;
         pdu->header.timeStamp = disIO->timeStamp();
         pdu->header.length = sizeof(ElectromagneticEmissionPDU);

         // Entity ID
         pdu->emittingEntityID.simulationID.siteIdentification = nib->getSiteID();
         pdu->emittingEntityID.simulationID.applicationIdentification = nib->getApplicationID();
         pdu->emittingEntityID.ID = nib->getPlayerID();

         // ---
         // Event ID
         // ---
         pdu->eventID.simulationID.siteIdentification = nib->getSiteID();
         pdu->eventID.simulationID.applicationIdentification = nib->getApplicationID();
         pdu->eventID.eventNumber = disIO->getNewEmissionEventID();

         // Changed Data Update(1); or State Change(0)
         if (stateChg)
            pdu->stateUpdateIndicator = ElectromagneticEmissionPDU::STATE_UPDATE;
         else
            pdu->stateUpdateIndicator = ElectromagneticEmissionPDU::CHANGED_DATA_UPDATE;

         // One system per PDU
         pdu->numberOfSystems = 1;

         // Pointer to emission system
         unsigned char* p = (reinterpret_cast<unsigned char*>(pdu)) + sizeof(ElectromagneticEmissionPDU);
         EmissionSystem* es = reinterpret_cast<EmissionSystem*>(p);

         // ---
         // Add the emission system to the end of the PDU and send it
         // ---
         unsigned short result = emissionSystemData2PDU(es);
         if (result > 0) {

            // Update our total length and number of systems
            pdu->header.length = result + pdu->header.length;
            p += result;
            es = reinterpret_cast<EmissionSystem*>(p);

            //pdu->dumpData();

            int length = pdu->header.length;
            if (basic::NetHandler::isNotNetworkByteOrder()) pdu->swapBytes();
            pduSent = disIO->sendData(reinterpret_cast<char*>(pdu), length);

            setEmPduExecTime(curExecTime);
         }

      } // End update required
   } // End NIB test

   return pduSent;
}

//------------------------------------------------------------------------------
// isUpdateRequired() -- check to see if an update is required for the
//                         electromagnetic emission PDU.
//------------------------------------------------------------------------------
bool EmissionPduHandler::isUpdateRequired(const LCreal curExecTime, bool* const stateChg, Nib* const nib)
{
   bool sc = false;
   enum { NO, YES, UNSURE } result = UNSURE;

   // Early out if we don't have everything we need!
   if (nib == nullptr) return NO;
   NetIO* const disIO = static_cast<NetIO*>(nib->getNetIO());
   if (disIO == nullptr) return NO;
   simulation::RfSensor* beam = getSensor();
   if (beam == nullptr) return NO;

   // ---
   // First -- Are we're past the minimum time?  (using 1/10th of the heart beat)
   //       -- Limits the number of PDUs for this system
   // ---
   // this really should not be necessary to limit number of PDUs, since params won't change often; if no change in param, no PDU sent.
   // possibly intended to reduce amount of processing expended for no PDUs sent?
   // otherwise, could be entirely removed.
   if (disIO->getVersion() >= NetIO::VERSION_7) {
      LCreal drTime = curExecTime - getEmPduExecTime();
      if ( drTime < (disIO->getHbtPduEe() / 10.0f) ) {
         result = NO;
      }
   }
   else {
      if ( (result == UNSURE) ) {
         LCreal drTime = curExecTime - getEmPduExecTime();
         if ( drTime < (disIO->getMaxTimeDR(nib) /10.0f) ) {
            result = NO;
         }
      }
   }

   // -------------
   // Then -- Compare current emission system values with the last PDU
   // -------------
   if ( (result == UNSURE)) {

      // ---
      // Emission system data
      // ---
      EmissionSystem es;  // used to build the final EmissionSystem
      es.emitterSystem.function = getEmitterFunction();
      es.emitterSystem.emitterIdentificationNumber = getEmitterIdNumber();
      es.emitterSystem.emitterName = getEmitterName();

      // ---
      // Simple radar with only one beam
      // ---
      unsigned char numberOfBeams = 0;
      unsigned char ib = 0;

      // ---
      // If the transmitter is emitting then create the beam data
      // ---
      bool playerOk = nib->getPlayer()->isActive() && !nib->getPlayer()->isDestroyed();
      if (playerOk && beam->isTransmitting()) {

         // Antenna (if any)
         const simulation::Antenna* const ant = beam->getAntenna();

         // Beam data
         numberOfBeams = 1;
         EmitterBeamData bd;
         bd.beamIDNumber = (ib+1);

         if (disIO->getVersion() >= NetIO::VERSION_7)
            bd.beamStatus = BS_ACTIVE;
         else
            bd.beamStatus = 0;

         // Beam parameters
         bd.parameterData.frequency = static_cast<float>(beam->getFrequency());            // Hz
         bd.parameterData.frequencyRange  = static_cast<float>(beam->getBandwidth());      // Hz

         // Compute effected radiated power (watts)
         LCreal power = beam->getPeakPower();
         LCreal loss = beam->getRfTransmitLoss();
         if (ant != nullptr) power = (power * static_cast<LCreal>(ant->getGain()));
         if (loss >= 1.0f) power = (power / loss);

         // Effected radiated power -- dBm (dB milliwatts)
         basic::Decibel db;
         db.setValue( power * 1000.0f );
         bd.parameterData.effectiveRadiatedPower = static_cast<float>(db.getValueDB());

         bd.parameterData.pulseRepetitiveFrequency = static_cast<float>(beam->getPRF());          // Hz (Average)
         bd.parameterData.pulseWidth = static_cast<float>(beam->getPulseWidth()) * 1000000.0f;    // uSec

         if (ant != nullptr) {
            bd.beamData.beamAzimuthCenter   = static_cast<float>(ant->getRefAzimuth());      // Radians
            bd.beamData.beamAzimuthSweep    = static_cast<float>(ant->getScanWidth()/2.0);   // Radians -- half angles
            bd.beamData.beamElevationCenter = static_cast<float>(ant->getRefElevation());    // Radians
            bd.beamData.beamElevationSweep  = static_cast<float>(ant->getScanHeight()/2.0);  // Radians -- half angles
         }
         else {
            // Default values
            bd.beamData.beamAzimuthCenter   = 0.0f;
            bd.beamData.beamAzimuthSweep    = 30.0f * static_cast<float>(basic::Angle::D2RCC);
            bd.beamData.beamElevationCenter = 0.0f;
            bd.beamData.beamElevationSweep  = 2.0f * static_cast<float>(basic::Angle::D2RCC);
         }
         bd.beamData.beamSweepSync = 0;

         // For now ... no tracks or jamming data
         if (disIO->getVersion() >= NetIO::VERSION_7) {
            bd.jammingTechnique.kind = 0;
            bd.jammingTechnique.category = 0;
            bd.jammingTechnique.subcat = 0;
            bd.jammingTechnique.specific = 0;
         }
         else {
            bd.jammingTechnique.setJammingModeSequence(0);
         }


         if (getEmitterFunction() != ESF_JAMMING) {
            // Radar
            if ( bd.beamData.beamAzimuthSweep == 0 || bd.beamData.beamAzimuthSweep >= (180.0f * basic::Angle::D2RCC) ) {
               // ... full azimuth sweep?  assume searching
               bd.beamFunction = BF_SEARCH;
               es.emitterSystem.function = ESF_EW; // override emitter sys function to EW when searching
            }
            else
               // ... limited az sweep?  assume acq/track
               bd.beamFunction = BF_ACQUISITION_AND_TRACKING;
         }
         else {
            // Jammer
            bd.beamFunction = BF_JAMMER;
            if (disIO->getVersion() >= NetIO::VERSION_7) {
               bd.jammingTechnique.kind = JT_NOISE;
            }
         }

         bd.beamParameterIndex = 0;
         bd.highDensityTracks      = EmitterBeamData::NOT_SELECTED;
         bd.numberOfTargetsInTrack = 0;

         // ---
         // Locate any targets that this emitter is tracking
         // ---
         TrackJamTargets tjt[MAX_TARGETS_IN_TJ_FIELD];
         unsigned char numTJT = 0;

         // Get the track list
         simulation::TrackManager* tm = beam->getTrackManager();
         if (tm != nullptr) {
            const int max1 = MAX_TARGETS_IN_TJ_FIELD + 1; // check for one more than the max (highDensityTracks)
            basic::safe_ptr<simulation::Track> trackList[max1];
            int n = tm->getTrackList(trackList,max1);
            if (n <= MAX_TARGETS_IN_TJ_FIELD) {

               // Locate players for these tracks and set the TrackJamTargets data for each ...
               for (int i = 0; i < n; i++) {
                  // Does the track have a target player that we can find the entity ID for?
                  const simulation::Player* tgt = trackList[i]->getTarget();
                  if (tgt != nullptr) {
                     unsigned short  tjtPlayerID = 0;
                     unsigned short  tjtSiteID = 0;
                     unsigned short  tjtAppID = 0;
                     if (tgt->isLocalPlayer()) {
                        tjtPlayerID = tgt->getID();
                        // Local player so use our site and app IDs
                        tjtSiteID = disIO->getSiteID();
                        tjtAppID  = disIO->getApplicationID();
                     }
                     else {
                        const Nib* const tjtNib = dynamic_cast<const Nib*>( tgt->getNib() );
                        if (tjtNib != nullptr) {
                           tjtPlayerID = tjtNib->getPlayerID();
                           tjtSiteID = tjtNib->getSiteID();
                           tjtAppID  = tjtNib->getApplicationID();
                        }
                     }
                     // Set the TJT data
                     tjt[numTJT].targetID.ID = tjtPlayerID;
                     tjt[numTJT].targetID.simulationID.siteIdentification = tjtSiteID;
                     tjt[numTJT].targetID.simulationID.applicationIdentification = tjtAppID;
                     tjt[numTJT].emitterID = 0;  // (DPG #### not being jammed)
                     tjt[numTJT].beamID    = 0;
                     numTJT++;
                  }
               }

            }
            else {
               // Lots of targets -- set the high density tracks flag
               bd.highDensityTracks = EmitterBeamData::SELECTED;
            }
         }

         // ---
         // compare & transfer T/J targets
         // ---
         for (int it = 0; it < numTJT; it++) {
            if ( tjt[it] != *getSavedTrackJamTargetData(ib,it) ) {
               setSavedTrackJamTargetData(ib, it, tjt[it]);
               result = YES;
            }
         }
         bd.numberOfTargetsInTrack = numTJT;

         // implement DISv7 parameter thresholds and other restrictions on parameters that could otherwise cause updated PDUs to be sent.
         float tmpbeamSweepSync = 0;
         float tmpbeamAzimuthCenter = 0;
         float tmpbeamElevationCenter = 0;
         if (disIO->getVersion() >= NetIO::VERSION_7) {

            // change in beamSweepSync should not force new PDU
            tmpbeamSweepSync = bd.beamData.beamSweepSync;
            bd.beamData.beamSweepSync = (*getSavedEmitterBeamData(ib)).beamData.beamSweepSync;

            // d) Changes in beam geometry descriptors exceed specified thresholds. Beam geometry descriptors
            // include beam azimuth center, beam azimuth sweep, beam elevation center and beam elevation
            // sweep. The azimuth and elevation thresholds shall be identified by the symbolic names
            // EE_AZ_THRSH and EE_EL_THRSH respectively. (See 4.2.8.3 for parameter details and default
            // values.)

            // store current values temporarily
            tmpbeamAzimuthCenter = bd.beamData.beamAzimuthCenter;
            tmpbeamElevationCenter = bd.beamData.beamElevationCenter;

            // do threshold tests - if new value is not over threshold, reset it in bd struct to old value, so that it will not affect
            // the comparison of new bd with old emitterBeamData: ( bd != *emitterBeamData[ib] ) below
            // (but if new bd will be sent, make sure to restore the new values that we've temporarily stored (above))
            if (std::fabs( (*getSavedEmitterBeamData(ib)).beamData.beamAzimuthCenter - bd.beamData.beamAzimuthCenter ) <= disIO->getEeAzThrsh()) {
               // did not exceed threshold, set current val to previous val, so it will not affect comparison
               bd.beamData.beamAzimuthCenter = (*getSavedEmitterBeamData(ib)).beamData.beamAzimuthCenter;
            }
            if (std::fabs( (*getSavedEmitterBeamData(ib)).beamData.beamElevationCenter - bd.beamData.beamElevationCenter ) <= disIO->getEeElThrsh()) {
               // did not exceed threshold, set current val to previous val, so it will not affect comparison
               bd.beamData.beamElevationCenter = (*getSavedEmitterBeamData(ib)).beamData.beamElevationCenter;
            }
         }


         // ---
         // Compute beam data length (in 32bit words, including the track/jam targets)
         // ---
         unsigned char lenB = sizeof(EmitterBeamData) + (numTJT * sizeof(TrackJamTargets));
         bd.beamDataLength = (lenB/4);

         // ---
         // compare & transfer the emitter beam data
         // ---
         if ( bd != *getSavedEmitterBeamData(ib) ) {

            if (disIO->getVersion() >= NetIO::VERSION_7) {
               // there will be an update PDU, make sure it includes current values.
               bd.beamData.beamAzimuthCenter = tmpbeamAzimuthCenter;
               bd.beamData.beamElevationCenter = tmpbeamElevationCenter;
               bd.beamData.beamSweepSync = tmpbeamSweepSync;
            }

            setSavedEmitterBeamData(ib,bd);
            result = YES;
         }

      } // End transmitter enabled (beam is emitting)
      else {
         // player is notOK, or not transmitting
         // this should only result in one YES result, thereafter no change, unless
         // player becomes OK, or transmitter is re-enabled
         network::dis::EmitterBeamData bd;
         bd.beamIDNumber = (ib+1);

         if (disIO->getVersion() >= NetIO::VERSION_7)
            bd.beamStatus = BS_INACTIVE;
         else
            bd.beamStatus = 0;

         bd.numberOfTargetsInTrack = 0;

         // ---
         // Compute beam data length (in 32bit words, including the track/jam targets)
         // ---
         unsigned char lenB = sizeof(network::dis::EmitterBeamData);
         bd.beamDataLength = (lenB/4);
         // ---
         // compare & transfer the emitter beam data
         // ---
         if ( bd != *getSavedEmitterBeamData(ib) ) {
            setSavedEmitterBeamData(ib,bd);
            result = YES;
         }
      } // End !(playerOK && transmitter enabled (beam is emitting))

      // Number of emitter beams
      es.numberOfBeams = numberOfBeams;

      // ---
      // Located at the emitting entity's CG
      //   (DPG #### we can get this from Gimbal/Antenna, but need make sure we account for multiple levels of gimbals)
      // ---
      es.location.component[0] = 0;
      es.location.component[1] = 0;
      es.location.component[2] = 0;

      // ---
      // compare & transfer the emission system data
      // ---
      if ( es != *getSavedEmissionSystemData() ) {
         setSavedEmissionSystemData(es);
         result = YES;
      }

      // ---
      // Last -- Timeout (use Max DR time) -- do this check after the data comparison
      //    to make sure all of the PDU data has been loaded
      // ---
      if (disIO->getVersion() >= NetIO::VERSION_7) {
         if ( playerOk && (result == UNSURE) && nib->getPlayer()->isLocalPlayer() ) {
            LCreal drTime = curExecTime - getEmPduExecTime();
            if ( drTime >= disIO->getHbtPduEe() ) {
               result = YES;
               sc = true;
            }
         }
      }
      else {
         if ( (result == UNSURE) && nib->getPlayer()->isLocalPlayer()) {
            LCreal drTime = curExecTime - getEmPduExecTime();
            if ( drTime >= disIO->getMaxTimeDR(nib) ) {
               result = YES;
               sc = true;
            }
         }
      }

   }

   // ---
   // Update the state changed flag (only if result is YES)
   // ---
   if (result == YES) {
      *stateChg = sc;
   }

   return (result == YES);
}

//------------------------------------------------------------------------------
// emissionSystemData2PDU()
//  -- Outputs the sensor/antenna's data to the Emission System structure
//------------------------------------------------------------------------------
unsigned short EmissionPduHandler::emissionSystemData2PDU(EmissionSystem* const es)
{
    // Make sure we have all the data we need
    if (es == nullptr) return 0;

    // ---
    // Copy the EmissionSystem structures (this does not copy the beam data)
    // ---
    *es = *( getSavedEmissionSystemData() );

    // total length in bytes
    unsigned short totalLength = sizeof(EmissionSystem);

    // ---
    // Copy the emitter beam data, plus the track/jam targets
    // ---

    // The EmitterBeamData structures follow the EmissionSystem structure
    unsigned char* p = reinterpret_cast<unsigned char*>(es) + sizeof(EmissionSystem);

    for (unsigned int ib = 0; ib < es->numberOfBeams && ib < MAX_EM_BEAMS; ib++) {

      // Copy emitter beam data
      EmitterBeamData* eb = reinterpret_cast<EmitterBeamData*>(p);
      *eb = *getSavedEmitterBeamData(ib);
      p += sizeof(EmitterBeamData);

      // The TrackJamTargets structures follow their EmitterBeamData structure
      int n = getSavedEmitterBeamData(ib)->numberOfTargetsInTrack;
      for (int it = 0; it < n && it < MAX_TARGETS_IN_TJ_FIELD; it++) {

         // Copy the Track/Jam target data
         TrackJamTargets* tjt = reinterpret_cast<TrackJamTargets*>(p);
         *tjt = *getSavedTrackJamTargetData(ib,it);
         p += sizeof(TrackJamTargets);
      }

      totalLength += (getSavedEmitterBeamData(ib)->beamDataLength*4);
    }

    // ---
    // Emission system data length is the size of the EmissionSystem structure
    // plus the emitter beam structures, which includes their track/jam targets.
    // ---
    es->systemDataLength = static_cast<unsigned char>(totalLength/4);   // in 32 bit words

    return totalLength;    // Returning the length of the Emission system data in bytes
}

} // End dis namespace
} // End network namespace
} // End oe namespace
