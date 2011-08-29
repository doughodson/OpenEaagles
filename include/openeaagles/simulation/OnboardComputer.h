//------------------------------------------------------------------------------
// Class:	OnboardComputer
//------------------------------------------------------------------------------
#ifndef __OnboardComputer_H_5D60536F_6A5C_4777_B076_7D4989D0A1C8__
#define __OnboardComputer_H_5D60536F_6A5C_4777_B076_7D4989D0A1C8__

#include "openeaagles/simulation/System.h"

namespace Eaagles {
   namespace Basic {
      class PairStream;
   }
namespace Simulation {

class Action;
class Track;
class TrackManager;

//------------------------------------------------------------------------------
// Class:	OnboardComputer
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
// Form name: OnboardComputer
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
   virtual int getShootList(SPtr<Track>* const tlist, const int max);
   virtual int getShootList(SPtr<const Track>* const tlist, const int max) const;

   // Basic::Component interface
   virtual void reset();
   virtual void updateData(const LCreal dt = 0.0);

protected:
   virtual void setNextToShoot(Track* const p);

   // Manage the active action
   virtual void actionManager(const LCreal dt);

   // System class Interface -- phase() callbacks
   virtual void process(const LCreal dt);     // Phase 3

private:
   SPtr<Action>   action;           // Current steerpoint action
   Track*         nextToShoot;      // Next to shoot track
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __OnboardComputer_H_5D60536F_6A5C_4777_B076_7D4989D0A1C8__
