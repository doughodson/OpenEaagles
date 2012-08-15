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
    tgtAquired = false;
    tgtPlayer = 0;
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
       tgtPlayer = 0;
    }
    actionState = org.actionState;
    tgtAquired = org.tgtAquired;
    if (tgtPlayer != 0) tgtPlayer->unref();
    tgtPlayer = 0;
    lockMode = org.lockMode;
    weaponSel = org.weaponSel;
}

void LifeForm::deleteData()
{
    if (tgtPlayer != 0) tgtPlayer->unref();
    tgtPlayer = 0;
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
      lookAngle = 0;
      tgtAquired = false;
      lockMode = SEARCHING;
      weaponSel = LF_GUN;
   }
   if (tgtPlayer != 0) tgtPlayer->unref();
   tgtPlayer = 0;

   BaseClass::reset();
}

void LifeForm::fire()
{
    Basic::Number* hdgObj = new Basic::Number(getHeadingR());
    Basic::Number* pitchObj = new Basic::Number(lookAngle * Basic::Angle::D2RCC);
    StoresMgr* mgr = getStoresManagement();
    if (mgr != 0) {
        if (getSimulation() != 0) {
            if (weaponSel == LF_MISSILE) {
                mgr->setGunSelected(false);
                Missile* missile = mgr->getNextMissile();
                if (missile != 0) {
                    missile->setSlotInitPitch(pitchObj);
                    missile->setSlotInitHeading(hdgObj);
                    missile->reset();
                    Missile* msl = mgr->releaseOneMissile();
                    if (msl != 0) {
                        if (tgtAquired && tgtPlayer != 0) msl->setTargetPlayer(tgtPlayer, true);
                    }
                }
            }
            else if (weaponSel == LF_GUN) {
                mgr->setGunSelected(true);
                Gun* myGun = mgr->getGun();
                if (myGun != 0) {
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
    LCreal tempX = lcAbs(ue);
    LCreal tempY = lcAbs(ve);
    
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
    double xVel = tempFwd * (lcCos(hdg));
    double yVel = tempFwd * (lcSin(hdg));
    
    // now calculate our sideways velocity
    double xxVel = tempSdws * (lcCos((hdg + (90 * (LCreal)Basic::Angle::D2RCC))));
    double yyVel = tempSdws * (lcSin((hdg + (90 * (LCreal)Basic::Angle::D2RCC))));
    
    
    // now add the vectors
    double newXVel = xVel + xxVel;
    double newYVel = yVel + yyVel;
    
    
    LCreal zVel = 0;
    setVelocity((LCreal)newXVel, (LCreal)newYVel, zVel);    
}
    else setVelocity(0, 0, 0);    
}

void LifeForm::look(const LCreal up, const LCreal sdws)
{
    if (getDamage() < 1) {
        if (lockMode != LOCKED) {
            lockMode = SEARCHING;
    // our up and sideways come in as -5 to 5, which is a rate to adjust heading
    osg::Vec3 old = getEulerAngles();
    LCreal hdg = old.z();
    LCreal ptc = lookAngle;
    LCreal tempSdws = sdws;
    LCreal tempUp = up;
    if (lcAbs(tempSdws) < 0.00005f) tempSdws = 0;
    if (lcAbs(tempUp) < 0.05f) tempUp = 0;    
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
            if (tgtPlayer != 0) tgtPlayer->unref();
            tgtPlayer = 0;
            osg::Vec3 myPos = getPosition();
            osg::Vec3 tgtPos;
            osg::Vec3 vecPos;
            LCreal az = 0, el = 0, range = 0, diffAz = 0, diffEl = 0;
            LCreal maxAz = (0.7f * (LCreal)Basic::Angle::D2RCC);
            LCreal maxEl = (0.7f * (LCreal)Basic::Angle::D2RCC);
            //LCreal maxRange = 1500.0f; // long range right now
            LCreal la = lookAngle * (LCreal)Basic::Angle::D2RCC;
            Simulation* sim = getSimulation();
            if (sim != 0) {
                Basic::PairStream* players = sim->getPlayers();
                if (players != 0) {
                    Basic::List::Item* item = players->getFirstItem();
                    while (item != 0 && !tgtAquired) {
                        Basic::Pair* pair = (Basic::Pair*)item->getValue();
                        if (pair != 0) {
                            Player* player = dynamic_cast<Player*>(pair->object());
                            if (player != 0 && player != this && !player->isMajorType(WEAPON) && !player->isDestroyed()) {
                                // ok, calculate our position from this guy
                                tgtPos = player->getPosition();
                                vecPos = tgtPos - myPos;
                                az = lcAtan2(vecPos.y(), vecPos.x());
                                range = (vecPos.x() * vecPos.x() + vecPos.y() * vecPos.y());
                                range = sqrt(range);
                                // now get our elevation
                                el = lcAtan2(-vecPos.z(), range);
                                diffAz = lcAbs(lcAepcRad(az - (LCreal) getHeadingR()));
                                diffEl = lcAbs(lcAepcRad(la - el));
                                if ((diffAz <= maxAz) && (diffEl <= maxEl)) {
                                    lockMode = TGT_IN_SIGHT;
                                    tgtAquired = true;
                                    if (tgtPlayer != player) {
                                        if (tgtPlayer != 0) tgtPlayer->unref();
                                        tgtPlayer = player;
                                        tgtPlayer->ref();
                                    }
                                }
                            }
                        }
                        item = item->getNext();
                    }
                    players->unref();
                    players = 0;
                }
            }
        }
        // else we are locking on target, and need to follow our target player
        else {
            if (tgtPlayer == 0) lockMode = SEARCHING;
            else {
                osg::Vec3 vecPos = tgtPlayer->getPosition() - getPosition();
                LCreal az = lcAtan2(vecPos.y(), vecPos.x());
                LCreal range = (vecPos.x() * vecPos.x() + vecPos.y() * vecPos.y());
                range = sqrt(range);
                // now get our elevation
                LCreal el = lcAtan2(-vecPos.z(), range);
                // now force that on us
                setLookAngle(el * (LCreal)Basic::Angle::R2DCC);
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
