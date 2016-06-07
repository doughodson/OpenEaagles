//------------------------------------------------------------------------------
// Class: PlaneState
//------------------------------------------------------------------------------

#include "openeaagles/ubf/behaviors/PlaneState.h"

#include "openeaagles/base/List.h"
#include "openeaagles/base/PairStream.h"

#include "openeaagles/simulation/Radar.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Simulation.h"

namespace oe {
namespace behaviors {

IMPLEMENT_SUBCLASS(PlaneState, "PlaneState")
EMPTY_SLOTTABLE(PlaneState)
EMPTY_DELETEDATA(PlaneState)
EMPTY_COPYDATA(PlaneState)
EMPTY_SERIALIZER(PlaneState)

PlaneState::PlaneState()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void PlaneState::initData()
{
   alive           = false;
   roll            = 0;
   pitch           = 0;
   rollRate        = 0;
   pitchRate       = 0;
   yawRate         = 0;
   heading         = 0;
   altitude        = 0;
   throttle        = 0;
   speed           = 0;
   pitchTrim       = 0;

   for (unsigned int i=0; i<MAX_TRACKS;i++) {
      pitchToTracked[i]    = 0.0;
      headingToTracked[i]  = 0.0;
      distanceToTracked[i] = 0.0;
   }

   targetTrack     = MAX_TRACKS;  // 0 is a valid target track, use MAX_TRACKS to signal
                                 // "no tgt track"
   numTracks       = 0;
   missileFired    = false;
   tracking        = false;
   missileFired    = false;
   incomingMissile = false;
   numEngines      = 0;
}

void PlaneState::reset()
{
   initData();
   BaseClass::reset();
}

void PlaneState::updateState(const base::Component* const actor)
{
   const simulation::AirVehicle* airVehicle = dynamic_cast<const simulation::AirVehicle*>(actor);
   setAlive(false);
   if (airVehicle != nullptr && airVehicle->isActive()) {
      setAltitude(airVehicle->getAltitude());
      setAlive(airVehicle->getMode() == simulation::Player::ACTIVE);
      setHeading(airVehicle->getHeading());
      setPitch(airVehicle->getPitch());
      setRoll(airVehicle->getRoll());
      osg::Vec3d angularVels = airVehicle->getAngularVelocities();
      setRollRate(angularVels.x());
      setPitchRate(angularVels.y());
      setYawRate(angularVels.z());
      setTracking(false);
      setTargetTrack(MAX_TRACKS);  // 0 is a valid target track, use MAX_TRACKS to
                                   // signal "no tgt track"
      setSpeed(airVehicle->getCalibratedAirspeed());
      setNumEngines(airVehicle->getNumberOfEngines());
      setIncomingMissile(false);
      setMissileFired(false);

      // determine if we have a missile to fire
#if 1
      const simulation::StoresMgr* stores = airVehicle->getStoresManagement();
      if (stores == nullptr || stores->getNextMissile() == nullptr) {
         // either we have no SMS, or we have no more missile
         setMissileFired(true);
      }
      else {
         // we have an sms, and we have a missile available
         // loop through player list and attempt to find out if one of our missiles is active
         // if there is an active missile, then for the time being, we do not have a missile to fire
         const simulation::Simulation* sim = airVehicle->getSimulation();
         const base::PairStream* players = sim->getPlayers();
         bool finished = false;
         for (const base::List::Item* item = players->getFirstItem(); item != nullptr && !finished; item = item->getNext()) {
            // Get the pointer to the target player
            const base::Pair* pair = static_cast<const base::Pair*>(item->getValue());
            const simulation::Player* player = static_cast<const simulation::Player*>(pair->object());
            if (player->isMajorType(simulation::Player::WEAPON) && (player->isActive() || player->isMode(simulation::Player::PRE_RELEASE)) && (player->getSide() == airVehicle->getSide())) {
               // our side has a weapon on-the-way/in-the-air;
               setMissileFired(true);
               finished=true;
            }
         }
      }
#else
      // this state class has no way to determine whether we've fired a missile other than checking to see if sms is out of missiles to fire.
      // which means, it will fire all its missiles at first target.
      const simulation::StoresMgr* stores = airVehicle->getStoresManagement();
      if (stores != 0) {
         const simulation::Missile* wpn = stores->getNextMissile();
         if (!wpn)
            setMissileFired(true);
      }
      else {
         // we have no SMS, we can't fire a missile;
         setMissileFired(true);
      }
#endif

      //const base::String* playerName = airVehicle->getName();
      // DH - DOES NOT COMPILE WITH CONST -- ????
      simulation::AirVehicle* airVehicleX = const_cast<simulation::AirVehicle*>(airVehicle);
      const base::Pair* sensorPair = airVehicleX->getSensorByType(typeid(simulation::Radar));

      if (sensorPair != nullptr) {
         const simulation::Radar* radar = static_cast<const simulation::Radar*>(sensorPair->object());
         if (radar != nullptr) {
            const simulation::TrackManager* trackManager = radar->getTrackManager();
            base::safe_ptr<simulation::Track> trackList[50];
            unsigned int nTracks = trackManager->getTrackList(trackList, 50);

            for (int trackIndex = nTracks -1; trackIndex >= 0; trackIndex--) {
               setHeadingToTracked(trackIndex, trackList[trackIndex]->getRelAzimuth());
               setPitchToTracked(trackIndex, trackList[trackIndex]->getElevation());
               setDistanceToTracked(trackIndex, trackList[trackIndex]->getRange());

               // do we have a live "target track"? (shootlist is 1-based)
               if (getTargetTrack()==MAX_TRACKS && (trackList[trackIndex]->getShootListIndex() == 1) && trackList[trackIndex]->getTarget()->isActive()  ) {
                  setTargetTrack(trackIndex);
               }
               setTracking(true);
               setNumTracks(nTracks);

               // hack to implement "missile warning"
               if (isIncomingMissile() == false) {
                  // is this track a weapon, and if so, is it targeting me?
                  simulation::Player* target = trackList[trackIndex]->getTarget();
                  simulation::Weapon* weapon = dynamic_cast<simulation::Weapon*> (target);
                  if (weapon!=nullptr && !weapon->isDead()) {
                     simulation::Player* wpntgt = weapon->getTargetPlayer();
                     if (wpntgt == airVehicle) {
                        setIncomingMissile(true);
                     }
                  }
               }

            }
         }
      }

      const simulation::OnboardComputer* oc = airVehicle->getOnboardComputer();
      if (oc != nullptr) {
         const simulation::TrackManager* rtm = oc->getTrackManagerByType(typeid(simulation::RwrTrkMgr));
         if(rtm !=nullptr) {
            base::safe_ptr<simulation::Track> trackList[50];
            unsigned int nTracks = rtm->getTrackList(trackList, 50);
            int newTracks = 0;
            for (unsigned int trackIndex = 0; trackIndex < nTracks; trackIndex++) {
               simulation::Player* target = trackList[trackIndex]->getTarget();
               bool alreadyTracked = false;
               for (unsigned int currTracks = 0; currTracks>getNumTracks(); currTracks++) {
                  // tracks are the same if the associated players are the same
                  if(trackList[currTracks]->getTarget()==target) {
                     alreadyTracked = true;
                     break;
                  }
               }

               if (!alreadyTracked && (getNumTracks() + newTracks) < MAX_TRACKS) {
                  int newTrackIndex = getNumTracks() + newTracks;
                  newTracks++;
                  setHeadingToTracked(newTrackIndex, trackList[trackIndex]->getRelAzimuth());
                  setPitchToTracked(newTrackIndex, trackList[trackIndex]->getElevation());
                  setDistanceToTracked(newTrackIndex, trackList[trackIndex]->getRange());
                  setTracking(true);

                  // update numTracks to new sum of radar + rwr tracks
                  setNumTracks(getNumTracks()+newTracks);
               }

               // do we have a live "target track"? (shootlist is 1-based)
               if (getTargetTrack()==MAX_TRACKS && (trackList[trackIndex]->getShootListIndex() == 1) && trackList[trackIndex]->getTarget()->isActive()  ) {
                  setTargetTrack(trackIndex);
               }

               // hack to implement "missile warning"
               if (isIncomingMissile() == false) {
                  // is this track a weapon, and if so, is it targeting me?
                  simulation::Weapon* weapon = dynamic_cast<simulation::Weapon*> (target);
                  if (weapon!=nullptr && !weapon->isDead()) {
                     simulation::Player* wpntgt = weapon->getTargetPlayer();
                     if (wpntgt == airVehicle) {
                        setIncomingMissile(true);
                     }
                  }
               }

            }
         }
      }
   }
   BaseClass::updateState(actor);
}

void PlaneState::setPitchToTracked(const unsigned int trackNumber, const double angle)
{
   if ( trackNumber<numTracks ) {
      pitchToTracked[trackNumber] = angle;
   }
}

double PlaneState::getPitchToTracked(const unsigned int trackNumber) const
{
   if ( trackNumber<numTracks ) {
      return pitchToTracked[trackNumber];
   }
   std::cout << trackNumber << " is out of bounds of the tracking array of PlaneState! Error!\n";
   return trackNumber;
}

void PlaneState::setHeadingToTracked(const unsigned int trackNumber, const double angle)
{
   if (trackNumber < numTracks) {
      headingToTracked[trackNumber] = angle;
   }
}

double PlaneState::getHeadingToTracked(const unsigned int trackNumber) const
{
   if (trackNumber < numTracks) {
      return headingToTracked[trackNumber];
   }
   std::cout << trackNumber << " is out of bounds of the tracking array of PlaneState! Error!\n";
   return trackNumber;
}

void PlaneState::setDistanceToTracked(const unsigned int trackNumber, const double distance)
{
   if (trackNumber < numTracks) {
      distanceToTracked[trackNumber] = distance;
   }
}

double PlaneState::getDistanceToTracked(const unsigned int trackNumber) const
{
   if (trackNumber < numTracks) {
      return distanceToTracked[trackNumber];
   }
   std::cout << trackNumber << " is out of bounds of the tracking array of PlaneState! Error!\n";
   return trackNumber;
}

}
}

