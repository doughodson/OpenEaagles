//------------------------------------------------------------------------------
// Class: OnboardComputer
//------------------------------------------------------------------------------
#ifndef __oe_simulation_OnboardComputer_H__
#define __oe_simulation_OnboardComputer_H__

#include "openeaagles/simulation/System.h"

namespace oe {
   namespace basic {
      class PairStream;
   }
namespace simulation {

class Action;
class Track;
class TrackManager;

//------------------------------------------------------------------------------
// Class: OnboardComputer
// Description: Base class for all Onboard computer systems
//
//    Use to model the avionics packages, and uses the system of systems
//    pattern, which allows computer models to manage other subsystem
//    computer models.
//
//    The on-board computer model is also responsible for managing the various
//    track managers.
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.h).
//
// Factory name: OnboardComputer
//------------------------------------------------------------------------------
class OnboardComputer : public System
{
    DECLARE_SUBCLASS(OnboardComputer,System)

public:
   OnboardComputer();

   // Track managers
   virtual TrackManager* getTrackManagerByType(const std::type_info& type);
   virtual const TrackManager* getTrackManagerByType(const std::type_info& type) const;
   virtual TrackManager* getTrackManagerByName(const char* const name);
   virtual const TrackManager* getTrackManagerByName(const char* const name) const;

   // Air to Air functions
   virtual Track* getNextTarget();                                      // Get the target track
   virtual int getShootList(Track* tlist[], const int max);
   virtual int getShootList(const Track* tlist[], const int max) const;
   virtual void updateShootList(const bool step = false);              // Updates the shoot list
   virtual bool requestNextToShoot(const Track* const nts);            // Request a track to shoot next

   // Trigger an action
   virtual void triggerAction(Action* const act);

   // Legacy function (will be removed in a future major release)
   virtual int getShootList(basic::safe_ptr<Track>* const tlist, const int max);
   virtual int getShootList(basic::safe_ptr<const Track>* const tlist, const int max) const;

   void reset() override;
   void updateData(const LCreal dt = 0.0) override;

protected:
   virtual void setNextToShoot(Track* const p);

   // Manage the active action
   virtual void actionManager(const LCreal dt);

   void process(const LCreal dt) override;

   bool shutdownNotification() override;

private:
   basic::safe_ptr<Action> action;  // Current steerpoint action
   Track*         nextToShoot;      // Next to shoot track
};

} // End simulation namespace
} // End oe namespace

#endif
