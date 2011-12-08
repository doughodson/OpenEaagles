//==============================================================================
// Entity State PDUs (Portions of NetIO and Nib)
//==============================================================================

#include "openeaagles/dis/NetIO.h"
#include "openeaagles/dis/Nib.h"
#include "openeaagles/dis/Ntm.h"
#include "openeaagles/dis/pdu.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/GroundVehicle.h"
#include "openeaagles/simulation/LifeForms.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Station.h"
#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Network {
namespace Dis {

static const unsigned int CAMOUFLAGE_BIT  = 0x00000001;   // Paint scheme (camouflage) bit (0 - Uniform color; 1 - Camouflaged)
static const unsigned int FLAMES_BIT      = 0x00008000;   // Flames appearance bit (0 - no flames, 1 - flames present)
static const unsigned int FROZEN_BIT      = 0x00200000;   // Frozen status (0 - not frozen; 1 - frozen)
static const unsigned int POWER_PLANT_BIT = 0x00400000;   // Power-plant status bit (0 - off; 1 - on)
static const unsigned int DEACTIVATE_BIT  = 0x00800000;   // State bit (0 - active; 1 - deactivated)

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

    // Reject PDUs with our appication and site IDs
    if (site == getSiteID() &&  app == getApplicationID()) return;

    // Search test (reject PDUs from players on our output list)
    Simulation::Nib* testNib = findDisNib(playerId, site, app, OUTPUT_NIB);
    if (testNib != 0) return;

    // ---
    // Find the Network Interface Block
    // ---
    Nib* nib = static_cast<Nib*>( findDisNib(playerId, site, app, INPUT_NIB) );
    
    // --- 
    // When we don't have a NIB, create one
    // --- 
    if (nib == 0) {
        nib = static_cast<Nib*>( createNewInputNib() );
        if (nib != 0) {
            nib->setPlayerID(playerId);
            if (pdu->entityMarking.characterSet == 1) {
               char name[12];
               lcStrcpy(name, 12, (char*)pdu->entityMarking.marking);
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
               Basic::String* fname = new Basic::String(cbuff);
               nib->setFederateName(fname);
               fname->unref();
            }

            nib->setDeadReckoning( Simulation::Nib::DeadReckoning( pdu->deadReckoningAlgorithm ) );

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
                nib->setSide(Simulation::Player::BLUE);
            }
            else if (pdu->forceID == OPPOSING_FORCE) {
                // opposing side is red, ...
                nib->setSide(Simulation::Player::RED);
            }
            else if (pdu->forceID == NEUTRAL_FORCE) {
                // Neutrals are white, ...
                nib->setSide(Simulation::Player::WHITE);
            }
            else  {
                // and everyone else is gray.
                nib->setSide(Simulation::Player::GRAY);
            }
            
            addNib2InputList(nib);
            nib->unref();
        }
    }


    // --- 
    // When we have a NIB, transfer our packet data to it.
    // --- 
    if (nib != 0) {
       nib->entityStatePdu2Nib(pdu);
    }
}


//------------------------------------------------------------------------------
// entityStatePdu2Nib() -- (Input support)
//  Transfers data from the incoming EntityStatePDU to the base NIB class variables.
//------------------------------------------------------------------------------
void Nib::entityStatePdu2Nib(const EntityStatePDU* const pdu)
{
   NetIO* const disIO = (NetIO*)(getNetIO());
   Simulation::Simulation* sim = disIO->getSimulation();

   // Mark the current time
   setTimeExec( (LCreal) sim->getExecTimeSec() );
   setTimeUtc( (LCreal) sim->getSysTimeOfDay() );

   // Player's ID, site and application codes
   setPlayerID( pdu->entityID.ID );
   setSiteID( pdu->entityID.simulationID.siteIdentification );
   setApplicationID( pdu->entityID.simulationID.applicationIdentification );

   // Get the geocentric position, velocity, acceleration, orientation and angular velocities
   // from the PDU and use them to reset the dead reckoning.
   {
      osg::Vec3d geocPos;
      geocPos[Basic::Nav::IX] = pdu->entityLocation.X_coord;
      geocPos[Basic::Nav::IY] = pdu->entityLocation.Y_coord;
      geocPos[Basic::Nav::IZ] = pdu->entityLocation.Z_coord;
      //std::cout << "entityStatePdu2Nib(): geoc POS(";
      //std::cout << geocPos[0] << ", ";
      //std::cout << geocPos[1] << ", ";
      //std::cout << geocPos[2] << ") ";
      //std::cout << std::endl;

      osg::Vec3d geocVel;
      geocVel[Basic::Nav::IX] = pdu->entityLinearVelocity.component[0];
      geocVel[Basic::Nav::IY] = pdu->entityLinearVelocity.component[1];
      geocVel[Basic::Nav::IZ] = pdu->entityLinearVelocity.component[2];
      //std::cout << "entityStatePdu2Nib(): geoc VEL(";
      //std::cout << geocVel[0] << ", ";
      //std::cout << geocVel[1] << ", ";
      //std::cout << geocVel[2] << ") ";
      //std::cout << std::endl;

      osg::Vec3d geocAcc;
      geocAcc[Basic::Nav::IX] = pdu->DRentityLinearAcceleration.component[0];
      geocAcc[Basic::Nav::IY] = pdu->DRentityLinearAcceleration.component[1];
      geocAcc[Basic::Nav::IZ] = pdu->DRentityLinearAcceleration.component[2];

      osg::Vec3d geocAngles;
      geocAngles[Basic::Nav::IPHI] = pdu->entityOrientation.phi;
      geocAngles[Basic::Nav::ITHETA] = pdu->entityOrientation.theta;
      geocAngles[Basic::Nav::IPSI] = pdu->entityOrientation.psi; 

      osg::Vec3d arates;
      arates[Basic::Nav::IX] = pdu->DRentityAngularVelocity.x_axis;
      arates[Basic::Nav::IY] = pdu->DRentityAngularVelocity.y_axis;
      arates[Basic::Nav::IZ] = pdu->DRentityAngularVelocity.z_axis;

      // (re)initialize the dead reckoning function
      resetDeadReckoning(
         pdu->deadReckoningAlgorithm,
         geocPos,
         geocVel,
         geocAcc,
         geocAngles,
         arates);
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
      unsigned int bits = ( (pdu->appearance >> 16) & 0x00000004 );        
      if (getPlayer() != 0 && getPlayer()->isMajorType(Simulation::Player::LIFE_FORM)) {
         Simulation::LifeForm* lf = dynamic_cast<Simulation::LifeForm*>(getPlayer());
         if (lf != 0) {
            // get our life form state (appearance bit 16 - 19)
            if (bits == 1) lf->setActionState(Simulation::LifeForm::UPRIGHT_STANDING);
            else if (bits == 2) lf->setActionState(Simulation::LifeForm::UPRIGHT_WALKING);
            else if (bits == 3) lf->setActionState(Simulation::LifeForm::UPRIGHT_RUNNING);
            else if (bits == 4) lf->setActionState(Simulation::LifeForm::PARACHUTING);
            else lf->setActionState(Simulation::LifeForm::UPRIGHT_STANDING);                    
         }
      }
   }

   // Active or inactive
   if ((pdu->appearance & DEACTIVATE_BIT) != 0) {
      // Player has just gone inactive
      setMode(Simulation::Player::INACTIVE);
   }
   else {
      setMode(Simulation::Player::ACTIVE);
   }

   // Process the articulated parameters and attached parts
   processArticulationParameters(pdu);
}

//------------------------------------------------------------------------------
// Process the articulation parameters 
//------------------------------------------------------------------------------
void Nib::processArticulationParameters(const EntityStatePDU* const pdu)
{
   Simulation::Player* p = getPlayer();
   if ( pdu->numberOfArticulationParameters > 0 && p != 0 ) {
      Simulation::AirVehicle* av = dynamic_cast<Simulation::AirVehicle*>(p);
      Simulation::GroundVehicle* gv = dynamic_cast<Simulation::GroundVehicle*>(p);

      // ---
      // Loop for all articulation parameters ...
      // ---

      for (unsigned int i = 0; i < pdu->numberOfArticulationParameters; i++) {

         const ArticulationParameter* ap = pdu->getArticulationParameter(i);

         // ---
         // Articulated Parts
         // Note: We're not worried about the 'change' count at this time.  We just
         // slave the IPlayer to the value in the articulated parameter.
         // ---
         if (ap->parameterTypeDesignator == ArticulationParameter::ARTICULATED_PART) {

            const unsigned int typeClass  = ap->parameterType & 0xffffffe0;
            const unsigned int typeMetric = ap->parameterType & 0x0000001f;
            const LCreal value = ap->parameterValue.value[0];

            switch (typeClass) {

               // Landing gear (air vehicles only)
               case ArticulationParameter::LANDING_GEAR :
                  if (av != 0 && typeMetric == ArticulationParameter::POSITION)
                     av->setGearHandleSwitch(value);
                  break;

                  // Weapon bay door(s) (air vehicles only)
               case ArticulationParameter::LEFT_WEAPON_BAY_DOOR :
               case ArticulationParameter::RIGHT_WEAPON_BAY_DOOR :
                  if (av != 0 && typeMetric == ArticulationParameter::POSITION)
                     av->setWeaponBayDoorSwitch(value);
                  break;

                  // Wing sweep (air vehicles only)
               case ArticulationParameter::WING_SWEEP :
                  if (av != 0 && typeMetric == ArticulationParameter::AZIMUTH)
                     av->setCmdWingSweepAngle(value);
                  break;

                  // Primary launcher position #1 (ground vehicles only)
               case ArticulationParameter::PRIMARY_LAUNCHER_1 :
                  if (gv != 0 && typeMetric == ArticulationParameter::ELEVATION)
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
               Simulation::StoresMgr* sms = p->getStoresManagement();

               // If needed, create the stores manager
               if (sms == 0) {
                  sms = new Simulation::StoresMgr();
                  Basic::Pair* pair = new Basic::Pair("storesMgr", sms);
                  sms->unref();   // pair owns it
                  p->addComponent(pair);
                  pair->unref(); // player owns it
               }

               // We need to make sure we have the attached weapon and that it's
               // either in INACTIVE mode (not launched) or LAUNCHED mode (kind == 0)
               if (sms != 0) {

                  Simulation::Weapon* wpn = 0;

                  // find the weapon at station 'sta'
                  Basic::PairStream* stores = sms->getStores();
                  if (stores != 0) {
                     Basic::List::Item* item = stores->getFirstItem();
                     while (item != 0 && wpn == 0) {
                        unsigned int s = 0;
                        Basic::Pair* pair = (Basic::Pair*) item->getValue();
                        const Basic::Identifier* slot = pair->slot();
                        if (slot->isNumber()) s = (unsigned int) slot->getNumber();
                        if (s == sta) {
                           wpn = (Simulation::Weapon*) pair->object();  // Found it
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
                     if (wpn == 0) {
                        NetIO* disIO = (NetIO*)(getNetIO());
                        const Ntm* ntm = disIO->findNtmByTypeCodes(
                              ap->parameterValue.entityType.kind,
                              ap->parameterValue.entityType.domain,
                              ap->parameterValue.entityType.country,
                              ap->parameterValue.entityType.category,
                              ap->parameterValue.entityType.subcategory,
                              ap->parameterValue.entityType.specific,
                              ap->parameterValue.entityType.extra
                           );
                        if (ntm != 0) {
                           const Simulation::Player* tp = ntm->getTemplatePlayer();
                           if (tp != 0 && tp->isMajorType(Simulation::Player::WEAPON)) {
                              // We've found the weapon that matches the entity type,
                              // so clone it and add it to the SMS with the correct
                              // station number
                              wpn = (Simulation::Weapon*) tp->clone();
                              char cbuf[20];
                              std::sprintf(cbuf,"%i",sta);
                              Basic::Pair* pair = new Basic::Pair(cbuf, wpn);
                              wpn->unref();   // pair owns it
                              sms->addComponent(pair);
                              pair->unref(); // sms owns it
                           }
                        }
                     }

                     // If we have the weapon then set it INACTIVE (not launched)
                     if (wpn != 0) wpn->setMode(Simulation::Player::INACTIVE);

                  }

                  // No weapon attached, so set our weapon (if any) to launched!
                  else if (wpn != 0) {
                     wpn->setMode(Simulation::Player::LAUNCHED);
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
   const Simulation::Player* player = getPlayer();
   if (player == 0) return ok;

   // Dummy weapon?
   const Simulation::Weapon* ww = dynamic_cast<const Simulation::Weapon*>( player );
   if (ww != 0) {
      if (ww->isDummy()) return ok;
   }

   if (isPlayerStateUpdateRequired(curExecTime)) {

      // Send a entity state PDU ...

      // Get our NetIO and the main simulation
      NetIO* disIO = (NetIO*)(getNetIO());
      Simulation::Simulation* sim = disIO->getSimulation();

      // Capture the player data, reset the dead reckoning and
      // mark the current time.
      playerState2Nib();

      // ---
      // Standard header stuff
      // ---
      char pduBuffer[NetIO::MAX_PDU_SIZE];
      EntityStatePDU* pdu = (EntityStatePDU*) &pduBuffer[0];

      pdu->header.protocolVersion = disIO->getVersion();
      pdu->header.exerciseIdentifier = disIO->getExerciseID();
      pdu->header.PDUType = NetIO::PDU_ENTITY_STATE;
      pdu->header.protocolFamily = NetIO::PDU_FAMILY_ENTITY_INFO;

      if (disIO->getTimeline() == Simulation::NetIO::UTC)
         pdu->header.timeStamp = disIO->makeTimeStamp( getTimeUtc(), true );
      else
         pdu->header.timeStamp = disIO->makeTimeStamp( getTimeExec(), false );

      // ---
      // Entity ID
      // ---
      pdu->entityID.simulationID.siteIdentification = getSiteID();
      pdu->entityID.simulationID.applicationIdentification = getApplicationID();
      pdu->entityID.ID = getPlayerID();

      // ---
      // Force ID: When mapping Player side to force IDs ...
      // ---
      if (getSide() == Simulation::Player::BLUE) {
         // blue's are friendly, ...
         pdu->forceID = NetIO::FRIENDLY_FORCE;     
      }
      else if (getSide() == Simulation::Player::RED) {
         // red's are not, ...
         pdu->forceID = NetIO::OPPOSING_FORCE;
      }
      else if (getSide() == Simulation::Player::WHITE) {
         // white is neutral, ...
         pdu->forceID = NetIO::NEUTRAL_FORCE;
      }
      else {
         // and everyone else is type OTHER.
         pdu->forceID = NetIO::OTHER_FORCE;
      }

      // ---
      // Entity Type and Info
      // ---
      pdu->entityType.kind                 = getEntityKind();
      pdu->entityType.domain               = getEntityDomain();
      pdu->entityType.country              = getEntityCountry();
      pdu->entityType.category             = getEntityCategory();
      pdu->entityType.subcategory          = getEntitySubcategory();
      pdu->entityType.specific             = getEntitySpecific();
      pdu->entityType.extra                = getEntityExtra();
      pdu->alternativeType.kind            = getEntityKind();
      pdu->alternativeType.domain          = getEntityDomain();
      pdu->alternativeType.country         = getEntityCountry();
      pdu->alternativeType.category        = getEntityCategory();
      pdu->alternativeType.subcategory     = getEntitySubcategory();
      pdu->alternativeType.specific        = getEntitySpecific();
      pdu->alternativeType.extra           = getEntityExtra();
      pdu->deadReckoningAlgorithm          = (unsigned char) getDeadReckoning();
      pdu->capabilites                     = 0x0;

      {
         const char* const pName = getPlayerName();
         size_t nameLen = strlen(pName);
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
      // Player position and orientation state data data
      // 1) All data is geocentric (ECEF)
      // 2) The playerState2Nib() function, which was called above, captures
      //    the state data and passed the state data to the dead reckoning
      //    system, and we're using this DR caputred data.
      // ---
      {
         // Entity Location
         osg::Vec3d geocPos = getDrPosition();
         pdu->entityLocation.X_coord = geocPos[Basic::Nav::IX];
         pdu->entityLocation.Y_coord = geocPos[Basic::Nav::IY];
         pdu->entityLocation.Z_coord = geocPos[Basic::Nav::IZ];

         // Linear Velocity
         osg::Vec3d geocVel = getDrVelocity();
         pdu->entityLinearVelocity.component[0] = (float)geocVel[0];
         pdu->entityLinearVelocity.component[1] = (float)geocVel[1];
         pdu->entityLinearVelocity.component[2] = (float)geocVel[2];
         //pdu->entityLinearVelocity.component[0] = 0;
         //pdu->entityLinearVelocity.component[1] = 0;
         //pdu->entityLinearVelocity.component[2] = 0;

         // Linear Acceleration
         osg::Vec3d geocAcc = getDrAcceleration();
         pdu->DRentityLinearAcceleration.component[0] = (float)geocAcc[0];
         pdu->DRentityLinearAcceleration.component[1] = (float)geocAcc[1];
         pdu->DRentityLinearAcceleration.component[2] = (float)geocAcc[2];
         //pdu->DRentityLinearAcceleration.component[0] = 0;
         //pdu->DRentityLinearAcceleration.component[1] = 0;
         //pdu->DRentityLinearAcceleration.component[2] = 0;

         // Orientation
         osg::Vec3d geocAngles = getDrEulerAngles();
         pdu->entityOrientation.phi   = (float)geocAngles[Basic::Nav::IPHI];
         pdu->entityOrientation.theta = (float)geocAngles[Basic::Nav::ITHETA];
         pdu->entityOrientation.psi   = (float)geocAngles[Basic::Nav::IPSI];

         // Angular velocities
         osg::Vec3d geocAngVel = getDrAngularVelocities();
         pdu->DRentityAngularVelocity.x_axis = (float)geocAngVel[Basic::Nav::IX];
         pdu->DRentityAngularVelocity.y_axis = (float)geocAngVel[Basic::Nav::IY];
         pdu->DRentityAngularVelocity.z_axis = (float)geocAngVel[Basic::Nav::IZ];
      }

      // ---
      // appearance bits generic to all domains (except munitions)
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
            if (isMode(Simulation::Player::DELETE_REQUEST) || player->isDead() )
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
               if (player->isMajorType(Simulation::Player::GROUND_VEHICLE)) {
                  // Subtract one to match DIS comouflage bits.
                  // Our camouflage type for DIS is the camouflage appearance bits
                  // plus one because our camouflage type of zero is no camouflage.
                  bits--;
                  pdu->appearance |= (bits << 17); 
               }
            }
         }

         // Life forms appearance bits
         if (player->isMajorType(Simulation::Player::LIFE_FORM)) {
            const Simulation::LifeForm* lf = dynamic_cast<const Simulation::LifeForm*>(player);
            if (lf != 0) {
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
                  if (lf->getActionState() == Simulation::LifeForm::UPRIGHT_STANDING) bits = 1;       // standing
                  else if (lf->getActionState() == Simulation::LifeForm::UPRIGHT_WALKING) bits = 2;   // walking
                  else if (lf->getActionState() == Simulation::LifeForm::UPRIGHT_RUNNING) bits = 3;   // running
                  else if (lf->getActionState() == Simulation::LifeForm::PARACHUTING) bits = 8;       // parachuting
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
      // Articulation parameters
      // ---
      pdu->numberOfArticulationParameters = manageArticulationParameters(pdu);
      //pdu->numberOfArticulationParameters = 0;

      // Size of the PDU package
      unsigned short length = sizeof(EntityStatePDU) + (pdu->numberOfArticulationParameters * sizeof(ArticulationParameter));
      pdu->header.length = length;

      if (Basic::NetHandler::isNotNetworkByteOrder()) pdu->swapBytes();
      ok = disIO->sendData( (char*) pdu, length );
   }
   return ok;
}

//------------------------------------------------------------------------------
// Manage the articulated parameters and attached parts
//------------------------------------------------------------------------------
unsigned char Nib::manageArticulationParameters(EntityStatePDU* const pdu)
{
   unsigned char cnt = 0;
   NetIO* const disIO = (NetIO*)(getNetIO());

   // First articulation parameter is just after the main PDU
   unsigned char *p = ((unsigned char *)pdu) + sizeof(*pdu);
   ArticulationParameter* ap = (ArticulationParameter*) p;

   // ---
   // Air Vehicle articulated parts and attachments
   // ---
   if ( getPlayer()->isMajorType(Simulation::Player::AIR_VEHICLE) ) {

      // Check wing sweep angle.
      if (getAPartWingSweepCnt() > 0) {
         // fill the articulation parameter structure
         ap->parameterTypeDesignator = ArticulationParameter::ARTICULATED_PART;
         ap->changeIndicator = (unsigned char) (getAPartWingSweepCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (ArticulationParameter::WING_SWEEP + ArticulationParameter::AZIMUTH);
         ap->parameterValue.value[0] = (float) getAPartWingSweep();  // radians
         ap->parameterValue.value[1] = 0;
         // Update part count & pointer
         cnt++;
         ap++;
      }

      // Check landing gear position.
      if (getAPartGearPosCnt() > 0) {
         // fill the articulation parameter structure
         ap->parameterTypeDesignator = ArticulationParameter::ARTICULATED_PART;
         ap->changeIndicator = (unsigned char) (getAPartGearPosCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (ArticulationParameter::LANDING_GEAR + ArticulationParameter::POSITION);
         ap->parameterValue.value[0] = float(getAPartPartGearPos())/100.0f;
         ap->parameterValue.value[1] = 0;
         // Update part count & pointer
         cnt++;
         ap++;
      }

      // Check weapon bay door position (send out both doors)
      if (getAPartBayDoorCnt() > 0) {

         // Left door
         ap->parameterTypeDesignator = ArticulationParameter::ARTICULATED_PART;
         ap->changeIndicator = (unsigned char) (getAPartBayDoorCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (ArticulationParameter::LEFT_WEAPON_BAY_DOOR + ArticulationParameter::POSITION);
         ap->parameterValue.value[0] = float(getAPartBayDoorPos())/100.0f;
         ap->parameterValue.value[1] = 0;

         // Update part count & pointer
         cnt++;
         ap++;

         // Right door
         ap->parameterTypeDesignator = ArticulationParameter::ARTICULATED_PART;
         ap->changeIndicator = (unsigned char) (getAPartBayDoorCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (ArticulationParameter::RIGHT_WEAPON_BAY_DOOR + ArticulationParameter::POSITION);
         ap->parameterValue.value[0] = float(getAPartBayDoorPos())/100.0f;
         ap->parameterValue.value[1] = 0;

         // Update part count & pointer
         cnt++;
         ap++;

      }

   }

   // ---
   // Ground Vehicle articulated parts and attachments
   // ---
   else if ( getPlayer()->isMajorType(Simulation::Player::GROUND_VEHICLE) ) {

      // Check launcher elevation angle
      if (getAPartLauncherElevationCnt() > 0) {
         // fill the articulation parameter structure
         ap->parameterTypeDesignator = ArticulationParameter::ARTICULATED_PART;
         ap->changeIndicator = (unsigned char) (getAPartLauncherElevationCnt() & 0xff);
         ap->id = 0;
         ap->parameterType = (ArticulationParameter::PRIMARY_LAUNCHER_1 + ArticulationParameter::ELEVATION);
         ap->parameterValue.value[0] = float( getAPartLauncherElevation() );
         ap->parameterValue.value[1] = 0;
         // Update part count & pointer
         cnt++;
         ap++;
      }

      // Check attached missiles
      unsigned int n = getAPartNumberAttachedNumMissiles();
      for (unsigned int i = 0; i < n; i++) {

         const Simulation::Missile* msl = getAPartAttachedMissile(i+1);

         // Find the missile's entity type
         if (apartMslTypes[i] == 0) {

            const Ntm* ntm = dynamic_cast<const Ntm*>( disIO->findNetworkTypeMapper(msl) );
            if (ntm != 0) {

               // found the NTM for the missile -- and it must be a DIS NTM
               apartMslTypes[i] = dynamic_cast<const Ntm*>(ntm);
               if (apartMslTypes[i] != 0) apartMslTypes[i]->ref();
            }

         }

         // We must have the missile's entity type to send the attached part ...
         if (apartMslTypes[i] != 0) {

            // fill the articulation parameter structure
            ap->parameterTypeDesignator = ArticulationParameter::ATTACHED_PART;
            ap->changeIndicator = (unsigned char) (getAPartAttacheMissileChangeCnt(i+1) & 0xff);
            ap->id = 1;                   // ATTACHED to LAUNCHER (above)
            ap->parameterType = (i+1);    // Station number
            if (msl->isMode(Simulation::Player::LAUNCHED)) {
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

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace
