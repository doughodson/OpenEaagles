//------------------------------------------------------------------------------
// Classes:	LifeForm
// Description: Life form models
//------------------------------------------------------------------------------
#if !defined(__LIFE_FORM_H_95FD838D_4877_4490_9B35_718513CE70E6__)
#define __LIFE_FORM_H_95FD838D_4877_4490_9B35_718513CE70E6__

#include "openeaagles/simulation/Player.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class LifeForm
// Description: Generic life form
// Form name: LifeForm
// GUID: {95FD838D-4877-4490-9B35-718513CE70E6}
//==============================================================================
class LifeForm : public Player  
{
    DECLARE_SUBCLASS(LifeForm,Player)

public:
    LifeForm();

    enum { UPRIGHT_STANDING = 1, UPRIGHT_WALKING = 2, UPRIGHT_RUNNING = 3, PARACHUTING = 8 };

    // Player interface
    virtual unsigned int getMajorType() const;
    // sets our actions (right now we can stand, walk, or run)
    virtual bool setActionState(const int x);
    
    int getActionState() const { return actionState; }
    
    // move our player (vector him according to strafe)
    virtual void move(const LCreal fwd, const LCreal sdws);

    // have our player look
    virtual void look(const LCreal up, const LCreal sdws);
    virtual bool setLookAngle(const LCreal x) { lookAngle = x; return true; }

    // overriden functions
    virtual bool setVelocity(const LCreal ue, const LCreal ve, const LCreal we);
    
    LCreal getLookAngle() const { return lookAngle; }
    bool isTargetAquired() const { return tgtAquired; }
    int getLockMode() const { return lockMode; }
    virtual void setLockedMode(const int x) { lockMode = x; }
    
    // get our "aim" and fire
    virtual void fire();
    
    enum { SEARCHING = 0, TGT_IN_SIGHT, LOCKED };
    enum { LF_GUN = 0, LF_MISSILE };
    
    virtual void setWeapon(const int x) { weaponSel = x; }
    int getWeaponSelMode() const { return weaponSel; }
    virtual void reset();
    
private:
    int actionState;    // the action we are doing (for DIS enumerations)
    LCreal lookAngle;   // the look angle we are looking at (up or down 90 degs)
    bool tgtAquired;    // we have a target in our crosshairs
    int lockMode;       // the mode we are in (searching, tgt in sight, locked)
    Player* tgtPlayer;  // our target player that we have aquired
    int weaponSel;      // the weapon we have selected
};

/*
//==============================================================================
// Class InfantryMan
// Description: Generic dismounted unarmed infantryman (carries a M4 carbine - default)
// GUID: {FEDB7E37-5C1A-431b-84BD-8290AEDE5CC7}
//==============================================================================
class InfantryMan : public LifeForm
{
    DECLARE_SUBCLASS(InfantryMan,LifeForm)

public:
    InfantryMan();
};

//==============================================================================
// Class Parachutist
// Description: Generic parachutist - default weapon (M4 carbine)
// GUID: {{6A95F20E-9C1A-47b7-A117-C5682054C225}}
//==============================================================================
class Parachutist : public LifeForm
{
    DECLARE_SUBCLASS(Parachutist,LifeForm)

public:
    Parachutist();
};
*/


} // End Simulation namespace
} // End Eaagles namespace

#endif // !defined(__LIFE_FORM_H_95FD838D_4877_4490_9B35_718513CE70E6__)
