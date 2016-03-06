//------------------------------------------------------------------------------
// Class: Nib
// Description: Portions of class defined to support entity state PDUs
//------------------------------------------------------------------------------

#include "openeaagles/networks/dis/NetIO.h"
#include "openeaagles/networks/dis/Nib.h"
#include "openeaagles/networks/dis/Ntm.h"
#include "openeaagles/networks/dis/pdu.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/GroundVehicle.h"
#include "openeaagles/simulation/LifeForms.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Station.h"
#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/base/Nav.h"
#include "openeaagles/base/NetHandler.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"

#include <cstdio>
#include <cmath>
#include <cstring>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace network {
namespace dis {

static const unsigned int CAMOUFLAGE_BIT  = 0x00000001;   // Paint scheme (camouflage) bit (0 - Uniform color; 1 - Camouflaged)
static const unsigned int FLAMES_BIT      = 0x00008000;   // Flames appearance bit (0 - no flames, 1 - flames present)
static const unsigned int FROZEN_BIT      = 0x00200000;   // Frozen status (0 - not frozen; 1 - frozen)
static const unsigned int POWER_PLANT_BIT = 0x00400000;   // Power-plant status bit (0 - off; 1 - on)
static const unsigned int DEACTIVATE_BIT  = 0x00800000;   // State bit (0 - active; 1 - deactivated)

//------------------------------------------------------------------------------
// entityStatePdu2Nib() -- (Input support)
//  Transfers data from the incoming EntityStatePDU to the base NIB class variables.
//------------------------------------------------------------------------------
void Nib::entityStatePdu2Nib(const EntityStatePDU* const pdu)
{
   NetIO* const disIO = static_cast<NetIO*>(getNetIO());
   simulation::Simulation* sim = disIO->getSimulation();

   // Mark the current time
   setTimeExec( static_cast<LCreal>(sim->getExecTimeSec()) );
   setTimeUtc( static_cast<LCreal>(sim->getSysTimeOfDay()) );

   // Player's ID, site and application codes
   setPlayerID( pdu->entityID.ID );
   setSiteID( pdu->entityID.simulationID.siteIdentification );
   setApplicationID( pdu->entityID.simulationID.applicationIdentification );

   // Get the geocentric position, velocity, acceleration, orientation and angular velocities
   // from the PDU and use them to reset the dead reckoning.
   {
      osg::Vec3d geocPos;
      geocPos[base::Nav::IX] = pdu->entityLocation.X_coord;
      geocPos[base::Nav::IY] = pdu->entityLocation.Y_coord;
      geocPos[base::Nav::IZ] = pdu->entityLocation.Z_coord;
      //std::cout << "entityStatePdu2Nib(): geoc POS(";
      //std::cout << geocPos[0] << ", ";
      //std::cout << geocPos[1] << ", ";
      //std::cout << geocPos[2] << ") ";
      //std::cout << std::endl;

      osg::Vec3d geocVel;
      geocVel[base::Nav::IX] = pdu->entityLinearVelocity.component[0];
      geocVel[base::Nav::IY] = pdu->entityLinearVelocity.component[1];
      geocVel[base::Nav::IZ] = pdu->entityLinearVelocity.component[2];
      //std::cout << "entityStatePdu2Nib(): geoc VEL(";
      //std::cout << geocVel[0] << ", ";
      //std::cout << geocVel[1] << ", ";
      //std::cout << geocVel[2] << ") ";
      //std::cout << std::endl;

      osg::Vec3d geocAcc;
      geocAcc[base::Nav::IX] = pdu->DRentityLinearAcceleration.component[0];
      geocAcc[base::Nav::IY] = pdu->DRentityLinearAcceleration.component[1];
      geocAcc[base::Nav::IZ] = pdu->DRentityLinearAcceleration.component[2];

      osg::Vec3d geocAngles;
      geocAngles[base::Nav::IPHI] = pdu->entityOrientation.phi;
      geocAngles[base::Nav::ITHETA] = pdu->entityOrientation.theta;
      geocAngles[base::Nav::IPSI] = pdu->entityOrientation.psi;

      osg::Vec3d arates;
      arates[base::Nav::IX] = pdu->DRentityAngularVelocity.x_axis;
      arates[base::Nav::IY] = pdu->DRentityAngularVelocity.y_axis;
      arates[base::Nav::IZ] = pdu->DRentityAngularVelocity.z_axis;

#if 0
      // (re)initialize the dead reckoning function
      resetDeadReckoning(
         pdu->deadReckoningAlgorithm,
         geocPos,
         geocVel,
         geocAcc,
         geocAngles,
         arates);
#else
      unsigned long disTimeStamp = pdu->header.timeStamp;
      double currentTime = getTimeExec();
      if (disTimeStamp & 0x01) {
          currentTime = getTimeUtc();
      }
      currentTime = std::fmod(currentTime, 3600.0);      // just get seconds after the hour
      double timeStamp = (static_cast<double>(disTimeStamp >> 1)) * 3600.0 / (static_cast<double>(0x7fffffff));
      double relTimeStamp = timeStamp + timeOffset;
      double diffTime = currentTime - relTimeStamp;

      if (std::fabs(diffTime) > 0.1) {     // If we get ahead of ourselves, or first pass
          timeOffset = currentTime - timeStamp;
          relTimeStamp = currentTime;
          diffTime = 0.0;
      }

      // (re)initialize the dead reckoning function
      resetDeadReckoning(
         pdu->deadReckoningAlgorithm,
         geocPos,
         geocVel,
         geocAcc,
         geocAngles,
         arates,
         diffTime);
#endif
   }

   // Frozen?
   bool frz = ((pdu->appearance & FROZEN_BIT) != 0);
   freeze(frz);

   // Damaged?  (Bits 3-4) Standard (IST-CF-03-01, May 5, 2003)
   {
      unsigned int bits = ( (pdu->appearance >> 3) & 0x00000003 );
      if (bits == 3)       setDamage(1.0f);
      else if (bits == 2)  setDamage(0.66f);
      else if (bits == 1)  setDamage(0.33f);
      else  setDamage(0.0);
   }

   // Smoking?  (Bits 5-6) Standard (IST-CF-03-01, May 5, 2003)
   {
      unsigned int bits = ( (pdu->appearance >> 5) & 0x00000003 );
      if (bits == 3)       setSmoke(1.0f);
      else if (bits == 2)  setSmoke(0.66f);
      else if (bits == 1)  setSmoke(0.33f);
      else  setSmoke(0.0);
   }

   // Flames? (Bit 15) Standard (IST-CF-03-01, May 5, 2003)
   {
      if ((pdu->appearance & FLAMES_BIT) != 0)
         setFlames(1.0f);
      else
         setFlames(0.0f);
   }

   // Camouflage type? (land and air only)   (bit 0 -- paint scheme and bits 17-18 for type)
   // Note: air platform appearance bits 17 and 18 are not used, but we're using them the same as land platforms
   {
      unsigned int bits = 0;
      {
         if ((pdu->appearance & CAMOUFLAGE_BIT) != 0) {
            bits = ( (pdu->appearance >> 17) & 0x00000003 );
            bits++;  // Our camouflage type is the DIS appearance bits plus one (because our zero is no camouflage)
         }
      }
      setCamouflageType(bits);
   }

   // Life form states
   {
      unsigned int bits = ( (pdu->appearance >> 16) & 0x0000000f );
      if (getPlayer() != nullptr && getPlayer()->isMajorType(simulation::Player::LIFE_FORM)) {
         simulation::LifeForm* lf = dynamic_cast<simulation::LifeForm*>(getPlayer());
         if (lf != nullptr) {
            // get our life form state (appearance bit 16 - 19)
            if (bits == 1) lf->setActionState(simulation::LifeForm::UPRIGHT_STANDING);
            else if (bits == 2) lf->setActionState(simulation::LifeForm::UPRIGHT_WALKING);
            else if (bits == 3) lf->setActionState(simulation::LifeForm::UPRIGHT_RUNNING);
            else if (bits == 4) lf->setActionState(simulation::LifeForm::KNEELING);
            else if (bits == 5) lf->setActionState(simulation::LifeForm::PRONE);
            else if (bits == 6) lf->setActionState(simulation::LifeForm::CRAWLING);
            else if (bits == 8) lf->setActionState(simulation::LifeForm::PARACHUTING);
            else if (bits == 9) lf->setActionState(simulation::LifeForm::JUMPING);
            else if (bits == 10) lf->setActionState(simulation::LifeForm::SITTING);
            else if (bits == 11) lf->setActionState(simulation::LifeForm::SQUATTING);
            else if (bits == 12) lf->setActionState(simulation::LifeForm::CROUCHING);
            else if (bits == 13) lf->setActionState(simulation::LifeForm::WADING);
            else if (bits == 14) lf->setActionState(simulation::LifeForm::SURRENDER);
            else if (bits == 15) lf->setActionState(simulation::LifeForm::DETAINED);
            else lf->setActionState(simulation::LifeForm::UPRIGHT_STANDING);
         }
      }
   }

   // Active or inactive
   if ((pdu->appearance & DEACTIVATE_BIT) != 0) {
      // Player has just gone inactive
      setMode(simulation::Player::INACTIVE);
   }
   else {
      setMode(simulation::Player::ACTIVE);
   }

   // Process the articulated parameters and attached parts
   processArticulationParameters(pdu);
}

//------------------------------------------------------------------------------
// Process the articulation parameters
//------------------------------------------------------------------------------
void Nib::processArticulationParameters(const EntityStatePDU* const pdu)
{
   simulation::Player* p = getPlayer();
   if ( pdu->numberOfArticulationParameters > 0 && p != nullptr ) {
      simulation::AirVehicle* av = dynamic_cast<simulation::AirVehicle*>(p);
      simulation::GroundVehicle* gv = dynamic_cast<simulation::GroundVehicle*>(p);

      // ---
      // Loop for all articulation parameters ...
      // ---

      for (unsigned int i = 0; i < pdu->numberOfArticulationParameters; i++) {

         const VpArticulatedPart* ap = pdu->getArticulationParameter(i);

         // ---
         // Articulated Parts
         // Note: We're not worried about the 'change' count at this time.  We just
         // slave the IPlayer to the value in the articulated parameter.
         // ---
         if (ap->parameterTypeDesignator == VpArticulatedPart::ARTICULATED_PART) {

            const unsigned int typeClass  = ap->parameterType & 0xffffffe0;
            const unsigned int typeMetric = ap->parameterType & 0x0000001f;
            const LCreal value = ap->parameterValue.value[0];

            switch (typeClass) {

               // Landing gear (air vehicles only)
               case VpArticulatedPart::LANDING_GEAR :
                  if (av != nullptr && typeMetric == VpArticulatedPart::POSITION)
                     av->setGearHandleSwitch(value);
                  break;

                  // Weapon bay door(s) (air vehicles only)
               case VpArticulatedPart::LEFT_WEAPON_BAY_DOOR :
               case VpArticulatedPart::RIGHT_WEAPON_BAY_DOOR :
                  if (av != nullptr && typeMetric == VpArticulatedPart::POSITION)
                     av->setWeaponBayDoorSwitch(value);
                  break;

                  // Wing sweep (air vehicles only)
               case VpArticulatedPart::WING_SWEEP :
                  if (av != nullptr && typeMetric == VpArticulatedPart::AZIMUTH)
                     av->setCmdWingSweepAngle(value);
                  break;

                  // Primary launcher position #1 (ground vehicles only)
               case VpArticulatedPart::PRIMARY_LAUNCHER_1 :
                  if (gv != nullptr && typeMetric == VpArticulatedPart::ELEVATION)
                     gv->setLauncherPosition(value);
                  break;

            }
         }

         // ---
         // Attached Parts (for now: only weapons on stations 1 .. MAX_AMSL)
         // Note: we'll need to create a StoresMgr for this IPlayer and
         // we'll need to lookup the weapon using the list of incoming
         // entity types and add it to the stores manager's stores list.
         // ---
         else {
            unsigned int sta = ap->parameterType; // station number

            if (sta >= 1 && sta <= MAX_AMSL) {
               simulation::StoresMgr* sms = p->getStoresManagement();

               // If needed, create the stores manager
               if (sms == nullptr) {
                  sms = new simulation::StoresMgr();
                  base::Pair* pair = new base::Pair("storesMgr", sms);
                  sms->unref();   // pair owns it
                  p->addComponent(pair);
                  pair->unref(); // player owns it
               }

               // We need to make sure we have the attached weapon and that it's
               // either in INACTIVE mode (not launched) or LAUNCHED mode (kind == 0)
               if (sms != nullptr) {

                  simulation::Weapon* wpn = nullptr;

                  // find the weapon at station 'sta'
                  base::PairStream* stores = sms->getStores();
                  if (stores != nullptr) {
                     base::List::Item* item = stores->getFirstItem();
                     while (item != nullptr && wpn == nullptr) {
                        unsigned int s = 0;
                        base::Pair* pair = static_cast<base::Pair*>(item->getValue());
                        const base::Identifier* slot = pair->slot();
                        if (slot->isNumber()) s = static_cast<unsigned int>(slot->getNumber());
                        if (s == sta) {
                           wpn = static_cast<simulation::Weapon*>(pair->object());  // Found it
                        }
                        item = item->getNext();
                     }
                     stores->unref();
                  }

                  // There's a weapon attached?
                  if (ap->parameterValue.entityType.kind != 0) {

                     // Yes ...

                     // But if we don't have the weapon yet then we'll need to look it up
                     // using our list of incoming entity types and add it to the SMS.
                     if (wpn == nullptr) {
                        NetIO* disIO = static_cast<NetIO*>(getNetIO());
                        const Ntm* ntm = disIO->findNtmByTypeCodes(
                              ap->parameterValue.entityType.kind,
                              ap->parameterValue.entityType.domain,
                              ap->parameterValue.entityType.country,
                              ap->parameterValue.entityType.category,
                              ap->parameterValue.entityType.subcategory,
                              ap->parameterValue.entityType.specific,
                              ap->parameterValue.entityType.extra
                           );
                        if (ntm != nullptr) {
                           const simulation::Player* tp = ntm->getTemplatePlayer();
                           if (tp != nullptr && tp->isClassType(typeid(simulation::Weapon)) ) {
                              // We've found the weapon that matches the entity type,
                              // so clone it and add it to the SMS with the correct
                              // station number
                              wpn = static_cast<simulation::Weapon*>(tp->clone());  // clone and cast to a Weapon
                              char cbuf[20];
                              std::sprintf(cbuf,"%i",sta);
                              base::Pair* pair = new base::Pair(cbuf, wpn);
                              wpn->unref();   // pair owns it
                              sms->addComponent(pair);
                              pair->unref(); // sms owns it
                           }
                        }
                     }

                     // If we have the weapon then set it INACTIVE (not launched)
                     if (wpn != nullptr) wpn->setMode(simulation::Player::INACTIVE);

                  }

                  // No weapon attached, so set our weapon (if any) to launched!
                  else if (wpn != nullptr) {
                     wpn->setMode(simulation::Player::LAUNCHED);
                  }

               } // end of SMS != 0 check

            } // end of station number range check

         } // end of attached parts

      } // for all articulation parameters

   } // if num articulation parameters > 0
}

//------------------------------------------------------------------------------
// entityStateManager() --  (Output support)
//    -- Update the entity object for this NIB(Player)
//------------------------------------------------------------------------------
bool Nib::entityStateManager(const LCreal curExecTime)
{
   bool ok = false;

   // Get the player pointer
   const simulation::Player* player = getPlayer();
   if (player == nullptr) return ok;

   // Dummy weapon?
   const simulation::Weapon* ww = dynamic_cast<const simulation::Weapon*>( player );
   if (ww != nullptr) {
      if (ww->isDummy()) return ok;
   }

   if (isPlayerStateUpdateRequired(curExecTime)) {

      //
      // Send an entity state PDU
      //   1) create buffer
      //   2) set state information
      //   3) send data
      //

      // Get our NetIO and the main simulation
      NetIO* disIO = static_cast<NetIO*>(getNetIO());
      simulation::Simulation* sim = disIO->getSimulation();

      // Capture the player data, reset the dead reckoning and
      // mark the current time.
      playerState2Nib();

      // ---
      // Create buffer and cast it as an entity state PDU
      // ---
      char pduBuffer[NetIO::MAX_PDU_SIZE];
      EntityStatePDU* pdu = (EntityStatePDU*) &pduBuffer[0];

      //
      // Entity state PDU structure
      // =========================================================
      // PDUHeader            header;
      // EntityIdentifierDIS  entityID;
      // uint8_t              forceID;
      // uint8_t              numberOfArticulationParameters;
      // EntityType           entityType;
      // EntityType           alternativeType;
      // VectorDIS            entityLinearVelocity;
      // WorldCoordinates     entityLocation;
      // EulerAngles          entityOrientation;
      // uint32_t             appearance;
      // uint8_t              deadReckoningAlgorithm;
      // uint8_t              otherParameters[15];
      // VectorDIS            DRentityLinearAcceleration;
      // AngularVelocityVectorDIS DRentityAngularVelocity;
      // EntityMarking        entityMarking;
      // uint32_t             capabilities;
      // =========================================================
      //

      // ---
      // Standard header (PDUHeader)
      // ---
      pdu->header.protocolVersion = disIO->getVersion();
      pdu->header.exerciseIdentifier = disIO->getExerciseID();
      pdu->header.PDUType = NetIO::PDU_ENTITY_STATE;
      pdu->header.protocolFamily = NetIO::PDU_FAMILY_ENTITY_INFO;
      //
      if (disIO->getTimeline() == simulation::NetIO::UTC)
         pdu->header.timeStamp = disIO->makeTimeStamp( getTimeUtc(), true );
      else
         pdu->header.timeStamp = disIO->makeTimeStamp( getTimeExec(), false );
      //
      pdu->header.status = 0;
      pdu->header.padding = 0;

      // ---
      // Entity ID (EntityIdentifierID)
      // ---
      pdu->entityID.simulationID.siteIdentification = getSiteID();
      pdu->entityID.simulationID.applicationIdentification = getApplicationID();
      pdu->entityID.ID = getPlayerID();

      // ---
      // Force ID: When mapping Player side to force IDs ...
      // ---
      if (getSide() == simulation::Player::BLUE) {
         // blue's are friendly, ...
         pdu->forceID = NetIO::FRIENDLY_FORCE;
      }
      else if (getSide() == simulation::Player::RED) {
         // red's are not, ...
         pdu->forceID = NetIO::OPPOSING_FORCE;
      }
      else if (getSide() == simulation::Player::WHITE) {
         // white is neutral, ...
         pdu->forceID = NetIO::NEUTRAL_FORCE;
      }
      else {
         // and everyone else is type OTHER.
         pdu->forceID = NetIO::OTHER_FORCE;
      }

      // ---
      // Entity type (EntityType)
      // ---
      pdu->entityType.kind                 = getEntityKind();
      pdu->entityType.domain               = getEntityDomain();
      pdu->entityType.country              = getEntityCountry();
      pdu->entityType.category             = getEntityCategory();
      pdu->entityType.subcategory          = getEntitySubcategory();
      pdu->entityType.specific             = getEntitySpecific();
      pdu->entityType.extra                = getEntityExtra();
      // ---
      // Alternative type (EntityType)
      // ---
      pdu->alternativeType.kind            = getEntityKind();
      pdu->alternativeType.domain          = getEntityDomain();
      pdu->alternativeType.country         = getEntityCountry();
      pdu->alternativeType.category        = getEntityCategory();
      pdu->alternativeType.subcategory     = getEntitySubcategory();
      pdu->alternativeType.specific        = getEntitySpecific();
      pdu->alternativeType.extra           = getEntityExtra();

      // ---
      // Player position and orientation state data data
      // 1) All data is geocentric (ECEF)
      // 2) The playerState2Nib() function, which was called above, captures
      //    the state data and passed the state data to the dead reckoning
      //    system, and we're using this DR captured data.
      // ---
      {
         // ---
         // Entity linear velocity (VectorDIS)
         // ---
         osg::Vec3d geocVel = getDrVelocity();
         pdu->entityLinearVelocity.component[0] = static_cast<float>(geocVel[0]);
         pdu->entityLinearVelocity.component[1] = static_cast<float>(geocVel[1]);
         pdu->entityLinearVelocity.component[2] = static_cast<float>(geocVel[2]);

         // ---
         // Entity location (WorldCoordinates)
         // ---
         osg::Vec3d geocPos = getDrPosition();
         pdu->entityLocation.X_coord = geocPos[base::Nav::IX];
         pdu->entityLocation.Y_coord = geocPos[base::Nav::IY];
         pdu->entityLocation.Z_coord = geocPos[base::Nav::IZ];

         // ---
         // Entity orientation (EulerAngles)
         // ---
         osg::Vec3d geocAngles = getDrEulerAngles();
         pdu->entityOrientation.phi   = static_cast<float>(geocAngles[base::Nav::IPHI]);
         pdu->entityOrientation.theta = static_cast<float>(geocAngles[base::Nav::ITHETA]);
         pdu->entityOrientation.psi   = static_cast<float>(geocAngles[base::Nav::IPSI]);
      }

      // ---
      // Appearance bits generic to all domains (except munitions)
      // ---
      {
         pdu->appearance = 0x0;

         // ---
         // Frozen?
         // ---
         if ( isFrozen() || sim->isFrozen() ) {
            pdu->appearance |= FROZEN_BIT;
         }

         // Deactive this entity?
         {
            if (isMode(simulation::Player::DELETE_REQUEST) || player->isDead() )
               pdu->appearance |= DEACTIVATE_BIT;
         }


         // Damage or health?  (Bits 3-4)
         {
            unsigned int bits = 0;
            if (getDamage() > 0.9f) bits = 3;       // Destroyed or Fatality
            else if (getDamage() > 0.5) bits = 2;   // Moderate
            else if (getDamage() > 0.0) bits = 1;   // Slight
            else bits = 0;                          // None
            pdu->appearance |= (bits << 3);
         }


         // Camouflage type
         // Note: air platform appearance bits 17 and 18 are not used, but we're using them the same as land platforms
         {
            unsigned int bits = getCamouflageType();
            if (bits > 0 && bits <= 4) {
               pdu->appearance |= CAMOUFLAGE_BIT;

               // Land based camouflage bits
               if (player->isMajorType(simulation::Player::GROUND_VEHICLE)) {
                  // Subtract one to match DIS camouflage bits.
                  // Our camouflage type for DIS is the camouflage appearance bits
                  // plus one because our camouflage type of zero is no camouflage.
                  bits--;
                  pdu->appearance |= (bits << 17);
               }
            }
         }

         // Life forms appearance bits
         if (player->isMajorType(simulation::Player::LIFE_FORM)) {
            const simulation::LifeForm* lf = dynamic_cast<const simulation::LifeForm*>(player);
            if (lf != nullptr) {
               // Health (aka damaged for other domains) same bits (3-4) - this is from the NIB, because it IS
               // updated
               // bits 5-8 compliance (not implemented)
               // bits 9 - 11 unused
               // bit 12 flashlight (not implemented)
               // bits 13-15 unused
               // bits 16 - 19 life form state
               // data is from the player, because NIB doesn't have actions associated with it
               {
                  unsigned int bits = 1;      // upright, standing still
                  if (lf->getActionState() == simulation::LifeForm::UPRIGHT_STANDING) bits = 1;       // standing
                  else if (lf->getActionState() == simulation::LifeForm::UPRIGHT_WALKING) bits = 2;   // walking
                  else if (lf->getActionState() == simulation::LifeForm::UPRIGHT_RUNNING) bits = 3;   // running
                  else if (lf->getActionState() == simulation::LifeForm::KNEELING) bits = 4;          // kneeling
                  else if (lf->getActionState() == simulation::LifeForm::PRONE) bits = 5;             // prone
                  else if (lf->getActionState() == simulation::LifeForm::CRAWLING) bits = 6;          // crawling
                  else if (lf->getActionState() == simulation::LifeForm::SWIMMING) bits = 7;          // swimming
                  else if (lf->getActionState() == simulation::LifeForm::PARACHUTING) bits = 8;       // parachuting
                  else if (lf->getActionState() == simulation::LifeForm::JUMPING) bits = 9;           // jumping
                  else if (lf->getActionState() == simulation::LifeForm::SITTING) bits = 10;          // sitting
                  else if (lf->getActionState() == simulation::LifeForm::SQUATTING) bits = 11;        // squatting
                  else if (lf->getActionState() == simulation::LifeForm::CROUCHING) bits = 12;        // crouching
                  else if (lf->getActionState() == simulation::LifeForm::WADING) bits = 13;           // wading
                  else if (lf->getActionState() == simulation::LifeForm::SURRENDER) bits = 14;        // surrender
                  else if (lf->getActionState() == simulation::LifeForm::DETAINED) bits = 15;         // detained
                  else bits = 1;
                  pdu->appearance |= (bits << 16);
               }
               // bit 20 unused
               // bit 21 frozen status (taken care of above)
               // bits 24 - 25 weapon 1 (not implemented)
               // bits 26-27 weapon 2 (N/I)
               // bits 28-29
            }
         }

         // Common Non-life form appearance bits
         else {

            // Smoking?  (Bits 5-6) Standard (IST-CF-03-01, May 5, 2003)
            {
               unsigned int bits = 0;
               if (getSmoke() > 0.9f) bits = 3;
               else if (getSmoke() > 0.5) bits = 2;
               else if (getSmoke() > 0.0) bits = 1;
               else bits = 0;
               pdu->appearance |= (bits << 5);
            }

            // Flames? (Bit 15) Standard (IST-CF-03-01, May 5, 2003)
            {
               if (getFlames() > 0.5f)
                  pdu->appearance |= FLAMES_BIT;
            }

            // Power plant status bit (just leave ON for now)
            pdu->appearance |= POWER_PLANT_BIT;
         }


      }

      // ---
      // Dead reckoning algorithm
      // ---
      pdu->deadReckoningAlgorithm = static_cast<unsigned char>(getDeadReckoning());

      // ---
      // Other parameters
      // ---
      for (unsigned int i=0; i<15; i++) {
          pdu->otherParameters[i] = 0;
      }

      // ---
      // Dead reckoning information
      // ---
      {
         // ---
         // Dead reckoning linear acceleration (VectorDIS)
         // ---
         osg::Vec3d geocAcc = getDrAcceleration();
         pdu->DRentityLinearAcceleration.component[0] = static_cast<float>(geocAcc[0]);
         pdu->DRentityLinearAcceleration.component[1] = static_cast<float>(geocAcc[1]);
         pdu->DRentityLinearAcceleration.component[2] = static_cast<float>(geocAcc[2]);

         // ---
         // Dead reckoning angular velocity (AngularVelocityVectorDIS)
         // ---
         osg::Vec3d geocAngVel = getDrAngularVelocities();
         pdu->DRentityAngularVelocity.x_axis = static_cast<float>(geocAngVel[base::Nav::IX]);
         pdu->DRentityAngularVelocity.y_axis = static_cast<float>(geocAngVel[base::Nav::IY]);
         pdu->DRentityAngularVelocity.z_axis = static_cast<float>(geocAngVel[base::Nav::IZ]);
      }

      // ---
      // Entity marking (EntityMarking)
      // ---
      {
         const char* const pName = getPlayerName();
         size_t nameLen = std::strlen(pName);
         for (unsigned int i = 0; i < EntityMarking::BUFF_SIZE; i++) {
            if (i < nameLen) {
               pdu->entityMarking.marking[i] = pName[i];
            }
            else {
               pdu->entityMarking.marking[i] = '\0';
            }
         }
         pdu->entityMarking.characterSet = 1;
      }

      // ---
      // Capabilities
      // ---
      pdu->capabilites = 0x0;

      // ---
      // Articulation parameters
      // ---
      pdu->numberOfArticulationParameters = manageArticulationParameters(pdu);

      // Size of the PDU package
      unsigned short length = sizeof(EntityStatePDU) + (pdu->numberOfArticulationParameters * sizeof(VpArticulatedPart));
      pdu->header.length = length;

      if (base::NetHandler::isNotNetworkByteOrder()) pdu->swapBytes();
      ok = disIO->sendData( reinterpret_cast<char*>(pdu), length );
   }
   return ok;
}

//------------------------------------------------------------------------------
// Manage the articulated parameters and attached parts
//------------------------------------------------------------------------------
unsigned char Nib::manageArticulationParameters(EntityStatePDU* const pdu)
{
   unsigned char cnt = 0;
   NetIO* const disIO = static_cast<NetIO*>(getNetIO());

   // First articulation parameter is just after the main PDU
   unsigned char *p = (reinterpret_cast<unsigned char *>(pdu)) + sizeof(*pdu);
   VpArticulatedPart* ap = reinterpret_cast<VpArticulatedPart*>(p);

   // ---
   // Air Vehicle articulated parts and attachments
   // ---
   if ( getPlayer()->isMajorType(simulation::Player::AIR_VEHICLE) ) {

      // Check wing sweep angle.
      if (getAPartWingSweepCnt() > 0) {
         // fill the articulation parameter structure
         ap->parameterTypeDesignator = VpArticulatedPart::ARTICULATED_PART;
         ap->changeIndicator = static_cast<unsigned char>(getAPartWingSweepCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (VpArticulatedPart::WING_SWEEP + VpArticulatedPart::AZIMUTH);
         ap->parameterValue.value[0] = static_cast<float>(getAPartWingSweep());  // radians
         ap->parameterValue.value[1] = 0;
         // Update part count & pointer
         cnt++;
         ap++;
      }

      // Check landing gear position.
      if (getAPartGearPosCnt() > 0) {
         // fill the articulation parameter structure
         ap->parameterTypeDesignator = VpArticulatedPart::ARTICULATED_PART;
         ap->changeIndicator = static_cast<unsigned char>(getAPartGearPosCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (VpArticulatedPart::LANDING_GEAR + VpArticulatedPart::POSITION);
         ap->parameterValue.value[0] = static_cast<float>(getAPartPartGearPos())/100.0f;
         ap->parameterValue.value[1] = 0;
         // Update part count & pointer
         cnt++;
         ap++;
      }

      // Check weapon bay door position (send out both doors)
      if (getAPartBayDoorCnt() > 0) {

         // Left door
         ap->parameterTypeDesignator = VpArticulatedPart::ARTICULATED_PART;
         ap->changeIndicator = static_cast<unsigned char>(getAPartBayDoorCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (VpArticulatedPart::LEFT_WEAPON_BAY_DOOR + VpArticulatedPart::POSITION);
         ap->parameterValue.value[0] = static_cast<float>(getAPartBayDoorPos())/100.0f;
         ap->parameterValue.value[1] = 0;

         // Update part count & pointer
         cnt++;
         ap++;

         // Right door
         ap->parameterTypeDesignator = VpArticulatedPart::ARTICULATED_PART;
         ap->changeIndicator = static_cast<unsigned char>(getAPartBayDoorCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (VpArticulatedPart::RIGHT_WEAPON_BAY_DOOR + VpArticulatedPart::POSITION);
         ap->parameterValue.value[0] = static_cast<float>(getAPartBayDoorPos())/100.0f;
         ap->parameterValue.value[1] = 0;

         // Update part count & pointer
         cnt++;
         ap++;

      }

   }

   // ---
   // Ground Vehicle articulated parts and attachments
   // ---
   else if ( getPlayer()->isMajorType(simulation::Player::GROUND_VEHICLE) ) {

      // Check launcher elevation angle
      if (getAPartLauncherElevationCnt() > 0) {
         // fill the articulation parameter structure
         ap->parameterTypeDesignator = VpArticulatedPart::ARTICULATED_PART;
         ap->changeIndicator = static_cast<unsigned char>(getAPartLauncherElevationCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (VpArticulatedPart::PRIMARY_LAUNCHER_1 + VpArticulatedPart::ELEVATION);
         ap->parameterValue.value[0] = static_cast<float>( getAPartLauncherElevation() );
         ap->parameterValue.value[1] = 0;
         // Update part count & pointer
         cnt++;
         ap++;
      }

      // Check attached missiles
      unsigned int n = getAPartNumberAttachedNumMissiles();
      for (unsigned int i = 0; i < n; i++) {

         const simulation::Missile* msl = getAPartAttachedMissile(i+1);

         // Find the missile's entity type
         if (apartMslTypes[i] == nullptr) {

            const Ntm* ntm = dynamic_cast<const Ntm*>( disIO->findNetworkTypeMapper(msl) );
            if (ntm != nullptr) {

               // found the NTM for the missile -- and it must be a DIS NTM
               apartMslTypes[i] = dynamic_cast<const Ntm*>(ntm);
               if (apartMslTypes[i] != nullptr) apartMslTypes[i]->ref();
            }

         }

         // We must have the missile's entity type to send the attached part ...
         if (apartMslTypes[i] != nullptr) {

            // fill the articulation parameter structure
            ap->parameterTypeDesignator = VpArticulatedPart::ATTACHED_PART;
            ap->changeIndicator = static_cast<unsigned char>(getAPartAttacheMissileChangeCnt(i+1) & 0xff);
            ap->id = 1;                   // ATTACHED to LAUNCHER (above)
            ap->parameterType = (i+1);    // Station number
            if (msl->isMode(simulation::Player::LAUNCHED)) {
               ap->parameterValue.entityType.kind = 0;
               ap->parameterValue.entityType.domain = 0;
               ap->parameterValue.entityType.country = 0;
               ap->parameterValue.entityType.category = 0;
               ap->parameterValue.entityType.subcategory = 0;
               ap->parameterValue.entityType.specific = 0;
               ap->parameterValue.entityType.extra = 0;
            }
            else {
               ap->parameterValue.entityType.kind = apartMslTypes[i]->getEntityKind();
               ap->parameterValue.entityType.domain = apartMslTypes[i]->getEntityDomain();
               ap->parameterValue.entityType.country = apartMslTypes[i]->getEntityCountry();
               ap->parameterValue.entityType.category = apartMslTypes[i]->getEntityCategory();
               ap->parameterValue.entityType.subcategory = apartMslTypes[i]->getEntitySubcategory();
               ap->parameterValue.entityType.specific = apartMslTypes[i]->getEntitySpecific();
               ap->parameterValue.entityType.extra = apartMslTypes[i]->getEntityExtra();
            }

            // Update part count & pointer
            cnt++;
            ap++;
         }
      }
   }

   return cnt;
}

} // End dis namespace
} // End Network namespace
} // End oe namespace
