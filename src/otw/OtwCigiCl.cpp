
#define CIGI_LITTLE_ENDIAN

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

// disable all of the unreferenced parameter warnings
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4100)
#endif

#include "openeaagles/otw/OtwCigiCl.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Bomb.h"
#include "openeaagles/simulation/Buildings.h"
#include "openeaagles/simulation/Effects.h"
#include "openeaagles/simulation/GroundVehicle.h"
#include "openeaagles/simulation/LifeForms.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Navigation.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/SamVehicles.h"
#include "openeaagles/simulation/Ships.h"
#include "openeaagles/simulation/SpaceVehicle.h"
#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/simulation/Weapon.h"

#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/SlotTable.h"
#include "openeaagles/basic/Thread.h"
#include "openeaagles/basic/osg/Vec4"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Angles.h"

#include "cigicl/CigiEntityCtrlV3.h"
#include "cigicl/CigiCompCtrlV3.h"
#include "cigicl/CigiIGCtrlV3.h"
#include "cigicl/CigiLosVectReqV3.h"
#include "cigicl/CigiViewCtrlV3.h"
#include "cigicl/CigiViewDefV3.h"
#include "cigicl/CigiArtPartCtrlV3.h"
#include "cigicl/CigiHatHotReqV3.h"
#include "cigicl/CigiSOFV3.h"
#include "cigicl/CigiHatHotRespV3.h"
#include "cigicl/CigiHatHotRespV3_2.h"
#include "cigicl/CigiHatHotXRespV3_2.h"
#include "cigicl/CigiLosRespV3.h"
#include "cigicl/CigiCollDetSegRespV3.h"
#include "cigicl/CigiCollDetVolRespV3.h"
#include "cigicl/CigiSensorRespV3.h"
#include "cigicl/CigiIGMsgV3.h"
#include "cigicl/CigiHostSession.h"
#include "cigicl/CigiBaseSignalProcessing.h"
#include "cigicl/CigiSensorCtrlV3.h"

#include <cstdio>

namespace Eaagles {
namespace Otw {

//==============================================================================
// OtwCigiCl class
//==============================================================================

IMPLEMENT_SUBCLASS(OtwCigiCl,"OtwCigiCl")
EMPTY_SERIALIZER(OtwCigiCl)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(OtwCigiCl)
   "cigi",                 // 1) Required CIGI handler
   "async",                // 2) True (non-zero) to run in CIGI async mode (default: false - CIGI sync)
   "hideOwnshipModel",     // 3) True to hide the ownship's model (default: true - ownship's model is not seen)
   "ownshipModel",         // 4) Ownship's model ID
   "mslTrailModel",        // 5) Missile Trail" effect model ID
   "smokePlumeModel",      // 6) Smoke Plume" effect model ID
   "airExplosionModel",    // 7) Air Explosion" effect model ID
   "groundExplosionModel", // 8) Ground Explosion" effect model ID
   "shipWakeModel",        // 9) Ship Wake" effect model ID
END_SLOTTABLE(OtwCigiCl)

// Map slot table to handles
BEGIN_SLOT_MAP(OtwCigiCl)
   ON_SLOT(1, setSlotCigi,                  CigiCl)
   ON_SLOT(2, setSlotASyncMode,             Basic::Number)
   ON_SLOT(3, setSlotHideOwnshipModel,      Basic::Number)
   ON_SLOT(4, setSlotOwnshipModel,          Basic::Number)
   ON_SLOT(5, setSlotMslTrailModel,         Basic::Number)
   ON_SLOT(6, setSlotSmokePlumeModel,       Basic::Number)
   ON_SLOT(7, setSlotAirExplosionModel,     Basic::Number)
   ON_SLOT(8, setSlotGroundExplosionModel,  Basic::Number)
   ON_SLOT(9, setSlotShipWakeModel,         Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
static const int MAX_BUF_SIZE = 1472;
static const LCreal LOS_REQ_TIMEOUT = 2.0;     // one second timeout

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
OtwCigiCl::OtwCigiCl() : cigi(nullptr)
{
   STANDARD_CONSTRUCTOR()

   cigi = nullptr;
   cigiInitialized = false;
   cigiInitFailed = false;

   asyncMode = false;
   hideOwn = true;
   resetRequest = true;

   entityIdCount = 0;
   elevReqIdCount = 0;

   elevReqFlg = false;
   elevReqTimer = 0;

   losRespLat = 0;
   losRespLon = 0;
   losRespAlt = 0;
   losRespRange = 0;
   losRespId = 0;
   losRespDataValid = true;
   losReqId = 0;
   newLosReq = true;
   losReqTimer = 0;

   for (unsigned int i = 0; i < NUM_BUFFERS; i++) {
      ownshipEC[i] = new CigiEntityCtrlV3();
      ownshipCC[i] = new CigiCompCtrlV3();
   }

   igc =  new CigiIGCtrlV3();
   los = new CigiLosVectReqV3();
   view = nullptr;
   fov = new CigiViewDefV3();
   sensor = nullptr;

   iw = NUM_BUFFERS;
   iw0 = NUM_BUFFERS;
   ir = NUM_BUFFERS;

   cmtOwnship = 302;
   cmtMslTrail = 1100;
   cmtSmokePlume = 1101;
   cmtAirExplosion = 1102;
   cmtGroundExplosion = 1103;
   cmtShipWake = 1104;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void OtwCigiCl::copyData(const OtwCigiCl& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      cigi = nullptr;

      for (unsigned int i = 0; i < NUM_BUFFERS; i++) {
         ownshipEC[i] = new CigiEntityCtrlV3();
         ownshipCC[i] = new CigiCompCtrlV3();
      }
      igc =  new CigiIGCtrlV3();
      los = new CigiLosVectReqV3();
      view = nullptr;
      fov = nullptr;

      iw = NUM_BUFFERS;
      iw0 = NUM_BUFFERS;
      ir = NUM_BUFFERS;
   }

   cigi = nullptr;
   if (org.cigi != nullptr) {
      cigi = org.cigi->clone();
   }
   cigiInitialized = false;
   cigiInitFailed = false;

   asyncMode = org.asyncMode;
   hideOwn = org.hideOwn;
   resetRequest = true;

   entityIdCount = 0;
   elevReqIdCount = 0;

   elevReqFlg = false;
   elevReqTimer = 0;

   losRespLat = 0;
   losRespLon = 0;
   losRespAlt = 0;
   losRespRange = 0;
   losRespId = 0;
   losRespDataValid = true;
   losReqId = 0;
   newLosReq = true;
   losReqTimer = 0;

   iw = NUM_BUFFERS;
   iw0 = NUM_BUFFERS;
   ir = NUM_BUFFERS;

   cmtOwnship = org.cmtOwnship;
   cmtMslTrail = org.cmtMslTrail;
   cmtSmokePlume = org.cmtSmokePlume;
   cmtAirExplosion = org.cmtAirExplosion;
   cmtGroundExplosion = org.cmtGroundExplosion;
   cmtShipWake = org.cmtShipWake;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void OtwCigiCl::deleteData()
{
   cigi = nullptr;

   for (unsigned int i = 0; i < NUM_BUFFERS; i++) {
      if (ownshipEC[i] != nullptr) { delete ownshipEC[i]; ownshipEC[i] = nullptr; }
      if (ownshipCC[i] != nullptr) { delete ownshipCC[i]; ownshipCC[i] = nullptr; }
   }
   if (igc != nullptr)  { delete igc;  igc = nullptr; }
   if (los != nullptr)  { delete los;  los = nullptr; }
   if (view != nullptr) { delete view; view = nullptr; }
   if (fov != nullptr)  { delete fov;  fov = nullptr; }
   iw = NUM_BUFFERS;
   iw0 = NUM_BUFFERS;
   ir = NUM_BUFFERS;
}

//------------------------------------------------------------------------------
// modelFactory() -- Create OtwModel objects unique to interface
//------------------------------------------------------------------------------
Simulation::OtwModel* OtwCigiCl::modelFactory()
{
   OtwModelCigiCl* p = new OtwModelCigiCl();
   p->setID( ++entityIdCount );
   return p;
}

//------------------------------------------------------------------------------
// hotFactory() -- Create OtwHot objects unique to interface
//------------------------------------------------------------------------------
Simulation::OtwModel* OtwCigiCl::hotFactory()
{
   OtwModelCigiCl* p = new OtwModelCigiCl();
   p->setID( ++elevReqIdCount );
   return p;
}

//------------------------------------------------------------------------------
// setASyncMode() -- Sets/Clears the async mode flag
//------------------------------------------------------------------------------
bool OtwCigiCl::setASyncMode(const bool f)
{
   asyncMode = f;
   return true;
}

//------------------------------------------------------------------------------
// setHideOwnshipModel() -- Sets the hide ownship flag
//------------------------------------------------------------------------------
bool OtwCigiCl::setHideOwnshipModel(const bool f)
{
   hideOwn = f;
   return true;
}

//------------------------------------------------------------------------------
// updateData() -- Update non-time critical (background) stuff here
//------------------------------------------------------------------------------
void OtwCigiCl::updateData(const LCreal dt)
{
   // ---
   // Init the static CIGI system (only once for all instances)
   // ---
   if (cigi != nullptr && !cigiInitialized && !cigiInitFailed) {
      cigiInitialized = cigi->initialize(this);
      cigiInitFailed = !cigiInitialized;
   }

   // ---
   // Init the static CIGI system (only once for all instances)
   // ---
   if (cigi != nullptr && cigi->isInitialized()) {
      cigi->frameBg();
   }

   // ---
   // LOS request timeout control
   // ---
   if ( isLosRequestPending() ) {
      losReqTimer -= dt;
      if (losReqTimer <= 0.0) {
         // timeout!!!!
         losRespDataValid = false;
         losRespId = losReqId;
         losReqTimer = 0.0;
      }
   }

   // ---
   // Elevation request timeout control
   // ---
   if ( elevReqFlg ) {
      elevReqTimer -= dt;
      if (elevReqTimer <= 0.0) {
         // timeout!!!!
         elevReqFlg = false;
         elevReqTimer = 0.0;
      }
   }

   // Update base classes stuff
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// sendOwnshipAndModels() - Send state data for ownship and models
//------------------------------------------------------------------------------
void OtwCigiCl::sendOwnshipAndModels()
{
   // update the write buffer index
   iw++;
   if (iw >= NUM_BUFFERS) iw = 0;

   // Update ownship state entity
   bool ok = updateOwnshipModel();

   if (ok) {
      // only if we have a good ownship model update our models
      updateModels();
   }

   // set the index of the last write buffer
   iw0 = iw;
}

//------------------------------------------------------------------------------
// sendElevationRequests() - Sends terrain height requests
//------------------------------------------------------------------------------
void OtwCigiCl::sendElevationRequests()
{
   // Handled by Start-Of-Frame
}

//------------------------------------------------------------------------------
// recvElevations() - Receives terrain height data
//------------------------------------------------------------------------------
void OtwCigiCl::recvElevations()
{
   // Handled by CIGI callback
}

//------------------------------------------------------------------------------
// frameSync() - Trigger the frame update
//------------------------------------------------------------------------------
void OtwCigiCl::frameSync()
{
   if (cigi != nullptr && cigi->isInitialized()) {
      if (isASyncMode()) sendCigiData();
      cigi->frameSync();
   }
}

//------------------------------------------------------------------------------
// reset() - Reset OTW
//------------------------------------------------------------------------------
void OtwCigiCl::reset()
{
   BaseClass::reset();

   losRespDataValid = false;
   newLosReq = false;
   losReqTimer = false;
   losRespId = losReqId;

   resetRequest = true;
}

//------------------------------------------------------------------------------
// updateOwnshipModel() -- Update the ownship model; returns true if ok
//------------------------------------------------------------------------------
bool OtwCigiCl::updateOwnshipModel()
{
   // Ownship active and type air vehicle?
   bool active = false;
   if (getOwnship() != nullptr) active = getOwnship()->isActive() || getOwnship()->isMode(Simulation::Player::PRE_RELEASE);

   const Simulation::Player* av = getOwnship();

   if (active && av != nullptr && getOwnshipEntityControlPacket(iw) != nullptr) {
      // We have an active, AirVehicle type ownship and an entity control packet ...

      CigiEntityCtrlV3* ec = getOwnshipEntityControlPacket(iw);
      setCommonModelData(ec,0,av); // Note: Ownship is always entity #0
      if (isOwnshipModelHidden()) {
         ec->SetEntityState(CigiEntityCtrlV3::Standby);
      }
      else {
         // if we are being shown, make sure we update our model
         ec->SetEntityState(CigiEntityCtrlV3::Active);
      }
      ec->SetEntityType(cmtOwnship);
      ec->SetAttachState(CigiEntityCtrlV3::Detach);
      ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      ec->SetAnimationState(CigiEntityCtrlV3::Stop);
      ec->SetAlpha(255);
      if (getOwnshipComponentControlPacket(iw) != nullptr) {
         const Simulation::LifeForm* player = dynamic_cast<const Simulation::LifeForm*>(getOwnship());
         CigiCompCtrlV3* animation = ownshipCC[iw];
         if (animation != nullptr) {
            animation->SetCompClassV3(CigiCompCtrlV3::EntityV3);
            animation->SetInstanceID(0);
            animation->SetCompID(8); // Change Model Animation
            animation->SetCompState(STANDING);
            animation->SetCompData(0.0f, 0);
            animation->SetCompData(0.0f, 1);

            if (player != nullptr) {
               // get our ownship models id and our model
               Simulation::LifeForm* lf = const_cast<Simulation::LifeForm*>(static_cast<const Simulation::LifeForm*>(player));
               if (lf != nullptr) {
                  if (lf->getDamage() < 1) {
                     // Choose Animation state
                     if (lf->getActionState() == Simulation::LifeForm::UPRIGHT_STANDING) {
                        animation->SetCompState(STANDING);
                     }
                     else if (lf->getActionState() == Simulation::LifeForm::UPRIGHT_WALKING) {
                        animation->SetCompState(WALK);
                     }
                     else if (lf->getActionState() == Simulation::LifeForm::UPRIGHT_RUNNING) {
                        animation->SetCompState(RUN);
                     }
                     else {
                        // default is standing
                        animation->SetCompState(STANDING);
                     }
                  }
                  else animation->SetCompState(DEAD);
               }
            }
         }
      }
   }

   return true;
}

//------------------------------------------------------------------------------
// updateModels() -- Update the other models; returns number of active models
//------------------------------------------------------------------------------
int OtwCigiCl::updateModels()
{
   int n = 0;

   // Do we have models?
   Simulation::OtwModel** const table = getModelTable();
   if (table != nullptr && getModelTableSize() > 0) {

      // For all active models in the table ...
      for (unsigned short i = 0; i < getModelTableSize(); i++) {
         Basic::safe_ptr<OtwModelCigiCl> model( static_cast<OtwModelCigiCl*>(table[i]) );
         if (model != nullptr) {

            if (model->getState() != Simulation::OtwModel::INACTIVE) {
               unsigned short entity = model->getID()* 8 + 1; // Save a block of four entities per model
               //  (id*8+1) is parent entity
               //  (id*8+2) is smoke trail entity
               //  (id*8+3) is air explosion entity
               //  (id*8+4) is ground explosion entity
               //  (id*8+5) is attached part entity

               // Get the player
               const Simulation::Player* player = model->getPlayer();


               // Set the model data and ...
               if (player->isMajorType(Simulation::Player::AIR_VEHICLE)) {
                  setAirVehicleData(model, entity, static_cast<const Simulation::AirVehicle*>(player));
               }
               else if (player->isMajorType(Simulation::Player::GROUND_VEHICLE)) {
                  setGndVehicleData(model, entity, static_cast<const Simulation::GroundVehicle*>(player));
               }
               else if (player->isMajorType(Simulation::Player::SHIP)) {
                  setShipData(model, entity, static_cast<const Simulation::Ship*>(player));
               }
               else if (player->isMajorType(Simulation::Player::SPACE_VEHICLE)) {
                  setSpaceVehicleData(model, entity, static_cast<const Simulation::SpaceVehicle*>(player));
               }
               else if (player->isMajorType(Simulation::Player::LIFE_FORM)) {
                  setLifeFormData(model, entity, static_cast<const Simulation::LifeForm*>(player));
               }
               else if (player->isMajorType(Simulation::Player::BUILDING)) {
                  setBuildingData(model, entity, static_cast<const Simulation::Building*>(player));
               }
               else if (player->isMajorType(Simulation::Player::WEAPON)) {
                  const Simulation::Effects* effect      = dynamic_cast<const Simulation::Effects*>(model->getPlayer());
                  const Simulation::Missile* msl      = dynamic_cast<const Simulation::Missile*>(model->getPlayer());
                  const Simulation::Weapon* wpn       = dynamic_cast<const Simulation::Weapon*>(model->getPlayer());
                  if (effect != nullptr)     // Effects before general weapons (because effects are also weapons)
                     setEffectsData(model, entity, effect);
                  else if (msl != nullptr)   // Missiles before general weapons (because missiles are also weapons)
                     setMissileData(model, entity, msl);
                  else if (wpn != nullptr)
                     setWeaponData(model, entity, wpn);
               }

            }
         }
      }
   }

   return n;
}

//------------------------------------------------------------------------------
// setCommonModelData() --
//  -- Sets a CigiEntityCtrlV3 structure with common data entity data
//------------------------------------------------------------------------------
bool OtwCigiCl::setCommonModelData(CigiEntityCtrlV3* const ec, const unsigned short entity, const Simulation::Player* const p)
{
   bool ok = (ec != nullptr && p != nullptr);

   if (ok) {
      // Set entity id, parent and state
      ec->SetEntityID(entity);
      ec->SetParentID(0);
      //ec->parent_id = -1;

      // Set angles
      ec->SetRoll(static_cast<float>(p->getRollD()));
      ec->SetPitch(static_cast<float>(p->getPitchD()));
      float hdg = static_cast<float>(p->getHeadingD());
      if (hdg < 0.0) hdg += 360.0f;
      if (hdg >= 360.0f) hdg = 0.0;
      ec->SetYaw(hdg,false);

      // Set position
      ec->SetLat(p->getLatitude());
      ec->SetLon(p->getLongitude());
      ec->SetAlt(p->getAltitude());
   }
   return ok;
}

//------------------------------------------------------------------------------
// setAirVehicleData()
//  -- Sets a CigiEntityCtrlV3 structure to an air vheicle's state
//------------------------------------------------------------------------------
bool OtwCigiCl::setAirVehicleData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::AirVehicle* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec = m->parentEC[iw];

   // Smoke control block, ...
   if (m->smokeEC[iw] == nullptr) {
      m->smokeEC[iw] = new CigiEntityCtrlV3();
      m->smokeActive = false;
   }
   CigiEntityCtrlV3* const smoke = m->smokeEC[iw];

   // As long as we're active, update the entity control
   if (m->getState() == Simulation::OtwModel::ACTIVE) {
      bool ok = setCommonModelData(ec,entity,p);
      if (ok) {
         // Set the entity type
         unsigned int tt = 0;
         const Simulation::Otm* otm = m->getTypeMapper();
         if (otm != nullptr) tt = otm->getTypeId();
         if (tt > 0xffff) tt = 0;   // unsigned short only
         ec->SetEntityType(static_cast<unsigned short>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      // Load the smoke entity control
      if (p->getSmoke() > 0.3) {
         // Set entity id, parent and state
         smoke->SetEntityID(entity+3); // Smoke is at main entity id plus one
         smoke->SetParentID(entity);

         smoke->SetRoll(0.0);
         smoke->SetPitch(0.0);
         smoke->SetYaw(0.0);
         smoke->SetAlt(0.0);
         smoke->SetLat(0.0);
         smoke->SetLon(0.0);

         smoke->SetEntityState(CigiEntityCtrlV3::Active);
         smoke->SetEntityType(cmtSmokePlume);
         smoke->SetAttachState(CigiEntityCtrlV3::Attach);
         smoke->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         //smoke->SetAnimationState(CigiEntityCtrlV3::Stop);
         smoke->SetAnimationState(CigiEntityCtrlV3::Continue);
         smoke->SetAlpha(170);
         m->smokeActive = true;
      }
      else {
         smoke->SetEntityState(CigiEntityCtrlV3::Standby);
      }
   }
   else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      smoke->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

//------------------------------------------------------------------------------
// setBuildingData() -- Sets a 'model_t' structure to a building's state
//------------------------------------------------------------------------------
bool OtwCigiCl::setBuildingData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Building* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec = m->parentEC[iw];

   // Smoke control block, ...
   if (m->smokeEC[iw] == nullptr) {
      m->smokeEC[iw] = new CigiEntityCtrlV3();
      m->smokeActive = false;
   }
   CigiEntityCtrlV3* const smoke = m->smokeEC[iw];

   // Damage control, ...
   if (m->damageCC[iw] == nullptr) {
      m->damageCC[iw] = new CigiCompCtrlV3();
      m->damageCC[iw]->SetCompClassV3(CigiCompCtrlV3::EntityV3);
      m->damageCC[iw]->SetInstanceID(entity);
      m->damageCC[iw]->SetCompID(6);            // Object State
      m->damageCC[iw]->SetCompState(1);         // 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
      m->damageCC[iw]->SetCompData(0.0f, 0);
      m->damageCC[iw]->SetCompData(0.0f, 1);
      m->damageActive = true;                   // true the first time to make sure the model state is good
   }
   CigiCompCtrlV3* const damage = m->damageCC[iw];

   // As long as we're active, update the entity control
   if (m->getState() == Simulation::OtwModel::ACTIVE) {
      bool ok = setCommonModelData(ec,entity,p);
      if (ok) {
         // Set the entity type
         unsigned int tt = 0;
         const Simulation::Otm* otm = m->getTypeMapper();
         if (otm != nullptr) tt = otm->getTypeId();
         if (tt > 0xffff) tt = 0;   // unsigned short only
         ec->SetEntityType(static_cast<unsigned short>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      // Load the smoke entity control

      if (p->getSmoke() > 0.1f) {
         // Set entity id, parent and state
         smoke->SetEntityID(entity+3); // Smoke is at main entity id plus one
         smoke->SetParentID(entity);

         smoke->SetRoll(0.0);
         smoke->SetPitch(0.0);
         smoke->SetYaw(0.0);
         smoke->SetAlt(0.0);
         smoke->SetLat(0.0);
         smoke->SetLon(0.0);

         smoke->SetEntityState(CigiEntityCtrlV3::Active);
         smoke->SetEntityType(cmtSmokePlume);
         smoke->SetAttachState(CigiEntityCtrlV3::Attach);
         smoke->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         //smoke->SetAnimationState(CigiEntityCtrlV3::Stop);
         smoke->SetAnimationState(CigiEntityCtrlV3::Continue);
         smoke->SetAlpha(170);
         m->smokeActive = true;
      }
      else {
         smoke->SetEntityState(CigiEntityCtrlV3::Standby);
      }

      if (p->getDamage() > 0.0) {
         if (p->isDestroyed()) {
            damage->SetCompState(3);    // 3 = DISTROYED
         }
         else {
            damage->SetCompState(2);    // 2 = DAMAGED
         }
         m->damageActive = true;
      }
      else {
         damage->SetCompState(1);       // 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
      }
   }
   else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      smoke->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

//------------------------------------------------------------------------------
// setGndVehicleData() -- Sets a 'model_t' structure to a ground vheicle's state
//------------------------------------------------------------------------------
bool OtwCigiCl::setGndVehicleData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::GroundVehicle* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec = m->parentEC[iw];

   // Smoke control block, ...
   if (m->smokeEC[iw] == nullptr) {
      m->smokeEC[iw] = new CigiEntityCtrlV3();
      m->smokeActive = false;
   }
   CigiEntityCtrlV3* const smoke = m->smokeEC[iw];

   // Damage control, ...
   if (m->damageCC[iw] == nullptr) {
      m->damageCC[iw] = new CigiCompCtrlV3();
      m->damageCC[iw]->SetCompClassV3(CigiCompCtrlV3::EntityV3);
      m->damageCC[iw]->SetInstanceID(entity);
      m->damageCC[iw]->SetCompID(6);       // Object State
      m->damageCC[iw]->SetCompState(1);    // 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
      m->damageCC[iw]->SetCompData(0.0f, 0);
      m->damageCC[iw]->SetCompData(0.0f, 1);
      m->damageActive = true;  // true the first time to make sure the model state is good
   }
   CigiCompCtrlV3* const damage = m->damageCC[iw];

   // Articulated launcher control
   if (m->launcherAPC[iw] == nullptr) {
      m->launcherAPC[iw] = new CigiArtPartCtrlV3();
      m->launcherApcActive = false;
   }
   CigiArtPartCtrlV3* const launcherAPC = m->launcherAPC[iw];

   // Attached part, ...
   if (m->attachedEC[iw] == nullptr) {
      m->attachedEC[iw] = new CigiEntityCtrlV3();
      m->attachedEcActive = false;
   }
   CigiEntityCtrlV3* const attachedPartEC = m->attachedEC[iw];

   // Damage control, ...
   if (m->attachedCC[iw] == nullptr) {
      m->attachedCC[iw] = new CigiCompCtrlV3();
      m->attachedCcActive = false;
   }
   CigiCompCtrlV3* const attachedPartCC = m->attachedCC[iw];

   // As long as we're active, update the entity control
   if (m->getState() == Simulation::OtwModel::ACTIVE) {
      bool ok = setCommonModelData(ec,entity,p);
      if (ok) {

         // Set the entity type
         unsigned int tt = 0;
         const Simulation::Otm* otm = m->getTypeMapper();
         if (otm != nullptr) tt = otm->getTypeId();
         if (tt > 0xffff) tt = 0;   // unsigned short only
         ec->SetEntityType(static_cast<unsigned short>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      // Load the smoke entity control
      if (p->getSmoke() > 0.1f) {
         // Set entity id, parent and state
         smoke->SetEntityID(entity+3); // Smoke is at main entity id plus one
         smoke->SetParentID(entity);

         smoke->SetRoll(0.0);
         smoke->SetPitch(0.0);
         smoke->SetYaw(0.0);
         smoke->SetAlt(0.0);
         smoke->SetLat(0.0);
         smoke->SetLon(0.0);

         smoke->SetEntityState(CigiEntityCtrlV3::Active);
         smoke->SetEntityType(cmtSmokePlume);
         smoke->SetAttachState(CigiEntityCtrlV3::Attach);
         smoke->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         //smoke->SetAnimationState(CigiEntityCtrlV3::Stop);
         smoke->SetAnimationState(CigiEntityCtrlV3::Continue);
         smoke->SetAlpha(170);
         m->smokeActive = true;
      }
      else {
         smoke->SetEntityState(CigiEntityCtrlV3::Standby);
      }

      if (p->getDamage() > 0.0) {
         if (p->isDestroyed()) {
            damage->SetCompState(3);   // 3 = DISTROYED
         }
         else {
            damage->SetCompState(2);   // 2 = DAMAGED
         }
         m->damageActive = true;
      }
      else {
         damage->SetCompState(1);      // 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
      }

   }
   else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      smoke->SetEntityState(CigiEntityCtrlV3::Standby);
   }

   // NAZ-543 Articulated launcher and attached missile
   if (ec->GetEntityType() == 971) {

      unsigned int apartNumMissiles = 0; // Number of attached missiles

      // find all attached missiles
      const Simulation::StoresMgr* sm = p->getStoresManagement();
      if (sm != nullptr) {
         const Basic::PairStream* stores = sm->getStores();
         if (stores != nullptr) {
            const Basic::List::Item* item = stores->getFirstItem();
            while (item != nullptr && apartNumMissiles == 0) {
               const Basic::Pair* pair = static_cast<const Basic::Pair*>(item->getValue());
               if (pair != nullptr) {
                  const Simulation::Missile* msl = dynamic_cast<const Simulation::Missile*>( pair->object() );
                  if (msl != nullptr) apartNumMissiles++;
               }
               item = item->getNext();
            }
            stores->unref();
            stores = nullptr;
         }
      }

      // Launcher articulation
      launcherAPC->SetEntityID(entity);
      launcherAPC->SetArtPartID(1);       // for MAZ-543; 1 is the launcher
      launcherAPC->SetPitchEn(true);  // Pitch enabled
      launcherAPC->SetPitch( static_cast<float>(p->getLauncherPosition() * Basic::Angle::R2DCC) );
      m->launcherApcActive = true;

      // Attached missile
      attachedPartEC->SetEntityID(entity + 4);  // attached part
      attachedPartEC->SetParentID(entity);
      attachedPartEC->SetRoll(0.0f);
      attachedPartEC->SetPitch(0.0f);
      attachedPartEC->SetYaw(0.0f);
      attachedPartEC->SetAlt(0.0f);
      attachedPartEC->SetLat(0.0);
      attachedPartEC->SetLon(0.0);
      attachedPartEC->SetEntityType(917);     // SCUD missile type
      attachedPartEC->SetAttachState(CigiEntityCtrlV3::Attach);
      attachedPartEC->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      attachedPartEC->SetAnimationState(CigiEntityCtrlV3::Stop);
      attachedPartEC->SetAlpha(255);
      if (apartNumMissiles > 0) {

         attachedPartEC->SetEntityState(CigiEntityCtrlV3::Active);
         m->attachedEcActive = true;

         // Attached missile to launcher's articulation
         attachedPartCC->SetCompClassV3(CigiCompCtrlV3::EntityV3);
         attachedPartCC->SetInstanceID(attachedPartEC->GetEntityID());
         attachedPartCC->SetCompID(9); // 9 value is for stores attachment to locations
         attachedPartCC->SetCompState(0);
         attachedPartCC->SetCompData(0.0f, 0);
         attachedPartCC->SetCompData(0.0f, 1);
         m->attachedCcActive = true;

      }
      else {
         attachedPartEC->SetEntityState(CigiEntityCtrlV3::Standby);
      }

   }
   else {
      // Not articulation or attached parts
      launcherAPC->SetPitchEn(false);
      attachedPartEC->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

//------------------------------------------------------------------------------
// setEffectsData() -- Sets a 'model_t' structure to a effects' state
//------------------------------------------------------------------------------
bool OtwCigiCl::setEffectsData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Effects* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec = m->parentEC[iw];

   // a smoke trail entity control block, ...
   if (m->trailEC[iw] == nullptr) {
      m->trailEC[iw] = new CigiEntityCtrlV3();
      m->trailActive = false;
   }
   CigiEntityCtrlV3* const trail = m->trailEC[iw];

   if (m->getState() == Simulation::OtwModel::ACTIVE) {

      // As long as we're active, update the entity control
      if (p->isClassType(typeid(Simulation::Decoy))) {
         // Decoy

         // Load the parent entity control
         setCommonModelData(ec,entity,p);

         // Set the entity type
         unsigned int tt = 0;
         const Simulation::Otm* otm = m->getTypeMapper();
         if (otm != nullptr) tt = otm->getTypeId();
         if (tt > 0xffff) tt = 0;   // unsigned short only
         ec->SetEntityType(static_cast<unsigned short>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
         m->parentActive = true;

         // Load the smoke trail entity control
         {
            // Set entity id, parent and state
            trail->SetEntityID(entity+1); // Trails are at main entity id plus one
            trail->SetParentID(entity);

            trail->SetRoll(0.0f);
            trail->SetPitch(0.0f);
            trail->SetYaw(0.0f);
            trail->SetAlt(0.0f);
            trail->SetLat(0.0);
            trail->SetLon(0.0);

            trail->SetEntityState(CigiEntityCtrlV3::Active);
            trail->SetEntityType(cmtMslTrail);
            trail->SetAttachState(CigiEntityCtrlV3::Attach);
            trail->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
            trail->SetAnimationState(CigiEntityCtrlV3::Stop);
            //trail->SetAnimationState(CigiEntityCtrlV3::Continue);
            trail->SetAlpha(170);
            m->trailActive = true;
         }
      }

      else if (p->isClassType(typeid(Simulation::Flare))) {
         // FLARES
         bool ok = setCommonModelData(ec,entity,p);
         if (ok) {

            // Set the entity type
            unsigned int tt = 0;
            const Simulation::Otm* otm = m->getTypeMapper();
            if (otm != nullptr) tt = otm->getTypeId();
            if (tt > 0xffff) tt = 0;   // unsigned short only
            ec->SetEntityType(static_cast<unsigned short>(tt));

            // Set entity control data
            ec->SetEntityState(CigiEntityCtrlV3::Active);
            ec->SetAttachState(CigiEntityCtrlV3::Detach);
            ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
            ec->SetAnimationState(CigiEntityCtrlV3::Continue);
            ec->SetAlpha(255);
         }
         m->parentActive = ok;
      }
      else {
         // Unknown type
         ec->SetEntityState(CigiEntityCtrlV3::Standby);
         trail->SetEntityState(CigiEntityCtrlV3::Standby);
      }
   }
   else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      trail->SetEntityState(CigiEntityCtrlV3::Standby);
   }

   return true;
}

//------------------------------------------------------------------------------
// setLifeFormData() -- Sets a 'model_t' structure to a lifeform's state
//------------------------------------------------------------------------------
bool OtwCigiCl::setLifeFormData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::LifeForm* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec = m->parentEC[iw];

   // Animation control, ...
   if (m->animationCC[iw] == nullptr) {
      m->animationCC[iw] = new CigiCompCtrlV3();
      m->animationCC[iw]->SetCompClassV3(CigiCompCtrlV3::EntityV3);
      m->animationCC[iw]->SetInstanceID(entity);
      m->animationCC[iw]->SetCompID(8); // Change Model Animation
      m->animationCC[iw]->SetCompState(STANDING);
      m->animationCC[iw]->SetCompData(0.0f, 0);
      m->animationCC[iw]->SetCompData(0.0f, 1);
      m->animationActive = true;  // true the first time to make sure the model state is good
   }
   CigiCompCtrlV3* const animation = m->animationCC[iw];

   // As long as we're active, update the entity control
   if (m->getState() == Simulation::OtwModel::ACTIVE) {
      bool ok = setCommonModelData(ec,entity,p);
      if (ok) {
         // Set the entity type
         unsigned int tt = 0;
         const Simulation::Otm* otm = m->getTypeMapper();
         if (otm != nullptr) tt = otm->getTypeId();
         if (tt > 0xffff) tt = 0;   // unsigned short only
         ec->SetEntityType(static_cast<unsigned short>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      Simulation::LifeForm* lf = const_cast<Simulation::LifeForm*>(static_cast<const Simulation::LifeForm*>(p));

      if (lf->getDamage() <= 0.9f) {
         // Choose Animation state
         if (lf->getActionState() == Simulation::LifeForm::UPRIGHT_STANDING) {
            animation->SetCompState(STANDING);
         }
         else if (lf->getActionState() == Simulation::LifeForm::UPRIGHT_WALKING) {
            animation->SetCompState(WALK);
         }
         else if (lf->getActionState() == Simulation::LifeForm::UPRIGHT_RUNNING) {
            animation->SetCompState(RUN);
         }
         else {
            // default is standing
            animation->SetCompState(STANDING);
         }
      }
      else {
         // our damage is enough to kill us, so we are DEAD
         animation->SetCompState(DEAD);
      }
      m->animationActive = true;
   }
   else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

//------------------------------------------------------------------------------
// setMissileData() -- Sets a 'model_t' structure to a missile's state
//------------------------------------------------------------------------------
bool OtwCigiCl::setMissileData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Missile* const p)
{
   // Make sure we have an entity control block, ...
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
      m->parentActive = false;
   }
   CigiEntityCtrlV3* const ec = m->parentEC[iw];

   // a smoke trail entity control block, ...
   if (m->trailEC[iw] == nullptr) {
      m->trailEC[iw] = new CigiEntityCtrlV3();
      m->trailActive = false;
   }
   CigiEntityCtrlV3* const trail = m->trailEC[iw];

   // an air explosion entity control block, ...
   if (m->explosionEC[iw] == nullptr) {
      m->explosionEC[iw] = new CigiEntityCtrlV3();
      m->explosionEC[iw]->SetEntityID(entity+2); // Explosions are at main entity_id plus two
      m->explosionEC[iw]->SetParentID(entity);

      m->explosionEC[iw]->SetRoll(0.0f);
      m->explosionEC[iw]->SetPitch(0.0f);
      m->explosionEC[iw]->SetYaw(0.0f);
      m->explosionEC[iw]->SetAlt(0.0f);
      m->explosionEC[iw]->SetLat(0.0);
      m->explosionEC[iw]->SetLon(0.0);

      m->explosionEC[iw]->SetEntityState(CigiEntityCtrlV3::Active);
      m->explosionEC[iw]->SetEntityType(cmtAirExplosion);
      m->explosionEC[iw]->SetAttachState(CigiEntityCtrlV3::Detach);
      m->explosionEC[iw]->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      m->explosionEC[iw]->SetAnimationState(CigiEntityCtrlV3::Stop);
      m->explosionEC[iw]->SetAlpha(255);
      m->explosionActive = true;  // True the first time to make sure it's loaded
      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << "explosion: create" << m->explosionEC[iw]->entity_id << std::endl;
      //}
   }
   CigiEntityCtrlV3* const explosion = m->explosionEC[iw];

   // As long as we're active, update the entity control
   if (m->getState() == Simulation::OtwModel::ACTIVE) {
      // Load the parent entity control
      setCommonModelData(ec,entity,p);

      // Set the entity type
      unsigned int tt = 0;
      const Simulation::Otm* otm = m->getTypeMapper();
      if (otm != nullptr) tt = otm->getTypeId();
      if (tt > 0xffff) tt = 0;   // unsigned shorts only
      ec->SetEntityType(static_cast<unsigned short>(tt));

      // Set entity control data
      ec->SetEntityState(CigiEntityCtrlV3::Active);
      ec->SetAttachState(CigiEntityCtrlV3::Detach);
      ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      ec->SetAnimationState(CigiEntityCtrlV3::Stop);
      ec->SetAlpha(255);
      m->parentActive = true;

      // Load the smoke trail entity control
      {
         // Set entity id, parent and state
         trail->SetEntityID(entity+1); // Trails are at main entity id plus one
         trail->SetParentID(entity);

         trail->SetRoll(0.0f);
         trail->SetPitch(0.0f);
         trail->SetYaw(0.0f);
         trail->SetAlt(0.0f);
         trail->SetLat(0.0);
         trail->SetLon(0.0);

         //trail->Seteffect_state = EFFECT_ANIMATION_STATE_PLAY;
         trail->SetEntityState(CigiEntityCtrlV3::Active);
         trail->SetEntityType(cmtMslTrail);
         trail->SetAttachState(CigiEntityCtrlV3::Attach);
         trail->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         trail->SetAnimationState(CigiEntityCtrlV3::Stop);
         //trail->SetAnimationState(CigiEntityCtrlV3::Continue);
         trail->SetAlpha(170);
         m->trailActive = true;
      }
   }
   else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      trail->SetEntityState(CigiEntityCtrlV3::Standby);
      if (m->getState() == Simulation::OtwModel::DEAD) {
         // Start air explosion at last known location of missile
         explosion->SetRoll(ec->GetRoll());
         explosion->SetPitch(ec->GetPitch());
         float yaw = ec->GetYaw();
         if (yaw < 0.0) yaw += 360.0f;
         if (yaw >= 360.0f) yaw = 0.0;
         explosion->SetYaw(yaw,false);
         if (ec->GetAlt() > p->getTerrainElevation())
            explosion->SetAlt(ec->GetAlt());
         else
            explosion->SetAlt(p->getTerrainElevation());
         explosion->SetLat(ec->GetLat());
         explosion->SetLon(ec->GetLon());
         //explosion->SetAnimationState(CigiEntityCtrlV3::Continue);
         explosion->SetAnimationState(CigiEntityCtrlV3::Play);
         m->explosionActive = true;  // True to start to sequence
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// setShipData() -- Sets a 'model_t' structure to a ship's state
//------------------------------------------------------------------------------
bool OtwCigiCl::setShipData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Ship* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec = m->parentEC[iw];

   // a wake entity control block, ...
   if (m->trailEC[iw] == nullptr) {
      m->trailEC[iw] = new CigiEntityCtrlV3();
      m->trailActive = false;
   }
   CigiEntityCtrlV3* const wake = m->trailEC[iw];

   // As long as we're active, update the entity control
   if (m->getState() == Simulation::OtwModel::ACTIVE) {
      bool ok = setCommonModelData(ec,entity,p);
      if (ok) {

         // Set the entity type
         unsigned int tt = 0;
         const Simulation::Otm* otm = m->getTypeMapper();
         if (otm != nullptr) tt = otm->getTypeId();
         if (tt > 0xffff) tt = 0;   // unsigned shorts only
         ec->SetEntityType(static_cast<unsigned short>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      // Load the wake entity control
      {
         // Set entity id, parent and state
         wake->SetEntityID(entity+1); // Trails (wakes) are at parent entity plus one
         wake->SetParentID(entity);

         wake->SetRoll(0.0f);
         wake->SetPitch(0.0f);
         wake->SetYaw(0.0f);
         wake->SetAlt(0.0f);
         wake->SetLat(0.0);
         wake->SetLon(0.0);

         wake->SetEntityState(CigiEntityCtrlV3::Active);
         wake->SetEntityType(cmtShipWake);
         wake->SetAttachState(CigiEntityCtrlV3::Attach);
         wake->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         wake->SetAnimationState(CigiEntityCtrlV3::Continue);
         wake->SetAlpha(170);
         m->trailActive = true;
      }
   }
   else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      wake->SetEntityState(CigiEntityCtrlV3::Standby);
   }

   return true;
}

//------------------------------------------------------------------------------
// setSpaceVehicleData()
//  -- Sets a CigiEntityCtrlV3 structure to a space vheicle's state
//------------------------------------------------------------------------------
bool OtwCigiCl::setSpaceVehicleData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::SpaceVehicle* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec = m->parentEC[iw];

   // As long as we're active, update the entity control
   if (m->getState() == Simulation::OtwModel::ACTIVE) {
      bool ok = setCommonModelData(ec,entity,p);
      if (ok) {
         // Set the entity type
         unsigned int tt = 0;
         const Simulation::Otm* otm = m->getTypeMapper();
         if (otm != nullptr) tt = otm->getTypeId();
         if (tt > 0xffff) tt = 0;   // unsigned short only
         ec->SetEntityType(static_cast<unsigned short>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;
   }
   else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

//------------------------------------------------------------------------------
// setWeaponData() -- Sets a 'model_t' structure to a weapon's state
//------------------------------------------------------------------------------
bool OtwCigiCl::setWeaponData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Weapon* const p)
{
   // Make sure we have an entity control block, ...
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
      m->parentActive = false;
   }
   CigiEntityCtrlV3* const ec = m->parentEC[iw];

   // an ground explosion entity control block, ...
   if (m->explosionEC[iw] == nullptr) {
      m->explosionEC[iw] = new CigiEntityCtrlV3();
      m->explosionEC[iw]->SetEntityID(entity+2); // Explosions are at main entity_id plus two
      m->explosionEC[iw]->SetParentID(entity);

      m->explosionEC[iw]->SetRoll(0.0f);
      m->explosionEC[iw]->SetPitch(0.0f);
      m->explosionEC[iw]->SetYaw(0.0f);
      m->explosionEC[iw]->SetAlt(0.0f);
      m->explosionEC[iw]->SetLat(0.0);
      m->explosionEC[iw]->SetLon(0.0);

      m->explosionEC[iw]->SetEntityState(CigiEntityCtrlV3::Active);
      m->explosionEC[iw]->SetEntityType(cmtAirExplosion);
      m->explosionEC[iw]->SetAttachState(CigiEntityCtrlV3::Detach);
      m->explosionEC[iw]->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      m->explosionEC[iw]->SetAnimationState(CigiEntityCtrlV3::Stop);
      m->explosionEC[iw]->SetAlpha(255);
      m->explosionActive = true;  // True the first time to make sure it's loaded

      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << "explosion: create" << m->explosionEC[iw]->entity_id << std::endl;
      //}
   }
   CigiEntityCtrlV3* const explosion = m->explosionEC[iw];

   // As long as we're active, update the entity control
   if (m->getState() == Simulation::OtwModel::ACTIVE) {
      // Load the parent entity control
      setCommonModelData(ec,entity,p);

      // Set the entity type
      unsigned int tt = 0;
      const Simulation::Otm* otm = m->getTypeMapper();
      if (otm != nullptr) tt = otm->getTypeId();
      if (tt > 0xffff) tt = 0;   // unsigned shorts only
      ec->SetEntityType(static_cast<unsigned short>(tt));

      // Set entity control data
      ec->SetEntityState(CigiEntityCtrlV3::Active);
      ec->SetAttachState(CigiEntityCtrlV3::Detach);
      ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      ec->SetAnimationState(CigiEntityCtrlV3::Stop);
      ec->SetAlpha(255);
      m->parentActive = true;
   }
   else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      if (m->getState() == Simulation::OtwModel::DEAD) {
         // Start air explosion at last known location of missile
         explosion->SetRoll(ec->GetRoll());
         explosion->SetPitch(ec->GetPitch());
         float yaw = ec->GetYaw();
         if (yaw < 0.0) yaw += 360.0f;
         if (yaw >= 360.0f) yaw = 0.0;
         explosion->SetYaw(yaw,false);
         if (ec->GetAlt() > p->getTerrainElevation())
            explosion->SetAlt(ec->GetAlt());
         else
            explosion->SetAlt(p->getTerrainElevation());
         explosion->SetLat(ec->GetLat());
         explosion->SetLon(ec->GetLon());
         //explosion->SetAnimationState(CigiEntityCtrlV3::Continue);
         explosion->SetAnimationState(CigiEntityCtrlV3::Play);
         m->explosionActive = true;  // True to start to sequence
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Line of sight request -- returns true of request was issued
//------------------------------------------------------------------------------
bool OtwCigiCl::lineOfSightRequest(
                                   const double lat,          // Source latitude         (deg)
                                   const double lon,          // Source longitude        (deg)
                                   const double alt,          // Source altitude         (m)
                                   const LCreal hdg,          // Source heading          (deg)
                                   const LCreal pitch,        // Source pitch            (deg)
                                   const LCreal minRange,     // Request minimum range   (m)
                                   const LCreal maxRange      // Request maximum range   (m)
                                   )
{
   bool ok = false;
   if ( !isLosRequestPending() ) {
      CigiLosVectReqV3* losReq = getLosRangeRequestPacket();
      if (losReq != nullptr) {
         losReq->SetSrcLat(lat);
         losReq->SetSrcLon(lon);
         losReq->SetSrcAlt(alt);
         losReq->SetVectAz( static_cast<float>(-hdg) );
         losReq->SetVectEl( static_cast<float>(pitch) );
         losReq->SetMinRange( static_cast<float>(minRange) );
         losReq->SetMaxRange( static_cast<float>(maxRange) );
         newLosReq = true;             // new request,
         losRespDataValid = false;     // so, the previous data is not valid
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Get Line of sight -- returns true of the LOS data is valid
//------------------------------------------------------------------------------
bool OtwCigiCl::getLineOfSightData(
                                   double* const lat,      // Point latitude         (deg)
                                   double* const lon,      // Point longitude        (deg)
                                   double* const alt,      // Point altitude         (m)
                                   LCreal* const rng,      // Range to point         (m)
                                   int* const material     // Material code
                                   )
{
   bool ok = losRespDataValid;
   if (ok) {
      if (lat != nullptr) *lat = losRespLat;
      if (lon != nullptr) *lon = losRespLon;
      if (alt != nullptr) *alt = losRespAlt;
      if (rng != nullptr) *rng = losRespRange;
      if (material != nullptr) *material = 0;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

// Set CIGI Handler
bool OtwCigiCl::setSlotCigi(CigiCl* const msg)
{
   cigi = msg;
   return true;
}

// Set/clear the ASYNC mode
bool OtwCigiCl::setSlotASyncMode(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setASyncMode(msg->getBoolean());
   }
   return ok;
}

// Set/clear the hide ownship model flag
bool OtwCigiCl::setSlotHideOwnshipModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setHideOwnshipModel(msg->getBoolean());
   }
   return ok;
}

bool OtwCigiCl::setSlotOwnshipModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setOwnshipModelId(static_cast<unsigned short>(v));
      }
   }
   return ok;
}

bool OtwCigiCl::setSlotMslTrailModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setMslTrailModelId(static_cast<unsigned short>(v));
      }
   }
   return ok;
}

bool OtwCigiCl::setSlotSmokePlumeModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setSmokePlumeModelId(static_cast<unsigned short>(v));
      }
   }
   return ok;
}

bool OtwCigiCl::setSlotAirExplosionModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setAirExplosionModelId(static_cast<unsigned short>(v));
      }
   }
   return ok;
}

bool OtwCigiCl::setSlotGroundExplosionModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setGroundExplosionModelId(static_cast<unsigned short>(v));
      }
   }
   return ok;
}

bool OtwCigiCl::setSlotShipWakeModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setShipWakeModelId(static_cast<unsigned short>(v));
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* OtwCigiCl::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// sendCigiData() -- Handles sending CIGI data to the visual system
//------------------------------------------------------------------------------
bool OtwCigiCl::sendCigiData()
{
   // Select the 'read' buffer index
   if (iw0 == NUM_BUFFERS) return false;
   ir = iw0;

   // ---
   // Start the message.  This MUST come before any CigiAddPacket*()
   // functions.
   // ---
   cigi->startMessage();

   // ---
   // Now we must add an IG Control.
   // ---
   CigiIGCtrlV3* ig_cp = getIgControlPacket();
   ig_cp->SetDatabaseID(0);
   ig_cp->SetIGMode(CigiIGCtrlV3::Operate);
   if (resetRequest) {
      ig_cp->SetIGMode(CigiIGCtrlV3::Standby);
      resetRequest = false;
   }
   ig_cp->SetFrameCntr(ig_cp->GetFrameCntr() + 1);
   ig_cp->SetTimeStamp(0);
   cigi->addPacketIGCtrl(ig_cp);

   // ---
   // And add an Entity Control packet for the "ownship."
   // ---
   cigi->addPacketEntityCtrl(getOwnshipEntityControlPacket(ir));
   cigi->addPacketComponentCtrl(getOwnshipComponentControlPacket(ir));


   // ---
   // Send all entity controls from the model table
   // ---
   {
      // Do we have models?
      OtwModelCigiCl** const table = (OtwModelCigiCl**) getModelTable();
      if (table != nullptr && getModelTableSize() > 0) {

         // Need at least this much in the buffer to add another entity
         int padding = (sizeof(CigiEntityCtrlV3) * 2) + sizeof(CigiHatHotReqV3);
         if (newLosReq && getLosRangeRequestPacket() != nullptr) padding += sizeof(CigiLosVectReqV3);
         if (getViewControlPacket() != nullptr) padding += sizeof(CigiViewCtrlV3);
         if (getViewDefinitionPacket() != nullptr) padding += sizeof(CigiViewDefV3);

         // Add all of the models (that we can) to the buffer.
         int sendSize = cigi->getOutgoingBufferSize();
         int maxAge = getModelTableSize();
         for (unsigned short i = 0; i < getModelTableSize() && sendSize < (MAX_BUF_SIZE - padding); i++) {
            Basic::safe_ptr<OtwModelCigiCl> model( static_cast<OtwModelCigiCl*>(table[i]) );
            if (model != nullptr) {

               // For all active models in the table ...
               // (send ground models only after 'maxAge' frames)
               // (always update the inactive models to clear them)
               model->incAgeCount();
               bool updateThisOne = !model->isGroundPlayer || model->isState(Simulation::OtwModel::OUT_OF_RANGE) || (model->isGroundPlayer && model->getAgeCount() >= maxAge);

               if (updateThisOne) {
                  //if (model->isGroundPlayer) {
                  //   if (isMessageEnabled(MSG_DEBUG)) {
                  //   std::cout << "model: " << model->getID() << ", pid=" << model->getPlayer()->getID() << std::endl;
                  //}
                  //}
                  // Explosion?
                  if (model->explosionActive && model->explosionEC[ir] != nullptr) {
                     cigi->addPacketEntityCtrl(model->explosionEC[ir]);
                     model->explosionActive = false;
                  }

                  if (model->parentActive && model->parentEC[ir] != nullptr) {
                     cigi->addPacketEntityCtrl(model->parentEC[ir]);
                     model->parentActive = (model->parentEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);

                     // Trail effect?
                     if (model->trailActive && model->trailEC[ir] != nullptr) {
                        cigi->addPacketEntityCtrl(model->trailEC[ir]);
                        model->trailActive = (model->trailEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);
                     }

                     // Smoke affect?
                     if (model->smokeActive && model->smokeEC[ir] != nullptr) {
                        cigi->addPacketEntityCtrl(model->smokeEC[ir]);
                        model->smokeActive = (model->smokeEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);
                     }

                     // Animation state?
                     if (model->animationActive && model->animationCC[ir] != nullptr) {
                        cigi->addPacketComponentCtrl(model->animationCC[ir]);
                        model->animationActive = (model->animationCC[ir]->GetCompState() > 0);
                     }

                     // Damage state?
                     if (model->damageActive && model->damageCC[ir] != nullptr) {
                        cigi->addPacketComponentCtrl(model->damageCC[ir]);
                        model->damageActive = (model->damageCC[ir]->GetCompState() > 1);
                     }

                     // Launcher articulated state?
                     if (model->launcherApcActive && model->launcherAPC[ir] != nullptr) {
                        cigi->addPacketArtPartCtrl(model->launcherAPC[ir]);
                        model->launcherApcActive = (model->launcherAPC[ir]->GetPitchEn());
                     }

                     // Attached part?
                     if (model->attachedEcActive && model->attachedEC[ir] != nullptr) {
                        cigi->addPacketEntityCtrl(model->attachedEC[ir]);
                        model->attachedEcActive = (model->attachedEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);
                     }

                     // Attached part component control?
                     if (model->attachedCcActive && model->attachedCC[ir] != nullptr && model->attachedEC[ir] != nullptr) {
                        cigi->addPacketComponentCtrl(model->attachedCC[ir]);
                        // we come and go with the attached part
                        model->attachedCcActive = (model->attachedEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);
                     }

                     // Clear the model?
                     if (model->getState() != Simulation::OtwModel::ACTIVE) {
                        model->setState( Simulation::OtwModel::CLEARED );
                     }
                  }
                  model->setAgeCount(0);
                  sendSize = cigi->getOutgoingBufferSize();
               }
            }
         }
      }
   }

   // ---
   // Send all elevation (Height-Of-Terrain) requests
   //   (only if we don't have an outstanding request)
   // ---
   if (!isElevationRequestPending()) {
      // Do we have a elevation request entries?
      OtwModelCigiCl** const table = reinterpret_cast<OtwModelCigiCl**>( getElevationTable() );
      if (table != nullptr && getElevationTableSize() > 0) {
         int maxAge = getElevationTableSize() * 4;

         // For all active elevation requests in the table ...
         // -- look for the oldest request ---
         Basic::safe_ptr<OtwModelCigiCl> oldest( nullptr );
         Basic::safe_ptr<OtwModelCigiCl> model( nullptr );
         for (unsigned short i = 0; i < getElevationTableSize(); i++) {
            model = table[i];
            if (model != nullptr) {
               // Must be active and haven't been requested for at least TBD frames ...
               if (model->isHotActive() && model->getReqCount() >= maxAge) {
                  if (oldest == nullptr) oldest = model;
                  else if (model->getReqCount() > oldest->getReqCount()) oldest = model;
               }
            }
         }
         model = nullptr;

         if (oldest != nullptr) {

            int idx = -1;
            for (unsigned short i = 0; idx < 0 && i < getElevationTableSize(); i++) {
               if (table[i] == oldest) idx = i;
            }

            if (idx >= 0) {
               CigiHatHotReqV3 hotRequest;
               hotRequest.SetHatHotID(table[idx]->getID());

               // Requested Position (lat/lon)
               double hotLat(0.0), hotLon(0.0);
               oldest->getPlayer()->getPositionLL(&hotLat, &hotLon);
               hotRequest.SetLat(hotLat);
               hotRequest.SetLon(hotLon);
               hotRequest.SetReqType(CigiHatHotReqV3::HOT);
               //osg::Vec3 pos = oldest->getPlayer()->getPosition();
               //LCreal alt;
               //Basic::Nav::convertPosVec2LL(
               //         getRefLatitude(), getRefLongitude(),
               //         pos,
               //         &hotRequest.lat, &hotRequest.lon, &alt);

               cigi->addPacketHatHotReq(&hotRequest);
               oldest->setReqCount(0);

               elevationRequestSend();
            }
            oldest = nullptr;
         }
      }
   }

   // ---
   // Optional LOS request packet
   // ---
   {
      CigiLosVectReqV3* los0 = getLosRangeRequestPacket();
      if (isNewLosequested() && los0 != nullptr) {
         los->SetLosID(getNexLosId());
         cigi->addPacketLosRangeReq(los0);
         losRequestSend();
      }
   }

   // ---
   // Optional view control and definition packets
   // ---
   if (getSensorControlPacket() != nullptr) {
      CigiSensorCtrlV3* mySensor = getSensorControlPacket();
      cigi->addPacketSensorCtrl(mySensor);
   }

   // ---
   // Optional view control and definition packets
   // ---
   if (getViewControlPacket() != nullptr) {
      CigiViewCtrlV3* myView = getViewControlPacket();
      //if ( getMaxModels() == 20 ) {
      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << myView->entity_id << std::endl;
      //std::cout << myView->view_id << std::endl;
      //std::cout << myView->group << std::endl;
      //std::cout << myView->heading_enable << std::endl;
      //std::cout << myView->pitch_enable << std::endl;
      //std::cout << myView->roll_enable << std::endl;
      //std::cout << myView->xoff_enable << std::endl;
      //std::cout << myView->yoff_enable << std::endl;
      //std::cout << myView->zoff_enable << std::endl;
      //std::cout << myView->x_offset << std::endl;
      //std::cout << myView->y_offset << std::endl;
      //std::cout << myView->z_offset << std::endl;
      //std::cout << myView->heading_offset << std::endl;
      //std::cout << myView->pitch_offset << std::endl;
      //std::cout << myView->roll_offset << std::endl;
      //}
      //}

      cigi->addPacketViewCtrl(myView);
   }
   if (getViewDefinitionPacket() != nullptr) {
      cigi->addPacketViewDef(getViewDefinitionPacket());
      //CigiViewDefV3* myView = getViewDefinitionPacket();
      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << "VIEW DEFINITION PACKET PARAMETERS: " << std::endl;
      //std::cout << "Group = " << myView->group << std::endl;
      //std::cout << "Mirror = " << myView->mirror << std::endl;
      //std::cout << "Track Assign = " << myView->track_assign << std::endl;
      //std::cout << "Type = " << myView->type << std::endl;
      //std::cout << "View ID = " << myView->view_id << std::endl;
      //std::cout << "Bottom enable = " << myView->bottom_enable << std::endl;
      //std::cout << "Top enable = " << myView->top_enable << std::endl;
      //std::cout << "Left enable = " << myView->left_enable << std::endl;
      //std::cout << "Right enable = " << myView->right_enable << std::endl;
      //std::cout << "Near Enabled = " << myView->near_enable << std::endl;
      //std::cout << "Far enabled = " << myView->far_enable << std::endl;
      //std::cout << "Far = " << myView->fov_far << std::endl;
      //std::cout << "Near = " << myView->fov_near << std::endl;
      //std::cout << "Left = " << myView->fov_left << std::endl;
      //std::cout << "Right = " << myView->fov_right << std::endl;
      //std::cout << "Top = " << myView->fov_top << std::endl;
      //std::cout << "Bottom = " << myView->fov_bottom << std::endl;
      //}

   }

   // ---
   // End the message.
   // ---
   cigi->endMessage();

   return true;
}

//------------------------------------------------------------------------------
// startOfFrame() -- Handles Start of Frame packets
//------------------------------------------------------------------------------
void OtwCigiCl::startOfFrame(const CigiSOFV3* const)
{
   if (isSyncMode()) sendCigiData();
}

//------------------------------------------------------------------------------
// losResp() -- Handles Line-Of-Sight Response packets
//------------------------------------------------------------------------------
void OtwCigiCl::losResp(const CigiLosRespV3* const p)
{
   if ( p != nullptr && isLosRequestPending() && losReqTimer > 0.0 ) {
      // We do have a pending request that hasn't timed out,
      // but is this it?
      if (p->GetLosID() == losReqId) {
         // We've the packet that we're looking for,
         // but is it valid (the IG found an intersection point)
         if (p->GetValid()) {
            // We have valid data
            losRespDataValid = true;
            losRespLat = p->GetLatitude();
            losRespLon = p->GetLongitude();
            losRespAlt = p->GetAltitude();
            losRespRange = static_cast<LCreal>(p->GetRange());
         }
         else {
            // Don't have a valid point
            losRespDataValid = false;
         }
         losRespId = p->GetLosID();
         losReqTimer = 0;
      }
   }
}

//------------------------------------------------------------------------------
// LOS request has been sent to the IG
//------------------------------------------------------------------------------
void OtwCigiCl::losRequestSend()
{
   newLosReq = false;
   losReqTimer = LOS_REQ_TIMEOUT;
}

//------------------------------------------------------------------------------
// Elevation request has been sent to the IG
//------------------------------------------------------------------------------
void OtwCigiCl::elevationRequestSend()
{
   elevReqFlg = true;   // active request
   elevReqTimer = 0.5f; // time-out
}

//------------------------------------------------------------------------------
// collisionSegmentResp() -- Handles Collision Segment Response packets
//------------------------------------------------------------------------------
void OtwCigiCl::collisionSegmentResp(const CigiCollDetSegRespV3* const p)
{
   if (p != nullptr) {
      std::printf("<< Incoming\n");
#if 0
      CigiPrintCollisionSegmentRespPacket(p, 2);
#endif
      std::printf("\n");
   }
}

//------------------------------------------------------------------------------
// sensorResp() -- Handles Sensor Response packets
//------------------------------------------------------------------------------
void OtwCigiCl::sensorResp(const CigiSensorRespV3* const p)
{
   if (p != nullptr) {
      std::printf("<< Incoming\n");
#if 0
      CigiPrintSensorResponsePacket(p, 2);
#endif
      std::printf("\n");
   }
}

//------------------------------------------------------------------------------
// hatHotResp() -- Handles Height-Above-Terrain and Height-Of-Terrain Response packets
//------------------------------------------------------------------------------
void OtwCigiCl::hatHotResp(const CigiHatHotRespV3* const p)
{
   // Valid?
   if (p != nullptr && p->GetValid()) {
      // Yes and the hot_id is the elevation table index
      unsigned short id = p->GetHatHotID();

      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << "hotResp: id = " << id << std::endl;
      //}

      OtwModelCigiCl** const table = reinterpret_cast<OtwModelCigiCl**>( getElevationTable() );
      Basic::safe_ptr<OtwModelCigiCl> model(nullptr);
      for (unsigned int i = 0; i < getElevationTableSize() && model == nullptr; i++) {
         if (table[i]->getID() == id) model = table[i];
      }
      if (model != nullptr) {
         if (model->isHotActive() && model->getPlayer() != nullptr) {
            // When the player and elevation table are still valid, store
            // the terrain elevation (meters)
            model->getPlayer()->setTerrainElevation(static_cast<LCreal>(p->GetHot()));

            //if (isMessageEnabled(MSG_DEBUG)) {
            //   std::cout << "hotResp: alt = --, pid = " << model->getPlayer()->getID() << std::endl;
            //}
         }
      }
      elevReqFlg = false;
      elevReqTimer = 0.0;
   }
}

//------------------------------------------------------------------------------
// collisionVolumeResp() -- Handles Collision Volume Response packets
//------------------------------------------------------------------------------
void OtwCigiCl::collisionVolumeResp(const CigiCollDetVolRespV3* const p)
{
   if (p != nullptr) {
      std::printf("<< Incoming\n");
#if 0
      CigiPrintCollisionVolumeRespPacket(p, 2);
#endif
      std::printf("\n");
   }
}

//------------------------------------------------------------------------------
// igResponse() -- Handles IG Response packets
//------------------------------------------------------------------------------
void OtwCigiCl::igResponse(const CigiIGMsgV3* const p)
{
   if (p != nullptr) {
      std::printf("<< Incoming\n");
#if 0
      CigiPrintIGResponseMessagePacket(p, 2);
#endif
      std::printf("\n");
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool OtwCigiCl::setSensorControlPacket(CigiSensorCtrlV3* const p)
{
   sensor = p;
   return true;
}

bool OtwCigiCl::setViewControlPacket(CigiViewCtrlV3* const p)
{
   view = p;
   return true;
}

bool OtwCigiCl::setViewDefinitionPacket(CigiViewDefV3* const p)
{
   fov = p;
   return true;
}

// Ownship's model ID
bool OtwCigiCl::setOwnshipModelId(const unsigned short v)
{
   cmtOwnship = v;
   return true;
}

// "Missile Trail" effect model ID
bool OtwCigiCl::setMslTrailModelId(const unsigned short v)
{
   cmtMslTrail = v;
   return true;
}

// "Smoke Plume" effect model ID
bool OtwCigiCl::setSmokePlumeModelId(const unsigned short v)
{
   cmtSmokePlume = v;
   return true;
}

// "Air Explosion" effect model ID
bool OtwCigiCl::setAirExplosionModelId(const unsigned short v)
{
   cmtAirExplosion = v;
   return true;
}

// "Ground Explosion" effect model ID
bool OtwCigiCl::setGroundExplosionModelId(const unsigned short v)
{
   cmtGroundExplosion = v;
   return true;
}

// "Ship Wake" effect model ID
bool OtwCigiCl::setShipWakeModelId(const unsigned short v)
{
   cmtShipWake = v;
   return true;
}

//==============================================================================
// CigiCl class
//==============================================================================

IMPLEMENT_ABSTRACT_SUBCLASS(CigiCl,"CigiClAbstract")
EMPTY_SLOTTABLE(CigiCl)
EMPTY_SERIALIZER(CigiCl)

//------------------------------------------------------------------------------
// Static data
//------------------------------------------------------------------------------
bool CigiCl::cigiInitialized = false;    // CIGI has been initialized
bool CigiCl::cigiInitFailed  = false;    // CIGI initialization has failed

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
CigiCl::CigiCl()
{
   STANDARD_CONSTRUCTOR()

   session = nullptr;
   otwCigi = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void CigiCl::copyData(const CigiCl& org, const bool)
{
   BaseClass::copyData(org);

   session = nullptr;
   otwCigi = nullptr;  // don't unref();
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void CigiCl::deleteData()
{
   if (session != nullptr) {
      delete session;
   }
   session = nullptr;
   otwCigi = nullptr;  // don't unref();
}


//------------------------------------------------------------------------------
// initialize() -- Initialize this CIGI session
//------------------------------------------------------------------------------
bool CigiCl::initialize(OtwCigiCl* const p)
{
   // Our parent OTW CIGI handler
   // (no need to ref(), it owns us and it's not going to be deleted before us)
   otwCigi = p;

   // ---
   // Init the static CIGI system (only once for all instances)
   // ---
   if (!cigiInitialized && !cigiInitFailed) {
      cigiInitialized = initCigi();
      cigiInitFailed = !cigiInitialized;
   }

   // ---
   // Create the session
   // ---
   if (cigiInitialized) {
      session = new CigiHostSession(4, MAX_BUF_SIZE, 4, MAX_BUF_SIZE);
      //session->SetCigiVersion(2);     // CGB only force this if necessary
      // CGBCGB ??? session->SetSynchronous(p->isSyncMode());
   }

   return session != nullptr;
}

//------------------------------------------------------------------------------
// True if we've been successfully initialized
//------------------------------------------------------------------------------
bool CigiCl::isInitialized()
{
   return cigiInitialized && (session != nullptr);
}

//------------------------------------------------------------------------------
// initCigi() -- Initialize the static CIGI system
//------------------------------------------------------------------------------
bool CigiCl::initCigi()
{
   std::cout << "CigiCl::initCigi(): CIGI initialized!" << std::endl;
   return true;
}

//------------------------------------------------------------------------------
// R/T frame sync -- called from OtwCigiCl::frameSync() in the R/T thread
//------------------------------------------------------------------------------
void CigiCl::frameSync()
{
}

//------------------------------------------------------------------------------
// Background frame sync -- called from OtwCigiCl::updateData() in the background
//------------------------------------------------------------------------------
void CigiCl::frameBg()
{
}

//==============================================================================
// CigiClNetworkSignalProcessing class
//==============================================================================
class CigiClNetworkSignalProcessing : public CigiBaseSignalProcessing {
public:
   inline CigiClNetworkSignalProcessing(OtwCigiCl* parent) : p(parent) { }
   virtual ~CigiClNetworkSignalProcessing();

   virtual void OnSOF(CigiBasePacket* packet);
   virtual void OnCollDetSegResp(CigiBasePacket* packet);
   virtual void OnCollDetVolResp(CigiBasePacket* packet);
   virtual void OnHatHotResp(CigiBasePacket* packet);
   virtual void OnHotResp(CigiBasePacket *Packet);
   virtual void OnIGMsg(CigiBasePacket* packet);
   virtual void OnLosResp(CigiBasePacket* packet);
   virtual void OnSensorResp(CigiBasePacket* packet);

private:
   OtwCigiCl* p;
};

CigiClNetworkSignalProcessing::~CigiClNetworkSignalProcessing()
{
   p = nullptr;
}

//------------------------------------------------------------------------------
// callbacks -- Pass control to the correct object (session's) member function
//------------------------------------------------------------------------------

void CigiClNetworkSignalProcessing::OnSOF(CigiBasePacket* packet)
{
   if (p != nullptr) p->startOfFrame( dynamic_cast<CigiSOFV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnCollDetSegResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->collisionSegmentResp( dynamic_cast<CigiCollDetSegRespV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnCollDetVolResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->collisionVolumeResp( dynamic_cast<CigiCollDetVolRespV3*> (packet) );
}


void CigiClNetworkSignalProcessing::OnHotResp(CigiBasePacket* packet)
{
   if (p != nullptr) {

      CigiHatHotRespV3* m3 = dynamic_cast<CigiHatHotRespV3*> (packet);
      if (m3 != nullptr) {
         p->hatHotResp( m3 );
      }
      else {

         CigiHatHotRespV3_2* m3_2 = dynamic_cast<CigiHatHotRespV3_2*> (packet);
         if (m3_2 != nullptr) {
            CigiHatHotRespV3 resp;
            resp.SetHatHotID(m3_2->GetHatHotID());
            resp.SetHot(m3_2->GetHot());
            resp.SetValid(m3_2->GetValid());
            p->hatHotResp( &resp );
         }
         else {
            CigiHatHotXRespV3_2* mx3_2 = dynamic_cast<CigiHatHotXRespV3_2*> (packet);
            if (mx3_2 != nullptr) {
               CigiHatHotRespV3 resp;
               resp.SetHatHotID(mx3_2->GetHatHotID());
               resp.SetHot(mx3_2->GetHot());
               resp.SetValid(mx3_2->GetValid());
               p->hatHotResp( &resp );
            }
         }

      }
   }
}


void CigiClNetworkSignalProcessing::OnHatHotResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->hatHotResp( dynamic_cast<CigiHatHotRespV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnIGMsg(CigiBasePacket* packet)
{
   if (p != nullptr) p->igResponse( dynamic_cast<CigiIGMsgV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnLosResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->losResp( dynamic_cast<CigiLosRespV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnSensorResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->sensorResp( dynamic_cast<CigiSensorRespV3*> (packet) );
}

//==============================================================================
// CigiClNetwork main loop thread
//==============================================================================

class NetThread : public Basic::ThreadSingleTask {
   DECLARE_SUBCLASS(NetThread,Basic::ThreadSingleTask)
public: NetThread(Basic::Component* const parent, const LCreal priority);
private: virtual unsigned long userFunc();
};

IMPLEMENT_SUBCLASS(NetThread,"NetThread")
EMPTY_SLOTTABLE(NetThread)
EMPTY_COPYDATA(NetThread)
EMPTY_DELETEDATA(NetThread)
EMPTY_SERIALIZER(NetThread)

NetThread::NetThread(Basic::Component* const parent, const LCreal priority)
: Basic::ThreadSingleTask(parent, priority)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long NetThread::userFunc()
{
   CigiClNetwork* otwNet = dynamic_cast<CigiClNetwork*>( getParent() );
   if (otwNet != nullptr) {
      if (otwNet->isMessageEnabled(MSG_INFO)) {
         std::cout << "NetThread::userFunc(): CigiClNetwork = " << otwNet << std::endl;
      }
      otwNet->mainLoop();
   }
   return 0;
}

//==============================================================================
// CigiClNetwork class
//==============================================================================

IMPLEMENT_SUBCLASS(CigiClNetwork,"CigiClNetwork")
EMPTY_SERIALIZER(CigiClNetwork)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(CigiClNetwork)
"netInput",             // 1) Network input handler
"netOutput",            // 2) Network output handler
END_SLOTTABLE(CigiClNetwork)

// Map slot table to handles
BEGIN_SLOT_MAP(CigiClNetwork)
   ON_SLOT(1, setSlotNetInput,  Basic::NetHandler)
   ON_SLOT(2, setSlotNetOutput, Basic::NetHandler)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
CigiClNetwork::CigiClNetwork()
 : netInput(nullptr), netOutput(nullptr), thread(nullptr), msgIn(nullptr), msgOut(nullptr), sigProcessor(nullptr)
{
   STANDARD_CONSTRUCTOR()

   networkInitialized = false;
   networkInitFailed = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void CigiClNetwork::copyData(const CigiClNetwork& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      netInput = nullptr;
      netOutput = nullptr;
   }

   // We need to init this ourselves, so ...
   netInput = nullptr;
   netOutput = nullptr;
   networkInitialized = false;
   networkInitFailed = false;
   sigProcessor = nullptr;

   if (thread != nullptr) {
      thread->terminate();
      thread = nullptr;
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void CigiClNetwork::deleteData()
{
   netInput = nullptr;
   netOutput = nullptr;
   if (sigProcessor != nullptr) {
      delete sigProcessor;
   }
   if (thread != nullptr) {
      thread->terminate();
      thread = nullptr;
   }
   sigProcessor = nullptr;
}


//------------------------------------------------------------------------------
// initialize() -- Initialize this CIGI session
//------------------------------------------------------------------------------
bool CigiClNetwork::initialize(OtwCigiCl* const p)
{
   bool ok = BaseClass::initialize(p);

   if (ok) {
      // ---
      // Set the callback functions
      // ---
      if (sigProcessor == nullptr) {
         OtwCigiCl* parent = getOtwCigi();
         sigProcessor = new CigiClNetworkSignalProcessing(parent);
         if (sigProcessor == nullptr) {
            return false;
         }
      }

      CigiHostSession* hostSession = getSession();
      msgOut = &(hostSession->GetOutgoingMsgMgr());
      msgIn = &(hostSession->GetIncomingMsgMgr());
      msgIn->SetReaderCigiVersion(2);
      msgIn->UsingIteration(false);
      msgIn->RegisterSignalProcessor(sigProcessor);

      // ---
      // Initialize the network handlers
      // ---
      if (!networkInitialized && !networkInitFailed) {
         networkInitialized = initCigiNetwork();
         networkInitFailed = !networkInitialized;
      }

      // ---
      // Create a network thread to handle the blocked network traffic
      // ---
      if (ok && networkInitialized && (thread == nullptr)) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "CREATING CIGI process" << std::endl;
         }
         createCigiProcess();
      }

      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "CigiClNetwork::initialize(): CIGI handler initialized!" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// createCigiProcess() -- Create the CIGI thread
//------------------------------------------------------------------------------
bool CigiClNetwork::createCigiProcess()
{
   if ( thread == nullptr ) {

      // parent -> our OTW manager
      thread = new NetThread(this, 0.6);
      thread->unref(); // 'thread' is a safe_ptr<>

      bool ok = thread->create();
      if (!ok) {
         thread = nullptr;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "CigiClNetwork::createCigiProcess(): ERROR, failed to create the thread!" << std::endl;
         }
      }
   }
   return (thread != nullptr);
}

//------------------------------------------------------------------------------
// True if we've been successfully initialized
//------------------------------------------------------------------------------
bool CigiClNetwork::isInitialized()
{
   return BaseClass::isInitialized() && networkInitialized && (thread != nullptr);
}

//------------------------------------------------------------------------------
// initCigiClNetwork() -- Init the network
//------------------------------------------------------------------------------
bool CigiClNetwork::initCigiNetwork()
{
   bool ok = true;

   // Initialize network input handler
   if (netInput != nullptr) {
      if (netInput->initNetwork(false)) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "netInput Initialize OK" << std::endl;
         }
      }
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "CigiClNetwork::initNetwork(): failure to find the network input handler (see slot 'netInput')" << std::endl;
      }
      ok = false;
   }

   // Initialize network output handler
   if (netOutput != nullptr) {
      if (netOutput->initNetwork(true)) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "netOutput Initialize OK" << std::endl;
         }
      }
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "CigiClNetwork::initNetwork(): failure to find the network output handler (see slot 'netOutput')" << std::endl;
      }
      ok = false;
   }

   return ok;
}

//------------------------------------------------------------------------------
// CIGI functions
//------------------------------------------------------------------------------

// Start the message.  This MUST come before any CigiAddPacket*() functions.
void CigiClNetwork::startMessage()
{
   msgOut->BeginMsg();
}

// End the message.
void CigiClNetwork::endMessage()
{
#if 1
   msgOut->UpdateFrameCntr();
   msgOut->LockMsg();

   int sendSize = 0;
   unsigned char* sendBuff = msgOut->GetMsg(sendSize);
   netOutput->sendData( reinterpret_cast<char*>(sendBuff), sendSize );

   msgOut->UnlockMsg();
#else
   // CGB this is sync mode, and I think it only works for version 3 in the class lib right now
   msgOut->LockMsg();

   int sendSize = 0;
   unsigned char* sendBuff = msgOut->GetMsg(sendSize);
   unsigned char* recvBuff = msgIn->GetMsgBuffer();

   msgOut->UpdateFrameCntr(sendBuff, recvBuff);

   netOutput->sendData( reinterpret_cast<char*>(sendBuff), sendSize );

   msgOut->UnlockMsg();
#endif
}

int CigiClNetwork::getOutgoingBufferSize()
{
   int sendSize = 0;
   msgOut->GetMsg(sendSize);
   return sendSize;
}

// Add an IG Control packet
void CigiClNetwork::addPacketIGCtrl(CigiIGCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a View Control packet
void CigiClNetwork::addPacketSensorCtrl(CigiSensorCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a View Control packet
void CigiClNetwork::addPacketViewCtrl(CigiViewCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a View Definition packet
void CigiClNetwork::addPacketViewDef(CigiViewDefV3* const p)
{
   *msgOut << *p;
}

// Add an entity control packet
void CigiClNetwork::addPacketEntityCtrl(CigiEntityCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a Component Control packet
void CigiClNetwork::addPacketComponentCtrl(CigiCompCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a Articulated Part Control packet
void CigiClNetwork::addPacketArtPartCtrl(CigiArtPartCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a Height Of Terrain (HOT) Request packet
void CigiClNetwork::addPacketHatHotReq(CigiHatHotReqV3* const p)
{
   *msgOut << *p;
}

// Add a LOS/Range Request packet
void CigiClNetwork::addPacketLosRangeReq(CigiLosVectReqV3* const p)
{
   *msgOut << *p;
}

//=============================================================================
// Main loop
//=============================================================================
void CigiClNetwork::mainLoop()
{
   // ---
   // Receive and process CIGI packets from the visual system
   // ---
   if (netInput != nullptr && netOutput != nullptr) {
      while ( !getOtwCigi()->isShutdown() ) {

         msgIn->AdvanceCrntBuffer();
         unsigned char* rcvbuff = msgIn->GetMsgBuffer();
         int recvCnt = netInput->recvData(reinterpret_cast<char*>(rcvbuff), MAX_BUF_SIZE);

         if (recvCnt > 0) {
            msgIn->SetCrntMsgSize(recvCnt);
            msgIn->ProcessIncomingMsg();
         }
         else {
            msgIn->SetCrntMsgSize(0);
         }
      }
   }
}

//------------------------------------------------------------------------------
// get a pre-ref'd pointers to the network handlers
//------------------------------------------------------------------------------
Basic::NetHandler* CigiClNetwork::getInputHandler()
{
   return netInput.getRefPtr();
}

Basic::NetHandler* CigiClNetwork::getOutputHandler()
{
   return netOutput.getRefPtr();
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

// Set Network Input Handler
bool CigiClNetwork::setSlotNetInput(Basic::NetHandler* const msg)
{
   netInput = msg;
   return true;
}

// Net Network Output Handler
bool CigiClNetwork::setSlotNetOutput(Basic::NetHandler* const msg)
{
   netOutput = msg;
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* CigiClNetwork::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//==============================================================================
// OtwModelCigiCl class
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(OtwModelCigiCl,"OtwModelCigiCl")
EMPTY_SERIALIZER(OtwModelCigiCl)

// ---
// constructor
// ---
OtwModelCigiCl::OtwModelCigiCl()
{
   STANDARD_CONSTRUCTOR()

   for (unsigned int i = 0; i < OtwCigiCl::NUM_BUFFERS; i++) {
      parentEC[i]    = nullptr;
      trailEC[i]     = nullptr;
      explosionEC[i] = nullptr;
      smokeEC[i]     = nullptr;
      damageCC[i]    = nullptr;
      animationCC[i] = nullptr;
      attachedEC[i]  = nullptr;
      launcherAPC[i] = nullptr;
      attachedCC[i]  = nullptr;
   }

   parentActive = false;
   trailActive = false;
   explosionActive = false;
   smokeActive = false;
   damageActive = false;
   animationActive = false;
   attachedEcActive = false;
   launcherApcActive = false;
   attachedCcActive = false;
   isGroundPlayer = false;
   effectsTimer = 0.0;
   id = 0;
}

// ---
// copyData() -- copy member data
// ---
void OtwModelCigiCl::copyData(const OtwModelCigiCl& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      for (unsigned int i = 0; i < OtwCigiCl::NUM_BUFFERS; i++) {
         parentEC[i]    = nullptr;
         trailEC[i]     = nullptr;
         explosionEC[i] = nullptr;
         smokeEC[i]     = nullptr;
         damageCC[i]    = nullptr;
         animationCC[i] = nullptr;
         attachedEC[i]  = nullptr;
         launcherAPC[i] = nullptr;
         attachedCC[i]  = nullptr;
      }
   }

   parentActive = false;
   trailActive = false;
   explosionActive = false;
   smokeActive = false;
   damageActive = false;
   animationActive = false;
   attachedEcActive = false;
   launcherApcActive = false;
   attachedCcActive = false;
   isGroundPlayer = false;
   effectsTimer = 0.0;
   id = 0;
}

// ---
// deleteData() -- delete member data
// ---
void OtwModelCigiCl::deleteData()
{
   for (unsigned int i = 0; i < OtwCigiCl::NUM_BUFFERS; i++) {
      if (parentEC[i]    != nullptr) { delete parentEC[i]; parentEC[i] = nullptr;       }
      if (trailEC[i]     != nullptr) { delete trailEC[i]; trailEC[i] = nullptr;         }
      if (explosionEC[i] != nullptr) { delete explosionEC[i]; explosionEC[i] = nullptr; }
      if (smokeEC[i]     != nullptr) { delete smokeEC[i]; smokeEC[i] = nullptr;         }
      if (damageCC[i]    != nullptr) { delete damageCC[i]; damageCC[i] = nullptr;       }
      if (animationCC[i] != nullptr) { delete animationCC[i]; animationCC[i] = nullptr; }
      if (attachedEC[i]  != nullptr) { delete attachedEC[i]; attachedEC[i] = nullptr;   }
      if (launcherAPC[i] != nullptr) { delete launcherAPC[i]; launcherAPC[i] = nullptr; }
      if (attachedCC[i]  != nullptr) { delete attachedCC[i]; attachedCC[i] = nullptr;   }
   }
}

// ---
// initialize() -- initialize the model
// ---
void OtwModelCigiCl::initialize(Simulation::Player* const p)
{
   isGroundPlayer = p->isClassType(typeid(Simulation::GroundVehicle));
   BaseClass::initialize(p);
}

// ---
// clear() -- clears out the model
// ---
void OtwModelCigiCl::clear()
{
   parentActive = false;
   trailActive = false;
   explosionActive = false;
   smokeActive = false;
   animationActive = false;
   damageActive = false;
   attachedEcActive = false;
   launcherApcActive = false;
   attachedCcActive = false;
   isGroundPlayer = false;

   BaseClass::clear();
}

} // End Otw namespace
} // End Eaagles namespace
