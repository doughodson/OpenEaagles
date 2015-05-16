#include "openeaagles/simulation/OnboardComputer.h"

#include "openeaagles/simulation/Actions.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/TrackManager.h"

#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Pair.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(OnboardComputer,"OnboardComputer")
EMPTY_SERIALIZER(OnboardComputer)

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
OnboardComputer::OnboardComputer()
{
   STANDARD_CONSTRUCTOR()

   nextToShoot = nullptr;
   action = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void OnboardComputer::copyData(const OnboardComputer& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      nextToShoot = nullptr;
      action = nullptr;
   }

   action = nullptr; // Need to trigger the action

   // Clear next to shoot list
   setNextToShoot(nullptr);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void OnboardComputer::deleteData()
{
   setNextToShoot(nullptr);
   action = nullptr;
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool OnboardComputer::shutdownNotification()
{
   setNextToShoot(nullptr);
   action = nullptr;
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void OnboardComputer::reset()
{
   BaseClass::reset();

   // ---
   // Cancel the current steerpoint action (if nay)
   // ---
   if (action != nullptr) {
      action->cancel();
      action = nullptr;
   }

   // Clear shoot list
   setNextToShoot(nullptr);
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void OnboardComputer::process(const LCreal dt)
{
   BaseClass::process(dt);
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void OnboardComputer::updateData(const LCreal dt)
{
   BaseClass::updateData(dt);

   // Manage active actions
   actionManager(dt);

   // Update the shoot list
   updateShootList();
}

//------------------------------------------------------------------------------
// trigger the 'to' steerpoint's action (if any)
//------------------------------------------------------------------------------
void OnboardComputer::triggerAction(Action* const toAction)
{
   // ---
   // Cancel any previous action
   // ---
   if (action != nullptr) {
      action->cancel();
   }

   // ---
   // find and start the current 'to' steerpont action
   // ---
   if (toAction != nullptr) {
      // trigger the action
      toAction->trigger(this);

      if (toAction->isInProgress()) {
         // Wasn't a one-shot, so remember this action
         action = toAction;
      }
   }
}


//------------------------------------------------------------------------------
// Manage the active action
//------------------------------------------------------------------------------
void OnboardComputer::actionManager(const LCreal dt)
{
   if (action != nullptr) {
      // update the action
      action->process(dt);
      if (action->isCompleted()) {
         // The action has finished
         action = nullptr;
      }
   }
}

//------------------------------------------------------------------------------
// getShootList() -- returns the shoot list
//------------------------------------------------------------------------------

int OnboardComputer::getShootList(Track* tlist[], const int max)
{
    int n = 0;
    if (nextToShoot != nullptr && tlist != nullptr && max > 0) {
        nextToShoot->ref();
        tlist[0] = nextToShoot;
        n = 1;
    }
    return n;
}

int OnboardComputer::getShootList(const Track* tlist[], const int max) const
{
    int n = 0;
    if (nextToShoot != nullptr && tlist != nullptr && max > 0) {
        nextToShoot->ref();
        tlist[0] = nextToShoot;
        n = 1;
    }
    return n;
}

int OnboardComputer::getShootList(Basic::safe_ptr<Track>* const tlist, const int max)
{
    int n = 0;
    if (nextToShoot != nullptr && tlist != nullptr && max > 0) {
        tlist[0] = nextToShoot;
        n = 1;
    }
    return n;
}

int OnboardComputer::getShootList(Basic::safe_ptr<const Track>* const tlist, const int max) const
{
    int n = 0;
    if (nextToShoot != nullptr && tlist != nullptr && max > 0) {
        tlist[0] = nextToShoot;
        n = 1;
    }
    return n;
}

//------------------------------------------------------------------------------
// getNextTarget() -- Find the next target to shoot
//------------------------------------------------------------------------------
Track* OnboardComputer::getNextTarget()
{
    Basic::safe_ptr<Track> trackList[2];
    int n = getShootList(trackList,2);
    Track* trk = nullptr;
    if (n > 0) {
       trk = trackList[0];
    }
    return trk;
}

//------------------------------------------------------------------------------
// updateShootList() -- Update the shoot list.  When the step flag is true,
//                      force step to the next target.
//------------------------------------------------------------------------------
void OnboardComputer::updateShootList(const bool step)
{
   // Temporary next to shoot indexes
   int cNTS = -1;  // Current
   int nNTS = -1;  // New

   // First, let's get the active track list
   const unsigned int MAX_TRKS = 20;
   Basic::safe_ptr<Track> trackList[MAX_TRKS];

   int n = 0;
   TrackManager* tm = getTrackManagerByType(typeid(AirTrkMgr));
   // fall back to whatever TM we have, if we don't have an AirTrkMgr
   if (tm == nullptr) tm = getTrackManagerByType(typeid(TrackManager));
   if (tm != nullptr) n = tm->getTrackList(trackList,MAX_TRKS);

   if (n > 0) {

      // ---
      // Find the current next-to-shoot index
      // ---
      Track* nts = nextToShoot;
      if (nextToShoot != nullptr) {
         for (int i = 0; i < n && cNTS < 0; i++) {
            if (nts == trackList[i]) cNTS = i;
         }
      }

      // ---
      // Update the next to shoot?
      // ---
      if (cNTS < 0) {
         // 1) When we don't have or couldn't find our NTS, pick the closest track
         //    that has a valid ground speed.
         int i = 0;
         do {
            if (trackList[i]->getGroundSpeed() >= 1.0) {
               if (nNTS >= 0) {
                  // is this one closer?
                  if (trackList[i]->getRange() < trackList[nNTS]->getRange()) {
                     nNTS = i;
                  }
               }
               else {
                  // only one so far
                  nNTS = i;
               }
            }
            i = i + 1;
            if (i >= n) i = 0;
         } while (i != 0);
      }
      else if (step) {
         // 2) When a target step has been requested ...
         int i = cNTS + 1;
         if (i >= n) i = 0;
         do {
            if (trackList[i]->getGroundSpeed() >= 50.0f) {
               nNTS = i;
            }
            i = i + 1;
            if (i >= n) i = 0;
         } while (nNTS < 0 && i != cNTS);
      }
      else {
         // 3) Keep the same next-to-shoot track
         nNTS = cNTS;
      }

      // ---
      // update the shoot list index values in the tracks
      // ---
      if (nNTS >= 0) {
         // Start at the next-to-shoot
         int idx = nNTS;
         for (int i = 0; i < n; i++) {
            trackList[idx++]->setShootListIndex(i+1);
            if (idx >= n) idx = 0;
         }
      }
      else {
         // When there isn't a next to shoot, clear all
         for (int i = 0; i < n; i++) trackList[i]->setShootListIndex(0);
      }
   }

   // Update the next to shoot
   if (nNTS >= 0) setNextToShoot( trackList[nNTS] );
   else setNextToShoot(nullptr);
}

//------------------------------------------------------------------------------
// requestNextToShoot() -- Requests a specific track to be the next-to-shoot;
//                         Returns true if successful
//------------------------------------------------------------------------------
bool OnboardComputer::requestNextToShoot(const Track* const nts)
{
   bool ok = false;
   if (nts != nullptr) {

      // First, let's get the active track list
      const unsigned int MAX_TRKS = 20;
      Basic::safe_ptr<Track> trackList[MAX_TRKS];

      int n = 0;
      TrackManager* tm = getTrackManagerByType(typeid(AirTrkMgr));
      // fall back to whatever TM we have, if we don't have an AirTrkMgr
      if (tm == nullptr) tm = getTrackManagerByType(typeid(TrackManager));
      if (tm != nullptr) n = tm->getTrackList(trackList,MAX_TRKS);

      if (n > 0) {

         // try to find the requested track and save its index
         int nNTS = -1;
         for (int i = 0; i < n && nNTS < 0; i++) {
            if (nts == trackList[i]) nNTS = i;
         }

         // update the shoot list index values in the tracks
         if (nNTS >= 0) {
            // Start at the next-to-shoot
            int idx = nNTS;
            for (int i = 0; i < n; i++) {
               trackList[idx++]->setShootListIndex(i+1);
               if (idx >= n) idx = 0;
            }
            ok = true;
         }

         // Update the next to shoot
         if (ok) setNextToShoot(trackList[nNTS]);
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setNextToShoot() -- set the next to shoot track
//------------------------------------------------------------------------------
void OnboardComputer::setNextToShoot(Track* const p)
{
   if (nextToShoot != nullptr) {
      nextToShoot->unref();
      nextToShoot = nullptr;
   }
   nextToShoot = p;
   if (nextToShoot != nullptr) {
      nextToShoot->ref();
   }
}

//------------------------------------------------------------------------------
// getTrackManagerByType() -- return the first track manager of type
//------------------------------------------------------------------------------
TrackManager* OnboardComputer::getTrackManagerByType(const std::type_info& type)
{
   TrackManager* p = nullptr;
   Basic::Pair* pair = findByType(type);
   if (pair != nullptr) {
      p = dynamic_cast<TrackManager*>( pair->object() );
   }
   return p;
}

const TrackManager* OnboardComputer::getTrackManagerByType(const std::type_info& type) const
{
   const TrackManager* p = nullptr;
   const Basic::Pair* pair = findByType(type);
   if (pair != nullptr) {
      p = dynamic_cast<const TrackManager*>( pair->object() );
   }
   return p;
}

//------------------------------------------------------------------------------
// getTrackManagerByName() -- return a track manager by name
//------------------------------------------------------------------------------
TrackManager* OnboardComputer::getTrackManagerByName(const char* const name)
{
   TrackManager* p = nullptr;
   Basic::Pair* pair = findByName(name);
   if (pair != nullptr) {
      p = dynamic_cast<TrackManager*>( pair->object() );
   }
   return p;
}

const TrackManager* OnboardComputer::getTrackManagerByName(const char* const name) const
{
   const TrackManager* p = nullptr;
   const Basic::Pair* pair = findByName(name);
   if (pair != nullptr) {
      p = dynamic_cast<const TrackManager*>( pair->object() );
   }
   return p;
}

} // End Simulation namespace
} // End Eaagles namespace

