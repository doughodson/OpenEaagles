
#define CIGI_LITTLE_ENDIAN

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

#include "openeaagles/otw/OtwCigiClV2.h"

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

#include "openeaagles/basic/SlotTable.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/osg/Vec4"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Angles.h"

#include "cigicl/CigiEntityCtrlV2.h"
#include "cigicl/CigiCompCtrlV2.h"
#include "cigicl/CigiIGCtrlV2.h"
#include "cigicl/CigiLosVectReqV2.h"
#include "cigicl/CigiViewCtrlV2.h"
#include "cigicl/CigiViewDefV2.h"
#include "cigicl/CigiArtPartCtrlV2.h"
#include "cigicl/CigiHotReqV2.h"
#include "cigicl/CigiSOFV2.h"
#include "cigicl/CigiHatRespV2.h"
#include "cigicl/CigiHotRespV2.h"
#include "cigicl/CigiLosRespV2.h"
#include "cigicl/CigiCollDetSegRespV2.h"
#include "cigicl/CigiCollDetVolRespV2.h"
#include "cigicl/CigiSensorRespV2.h"
#include "cigicl/CigiIGMsgV2.h"
#include "cigicl/CigiHostSession.h"
#include "cigicl/CigiBaseSignalProcessing.h"
#include "cigicl/CigiIncomingMsg.h"
#include "cigicl/CigiOutgoingMsg.h"

#include <cstdio>

namespace Eaagles {
namespace Otw {

//==============================================================================
// OtwCigiClV2 class
//==============================================================================

IMPLEMENT_SUBCLASS(OtwCigiClV2,"OtwCigiClV2")
EMPTY_SERIALIZER(OtwCigiClV2)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(OtwCigiClV2)
   "cigi",                 // 1) Required CIGI handler
   "async",                // 2) True (non-zero) to run in CIGI async mode (default: false - CIGI sync)
   "hideOwnshipModel",     // 3) True to hide the ownship's model (default: false - ownship's model is seen)
   "ownshipModel",         // 4) Ownship's model ID
   "mslTrailModel",        // 5) Missile Trail" effect model ID
   "smokePlumeModel",      // 6) Smoke Plume" effect model ID
   "airExplosionModel",    // 7) Air Explosion" effect model ID
   "groundExplosionModel", // 8) GroundE xplosion" effect model ID
   "shipWakeModel",        // 9) Ship Wake" effect model ID
END_SLOTTABLE(OtwCigiClV2)

// Map slot table to handles
BEGIN_SLOT_MAP(OtwCigiClV2)
    ON_SLOT(1, setSlotCigi,                  CigiClV2);
    ON_SLOT(2, setSlotASyncMode,             Basic::Number);
    ON_SLOT(3, setSlotHideOwnshipModel,      Basic::Number);
    ON_SLOT(4, setSlotOwnshipModel,          Basic::Number);
    ON_SLOT(5, setSlotMslTrailModel,         Basic::Number);
    ON_SLOT(6, setSlotSmokePlumeModel,       Basic::Number);
    ON_SLOT(7, setSlotAirExplosionModel,     Basic::Number);
    ON_SLOT(8, setSlotGroundExplosionModel,  Basic::Number);
    ON_SLOT(9, setSlotShipWakeModel,         Basic::Number);
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
static const int MAX_BUF_SIZE = 1472;
static const LCreal LOS_REQ_TIMEOUT = 2.0f;     // one second timeout

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
OtwCigiClV2::OtwCigiClV2() : cigi(0)
{
   STANDARD_CONSTRUCTOR()

   cigi = nullptr;
   cigiInitialized = false;
   cigiInitFailed = false;

   asyncMode = false;
   hideOwn = false;
   resetRequest = true;

   entityIdCount = 0;
   elevReqIdCount = 0;

   elevReqFlg = false;
   elevReqTimer = 0;

   losRespLat = 0;
   losRespLon = 0;
   losRespAlt = 0;
   losRespRange = 0;
   losRespMaterialCode = 0;
   losRespId = 0;
   losRespDataValid = true;
   losReqId = 0;
   newLosReq = true;
   losReqTimer = 0;

   for (unsigned int i = 0; i < NUM_BUFFERS; i++) {
      ownshipEC[i] = new CigiEntityCtrlV2();
      ownshipCC[i] = new CigiCompCtrlV2();
   }

   igc =  new CigiIGCtrlV2();
   los = new CigiLosVectReqV2();
   view = nullptr;
   fov = new CigiViewDefV2();

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
void OtwCigiClV2::copyData(const OtwCigiClV2& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      cigi = nullptr;

	for (unsigned int i = 0; i < NUM_BUFFERS; i++) {
	    ownshipEC[i] = new CigiEntityCtrlV2();
	    ownshipCC[i] = new CigiCompCtrlV2();
	}
	igc =  new CigiIGCtrlV2();
	los = new CigiLosVectReqV2();
	view = nullptr;
	fov = nullptr;

	iw = NUM_BUFFERS;
	iw0 = NUM_BUFFERS;
	ir = NUM_BUFFERS;
   }

   cigi = nullptr;
   if (org.cigi != nullptr) {
      cigi = (CigiClV2*) org.cigi->clone();
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
   losRespMaterialCode = 0;
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
void OtwCigiClV2::deleteData()
{
   cigi = 0;

   for (unsigned int i = 0; i < NUM_BUFFERS; i++) {
      if (ownshipEC[i] != nullptr) { delete ownshipEC[i]; ownshipEC[i] = nullptr; }
      if (ownshipCC[i] != nullptr) { delete ownshipCC[i]; ownshipCC[i] = nullptr; }
   }
   if (igc != nullptr) { delete igc; igc = nullptr; }
   if (los != nullptr) { delete los; los = nullptr; }
   if (view != nullptr) { delete view; view = nullptr; }
   if (fov != nullptr) { delete fov; fov = nullptr; }
   iw = NUM_BUFFERS;
   iw0 = NUM_BUFFERS;
   ir = NUM_BUFFERS;
}

//------------------------------------------------------------------------------
// modelFactory() -- Create OtwModel objects unique to interface
//------------------------------------------------------------------------------
Simulation::OtwModel* OtwCigiClV2::modelFactory()
{
   OtwModelCigiClV2* p = new OtwModelCigiClV2();
   p->setID( ++entityIdCount );
   return p;
}

//------------------------------------------------------------------------------
// hotFactory() -- Create OtwHot objects unique to interface
//------------------------------------------------------------------------------
Simulation::OtwModel* OtwCigiClV2::hotFactory()
{
   OtwModelCigiClV2* p = new OtwModelCigiClV2();
   p->setID( ++elevReqIdCount );
   return p;
}

//------------------------------------------------------------------------------
// setASyncMode() -- Sets/Clears the async mode flag
//------------------------------------------------------------------------------
bool OtwCigiClV2::setASyncMode(const bool f)
{
   asyncMode = f;
   return true;
}

//------------------------------------------------------------------------------
// setHideOwnshipModel() -- Sets the hide ownship flag
//------------------------------------------------------------------------------
bool OtwCigiClV2::setHideOwnshipModel(const bool f)
{
   hideOwn = f;
   return true;
}

//------------------------------------------------------------------------------
// updateData() -- Update non-time critical (background) stuff here
//------------------------------------------------------------------------------
void OtwCigiClV2::updateData(const LCreal dt)
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
void OtwCigiClV2::sendOwnshipAndModels()
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
void OtwCigiClV2::sendElevationRequests()
{
   // Handled by Start-Of-Frame
}

//------------------------------------------------------------------------------
// recvElevations() - Receives terrain height data
//------------------------------------------------------------------------------
void OtwCigiClV2::recvElevations()
{
   // Handled by CIGI callback
}

//------------------------------------------------------------------------------
// frameSync() - Trigger the frame update
//------------------------------------------------------------------------------
void OtwCigiClV2::frameSync()
{
   if (cigi != nullptr && cigi->isInitialized()) {
      if (isASyncMode()) sendCigiData();
      cigi->frameSync();
   }
}

//------------------------------------------------------------------------------
// reset() - Reset OTW
//------------------------------------------------------------------------------
void OtwCigiClV2::reset()
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
bool OtwCigiClV2::updateOwnshipModel()
{
    // Ownship active and type air vehicle?
    bool active = false;
    if (getOwnship() != nullptr) active = getOwnship()->isActive();
    const Simulation::Player* av = getOwnship();

    if (active && av != nullptr && getOwnshipEntityControlPacket(iw) != nullptr) {
        // We have an active, AirVehicle type ownship and an entity control packet ...

        CigiEntityCtrlV2* ec = getOwnshipEntityControlPacket(iw);
        setCommonModelData(ec,0,av); // Note: Ownship is always entity #0
        if (isOwnshipModelHidden()) {
            ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
        }
        else {
            // if we are being shown, make sure we update our model
            ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
        }
        ec->SetEntityType(cmtOwnship);
        ec->SetAttachState(CigiEntityCtrlV2::Detach);
        ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
        ec->SetEffectState(CigiEntityCtrlV2::StopV2);
        ec->SetOpacity(100.0f);
        ec->SetTemp(0.0f);
        if (getOwnshipComponentControlPacket(iw) != 0) {
            const Simulation::LifeForm* player = dynamic_cast<const Simulation::LifeForm*>(getOwnship());
            CigiCompCtrlV2* animation = ownshipCC[iw];
            animation->SetCompClassV2(CigiCompCtrlV2::EntityV2);
            animation->SetInstanceID(0);
            animation->SetCompID(8); // Change Model Animation
            animation->SetCompState(STANDING);
            animation->SetCompData(0.0f, 0);
            animation->SetCompData(0.0f, 1);

            if (player != 0 && animation != 0) {
                // get our ownship models id and our model
                Simulation::LifeForm* lf = (Simulation::LifeForm*)player;
                if (lf != 0) {
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

    return true;
}

//------------------------------------------------------------------------------
// updateModels() -- Update the other models; returns number of active models
//------------------------------------------------------------------------------
int OtwCigiClV2::updateModels()
{
    int n = 0;

    // Do we have models?
    Simulation::OtwModel** const table = getModelTable();
    if (table != nullptr && getModelTableSize() > 0) {

        // For all active models in the table ...
        for (unsigned short i = 0; i < getModelTableSize(); i++) {
            Basic::safe_ptr<OtwModelCigiClV2> model( (OtwModelCigiClV2*) table[i] );
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
                     setAirVehicleData(model, entity, (const Simulation::AirVehicle*) player);
                  }
                  else if (player->isMajorType(Simulation::Player::GROUND_VEHICLE)) {
                     setGndVehicleData(model, entity, (const Simulation::GroundVehicle*) player);
                  }
                  else if (player->isMajorType(Simulation::Player::SHIP)) {
                     setShipData(model, entity, (const Simulation::Ship*) player);
                  }
                  else if (player->isMajorType(Simulation::Player::SPACE_VEHICLE)) {
                     setSpaceVehicleData(model, entity, (const Simulation::SpaceVehicle*) player);
                  }
                  else if (player->isMajorType(Simulation::Player::LIFE_FORM)) {
                     setLifeFormData(model, entity, (const Simulation::LifeForm*) player);
                  }
                  else if (player->isMajorType(Simulation::Player::BUILDING)) {
                     setBuildingData(model, entity, (const Simulation::Building*) player);
                  }
                  else if (player->isMajorType(Simulation::Player::WEAPON)) {
                     const Simulation::Effects* effect   = dynamic_cast<const Simulation::Effects*>(model->getPlayer());
                     const Simulation::Missile* msl      = dynamic_cast<const Simulation::Missile*>(model->getPlayer());
                     const Simulation::Weapon* wpn       = dynamic_cast<const Simulation::Weapon*>(model->getPlayer());
                     if (effect != nullptr) // Effects before general weapons (because effects are also weapons)
                        setEffectsData(model, entity, effect);
                     else if (msl != nullptr) // Missiles before general weapons (because missiles are also weapons)
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
//  -- Sets a CigiEntityCtrlV2 structure with common data entity data
//------------------------------------------------------------------------------
bool OtwCigiClV2::setCommonModelData(CigiEntityCtrlV2* const ec, const unsigned short entity, const Simulation::Player* const p)
{
    bool ok = (ec != nullptr && p != nullptr);

    if (ok) {
        // Set entity id, parent and state
        ec->SetEntityID(entity);
        ec->SetParentID(0);
        //ec->parent_id = -1;

        // Set angles
        ec->SetRoll(p->getRollD());
        ec->SetPitch(p->getPitchD());
        LCreal hdg = p->getHeadingD();
        if (hdg < 0.0) {
            hdg += 360.0;
        }
        ec->SetHeading(hdg);

        osg::Vec3 pos = p->getPosition();

        // Set position
        double lat(0.0), lon(0.0), alt(0.0);
        Basic::Nav::convertPosVec2LL(getRefLatitude(), getRefLongitude(), pos, &lat, &lon, &alt);
        ec->SetLat(lat);
        ec->SetLon(lon);
        ec->SetAlt(alt);
    }
    return ok;
}

//------------------------------------------------------------------------------
// setAirVehicleData()
//  -- Sets a CigiEntityCtrlV2 structure to an air vheicle's state
//------------------------------------------------------------------------------
bool OtwCigiClV2::setAirVehicleData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::AirVehicle* const p)
{
    // Make sure we have an entity control block
    if (m->parentEC[iw] == nullptr) {
        m->parentEC[iw] = new CigiEntityCtrlV2();
    }
    CigiEntityCtrlV2* const ec = m->parentEC[iw];

    // Smoke control block, ...
    if (m->smokeEC[iw] == nullptr) {
        m->smokeEC[iw] = new CigiEntityCtrlV2();
        m->smokeActive = false;
    }
    CigiEntityCtrlV2* const smoke = m->smokeEC[iw];

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
            ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            ec->SetAttachState(CigiEntityCtrlV2::Detach);
            ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            ec->SetEffectState(CigiEntityCtrlV2::StopV2);
            ec->SetOpacity(100.0f);
            ec->SetTemp(0.0f);
        }
        m->parentActive = ok;

        // Load the smoke entity control
        if (p->getSmoke() > 0.3f) {
            // Set entity id, parent and state
            smoke->SetEntityID(entity+3); // Smoke is at main entity id plus one
            smoke->SetParentID(entity);

            smoke->SetRoll(0.0);
            smoke->SetPitch(0.0);
            smoke->SetHeading(0.0);
            smoke->SetAlt(0.0);
            smoke->SetLat(0.0);
            smoke->SetLon(0.0);

            smoke->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            smoke->SetEntityType(cmtSmokePlume);
            smoke->SetAttachState(CigiEntityCtrlV2::Attach);
            smoke->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            //smoke->SetEffectState(CigiEntityCtrlV2::StopV2);
            smoke->SetEffectState(CigiEntityCtrlV2::PlayV2);
            smoke->SetOpacity(66.6f);
            smoke->SetTemp(0.0f);
            m->smokeActive = true;
        }
        else {
            smoke->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
        }

    }
    else {
        ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
        smoke->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
    }
    return true;
}

//------------------------------------------------------------------------------
// setBuildingData() -- Sets a 'model_t' structure to a building's state
//------------------------------------------------------------------------------
bool OtwCigiClV2::setBuildingData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Building* const p)
{
    // Make sure we have an entity control block
    if (m->parentEC[iw] == nullptr) {
        m->parentEC[iw] = new CigiEntityCtrlV2();
    }
    CigiEntityCtrlV2* const ec = m->parentEC[iw];

    // Smoke control block, ...
    if (m->smokeEC[iw] == nullptr) {
        m->smokeEC[iw] = new CigiEntityCtrlV2();
        m->smokeActive = false;
    }
    CigiEntityCtrlV2* const smoke = m->smokeEC[iw];

    // Damage control, ...
    if (m->damageCC[iw] == nullptr) {
        m->damageCC[iw]->SetCompClassV2(CigiCompCtrlV2::EntityV2);
        m->damageCC[iw]->SetInstanceID(entity);
        m->damageCC[iw]->SetCompID(6); // Object State
        m->damageCC[iw]->SetCompState(1);	// 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
        m->damageCC[iw]->SetCompData(0.0f, 0);
        m->damageCC[iw]->SetCompData(0.0f, 1);
        m->damageActive = true;  // true the first time to make sure the model state is good
    }
    CigiCompCtrlV2* const damage = m->damageCC[iw];

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
            ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            ec->SetAttachState(CigiEntityCtrlV2::Detach);
            ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            ec->SetEffectState(CigiEntityCtrlV2::StopV2);
            ec->SetOpacity(100.0f);
            ec->SetTemp(0.0f);
        }
        m->parentActive = ok;

        // Load the smoke entity control
        if (p->getSmoke() > 0.1f) {
            // Set entity id, parent and state
            smoke->SetEntityID(entity+3); // Smoke is at main entity id plus one
            smoke->SetParentID(entity);

            smoke->SetRoll(0.0);
            smoke->SetPitch(0.0);
            smoke->SetHeading(0.0);
            smoke->SetAlt(0.0);
            smoke->SetLat(0.0);
            smoke->SetLon(0.0);

            smoke->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            smoke->SetEntityType(cmtSmokePlume);
            smoke->SetAttachState(CigiEntityCtrlV2::Attach);
            smoke->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            //smoke->SetEffectState(CigiEntityCtrlV2::StopV2);
            smoke->SetEffectState(CigiEntityCtrlV2::PlayV2);
            smoke->SetOpacity(66.6f);
            smoke->SetTemp(0.0f);
            m->smokeActive = true;
        }
        else {
            smoke->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
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
           damage->SetCompState(1);       // 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
        }

    }
    else {
        ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
        smoke->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
    }
    return true;
}

//------------------------------------------------------------------------------
// setGndVehicleData() -- Sets a 'model_t' structure to a ground vheicle's state
//------------------------------------------------------------------------------
bool OtwCigiClV2::setGndVehicleData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::GroundVehicle* const p)
{
    // Make sure we have an entity control block
    if (m->parentEC[iw] == nullptr) {
        m->parentEC[iw] = new CigiEntityCtrlV2();
    }
    CigiEntityCtrlV2* const ec = m->parentEC[iw];

    // Smoke control block, ...
    if (m->smokeEC[iw] == nullptr) {
        m->smokeEC[iw] = new CigiEntityCtrlV2();
        m->smokeActive = false;
    }
    CigiEntityCtrlV2* const smoke = m->smokeEC[iw];

    // Damage control, ...
    if (m->damageCC[iw] == nullptr) {
        m->damageCC[iw]->SetCompClassV2(CigiCompCtrlV2::EntityV2);
        m->damageCC[iw]->SetInstanceID(entity);
        m->damageCC[iw]->SetCompID(6); // Object State
        m->damageCC[iw]->SetCompState(1);	// 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
        m->damageCC[iw]->SetCompData(0.0f, 0);
        m->damageCC[iw]->SetCompData(0.0f, 1);
        m->damageActive = true;  // true the first time to make sure the model state is good
    }
    CigiCompCtrlV2* const damage = m->damageCC[iw];

    // Articulated launcher control
    if (m->launcherAPC[iw] == nullptr) {
        m->launcherAPC[iw] = new CigiArtPartCtrlV2();
        m->launcherApcActive = false;
    }
    CigiArtPartCtrlV2* const launcherAPC = m->launcherAPC[iw];

    // Attached part, ...
    if (m->attachedEC[iw] == nullptr) {
        m->attachedEC[iw] = new CigiEntityCtrlV2();
        m->attachedEcActive = false;
    }
    CigiEntityCtrlV2* const attachedPartEC = m->attachedEC[iw];

    // Damage control, ...
    if (m->attachedCC[iw] == nullptr) {
        m->attachedCC[iw] = new CigiCompCtrlV2();
        m->attachedCcActive = false;
    }
    CigiCompCtrlV2* const attachedPartCC = m->attachedCC[iw];

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
            ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            ec->SetAttachState(CigiEntityCtrlV2::Detach);
            ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            ec->SetEffectState(CigiEntityCtrlV2::StopV2);
            ec->SetOpacity(100.0f);
            ec->SetTemp(0.0f);
        }
        m->parentActive = ok;

        // Load the smoke entity control
        if (p->getSmoke() > 0.1f) {
            // Set entity id, parent and state
            smoke->SetEntityID(entity+3); // Smoke is at main entity id plus one
            smoke->SetParentID(entity);

            smoke->SetRoll(0.0);
            smoke->SetPitch(0.0);
            smoke->SetHeading(0.0);
            smoke->SetAlt(0.0);
            smoke->SetLat(0.0);
            smoke->SetLon(0.0);

            smoke->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            smoke->SetEntityType(cmtSmokePlume);
            smoke->SetAttachState(CigiEntityCtrlV2::Attach);
            smoke->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            //smoke->SetEffectState(CigiEntityCtrlV2::StopV2);
            smoke->SetEffectState(CigiEntityCtrlV2::PlayV2);
            smoke->SetOpacity(66.6f);
            smoke->SetTemp(0.0f);
            m->smokeActive = true;
        }
        else {
            smoke->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
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
        ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
        smoke->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
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
                const Basic::Pair* pair = (Basic::Pair*) item->getValue();
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
       launcherAPC->SetPitch(p->getLauncherPosition() * (LCreal) Basic::Angle::R2DCC);
       m->launcherApcActive = true;

       // Attached missile
       attachedPartEC->SetEntityID(entity + 4);  // attached part
       attachedPartEC->SetParentID(entity);
       attachedPartEC->SetRoll(0.0f);
       attachedPartEC->SetPitch(0.0f);
       attachedPartEC->SetHeading(0.0f);
       attachedPartEC->SetAlt(0.0f);
       attachedPartEC->SetLat(0.0);
       attachedPartEC->SetLon(0.0);
       attachedPartEC->SetEntityType(917);     // SCUD missile type
       attachedPartEC->SetAttachState(CigiEntityCtrlV2::Attach);
       attachedPartEC->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
       attachedPartEC->SetEffectState(CigiEntityCtrlV2::StopV2);
       attachedPartEC->SetOpacity(100.0f);
       attachedPartEC->SetTemp(0.0f);
       if (apartNumMissiles > 0) {

          attachedPartEC->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
          m->attachedEcActive = true;

          // Attached missile to launcher's articulation
          attachedPartCC->SetCompClassV2(CigiCompCtrlV2::EntityV2);
          attachedPartCC->SetInstanceID(attachedPartEC->GetEntityID());
          attachedPartCC->SetCompID(9); // 9 value is for stores attachment to locations
          attachedPartCC->SetCompState(0);
          attachedPartCC->SetCompData(0.0f, 0);
          attachedPartCC->SetCompData(0.0f, 1);
          m->attachedCcActive = true;

       }
       else {
          attachedPartEC->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
       }

    }
    else {
       // Not articulation or attached parts
       launcherAPC->SetPitchEn(false);
       attachedPartEC->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
    }
    return true;
}

//------------------------------------------------------------------------------
// setEffectsData() -- Sets a 'model_t' structure to a effects' state
//------------------------------------------------------------------------------
bool OtwCigiClV2::setEffectsData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Effects* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV2();
   }
   CigiEntityCtrlV2* const ec = m->parentEC[iw];

   // a smoke trail entity control block, ...
   if (m->trailEC[iw] == nullptr) {
      m->trailEC[iw] = new CigiEntityCtrlV2();
      m->trailActive = false;
   }
   CigiEntityCtrlV2* const trail = m->trailEC[iw];

   if (m->getState() == Simulation::OtwModel::ACTIVE) {

      // As long as we're active, update the entity control
      if (p->isClassType(typeid(Simulation::Decoy))) {
         // Decoy

         // Load the parent entity control
         setCommonModelData(ec,entity,p);

         // Set the entity type
         unsigned int tt = 0;
         const Simulation::Otm* otm = m->getTypeMapper();
         if (otm != 0) tt = otm->getTypeId();
         if (tt > 0xffff) tt = 0;   // unsigned short only
         ec->SetEntityType(static_cast<unsigned short>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
         ec->SetAttachState(CigiEntityCtrlV2::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
         ec->SetEffectState(CigiEntityCtrlV2::StopV2);
         ec->SetOpacity(100.0f);
         ec->SetTemp(0.0f);
         m->parentActive = true;

         // Load the smoke trail entity control
         {
            // Set entity id, parent and state
            trail->SetEntityID(entity+1); // Trails are at main entity id plus one
            trail->SetParentID(entity);

            trail->SetRoll(0.0f);
            trail->SetPitch(0.0f);
            trail->SetHeading(0.0f);
            trail->SetAlt(0.0f);
            trail->SetLat(0.0);
            trail->SetLon(0.0);

            trail->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            trail->SetEntityType(cmtMslTrail);
            trail->SetAttachState(CigiEntityCtrlV2::Attach);
            trail->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            trail->SetEffectState(CigiEntityCtrlV2::StopV2);
            //trail->SetEffectState(CigiEntityCtrlV2::PlayV2);
            trail->SetOpacity(66.6f);
            trail->SetTemp(0.0f);
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
            ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            ec->SetAttachState(CigiEntityCtrlV2::Detach);
            ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            ec->SetEffectState(CigiEntityCtrlV2::PlayV2);
            ec->SetOpacity(100.0f);
            ec->SetTemp(0.0f);
         }
         m->parentActive = ok;
      }
      else {
         // Unknown type
         ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
         trail->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
      }

   }
   else {
      ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
      trail->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
   }

    return true;
}

//------------------------------------------------------------------------------
// setLifeFormData() -- Sets a 'model_t' structure to a lifeform's state
//------------------------------------------------------------------------------
bool OtwCigiClV2::setLifeFormData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::LifeForm* const p)
{

    // Make sure we have an entity control block
    if (m->parentEC[iw] == nullptr) {
        m->parentEC[iw] = new CigiEntityCtrlV2();
    }
    CigiEntityCtrlV2* const ec = m->parentEC[iw];

    // Animation control, ...
    if (m->animationCC[iw] == nullptr) {
        m->animationCC[iw] = new CigiCompCtrlV2();
        m->animationCC[iw]->SetCompClassV2(CigiCompCtrlV2::EntityV2);
        m->animationCC[iw]->SetInstanceID(entity);
        m->animationCC[iw]->SetCompID(8); // Change Model Animation
        m->animationCC[iw]->SetCompState(STANDING);
        m->animationCC[iw]->SetCompData(0.0f, 0);
        m->animationCC[iw]->SetCompData(0.0f, 1);
        m->animationActive = true;  // true the first time to make sure the model state is good
    }
    CigiCompCtrlV2* const animation = m->animationCC[iw];

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
            ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            ec->SetAttachState(CigiEntityCtrlV2::Detach);
            ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            ec->SetEffectState(CigiEntityCtrlV2::StopV2);
            ec->SetOpacity(100.0f);
            ec->SetTemp(0.0f);
        }
        m->parentActive = ok;

        Simulation::LifeForm* lf = (Simulation::LifeForm*)p;

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
        ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
    }
    return true;
}


//------------------------------------------------------------------------------
// setMissileData() -- Sets a 'model_t' structure to a missile's state
//------------------------------------------------------------------------------
bool OtwCigiClV2::setMissileData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Missile* const p)
{
    // Make sure we have an entity control block, ...
    if (m->parentEC[iw] == nullptr) {
        m->parentEC[iw] = new CigiEntityCtrlV2();
        m->parentActive = false;
    }
    CigiEntityCtrlV2* const ec = m->parentEC[iw];

    // a smoke trail entity control block, ...
    if (m->trailEC[iw] == nullptr) {
        m->trailEC[iw] = new CigiEntityCtrlV2();
        m->trailActive = false;
    }
    CigiEntityCtrlV2* const trail = m->trailEC[iw];

    // an air explosion entity control block, ...
    if (m->explosionEC[iw] == nullptr) {
        m->explosionEC[iw] = new CigiEntityCtrlV2();
        m->explosionEC[iw]->SetEntityID(entity+2); // Explosions are at main entity_id plus two
        m->explosionEC[iw]->SetParentID(entity);

        m->explosionEC[iw]->SetRoll(0.0f);
        m->explosionEC[iw]->SetPitch(0.0f);
        m->explosionEC[iw]->SetHeading(0.0f);
        m->explosionEC[iw]->SetAlt(0.0f);
        m->explosionEC[iw]->SetLat(0.0);
        m->explosionEC[iw]->SetLon(0.0);

        m->explosionEC[iw]->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
        m->explosionEC[iw]->SetEntityType(cmtAirExplosion);
        m->explosionEC[iw]->SetAttachState(CigiEntityCtrlV2::Detach);
        m->explosionEC[iw]->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
        m->explosionEC[iw]->SetEffectState(CigiEntityCtrlV2::StopV2);
        m->explosionEC[iw]->SetOpacity(100.0f);
        m->explosionEC[iw]->SetTemp(0.0f);
        m->explosionActive = true;  // True the first time to make sure it's loaded
        //std::cout << "explosion: create" << m->explosionEC[iw]->entity_id << std::endl;
    }
    CigiEntityCtrlV2* const explosion = m->explosionEC[iw];

    // As long as we're active, update the entity control
    if (m->getState() == Simulation::OtwModel::ACTIVE) {
        // Load the parent entity control
        setCommonModelData(ec,entity,p);

        // Set the entity type
        unsigned int tt = 0;
        const Simulation::Otm* otm = m->getTypeMapper();
        if (otm != 0) tt = otm->getTypeId();
        if (tt > 0xffff) tt = 0;   // unsigned shorts only
        ec->SetEntityType(static_cast<unsigned short>(tt));

        // Set entity control data
        ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
        ec->SetAttachState(CigiEntityCtrlV2::Detach);
        ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
        ec->SetEffectState(CigiEntityCtrlV2::StopV2);
        ec->SetOpacity(100.0f);
        ec->SetTemp(0.0f);
        m->parentActive = true;

        // Load the smoke trail entity control
        {
            // Set entity id, parent and state
            trail->SetEntityID(entity+1); // Trails are at main entity id plus one
            trail->SetParentID(entity);

            trail->SetRoll(0.0f);
            trail->SetPitch(0.0f);
            trail->SetHeading(0.0f);
            trail->SetAlt(0.0f);
            trail->SetLat(0.0);
            trail->SetLon(0.0);

            //trail->Seteffect_state = EFFECT_ANIMATION_STATE_PLAY;
            trail->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            trail->SetEntityType(cmtMslTrail);
            trail->SetAttachState(CigiEntityCtrlV2::Attach);
            trail->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            trail->SetEffectState(CigiEntityCtrlV2::StopV2);
            //trail->SetEffectState(CigiEntityCtrlV2::PlayV2);
            trail->SetOpacity(66.6f);
            trail->SetTemp(0.0f);
            m->trailActive = true;
        }
    }
    else {
        ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
        trail->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
        if (m->getState() == Simulation::OtwModel::DEAD) {
           // Start air explosion at last known location of missile
           explosion->SetRoll(ec->GetRoll());
           explosion->SetPitch(ec->GetPitch());
           explosion->SetHeading(ec->GetHeading());
           if (ec->GetAlt() > p->getTerrainElevation())
              explosion->SetAlt(ec->GetAlt());
           else
              explosion->SetAlt(p->getTerrainElevation());
           explosion->SetLat(ec->GetLat());
           explosion->SetLon(ec->GetLon());
           //explosion->SetEffectState(CigiEntityCtrlV2::PlayV2);
           explosion->SetEffectState(CigiEntityCtrlV2::RestartV2);
           m->explosionActive = true;  // True to start to sequence
        }
    }
    return true;
}

//------------------------------------------------------------------------------
// setShipData() -- Sets a 'model_t' structure to a ship's state
//------------------------------------------------------------------------------
bool OtwCigiClV2::setShipData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Ship* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV2();
   }
   CigiEntityCtrlV2* const ec = m->parentEC[iw];

    // a wake entity control block, ...
    if (m->trailEC[iw] == nullptr) {
        m->trailEC[iw] = new CigiEntityCtrlV2();
        m->trailActive = false;
    }
   CigiEntityCtrlV2* const wake = m->trailEC[iw];

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
         ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
         ec->SetAttachState(CigiEntityCtrlV2::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
         ec->SetEffectState(CigiEntityCtrlV2::StopV2);
         ec->SetOpacity(100.0f);
         ec->SetTemp(0.0f);
      }
      m->parentActive = ok;

      // Load the wake entity control
      {
         // Set entity id, parent and state
         wake->SetEntityID(entity+1); // Trails (wakes) are at parent entity plus one
         wake->SetParentID(entity);

         wake->SetRoll(0.0f);
         wake->SetPitch(0.0f);
         wake->SetHeading(0.0f);
         wake->SetAlt(0.0f);
         wake->SetLat(0.0);
         wake->SetLon(0.0);

         wake->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
         wake->SetEntityType(cmtShipWake);
         wake->SetAttachState(CigiEntityCtrlV2::Attach);
         wake->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
         wake->SetEffectState(CigiEntityCtrlV2::PlayV2);
         wake->SetOpacity(66.6f);
         wake->SetTemp(0.0f);
         m->trailActive = true;
      }
   }
   else {
      ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
      wake->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
   }

   return true;
}

//------------------------------------------------------------------------------
// setSpaceVehicleData()
//  -- Sets a CigiEntityCtrlV2 structure to a space vheicle's state
//------------------------------------------------------------------------------
bool OtwCigiClV2::setSpaceVehicleData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::SpaceVehicle* const p)
{
    // Make sure we have an entity control block
    if (m->parentEC[iw] == nullptr) {
        m->parentEC[iw] = new CigiEntityCtrlV2();
    }
    CigiEntityCtrlV2* const ec = m->parentEC[iw];

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
            ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
            ec->SetAttachState(CigiEntityCtrlV2::Detach);
            ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
            ec->SetEffectState(CigiEntityCtrlV2::StopV2);
            ec->SetOpacity(100.0f);
            ec->SetTemp(0.0f);
        }
        m->parentActive = ok;
    }
    else {
        ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
    }
    return true;
}

//------------------------------------------------------------------------------
// setWeaponData() -- Sets a 'model_t' structure to a weapon's state
//------------------------------------------------------------------------------
bool OtwCigiClV2::setWeaponData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Weapon* const p)
{
    // Make sure we have an entity control block, ...
    if (m->parentEC[iw] == nullptr) {
        m->parentEC[iw] = new CigiEntityCtrlV2();
        m->parentActive = false;
    }
    CigiEntityCtrlV2* const ec = m->parentEC[iw];

    // an ground explosion entity control block, ...
    if (m->explosionEC[iw] == nullptr) {
        m->explosionEC[iw] = new CigiEntityCtrlV2();
        m->explosionEC[iw]->SetEntityID(entity+2); // Explosions are at main entity_id plus two
        m->explosionEC[iw]->SetParentID(entity);

        m->explosionEC[iw]->SetRoll(0.0f);
        m->explosionEC[iw]->SetPitch(0.0f);
        m->explosionEC[iw]->SetHeading(0.0f);
        m->explosionEC[iw]->SetAlt(0.0f);
        m->explosionEC[iw]->SetLat(0.0);
        m->explosionEC[iw]->SetLon(0.0);

        m->explosionEC[iw]->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
        m->explosionEC[iw]->SetEntityType(cmtAirExplosion);
        m->explosionEC[iw]->SetAttachState(CigiEntityCtrlV2::Detach);
        m->explosionEC[iw]->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
        m->explosionEC[iw]->SetEffectState(CigiEntityCtrlV2::StopV2);
        m->explosionEC[iw]->SetOpacity(100.0f);
        m->explosionEC[iw]->SetTemp(0.0f);
        m->explosionActive = true;  // True the first time to make sure it's loaded
        //std::cout << "explosion: create" << m->explosionEC[iw]->entity_id << std::endl;
    }
    CigiEntityCtrlV2* const explosion = m->explosionEC[iw];

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
        ec->SetEntityState(CigiEntityCtrlV2::LoadShowV2);
        ec->SetAttachState(CigiEntityCtrlV2::Detach);
        ec->SetCollisionDetectEn(CigiEntityCtrlV2::Disable);
        ec->SetEffectState(CigiEntityCtrlV2::StopV2);
        ec->SetOpacity(100.0f);
        ec->SetTemp(0.0f);
        m->parentActive = true;
    }
    else {
        ec->SetEntityState(CigiEntityCtrlV2::LoadHideV2);
        if (m->getState() == Simulation::OtwModel::DEAD) {
            // Start air explosion at last known location of missile
            explosion->SetRoll(ec->GetRoll());
            explosion->SetPitch(ec->GetPitch());
            explosion->SetHeading(ec->GetHeading());
            if (ec->GetAlt() > p->getTerrainElevation())
                explosion->SetAlt(ec->GetAlt());
            else
               explosion->SetAlt(p->getTerrainElevation());
            explosion->SetLat(ec->GetLat());
            explosion->SetLon(ec->GetLon());
            //explosion->SetEffectState(CigiEntityCtrlV2::PlayV2);
            explosion->SetEffectState(CigiEntityCtrlV2::RestartV2);
            m->explosionActive = true;  // True to start to sequence
        }
    }
    return true;
}

//------------------------------------------------------------------------------
// Line of sight request -- returns true of request was issued
//------------------------------------------------------------------------------
bool OtwCigiClV2::lineOfSightRequest(
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
      CigiLosVectReqV2* losReq = getLosRangeRequestPacket();
      if (losReq != nullptr) {
         losReq->SetSrcLat(lat);
         losReq->SetSrcLon(lon);
         losReq->SetSrcAlt(alt);
         losReq->SetVectAz(-hdg);
         losReq->SetVectEl(pitch);
         losReq->SetMinRange(minRange);
         losReq->SetMaxRange(maxRange);
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
bool OtwCigiClV2::getLineOfSightData(
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
      if (material != nullptr) *material = losRespMaterialCode;
   }
   return ok;
}


//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

// Set CIGI Handler
bool OtwCigiClV2::setSlotCigi(CigiClV2* const msg)
{
    cigi = msg;
    return true;
}

// Set/clear the ASYNC mode
bool OtwCigiClV2::setSlotASyncMode(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setASyncMode(msg->getBoolean());
   }
   return ok;
}

// Set/clear the hide ownship model flag
bool OtwCigiClV2::setSlotHideOwnshipModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setHideOwnshipModel(msg->getBoolean());
   }
   return ok;
}

bool OtwCigiClV2::setSlotOwnshipModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setOwnshipModelId((unsigned short) v);
      }
   }
   return ok;
}

bool OtwCigiClV2::setSlotMslTrailModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setMslTrailModelId((unsigned short) v);
      }
   }
   return ok;
}

bool OtwCigiClV2::setSlotSmokePlumeModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setSmokePlumeModelId((unsigned short) v);
      }
   }
   return ok;
}

bool OtwCigiClV2::setSlotAirExplosionModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setAirExplosionModelId((unsigned short) v);
      }
   }
   return ok;
}

bool OtwCigiClV2::setSlotGroundExplosionModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setGroundExplosionModelId((unsigned short) v);
      }
   }
   return ok;
}

bool OtwCigiClV2::setSlotShipWakeModel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xFFFF) {
         ok = setShipWakeModelId((unsigned short) v);
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* OtwCigiClV2::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// sendCigiData() -- Handles sending CIGI data to the visual system
//------------------------------------------------------------------------------
bool OtwCigiClV2::sendCigiData()
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
    CigiIGCtrlV2* ig_cp = getIgControlPacket();
    ig_cp->SetDatabaseID(0);
    ig_cp->SetIGMode(CigiIGCtrlV2::Operate);
    if (resetRequest) {
       ig_cp->SetIGMode(CigiIGCtrlV2::Standby);
       resetRequest = false;
    }
    ig_cp->SetTrackDeviceEn(CigiIGCtrlV2::Disable);
    ig_cp->SetBoresightTrackDevice(true);
    ig_cp->SetFrameCntr(ig_cp->GetFrameCntr() + 1);
    ig_cp->SetTimeStamp(0.0f);
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
        OtwModelCigiClV2** const table = (OtwModelCigiClV2**) getModelTable();
        if (table != nullptr && getModelTableSize() > 0) {

            // Need at least this much in the buffer to add another entity
            int padding = (sizeof(CigiEntityCtrlV2) * 2) + sizeof(CigiHotReqV2);
            if (newLosReq && getLosRangeRequestPacket() != nullptr) padding += sizeof(CigiLosVectReqV2);
            if (getViewControlPacket() != nullptr) padding += sizeof(CigiViewCtrlV2);
            if (getViewDefinitionPacket() != nullptr) padding += sizeof(CigiViewDefV2);

            // Add all of the models (that we can) to the buffer.
            int sendSize = cigi->getOutgoingBufferSize();
            int maxAge = getModelTableSize();
            for (unsigned short i = 0; i < getModelTableSize() && sendSize < (MAX_BUF_SIZE - padding); i++) {
                Basic::safe_ptr<OtwModelCigiClV2> model( (OtwModelCigiClV2*) table[i] );
                if (model != nullptr) {

                  // For all active models in the table ...
                  // (send ground models only after 'maxAge' frames)
                  // (always update the inactive models to clear them)
                  model->incAgeCount();
                  bool updateThisOne = !model->isGroundPlayer || model->isState(Simulation::OtwModel::OUT_OF_RANGE) || (model->isGroundPlayer && model->getAgeCount() >= maxAge);

                  if (updateThisOne) {
                     //if (model->isGroundPlayer) {
                     //   std::cout << "model: " << model->getID() << ", pid=" << model->getPlayer()->getID() << std::endl;
                     //}
                     // Explostion?
                     if (model->explosionActive && model->explosionEC[ir] != nullptr) {
                        cigi->addPacketEntityCtrl(model->explosionEC[ir]);
                        model->explosionActive = false;
                     }

                     if (model->parentActive && model->parentEC[ir] != nullptr) {
                        cigi->addPacketEntityCtrl(model->parentEC[ir]);
                        model->parentActive = (model->parentEC[ir]->GetEntityState() == CigiEntityCtrlV2::LoadShowV2);

                        // Trail effect?
                        if (model->trailActive && model->trailEC[ir] != nullptr) {
                              cigi->addPacketEntityCtrl(model->trailEC[ir]);
                              model->trailActive = (model->trailEC[ir]->GetEntityState() == CigiEntityCtrlV2::LoadShowV2);
                        }

                        // Smoke affect?
                        if (model->smokeActive && model->smokeEC[ir] != nullptr) {
                              cigi->addPacketEntityCtrl(model->smokeEC[ir]);
                              model->smokeActive = (model->smokeEC[ir]->GetEntityState() == CigiEntityCtrlV2::LoadShowV2);
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
                              model->attachedEcActive = (model->attachedEC[ir]->GetEntityState() == CigiEntityCtrlV2::LoadShowV2);
                        }

                        // Attached part component control?
                        if (model->attachedCcActive && model->attachedCC[ir] != nullptr && model->attachedEC[ir] != nullptr) {
                              cigi->addPacketComponentCtrl(model->attachedCC[ir]);
                              // we come and go with the attached part
                              model->attachedCcActive = (model->attachedEC[ir]->GetEntityState() == CigiEntityCtrlV2::LoadShowV2);
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
        OtwModelCigiClV2** const table = (OtwModelCigiClV2**) getElevationTable();
        if (table != nullptr && getElevationTableSize() > 0) {
            int maxAge = getElevationTableSize() * 4;

            // For all active elevation requests in the table ...
            // -- look for the oldest request ---
            Basic::safe_ptr<OtwModelCigiClV2> oldest( nullptr );
            Basic::safe_ptr<OtwModelCigiClV2> model( nullptr );
            for (unsigned short i = 0; i < getElevationTableSize(); i++) {
                model = table[i];
                if (model != nullptr) {
                  // Must be active and haven't been requested for at least TBD frames ...
                  if (model->isHotActive() && model->getReqCount() >= maxAge) {
                     if (oldest == 0) oldest = model;
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
                  CigiHotReqV2 hotRequest;
                  hotRequest.SetHatHotID(table[idx]->getID());

                  // Requested Position (lat/lon)
                  double hotLat(0.0), hotLon(0.0);
                  oldest->getPlayer()->getPositionLL(&hotLat, &hotLon);
                  hotRequest.SetLat(hotLat);
                  hotRequest.SetLon(hotLon);
                  //osg::Vec3 pos = oldest->getPlayer()->getPosition();
                  //LCreal alt;
                  //Basic::Nav::convertPosVec2LL(
                  //         getRefLatitude(), getRefLongitude(),
                  //         pos,
                  //         &hotRequest.lat, &hotRequest.lon, &alt);

                  cigi->addPacketHotReq(&hotRequest);
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
      CigiLosVectReqV2* los0 = getLosRangeRequestPacket();
      if (isNewLosequested() && los0 != nullptr) {
         los->SetLosID(getNexLosId());
         cigi->addPacketLosRangeReq(los0);
         losRequestSend();
      }
    }

    // ---
    // Optional view control and definition packets
    // ---
    if (getViewControlPacket() != nullptr) {
       CigiViewCtrlV2* myView = getViewControlPacket();
       //if ( getMaxModels() == 20 ) {
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

       cigi->addPacketViewCtrl(myView);
    }
    if (getViewDefinitionPacket() != nullptr) {
       cigi->addPacketViewDef(getViewDefinitionPacket());
	   //CigiViewDefV2* myView = getViewDefinitionPacket();
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
void OtwCigiClV2::startOfFrame(const CigiSOFV2* const)
{
   if (isSyncMode()) sendCigiData();
}

//------------------------------------------------------------------------------
// hatResp() -- Handles Height-Above-Terrain Response packets
//------------------------------------------------------------------------------
void OtwCigiClV2::hatResp(const CigiHatRespV2* const p)
{
   if (p != nullptr) {
      std::printf("<< Incoming\n");
#if 0
      CigiPrintHatResponsePacket(p, 2);
#endif
      std::printf("\n");
   }
}

//------------------------------------------------------------------------------
// losResp() -- Handles Line-Of-Sight Response packets
//------------------------------------------------------------------------------
void OtwCigiClV2::losResp(const CigiLosRespV2* const p)
{
   if ( p != nullptr && isLosRequestPending() && losReqTimer > 0.0 ) {
      // We do have a pending request that hasn't timed out,
      // but is this it?
      if (p->GetLosID() == losReqId) {
         // We've the packet that we're lookng for,
         // but is it valid (the IG found an intersection point)
         if (p->GetValid()) {
            // We have valid data
            losRespDataValid = true;
            losRespLat = p->GetLatitude();
            losRespLon = p->GetLongitude();
            losRespAlt = p->GetAltitude();
            losRespRange = p->GetRange();
            losRespMaterialCode = p->GetMaterial();
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
void OtwCigiClV2::losRequestSend()
{
   newLosReq = false;
   losReqTimer = LOS_REQ_TIMEOUT;
}

//------------------------------------------------------------------------------
// Elevation request has been sent to the IG
//------------------------------------------------------------------------------
void OtwCigiClV2::elevationRequestSend()
{
   elevReqFlg = true;   // active request
   elevReqTimer = 0.5f; // time-out
}


//------------------------------------------------------------------------------
// collisionSegmentResp() -- Handles Collision Segment Response packets
//------------------------------------------------------------------------------
void OtwCigiClV2::collisionSegmentResp(const CigiCollDetSegRespV2* const p)
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
void OtwCigiClV2::sensorResp(const CigiSensorRespV2* const p)
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
// hotResp() -- Handles Height-Of-Terrain Response packets
//------------------------------------------------------------------------------
void OtwCigiClV2::hotResp(const CigiHotRespV2* const p)
{
    // Valid?
    if (p != nullptr && p->GetValid()) {
        // Yes and the hot_id is the elevation table index
        unsigned short id = p->GetHatHotID();
        //std::cout << "hotResp: id = " << id << std::endl;
        OtwModelCigiClV2** const table = (OtwModelCigiClV2**) getElevationTable();
        Basic::safe_ptr<OtwModelCigiClV2> model(nullptr);
        for (unsigned int i = 0; i < getElevationTableSize() && model == nullptr; i++) {
           if (table[i]->getID() == id) model = table[i];
        }
        if (model != nullptr) {
            if (model->isHotActive() && model->getPlayer() != nullptr) {
               // When the player and elevation table are still valid, store
               // the terrain elevation (meters)
               model->getPlayer()->setTerrainElevation((LCreal)p->GetHot());
               //std::cout << "hotResp: alt = " << p->alt << ", pid = " << model->getPlayer()->getID() << std::endl;
            }
        }
        elevReqFlg = false;
        elevReqTimer = 0.0;
    }
}

//------------------------------------------------------------------------------
// collisionVolumeResp() -- Handles Collision Volume Response packets
//------------------------------------------------------------------------------
void OtwCigiClV2::collisionVolumeResp(const CigiCollDetVolRespV2* const p)
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
void OtwCigiClV2::igResponse(const CigiIGMsgV2* const p)
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

bool OtwCigiClV2::setViewControlPacket(CigiViewCtrlV2* const p)
{
   view = p;
   return true;
}

bool OtwCigiClV2::setViewDefinitionPacket(CigiViewDefV2* const p)
{
   fov = p;
   return true;
}

// Ownship's model ID
bool OtwCigiClV2::setOwnshipModelId(const unsigned short v)
{
   cmtOwnship = v;
   return true;
}

// "Missile Trail" effect model ID
bool OtwCigiClV2::setMslTrailModelId(const unsigned short v)
{
   cmtMslTrail = v;
   return true;
}

// "Smoke Plume" effect model ID
bool OtwCigiClV2::setSmokePlumeModelId(const unsigned short v)
{
   cmtSmokePlume = v;
   return true;
}

// "Air Explosion" effect model ID
bool OtwCigiClV2::setAirExplosionModelId(const unsigned short v)
{
   cmtAirExplosion = v;
   return true;
}

// "GroundE xplosion" effect model ID
bool OtwCigiClV2::setGroundExplosionModelId(const unsigned short v)
{
   cmtGroundExplosion = v;
   return true;
}

// "Ship Wake" effect model ID
bool OtwCigiClV2::setShipWakeModelId(const unsigned short v)
{
   cmtShipWake = v;
   return true;
}

//==============================================================================
// CigiClV2 class
//==============================================================================

IMPLEMENT_ABSTRACT_SUBCLASS(CigiClV2,"CigiClV2Abstract")
EMPTY_SLOTTABLE(CigiClV2)
EMPTY_SERIALIZER(CigiClV2)

//------------------------------------------------------------------------------
// Static data
//------------------------------------------------------------------------------
bool CigiClV2::cigiInitialized = false;    // CIGI has been initialized
bool CigiClV2::cigiInitFailed  = false;    // CIGI initialization has failed

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
CigiClV2::CigiClV2()
{
   STANDARD_CONSTRUCTOR()

   session = nullptr;
   otwCigi = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void CigiClV2::copyData(const CigiClV2& org, const bool)
{
   BaseClass::copyData(org);

   session = nullptr;
   otwCigi = nullptr;  // don't unref();
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void CigiClV2::deleteData()
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
bool CigiClV2::initialize(OtwCigiClV2* const p)
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
      //session->SetCigiVersion(2);	// CGB only force this if necessary
      // CGBCGB ??? session->SetSynchronous(p->isSyncMode());
   }

    return session != nullptr;
}

//------------------------------------------------------------------------------
// True if we've been successfully initialized
//------------------------------------------------------------------------------
bool CigiClV2::isInitialized()
{
    return cigiInitialized && (session != nullptr);
}

//------------------------------------------------------------------------------
// initCigi() -- Initialize the static CIGI system
//------------------------------------------------------------------------------
bool CigiClV2::initCigi()
{
    std::cout << "CigiClV2::initCigi(): CIGI initialized!" << std::endl;
    return true;
}

//------------------------------------------------------------------------------
// R/T frame sync -- called from OtwCigiClV2::frameSync() in the R/T thread
//------------------------------------------------------------------------------
void CigiClV2::frameSync()
{
}

//------------------------------------------------------------------------------
// Background frame sync -- called from OtwCigiClV2::updateData() in the background
//------------------------------------------------------------------------------
void CigiClV2::frameBg()
{
}

//==============================================================================
// CigiClV2NetworkSignalProcessing class
//==============================================================================
class CigiClV2NetworkSignalProcessing : public CigiBaseSignalProcessing {
public:
    inline CigiClV2NetworkSignalProcessing(OtwCigiClV2* parent) : p(parent) { }
    virtual ~CigiClV2NetworkSignalProcessing();

    virtual void OnSOF(CigiBasePacket* packet);
    virtual void OnCollDetSegResp(CigiBasePacket* packet);
    virtual void OnCollDetVolResp(CigiBasePacket* packet);
    virtual void OnHatResp(CigiBasePacket* packet);
    virtual void OnHotResp(CigiBasePacket* packet);
    virtual void OnIGMsg(CigiBasePacket* packet);
    virtual void OnLosResp(CigiBasePacket* packet);
    virtual void OnSensorResp(CigiBasePacket* packet);

private:
    OtwCigiClV2* p;
};

CigiClV2NetworkSignalProcessing::~CigiClV2NetworkSignalProcessing()
{
    p = nullptr;
}

//------------------------------------------------------------------------------
// callbacks -- Pass control to the correct object (session's) member function
//------------------------------------------------------------------------------

void CigiClV2NetworkSignalProcessing::OnSOF(CigiBasePacket* packet)
{
    if (p != nullptr) p->startOfFrame( dynamic_cast<CigiSOFV2*> (packet) );
}

void CigiClV2NetworkSignalProcessing::OnCollDetSegResp(CigiBasePacket* packet)
{
    if (p != nullptr) p->collisionSegmentResp( dynamic_cast<CigiCollDetSegRespV2*> (packet) );
}

void CigiClV2NetworkSignalProcessing::OnCollDetVolResp(CigiBasePacket* packet)
{
    if (p != nullptr) p->collisionVolumeResp( dynamic_cast<CigiCollDetVolRespV2*> (packet) );
}

void CigiClV2NetworkSignalProcessing::OnHatResp(CigiBasePacket* packet)
{
    if (p != nullptr) p->hatResp( dynamic_cast<CigiHatRespV2*> (packet) );
}

void CigiClV2NetworkSignalProcessing::OnHotResp(CigiBasePacket* packet)
{
    if (p != nullptr) p->hotResp( dynamic_cast<CigiHotRespV2*> (packet) );
}

void CigiClV2NetworkSignalProcessing::OnIGMsg(CigiBasePacket* packet)
{
    if (p != nullptr) p->igResponse( dynamic_cast<CigiIGMsgV2*> (packet) );
}

void CigiClV2NetworkSignalProcessing::OnLosResp(CigiBasePacket* packet)
{
    if (p != nullptr) p->losResp( dynamic_cast<CigiLosRespV2*> (packet) );
}

void CigiClV2NetworkSignalProcessing::OnSensorResp(CigiBasePacket* packet)
{
    if (p != nullptr) p->sensorResp( dynamic_cast<CigiSensorRespV2*> (packet) );
}


//==============================================================================
// CigiClV2Network class
//==============================================================================

IMPLEMENT_SUBCLASS(CigiClV2Network,"CigiClV2Network")
EMPTY_SERIALIZER(CigiClV2Network)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(CigiClV2Network)
   "netInput",             // 1) Network input handler
   "netOutput",            // 2) Network output handler
END_SLOTTABLE(CigiClV2Network)

// Map slot table to handles
BEGIN_SLOT_MAP(CigiClV2Network)
    ON_SLOT(1, setSlotNetInput,  Basic::NetHandler)
    ON_SLOT(2, setSlotNetOutput, Basic::NetHandler)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
CigiClV2Network::CigiClV2Network() : netInput(nullptr), netOutput(nullptr), sigProcessor(nullptr)
{
   STANDARD_CONSTRUCTOR()

   haveThread = false;
   networkInitialized = false;
   networkInitFailed = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void CigiClV2Network::copyData(const CigiClV2Network& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      netInput = nullptr;
      netOutput = nullptr;
   }

   // We need to init this ourselves, so ...
   netInput = nullptr;
   netOutput = nullptr;
   haveThread = false;
   networkInitialized = false;
   networkInitFailed = false;
   sigProcessor = nullptr;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void CigiClV2Network::deleteData()
{
   netInput = nullptr;
   netOutput = nullptr;
   if (sigProcessor != nullptr) {
	delete sigProcessor;
   }
   sigProcessor = nullptr;
}


//------------------------------------------------------------------------------
// initialize() -- Initialize this CIGI session
//------------------------------------------------------------------------------
bool CigiClV2Network::initialize(OtwCigiClV2* const p)
{
   bool ok = BaseClass::initialize(p);

   if (ok) {
      // ---
      // Set the callback functions
      // ---
      if (sigProcessor == nullptr) {
         OtwCigiClV2* parent = getOtwCigi();
         sigProcessor = new CigiClV2NetworkSignalProcessing(parent);
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
      // Initalize the network handlers
      // ---
      if (!networkInitialized && !networkInitFailed) {
         networkInitialized = initCigiNetwork();
         networkInitFailed = !networkInitialized;
      }

      // ---
      // Create a network thread to handle the blocked network traffic
      // ---
      if (ok && networkInitialized && !haveThread) {
         std::cout << "CREATING CIGI process" << std::endl;
         createCigiProcess();
      }

      std::cout << "CigiClV2Network::initialize(): CIGI handler initialized!" << std::endl;
   }
   return ok;
}

//------------------------------------------------------------------------------
// True if we've been successfully initialized
//------------------------------------------------------------------------------
bool CigiClV2Network::isInitialized()
{
   return BaseClass::isInitialized() && networkInitialized && haveThread;
}

//------------------------------------------------------------------------------
// initCigiClV2Network() -- Init the network
//------------------------------------------------------------------------------
bool CigiClV2Network::initCigiNetwork()
{
    bool ok = true;

    // Initialize network input handler
    if (netInput != nullptr) {
        if (netInput->initNetwork(false)) std::cout << "netInput Initialize OK" << std::endl;
    }
    else {
        std::cerr << "CigiClV2Network::initNetwork(): failure to find the network input handler (see slot 'netInput')" << std::endl;
        ok = false;
    }

    // Initialize network output handler
    if (netOutput != nullptr) {
        if (netOutput->initNetwork(true)) std::cout << "netOutput Initialize OK" << std::endl;
    }
    else {
        std::cerr << "CigiClV2Network::initNetwork(): failure to find the network output handler (see slot 'netOutput')" << std::endl;
        ok = false;
    }

    return ok;
}

//------------------------------------------------------------------------------
// CIGI functions
//------------------------------------------------------------------------------

// Start the message.  This MUST come before any CigiAddPacket*() functions.
void CigiClV2Network::startMessage()
{
    msgOut->BeginMsg();
}

// End the message.
void CigiClV2Network::endMessage()
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

int CigiClV2Network::getOutgoingBufferSize()
{
    int sendSize = 0;
    unsigned char* sendBuff = msgOut->GetMsg(sendSize);
    return sendSize;
}

// Add an IG Control packet
void CigiClV2Network::addPacketIGCtrl(CigiIGCtrlV2* const p)
{
    *msgOut << *p;
}

// Add a View Control packet
void CigiClV2Network::addPacketViewCtrl(CigiViewCtrlV2* const p)
{
    *msgOut << *p;
}

// Add a View Definition packet
void CigiClV2Network::addPacketViewDef(CigiViewDefV2* const p)
{
    *msgOut << *p;
}

// Add an entity control packet
void CigiClV2Network::addPacketEntityCtrl(CigiEntityCtrlV2* const p)
{
    *msgOut << *p;
}

// Add a Component Control packet
void CigiClV2Network::addPacketComponentCtrl(CigiCompCtrlV2* const p)
{
    *msgOut << *p;
}

// Add a Articulated Part Control packet
void CigiClV2Network::addPacketArtPartCtrl(CigiArtPartCtrlV2* const p)
{
    *msgOut << *p;
}

// Add a Height Of Terrain (HOT) Request packet
void CigiClV2Network::addPacketHotReq(CigiHotReqV2* const p)
{
    *msgOut << *p;
}

// Add a LOS/Range Request packet
void CigiClV2Network::addPacketLosRangeReq(CigiLosVectReqV2* const p)
{
    *msgOut << *p;
}

//------------------------------------------------------------------------------
// Thread functions
//------------------------------------------------------------------------------

#ifdef WIN32
// Windows thread #######################
static DWORD WINAPI cigiThreadFunc(LPVOID lpParam)
{
    CigiClV2Network* otwCigi = (CigiClV2Network*) lpParam;
    std::cout << "Thread: CigiClV2Network = " << otwCigi << std::endl;
    otwCigi->mainLoop();
    return 0;
}

#else
// Linux thread #######################
static void* cigiThreadFunc(void* lpParam)
{
    CigiClV2Network* otwCigi = (CigiClV2Network*) lpParam;
    std::cout << "Thread: CigiClV2Network = " << otwCigi << std::endl;
    otwCigi->mainLoop();
    return nullptr;
}
#endif

//------------------------------------------------------------------------------
// createCigiProcess() -- Create the CIGI thread
//------------------------------------------------------------------------------
bool CigiClV2Network::createCigiProcess()
{
#ifdef WIN32
    // Create Windows threat
    DWORD dwThreadId;
    CreateThread(
            NULL,                        // no security attributes
            0,                           // use default stack size
            cigiThreadFunc,               // thread function
            this,                        // argument to thread function
            0,                           // use default creation flags
            &dwThreadId);                // returns the thread identifier
#else
    // Create Linux process
    pthread_t thread;
    //int stat = pthread_create(&thread, &attr, cigiThreadFunc, this);
    int stat = pthread_create(&thread, 0, cigiThreadFunc, this);
    if (stat != 0) {
        std::cerr << "pthread_create status = " << stat << std::endl;
    }
#endif
    haveThread = true;
    return true;
}

//=============================================================================
// Main loop
//=============================================================================
void CigiClV2Network::mainLoop()
{
    // ---
    // Receive and processs CIGI packets from the visual system
   // ---
   if (netInput != nullptr && netOutput != nullptr) {
      bool ok = true;
      while (ok) {
         msgIn->AdvanceCrntBuffer();
         unsigned char* rcvbuff = msgIn->GetMsgBuffer();
         int recvCnt = netInput->recvData(reinterpret_cast<char*>(rcvbuff), MAX_BUF_SIZE);

         if (recvCnt > 0) {
            msgIn->SetCrntMsgSize(recvCnt);
            msgIn->ProcessIncomingMsg();
         } else {
            msgIn->SetCrntMsgSize(0);
            ok = false; // something was wrong
         }
      }
   }
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

// Set Network Input Handler
bool CigiClV2Network::setSlotNetInput(Basic::NetHandler* const msg)
{
    netInput = msg;
    return true;
}

// Net Network Output Handler
bool CigiClV2Network::setSlotNetOutput(Basic::NetHandler* const msg)
{
    netOutput = msg;
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* CigiClV2Network::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}
//==============================================================================
// OtwModelCigiClV2 class
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(OtwModelCigiClV2,"OtwModelCigiClV2")
EMPTY_SERIALIZER(OtwModelCigiClV2)

// ---
// constructor
// ---
OtwModelCigiClV2::OtwModelCigiClV2()
{
   STANDARD_CONSTRUCTOR()

   for (unsigned int i = 0; i < OtwCigiClV2::NUM_BUFFERS; i++) {
      parentEC[i] = nullptr;
      trailEC[i] = nullptr;
      explosionEC[i] = nullptr;
      smokeEC[i] = nullptr;
      damageCC[i] = nullptr;
      animationCC[i] = nullptr;
      attachedEC[i] = nullptr;
      launcherAPC[i] = nullptr;
      attachedCC[i] = nullptr;
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
}

// ---
// copyData() -- copy member data
// ---
void OtwModelCigiClV2::copyData(const OtwModelCigiClV2& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      for (unsigned int i = 0; i < OtwCigiClV2::NUM_BUFFERS; i++) {
         parentEC[i] = nullptr;
         trailEC[i] = nullptr;
         explosionEC[i] = nullptr;
         smokeEC[i] = nullptr;
         damageCC[i] = nullptr;
         animationCC[i] = nullptr;
         attachedEC[i] = nullptr;
         launcherAPC[i] = nullptr;
         attachedCC[i] = nullptr;
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
}

// ---
// deleteData() -- delete member data
// ---
void OtwModelCigiClV2::deleteData()
{
   for (unsigned int i = 0; i < OtwCigiClV2::NUM_BUFFERS; i++) {
      if (parentEC[i] != nullptr)    { delete parentEC[i]; parentEC[i] = nullptr; }
      if (trailEC[i] != nullptr)     { delete trailEC[i]; trailEC[i] = nullptr; }
      if (explosionEC[i] != nullptr) { delete explosionEC[i]; explosionEC[i] = nullptr; }
      if (smokeEC[i] != nullptr)     { delete smokeEC[i]; smokeEC[i] = nullptr; }
      if (damageCC[i] != nullptr)    { delete damageCC[i]; damageCC[i] = nullptr; }
      if (animationCC[i] != nullptr) { delete animationCC[i]; animationCC[i] = nullptr; }
      if (attachedEC[i] != nullptr)  { delete attachedEC[i]; attachedEC[i] = nullptr; }
      if (launcherAPC[i] != nullptr) { delete launcherAPC[i]; launcherAPC[i] = nullptr; }
      if (attachedCC[i] != nullptr)  { delete attachedCC[i]; attachedCC[i] = nullptr; }
   }
}

// ---
// initialize() -- initialize the model
// ---
void OtwModelCigiClV2::initialize(Simulation::Player* const p)
{
   isGroundPlayer = p->isClassType(typeid(Simulation::GroundVehicle));
   BaseClass::initialize(p);
}

// ---
// clear() -- clears out the model
// ---
void OtwModelCigiClV2::clear()
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
