
#ifndef __oe_models_LifeForm_H__
#define __oe_models_LifeForm_H__

#include "openeaagles/models/player/Player.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class LifeForm
// Description: Generic life form
// Factory name: LifeForm
//------------------------------------------------------------------------------
class LifeForm : public Player
{
    DECLARE_SUBCLASS(LifeForm, Player)

public:
    LifeForm();

    //enum { UPRIGHT_STANDING = 1, UPRIGHT_WALKING = 2, UPRIGHT_RUNNING = 3, PARACHUTING = 8 };
    enum { UPRIGHT_STANDING = 1, UPRIGHT_WALKING = 2, UPRIGHT_RUNNING = 3,
           KNEELING = 4, PRONE = 5, CRAWLING = 6, SWIMMING = 7, PARACHUTING = 8,
           JUMPING = 9, SITTING = 10, SQUATTING = 11, CROUCHING = 12,
           WADING = 13, SURRENDER = 14, DETAINED = 15 };


    virtual unsigned int getMajorType() const override;

    // sets our actions (right now we can stand, walk, or run)
    virtual bool setActionState(const int x);

    int getActionState() const                             { return actionState; }

    // move our player (vector him according to strafe)
    virtual void move(const double fwd, const double sdws);

    // have our player look
    virtual void look(const double up, const double sdws);
    virtual bool setLookAngle(const double x)              { lookAngle = x; return true; }

    virtual bool setVelocity(const double ue, const double ve, const double we) override;

    double getLookAngle() const                            { return lookAngle; }
    bool isTargetAquired() const                           { return tgtAquired; }
    int getLockMode() const                                { return lockMode; }
    virtual void setLockedMode(const int x)                { lockMode = x; }

    // get our "aim" and fire
    virtual void fire();

    enum { SEARCHING = 0, TGT_IN_SIGHT, LOCKED };
    enum { LF_GUN = 0, LF_MISSILE };

    virtual void setWeapon(const int x) { weaponSel = x; }
    int getWeaponSelMode() const { return weaponSel; }

    virtual void reset() override;

protected:
    virtual bool shutdownNotification() override;

private:
    int actionState {UPRIGHT_STANDING}; // the action we are doing (for DIS enumerations)
    double lookAngle {};                // the look angle we are looking at (up or down 90 degs)
    bool tgtAquired {};                 // we have a target in our crosshairs
    int lockMode {SEARCHING};           // the mode we are in (searching, tgt in sight, locked)
    Player* tgtPlayer {};               // our target player that we have aquired
    int weaponSel {LF_GUN};             // the weapon we have selected
};

}
}

#endif
