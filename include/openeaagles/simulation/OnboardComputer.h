//------------------------------------------------------------------------------
// Class: OnboardComputer
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_OnboardComputer_H__
#define __Eaagles_Simulation_OnboardComputer_H__

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
   virtual int getShootList(SPtr<Track>* const tlist, const int max);
   virtual int getShootList(SPtr<const Track>* const tlist, const int max) const;

   // Basic::Component interface
   void reset() override;
   void updateData(const LCreal dt = 0.0) override;

protected:
   virtual void setNextToShoot(Track* const p);

   // Manage the active action
   virtual void actionManager(const LCreal dt);

   // System class Interface -- phase() callbacks
   void process(const LCreal dt) override;     // Phase 3

   // Basic::Component protected interface
   bool shutdownNotification() override;

private:
   SPtr<Action>   action;           // Current steerpoint action
   Track*         nextToShoot;      // Next to shoot track
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
