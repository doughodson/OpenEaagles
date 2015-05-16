#include "openeaagles/simulation/LifeForms.h"

#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Guns.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/units/Angles.h"

#include <cmath>

namespace Eaagles {
namespace Simulation {

//==============================================================================
// class LifeForm
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(LifeForm,"LifeForm")
EMPTY_SERIALIZER(LifeForm)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
LifeForm::LifeForm()
{
    STANDARD_CONSTRUCTOR()
    static Basic::String generic("LifeForm");
    setType(&generic);
    actionState = UPRIGHT_STANDING;
    lookAngle = 0.0;
    tgtAquired = false;
    tgtPlayer = nullptr;
    lockMode = SEARCHING;
    weaponSel = LF_GUN;

    setTerrainOffset(1.0);      // default offset from terrain to CG
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void LifeForm::copyData(const LifeForm& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) {
       tgtPlayer = nullptr;
    }
    actionState = org.actionState;
    tgtAquired = org.tgtAquired;
    if (tgtPlayer != nullptr) tgtPlayer->unref();
    tgtPlayer = nullptr;
    lockMode = org.lockMode;
    weaponSel = org.weaponSel;
}

void LifeForm::deleteData()
{
    if (tgtPlayer != nullptr) tgtPlayer->unref();
    tgtPlayer = nullptr;
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool LifeForm::shutdownNotification()
{
   if (tgtPlayer != nullptr) tgtPlayer->unref();
   tgtPlayer = nullptr;
   return BaseClass::shutdownNotification();
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int LifeForm::getMajorType() const
{
    return LIFE_FORM;
}

bool LifeForm::setActionState(const int x)
{
    actionState = x;
    return true;
}

void LifeForm::reset()
{
   // do our resetting first
   if (isLocalPlayer()) {
      actionState = UPRIGHT_STANDING;
      lookAngle = 0.0;
      tgtAquired = false;
      lockMode = SEARCHING;
      weaponSel = LF_GUN;
   }
   if (tgtPlayer != nullptr) tgtPlayer->unref();
   tgtPlayer = nullptr;

   BaseClass::reset();
}

void LifeForm::fire()
{
    Basic::Number* hdgObj = new Basic::Number(getHeadingR());
    Basic::Number* pitchObj = new Basic::Number(lookAngle * Basic::Angle::D2RCC);
    StoresMgr* mgr = getStoresManagement();
    if (mgr != nullptr) {
        if (getSimulation() != nullptr) {
            if (weaponSel == LF_MISSILE) {
                mgr->setGunSelected(false);
                Missile* missile = mgr->getNextMissile();
                if (missile != nullptr) {
                    missile->setSlotInitPitch(pitchObj);
                    missile->setSlotInitHeading(hdgObj);
                    missile->reset();
                    Missile* msl = mgr->releaseOneMissile();
                    if (msl != nullptr) {
                        if (tgtAquired && tgtPlayer != nullptr) msl->setTargetPlayer(tgtPlayer, true);
                    }
                }
            }
            else if (weaponSel == LF_GUN) {
                mgr->setGunSelected(true);
                Gun* myGun = mgr->getGun();
                if (myGun != nullptr) {
                    myGun->setGunArmed(true);
                    Basic::Number* num = new Basic::Number(lookAngle * Basic::Angle::D2RCC);
                    myGun->setSlotPitch(num);
                    num->unref();
                    myGun->fireControl(true);
                }
           }
        }
    }
    hdgObj->unref();
    pitchObj->unref();
}

// override our set velocity, so we can determine if we are walking, running, or standing
bool LifeForm::setVelocity(const LCreal ue, const LCreal ve, const LCreal we)
{
    bool ok = BaseClass::setVelocity(ue, ve, we);
    // based on our velocity, we will run or walk, or stand still
    const LCreal tempX = lcAbs(ue);
    const LCreal tempY = lcAbs(ve);

    // we only change our appearance bit if we are parachuting
    if (actionState != PARACHUTING) {
        // test for running and walking
        if (tempX == 0 && tempY == 0) actionState = UPRIGHT_STANDING;
        if (tempX > 0 || tempY > 0) actionState = UPRIGHT_WALKING;
        if (tempX > 8 || tempY > 8) actionState = UPRIGHT_RUNNING;
    }

    return ok;
}

void LifeForm::move(const LCreal fwd, const LCreal sdws)
{
    if (getDamage() < 1) {
        osg::Vec3 old = getEulerAngles();
        LCreal hdg = old.z();

        LCreal tempFwd = fwd, tempSdws = sdws;

        // our deadband (if we are barely moving, just stop)
        if (lcAbs(tempFwd) < 0.9f) tempFwd = 0;
        if (lcAbs(tempSdws) < 0.9f) tempSdws = 0;
        const double xVel = tempFwd * (lcCos(hdg));
        const double yVel = tempFwd * (lcSin(hdg));

        // now calculate our sideways velocity
        const double xxVel = tempSdws * (lcCos((hdg + (90 * static_cast<LCreal>(Basic::Angle::D2RCC)))));
        const double yyVel = tempSdws * (lcSin((hdg + (90 * static_cast<LCreal>(Basic::Angle::D2RCC)))));

        // now add the vectors
        const double newXVel = xVel + xxVel;
        const double newYVel = yVel + yyVel;

        LCreal zVel = 0.0;
        setVelocity(static_cast<LCreal>(newXVel), static_cast<LCreal>(newYVel), zVel);
    }
    else setVelocity(0, 0, 0);
}

void LifeForm::look(const LCreal up, const LCreal sdws)
{
    if (getDamage() < 1) {
        if (lockMode != LOCKED) {
            lockMode = SEARCHING;
            // our up and sideways come in as -5 to 5, which is a rate to adjust heading
            const osg::Vec3 old = getEulerAngles();
            LCreal hdg = old.z();
            LCreal ptc = lookAngle;
            LCreal tempSdws = sdws;
            LCreal tempUp = up;
            if (lcAbs(tempSdws) < 0.00005) tempSdws = 0;
            if (lcAbs(tempUp) < 0.05) tempUp = 0;
            hdg += tempSdws;
            hdg = lcAepcRad(hdg);
            // we don't change our pitch when we look up and down, we only change our look angle, so we have to keep
            // that separate.  WE do, however, change our heading based on where we are looking, so that is correct
            ptc += tempUp;
            if (ptc > 90) ptc = 90;
            else if (ptc < -90) ptc = -90;
            //std::cout << "HEADING = " << hdg << std::endl;
            setLookAngle(ptc);
            osg::Vec3 eul(0, 0, hdg);
            setEulerAngles(eul);
            // now based on this we need to know if we have a target in our crosshairs...
            tgtAquired = false;
            if (tgtPlayer != nullptr) tgtPlayer->unref();
            tgtPlayer = nullptr;
            const osg::Vec3 myPos = getPosition();
            osg::Vec3 tgtPos;
            osg::Vec3 vecPos;
            LCreal az = 0.0, el = 0.0, range = 0.0, diffAz = 0.0, diffEl = 0.0;
            const LCreal maxAz = (0.7f * static_cast<LCreal>(Basic::Angle::D2RCC));
            const LCreal maxEl = (0.7f * static_cast<LCreal>(Basic::Angle::D2RCC));
            //LCreal maxRange = 1500.0f; // long range right now
            const LCreal la = lookAngle * static_cast<LCreal>(Basic::Angle::D2RCC);
            Simulation* sim = getSimulation();
            if (sim != nullptr) {
                Basic::PairStream* players = sim->getPlayers();
                if (players != nullptr) {
                    Basic::List::Item* item = players->getFirstItem();
                    while (item != nullptr && !tgtAquired) {
                        Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
                        if (pair != nullptr) {
                            Player* player = dynamic_cast<Player*>(pair->object());
                            if (player != nullptr && player != this && !player->isMajorType(WEAPON) && !player->isDestroyed()) {
                                // ok, calculate our position from this guy
                                tgtPos = player->getPosition();
                                vecPos = tgtPos - myPos;
                                az = lcAtan2(vecPos.y(), vecPos.x());
                                range = (vecPos.x() * vecPos.x() + vecPos.y() * vecPos.y());
                                range = std::sqrt(range);
                                // now get our elevation
                                el = lcAtan2(-vecPos.z(), range);
                                diffAz = lcAbs(lcAepcRad(az - static_cast<LCreal>(getHeadingR())));
                                diffEl = lcAbs(lcAepcRad(la - el));
                                if ((diffAz <= maxAz) && (diffEl <= maxEl)) {
                                    lockMode = TGT_IN_SIGHT;
                                    tgtAquired = true;
                                    if (tgtPlayer != player) {
                                        if (tgtPlayer != nullptr) tgtPlayer->unref();
                                        tgtPlayer = player;
                                        tgtPlayer->ref();
                                    }
                                }
                            }
                        }
                        item = item->getNext();
                    }
                    players->unref();
                    players = nullptr;
                }
            }
        }
        // else we are locking on target, and need to follow our target player
        else {
            if (tgtPlayer == nullptr) lockMode = SEARCHING;
            else {
                const osg::Vec3 vecPos = tgtPlayer->getPosition() - getPosition();
                const LCreal az = lcAtan2(vecPos.y(), vecPos.x());
                LCreal range = (vecPos.x() * vecPos.x() + vecPos.y() * vecPos.y());
                range = std::sqrt(range);
                // now get our elevation
                const LCreal el = lcAtan2(-vecPos.z(), range);
                // now force that on us
                setLookAngle(el * static_cast<LCreal>(Basic::Angle::R2DCC));
                setEulerAngles(0, 0, az);
            }
        }
    }
}

/*
//==============================================================================
// class InfantryMan - default grunt
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(InfantryMan,"InfantryMan")
EMPTY_SERIALIZER(InfantryMan)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
InfantryMan::InfantryMan()
{
    STANDARD_CONSTRUCTOR()
    static Basic::String generic("InfantryMan");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void InfantryMan::copyData(const InfantryMan& org, const bool)
{
    BaseClass::copyData(org);
}

void InfantryMan::deleteData()
{
}

//==============================================================================
// class Parachutist - grunt who jumps out of planes!
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Parachutist,"Parachutist")
EMPTY_SERIALIZER(Parachutist)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Parachutist::Parachutist()
{
    STANDARD_CONSTRUCTOR()
    static Basic::String generic("Parachutist");
    setType(&generic);
}

EMPTY_COPYDATA(Parachutist)
EMPTY_DELETEDATA(Parachutist)
*/
} // End Simulation namespace
} // End Eaagles namespace
