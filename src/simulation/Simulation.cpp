
#include "openeaagles/simulation/Simulation.hpp"

#include "openeaagles/simulation/IPlayer.hpp"

#include "openeaagles/simulation/environment/IAtmosphere.hpp"
#include "openeaagles/simulation/environment/ITerrain.hpp"

#include "openeaagles/dafif/AirportLoader.hpp"
#include "openeaagles/dafif/NavaidLoader.hpp"
#include "openeaagles/dafif/WaypointLoader.hpp"

#include "openeaagles/simulation/DataRecorder.hpp"
#include "openeaagles/simulation/INetIO.hpp"
#include "openeaagles/simulation/INib.hpp"
#include "openeaagles/simulation/Station.hpp"

#include "openeaagles/base/EarthModel.hpp"
#include "openeaagles/base/Identifier.hpp"
#include "openeaagles/base/LatLon.hpp"
#include "openeaagles/base/Nav.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/Thread.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Distances.hpp"
#include "openeaagles/base/units/Times.hpp"
#include "openeaagles/base/Statistic.hpp"
#include "openeaagles/base/util/system.hpp"

#include <cstring>
#include <cmath>

namespace oe {
namespace simulation {

//=============================================================================
// Declare the threads
//=============================================================================

class SimTcThread : public base::ThreadSyncTask {
   DECLARE_SUBCLASS(SimTcThread, base::ThreadSyncTask)
public:
   SimTcThread(base::Component* const parent, const double priority);

   // Parent thread signals start to this child thread with these parameters.
   void start(
      base::PairStream* const pl0,
      const double dt0,
      const unsigned int idx0,
      const unsigned int n0
   );

private:
   // ThreadSyncTask class function -- our userFunc()
   virtual unsigned long userFunc() override;

private:
   base::PairStream* pl0;
   double dt0;
   unsigned int idx0;
   unsigned int n0;
};

class SimBgThread : public base::ThreadSyncTask {
   DECLARE_SUBCLASS(SimBgThread,base::ThreadSyncTask)
public:
   SimBgThread(base::Component* const parent, const double priority);

   // Parent thread signals start to this child thread with these parameters.
   void start(
      base::PairStream* const pl0,
      const double dt0,
      const unsigned int idx0,
      const unsigned int n0
   );

private:
   // ThreadSyncTask class function -- our userFunc()
   virtual unsigned long userFunc() override;

private:
   base::PairStream* pl0;
   double dt0;
   unsigned int idx0;
   unsigned int n0;
};

IMPLEMENT_PARTIAL_SUBCLASS(Simulation, "Simulation")

BEGIN_SLOTTABLE(Simulation)
   "players",        //  1) All players
   "latitude",       //  2) Ref latitude
   "longitude",      //  3) Ref longitude
   "simulationTime", //  4) Simulation time
   "day",            //  5) Initial simulated day of month [ 1 .. 31 ]
   "month",          //  6) Initial simulated month [ 1 .. 12 ]
   "year",           //  7) Initial simulated year [ 1970 .. 2100 ]
   "airportLoader",  //  8) Airport database
   "navaidLoader",   //  9) NAVAID database
   "waypointLoader", // 10) Waypoint database
   "terrain",        // 11) Terrain elevation database
   "atmosphere",     // 12) Atmospheric model
   "firstWeaponId",  // 13) First Released Weapon ID (default: 10001)
   "numTcThreads",   // 14) Number of T/C threads to use with the player list
   "numBgThreads",   // 15) Number of background threads to use with the player list

   "gamingAreaRange", // 16) Max valid range of the simulation's gaming area or zero for unlimited
                      //     (default: zero -- unlimited range)

   "earthModel",      // 17) Earth model for geodetic lat/lon (default is WGS-84)

   "gamingAreaUseEarthModel" // 18) If true, use the 'earthModel' or its WGS-84 default for flat
                     //    earth projections between geodetic lat/lon and the gaming
                     //    area's NED coordinates.  Otherwise, use a standard spherical
                     //    earth with a radius of Nav::ERAD60. (default: false)
END_SLOTTABLE(Simulation)

BEGIN_SLOT_MAP(Simulation)
    ON_SLOT( 1, setSlotPlayers,         base::PairStream)

    ON_SLOT( 2, setSlotRefLatitude,     base::LatLon)
    ON_SLOT( 2, setSlotRefLatitude,     base::Number)

    ON_SLOT( 3, setSlotRefLongitude,    base::LatLon)
    ON_SLOT( 3, setSlotRefLongitude,    base::Number)

    ON_SLOT( 4, setSlotSimulationTime,  base::Time)
    ON_SLOT( 5, setSlotDay,             base::Number)
    ON_SLOT( 6, setSlotMonth,           base::Number)
    ON_SLOT( 7, setSlotYear,            base::Number)
    ON_SLOT( 8, setAirports,            dafif::AirportLoader)
    ON_SLOT( 9, setNavaids,             dafif::NavaidLoader)
    ON_SLOT(10, setWaypoints,           dafif::WaypointLoader)
    ON_SLOT(11, setSlotTerrain,         ITerrain)
    ON_SLOT(12, setSlotAtmosphere,      IAtmosphere)
    ON_SLOT(13, setSlotFirstWeaponId,   base::Number)
    ON_SLOT(14, setSlotNumTcThreads,    base::Number)
    ON_SLOT(15, setSlotNumBgThreads,    base::Number)
    ON_SLOT(16, setSlotGamingAreaRange, base::Distance)

    ON_SLOT(17, setSlotEarthModel,      base::EarthModel)
    ON_SLOT(17, setSlotEarthModel,      base::String)

    ON_SLOT(18, setSlotGamingAreaEarthModel, base::Number)

END_SLOT_MAP()

Simulation::Simulation() : newPlayerQueue(MAX_NEW_PLAYERS)
{
   STANDARD_CONSTRUCTOR()

   initData();
}

Simulation::Simulation(const Simulation& org) : newPlayerQueue(MAX_NEW_PLAYERS)
{
   STANDARD_CONSTRUCTOR()
   copyData(org,true);
}

Simulation::~Simulation()
{
   STANDARD_DESTRUCTOR()
}

Simulation& Simulation::operator=(const Simulation& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

Simulation* Simulation::clone() const
{
   return new Simulation(*this);
}

void Simulation::initData()
{
   origPlayers = nullptr;
   players = nullptr;
   airports = nullptr;
   navaids = nullptr;
   waypoints = nullptr;
   terrain = nullptr;
   atmosphere = nullptr;
   station = nullptr;

   em = nullptr;

   refLat = 0.0;
   refLon = 0.0;
   sinRlat = 0.0;
   cosRlat = 1.0;
   maxRefRange = 0.0;
   gaUseEmFlg = false;
   base::Nav::computeWorldMatrix(refLat, refLon, &wm);

   cycleCnt = 0;
   frameCnt = 0;
   phaseCnt = 0;

   execTime = 0.0;

   pcTime = 0.0;
   pcTvSec = 0;
   pcTvUSec = 0;

   simTime = 0.0;
   simTvSec = 0;
   simTvUSec = 0;
   simTimeSlaved = true;

   simTime0 = -1;
   simDay0 = 0;
   simMonth0 = 0;
   simYear0 = 0;

   eventID = 0;
   eventWpnID = 0;
   relWpnId = MIN_WPN_ID;

   reqTcThreads = 1;  // Default is one -- no additional T/C threads
   numTcThreads = 0;
   for (unsigned int i = 0; i < MAX_TC_THREADS; i++) {
      tcThreads[i] = nullptr;
   }
   tcThreadsFailed = false;

   reqBgThreads = 1;  // Default is one -- no additional background threads
   numBgThreads = 0;
   for (unsigned int i = 0; i < MAX_BG_THREADS; i++) {
      bgThreads[i] = nullptr;
   }
   bgThreadsFailed = false;
}

void Simulation::copyData(const Simulation& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // Find our own Station
   station = nullptr;

   // Unref our old stuff (if any)

   // Copy original players -- DPG need proper method to copy original player list
   if (origPlayers != nullptr) { origPlayers = nullptr; }
   if (org.origPlayers != nullptr) {
      origPlayers = org.origPlayers->clone();
      origPlayers->unref();  // safe_ptr<> has it
   }

   // Copy active players
   if (players != nullptr)     { players = nullptr; }
   if (org.players != nullptr) {
      players = org.players->clone();
      players->unref();  // safe_ptr<> has it
   }

   const dafif::AirportLoader* apLoader = org.airports;
   setAirports( const_cast<dafif::AirportLoader*>(static_cast<const dafif::AirportLoader*>(apLoader)) );

   const dafif::NavaidLoader* naLoader = org.navaids;
   setNavaids( const_cast<dafif::NavaidLoader*>(static_cast<const dafif::NavaidLoader*>(naLoader)) );

   const dafif::WaypointLoader* wpLoader = org.waypoints;
   setWaypoints( const_cast<dafif::WaypointLoader*>(static_cast<const dafif::WaypointLoader*>(wpLoader)) );

   if (org.terrain != nullptr) {
      ITerrain* copy = org.terrain->clone();
      setSlotTerrain( copy );
      copy->unref();
   }
   else {
      setSlotTerrain(nullptr);
   }

   if (org.atmosphere != nullptr) {
      IAtmosphere* copy = org.atmosphere->clone();
      setSlotAtmosphere( copy );
      copy->unref();
   }
   else {
      setSlotAtmosphere(nullptr);
   }

   setEarthModel( org.em );

   // Center of the gaming area
   refLat = org.refLat;
   refLon = org.refLon;
   sinRlat = org.sinRlat;
   cosRlat = org.cosRlat;
   maxRefRange = org.maxRefRange;
   gaUseEmFlg = org.gaUseEmFlg;
   wm = org.wm;

   // Timing
   cycleCnt = org.cycleCnt;
   frameCnt = org.frameCnt;
   phaseCnt = org.phaseCnt;

   execTime = org.execTime;

   pcTime = org.pcTime;
   pcTvSec = org.pcTvSec;
   pcTvUSec = org.pcTvUSec;

   simTime = org.simTime;
   simTvSec = org.simTvSec;
   simTvUSec = org.simTvUSec;
   simTimeSlaved = org.simTimeSlaved;

   simTime0 = org.simTime0;
   simDay0 = org.simDay0;
   simMonth0 = org.simMonth0;
   simYear0 = org.simYear0;

   eventID = org.eventID;
   eventWpnID = org.eventWpnID;
   relWpnId = org.relWpnId;

   // ---
   // Terminate our threads and only copy the required number of threads;
   // reset() will create new ones.
   // ---

   for (unsigned int i = 0; i < numTcThreads; i++) {
      tcThreads[i]->terminate();
      tcThreads[i]->unref();
      tcThreads[i] = nullptr;
   }
   numTcThreads = 0;
   tcThreadsFailed = false;
   reqTcThreads = org.reqTcThreads;

   for (unsigned int i = 0; i < numBgThreads; i++) {
      bgThreads[i]->terminate();
      bgThreads[i]->unref();
      bgThreads[i] = nullptr;
   }
   numBgThreads = 0;
   bgThreadsFailed = false;
   reqBgThreads = org.reqBgThreads;
}

void Simulation::deleteData()
{
   if (origPlayers != nullptr) { origPlayers = nullptr; }
   if (players != nullptr)     { players = nullptr; }

   setSlotAtmosphere( nullptr );
   setSlotTerrain( nullptr );
   setAirports( nullptr );
   setNavaids( nullptr );
   setWaypoints( nullptr );

   base::Pair* newPlayer = newPlayerQueue.get();
   while (newPlayer != nullptr) {
      newPlayer->unref();
      newPlayer = newPlayerQueue.get();
   }

   for (unsigned int i = 0; i < numTcThreads; i++) {
      tcThreads[i]->terminate();
      tcThreads[i]->unref();
      tcThreads[i] = nullptr;
    }
   numTcThreads = 0;
   tcThreadsFailed = false;

   for (unsigned int i = 0; i < numBgThreads; i++) {
      bgThreads[i]->terminate();
      bgThreads[i]->unref();
      bgThreads[i] = nullptr;
   }
   numBgThreads = 0;
   bgThreadsFailed = false;

   station = nullptr;
}

//------------------------------------------------------------------------------
// reset() -- Reset the simulation & players
//------------------------------------------------------------------------------
void Simulation::reset()
{
   // ---
   // Something old and something new ...
   // ... We're going to create a new player list.
   // ---
   base::safe_ptr<base::PairStream> newList( new base::PairStream() );
   newList->unref();  // 'newList' has it, so unref() from the 'new'

   // ---
   // Copy original players to the new list
   // ---
   {
      if (origPlayers != nullptr) {
         base::safe_ptr<base::PairStream> origPlayerList = origPlayers;
         base::List::Item* item = origPlayerList->getFirstItem();
         while (item != nullptr) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            IPlayer* ip = static_cast<IPlayer*>(pair->object());

            // reinstated the container pointer and player name
            ip->container(this);
            ip->setName(*pair->slot());

            // Insert the player into the new list in sorted order
            insertPlayerSort(pair, newList);
            item = item->getNext();
         }
      }
   }

   // ---
   // Copy the old networked players (IPlayers) to the new list
   // ---
   {
      if (players != nullptr) {
         base::safe_ptr<base::PairStream> origPlayerList = players;
         base::List::Item* item = origPlayerList->getFirstItem();
         while (item != nullptr) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            IPlayer* ip = static_cast<IPlayer*>(pair->object());
            if (ip->isNetworkedPlayer()) {

               // reinstated the container pointer and player name
               ip->container(this);
               ip->setName(*pair->slot());

               // Insert the IPlayer into the new list in sorted order
               insertPlayerSort(pair, newList);
            }
            item = item->getNext();
         }
      }
   }

   // ---
   // Swap the lists
   // ---
   players = newList;

   // ---
   // First time resetting the terrain database will load the data
   // ---
   if (terrain != nullptr) {
      std::cout << "Loading Terrain Data..." << std::endl;
      terrain->reset();
      std::cout << "Finished!" << std::endl;
   }


   // ---
   // Reset atmospheric model
   // ---
   if (atmosphere != nullptr) atmosphere->reset();

   // ---
   // Create the T/C thread pool
   // ---
   if (reqTcThreads > 1 && numTcThreads == 0 && !tcThreadsFailed) {

      // Use the T/C priority from our container Station.
      double pri = Station::DEFAULT_TC_THREAD_PRI;
      const Station* sta = static_cast<const Station*>(findContainerByType( typeid(Station) ));
      if (sta != nullptr) {
         pri = sta->getTimeCriticalPriority();
      }

      for (unsigned int i = 0; i < (reqTcThreads-1); i++) {
         tcThreads[numTcThreads] = new SimTcThread(this, pri);
         bool ok = tcThreads[numTcThreads]->create();
         if (ok) {
            std::cout << "Created T/C pool thread[" << i << "] = " << tcThreads[i] << std::endl;
            numTcThreads++;
         }
         else {
            tcThreads[numTcThreads]->unref();
            tcThreads[numTcThreads] = nullptr;
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "simulation::reset(): ERROR, failed to create a T/C pool thread!" << std::endl;
            }
         }
      }

      // If we still don't have any threads then something failed
      // and we don't want to try again.
      tcThreadsFailed = (reqTcThreads > 1 && numTcThreads == 0);

   }

   // ---
   // Create the background thread pool
   // ---
   if (reqBgThreads > 1 && numBgThreads == 0 && !bgThreadsFailed) {

      // Use the background priority from our container Station.
      double pri = Station::DEFAULT_BG_THREAD_PRI;
      const Station* sta = static_cast<const Station*>(findContainerByType( typeid(Station) ));
      if (sta != nullptr) {
         pri = sta->getBackgroundPriority();
      }

      for (unsigned int i = 0; i < (reqBgThreads-1); i++) {
         bgThreads[numBgThreads] = new SimBgThread(this, pri);
         bool ok = bgThreads[numBgThreads]->create();
         if (ok) {
            std::cout << "Created background pool thread[" << i << "] = " << bgThreads[i] << std::endl;
            numBgThreads++;
         }
         else {
            bgThreads[numBgThreads]->unref();
            bgThreads[numBgThreads] = nullptr;
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "simulation::reset(): ERROR, failed to create a background pool thread!" << std::endl;
            }
         }
      }

      // If we still don't have any threads then something failed
      // and we don't want to try again.
      bgThreadsFailed = (reqBgThreads > 1 && numBgThreads == 0);

   }

   // ---
   // Reset simulated time (if not slaved to UTC)
   // ---

   // Start with the current computer system date and time as seconds
   // since midnight, January 1, 1970 (UTC)
   base::getTime(&pcTvSec, &pcTvUSec);

   // Decompose into year, month, etc
   unsigned int cYear = 0;
   unsigned int cMonth = 0;
   unsigned int cDay = 0;
   unsigned int cHour = 0;
   unsigned int cMin = 0;
   unsigned int cSec = 0;
   base::convertSec2Ymdhms(pcTvSec, &cYear, &cMonth, &cDay, &cHour, &cMin, &cSec);
      //std::printf("RESET PC Times: y=%d; m=%d; d=%d; h=%d; m=%d; s=%d\n", cYear, cMonth, cDay, cHour, cMin, cSec);
      //std::printf("RESET PC sec = %d, uSec=%d\n", pcTvSec, pcTvUSec);

   // Computer time of day (seconds since midnight)
   pcTime = ( cHour * 3600.0 + cMin * 60.0 + cSec );

   // Simulated uSec same as the computer systems
   simTvUSec = pcTvUSec;

   simTimeSlaved = (simTime0 < 0) && (simDay0 == 0) && (simMonth0 == 0) && (simYear0 == 0);
   if (!simTimeSlaved) {
      // Replace year, month or date, as required
      if (simYear0 > 0) cYear = simYear0;
      if (simMonth0 > 0) cMonth = simMonth0;
      if (simDay0 > 0) cDay = simDay0;

      // Replace the time of day, as required
      if (simTime0 >= 0) {
         unsigned int t = simTime0;
         // Compute simulated hour of the date
         cHour = (t / 3600);
         t -= (cHour * 3600);
         // Compute simulated minute past the hour
         cMin  = t/60;
         t -= (cMin * 60);
         // Compute simulated second past the minute
         cSec = t;

         // Set simulated uSec set to 0; using pc microseconds is meaningless and introduces randomness where none is desired.
         simTvUSec = 0;
      }

      // Recompute simulated whole seconds since midnight, January 1, 1970
      base::convertYmdhms2Sec(cYear, cMonth, cDay, cHour, cMin, cSec, &simTvSec);
      //std::printf("RESET sim Times: y=%d; m=%d; d=%d; h=%d; m=%d; s=%d\n", cYear, cMonth, cDay, cHour, cMin, cSec);
      //std::printf("RESET SIM sec = %d, uSec=%d\n", simTvSec, simTvUSec);

      // simulated time of day (seconds since midnight)
      simTime = ( cHour * 3600.0 + cMin * 60.0 + cSec );
   }
   else {
      simTvSec = pcTvSec;
      simTime = pcTime;
   }

   // ---
   // Now reset the new player list
   // ---
   if (players != nullptr) {
      base::safe_ptr<base::PairStream> pl = players;
      base::List::Item* item = pl->getFirstItem();
      while (item != nullptr) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
         if (pair != nullptr) {
            IPlayer* ip = static_cast<IPlayer*>(pair->object());
            if (ip != nullptr) ip->event(RESET_EVENT);
         }
         item = item->getNext();
      }
   }

   BaseClass::reset();
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool Simulation::shutdownNotification()
{
   // ---
   // Shutdown our baseclass, which will notify our components
   // ---
   BaseClass::shutdownNotification();

   // ---
   // Tell everyone on our player list
   // ---
   base::PairStream* plist = getPlayers();
   if (plist != nullptr) {

      // Send shutdown to all players
      base::List::Item* item = plist->getFirstItem();
      while (item != nullptr) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
         base::Component* p = static_cast<base::Component*>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }

      // cleanup
      plist->unref();
      plist = nullptr;
   }

   // ---
   // Tell everyone on our 'new' player list, and remove
   // them from the list
   // ---
   base::Pair* newPlayer = newPlayerQueue.get();
   while (newPlayer != nullptr) {
      base::Component* p = static_cast<base::Component*>(newPlayer->object());
      p->event(SHUTDOWN_EVENT);
      newPlayer->unref();
      newPlayer = newPlayerQueue.get();
   }

   // ---
   // Tell the environments ...
   // ---
   if (atmosphere != nullptr) atmosphere->event(SHUTDOWN_EVENT);
   if (terrain != nullptr) terrain->event(SHUTDOWN_EVENT);

   // ---
   // Shut down the thread pools
   // ---
   if (numTcThreads > 0) {
      for (unsigned int i = 0; i < numTcThreads; i++) {
         // We're just going to make sure the threads not suspended,
         // and they'll check our shutdown flag.
         tcThreads[i]->signalStart();
      }
   }
   if (numBgThreads > 0) {
      for (unsigned int i = 0; i < numBgThreads; i++) {
         // We're just going to make sure the threads not suspended,
         // and they'll check our shutdown flag.
         bgThreads[i]->signalStart();
      }
   }

   return true;
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void Simulation::updateTC(const double dt)
{
   // ---
   // Update the executive time
   // ---
   execTime += dt;

   // ---
   // Update the computer's UTC time and date,
   // and set the current time of day
   // ---
   {
      unsigned long newPcTvUSec = pcTvUSec + static_cast<unsigned long>(dt * 1000000.0 + 0.5);
      // while loop is used instead of if statement in case dt > 1 sec (i.e., tcRate < 1 Hz)
      while (newPcTvUSec >= 1000000) {
         newPcTvUSec = newPcTvUSec - 1000000;
         pcTvSec++;
      }
      pcTvUSec = newPcTvUSec;
      //getTime(&pcTvSec, &pcTvUSec); /* test */

      unsigned int cYear = 0;
      unsigned int cMonth = 0;
      unsigned int cDay = 0;
      unsigned int cHour = 0;
      unsigned int cMin = 0;
      unsigned int cSec = 0;
      base::convertSec2Ymdhms(pcTvSec, &cYear, &cMonth, &cDay, &cHour, &cMin, &cSec);

      // Computer time of day (seconds since midnight)
      pcTime = ( cHour * 3600.0 + cMin * 60.0 + cSec + static_cast<double>(pcTvUSec) / 1000000.0 );

      //std::printf("ComputerTimes: y=%d; m=%d; d=%d; h=%d; m=%d; s=%d, us=%06d\n", cYear, cMonth, cDay, cHour, cMin, cSec, pcTvUSec);
      //std::printf("PC sec = %d\n", pcTvSec);
      //std::printf("PC Time = %f\n", pcTime);
   }

   // ---
   // Delta-Time (Frozen?)
   // ---
   double dt0 = dt;
   if (isFrozen()) dt0 = 0.0;

   // ---
   // Update simulated time
   // ---
   {
      // If slaved then the simulated time can not be frozen
      double deltaTime = dt0;
      if (simTimeSlaved) deltaTime = dt;

      unsigned long newSimTvUSec = simTvUSec + static_cast<unsigned long>(deltaTime * 1000000.0 + 0.5);
      // while loop is used instead of if statement in case dt > 1 sec (i.e., tcRate < 1 Hz)
      while (newSimTvUSec >= 1000000) {
         newSimTvUSec = newSimTvUSec - 1000000;
         simTvSec++;
      }

      simTvUSec = newSimTvUSec;
   }

   // compute the simulated time of day
   {
      unsigned int cYear = 0;
      unsigned int cMonth = 0;
      unsigned int cDay = 0;
      unsigned int cHour = 0;
      unsigned int cMin = 0;
      unsigned int cSec = 0;
      base::convertSec2Ymdhms(simTvSec, &cYear, &cMonth, &cDay, &cHour, &cMin, &cSec);

      // Computer time of day (seconds since midnight)
      simTime = ( cHour * 3600.0 + cMin * 60.0 + cSec + static_cast<double>(simTvUSec) / 1000000.0 );

      //std::printf("SimTimes: y=%d; m=%d; d=%d; h=%d; m=%d; s=%d, us=%06d\n", cYear, cMonth, cDay, cHour, cMin, cSec, simTvUSec);
      //std::printf("Sim sec = %d\n", simTvSec);
      //std::printf("simTime = %f\n", simTime);
   }

   // ---
   // Update base classes stuff
   // ---
   BaseClass::updateTC(dt0);

   // ---
   // Called once per frame -- Process 4 phases per frame
   // ---
   {
      // This locks the current player list for this time-critical frame
      base::safe_ptr<base::PairStream> currentPlayerList = players;

      for (unsigned int f = 0; f < 4; f++) {

         // Set the current phase
         setPhase(f);

         if (reqTcThreads == 1) {
            // Our single TC thread
            updateTcPlayerList(currentPlayerList, (dt0/4.0), 1, 1);
         }
         else if (numTcThreads > 0) {
            // multiple threads
            for (unsigned short i = 0; i < numTcThreads; i++) {

               // assign the threads from the pool
               unsigned int idx = (i+1);
               tcThreads[i]->start(currentPlayerList, (dt0/4.0), idx, reqTcThreads);
            }

            // we're the last thread
            updateTcPlayerList(currentPlayerList, (dt0/4.0), reqTcThreads, reqTcThreads);

            // Now wait for the other thread(s) to complete
            base::ThreadSyncTask** pp = reinterpret_cast<base::ThreadSyncTask**>(&tcThreads[0]);
            base::ThreadSyncTask::waitForAllCompleted(pp, numTcThreads);

         }
         else if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "simulation::updateTC() ERROR, invalid T/C thread setup";
            std::cerr << "; reqTcThreads = " << reqTcThreads;
            std::cerr << "; numTcThreads = " << numTcThreads;
            std::cerr << std::endl;
         }
      }
   }

   // Update frame & cycle counts
   int cframe = frame() + 1;
   if (cframe >= 16) {
      incCycle();
      cframe = 0;
   }
   setFrame(cframe);
   setPhase(0);
}

//------------------------------------------------------------------------------
// Time critical thread processing for every n'th player starting
// with the idx'th player
//------------------------------------------------------------------------------
void Simulation::updateTcPlayerList(
   base::PairStream* const playerList,
   const double dt,
   const unsigned int idx,
   const unsigned int n)
{
   if (playerList != nullptr) {
      unsigned int index = idx;
      unsigned int count = 0;
      base::List::Item* item = playerList->getFirstItem();
      while (item != nullptr) {
         count++;
         if (count == index) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            IPlayer* ip = static_cast<IPlayer*>(pair->object());
            ip->tcFrame(dt);
            index += n;
         }
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// updateData() -- update non-time critical stuff here
//------------------------------------------------------------------------------
void Simulation::updateData(const double dt)
{
    // Delta-Time (Frozen?)
    double dt0 = dt;
    if (isFrozen()) dt0 = 0.0;

    // Update base classes stuff
    BaseClass::updateData(dt0);

    // Update the player list
    updatePlayerList();

    // Update all players
    if (players != nullptr) {
         base::safe_ptr<base::PairStream> currentPlayerList = players;

         if (reqBgThreads == 1) {
            // Our single thread
            updateBgPlayerList(currentPlayerList, dt0, 1, 1);
         }
         else if (numBgThreads > 0) {
            // multiple threads
            for (unsigned short i = 0; i < numBgThreads; i++) {

               // assign the threads from the pool
               unsigned int idx = (i+1);
               bgThreads[i]->start(currentPlayerList, dt0, idx, reqBgThreads);
            }

            // we're the last thread
            updateBgPlayerList(currentPlayerList, dt0, reqBgThreads, reqBgThreads);

            // Now wait for the other thread(s) to complete
            base::ThreadSyncTask** pp = reinterpret_cast<base::ThreadSyncTask**>(&bgThreads[0]);
            base::ThreadSyncTask::waitForAllCompleted(pp, numBgThreads);

         }
         else if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "simulation::updateData() ERROR, invalid background thread setup";
            std::cerr << "; reqBgThreads = " << reqBgThreads;
            std::cerr << "; numBgThreads = " << numBgThreads;
            std::cerr << std::endl;
        }
    }

    // ---
    // Load DAFIF files (one pre frame)
    // ---
    if (airports != nullptr && airports->numberOfRecords() == 0) {
       // Load Airports
       airports->load();
    }
    else if (navaids != nullptr && navaids->numberOfRecords() == 0) {
       // Load Navaids
       navaids->load();
    }
    else if (waypoints != nullptr && waypoints->numberOfRecords() == 0) {
       // Load Waypoints
       waypoints->load();
    }

}

//------------------------------------------------------------------------------
// Background thread processing for every n'th player starting
// with the idx'th player
//------------------------------------------------------------------------------
void Simulation::updateBgPlayerList(
         base::PairStream* const playerList,
         const double dt,
         const unsigned int idx,
         const unsigned int n)
{
   if (playerList != nullptr) {
      unsigned int index = idx;
      unsigned int count = 0;
      base::List::Item* item = playerList->getFirstItem();
      while (item != nullptr) {
         count++;
         if (count == index) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            IPlayer* ip = static_cast<IPlayer*>(pair->object());
            ip->updateData(dt);
            index += n;
         }
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// printTimingStats() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Simulation::printTimingStats()
{
   const base::Statistic* ts = getTimingStats();
   int c = cycle();
   int f = frame() - 1;
   if (f < 0) {
      c--;
      f = 15;
   }
   std::cout << "simulation(" << c << "," << f << "): dt=" << ts->value() << ", ave=" << ts->mean() << ", max=" << ts->maxValue() << std::endl;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Returns the player list
base::PairStream* Simulation::getPlayers()
{
   return players.getRefPtr();
}

// Returns the player list (const version)
const base::PairStream* Simulation::getPlayers() const
{
   return players.getRefPtr();
}

// Returns a pointer to the EarthModel
const base::EarthModel* Simulation::getEarthModel() const
{
   return em;
}

// Gaming area using the earth model?
bool Simulation::isGamingAreaUsingEarthModel() const
{
   return gaUseEmFlg;
}

// Returns the reference latitude
double Simulation::getRefLatitude() const
{
   return refLat;
}

// Returns the reference longitude
double Simulation::getRefLongitude() const
{
   return refLon;
}

// Returns the sine of the reference latitude
double Simulation::getSinRefLat() const
{
   return sinRlat;
}

// Returns the cosine of the reference latitude
double Simulation::getCosRefLat() const
{
   return cosRlat;
}

// Max valid range (meters) of the gaming area or zero if there's no limit.
double Simulation::getMaxRefRange() const
{
   return maxRefRange;
}

// World transformation matrix:
const osg::Matrixd& Simulation::getWorldMat() const
{
   return wm;
}

// Real-time cycle counter
unsigned int Simulation::cycle() const
{
   return cycleCnt;
}

// Real-time frame counter [0 .. 15]
unsigned int Simulation::frame() const
{
   return frameCnt;
}

// Real-time phase counter [0 .. 3]
unsigned int Simulation::phase() const
{
   return phaseCnt;
}

// Returns the exec counter (R/T phases since start)
unsigned int Simulation::getExecCounter() const
{
   return ((cycleCnt << 6) + (frameCnt << 2) + phaseCnt);
}

// Returns executive time, which is time since start (sec)
double Simulation::getExecTimeSec() const
{
   return execTime;
}

// Returns computer systems time of day (UTC -- seconds since midnight)
double Simulation::getSysTimeOfDay() const
{
   return pcTime;
}

// Returns the simulated time of day (UTC -- seconds since midnight)
double Simulation::getSimTimeOfDay() const
{
   return simTime;
}

// Simulated time (UTC) values, where ...
void Simulation::getSimTimeValues(
      unsigned long* const simSec,  // (OUT) The whole seconds since midnight (00:00:00), January 1, 1970
      unsigned long* const simUSec  // (OUT) The number of microseconds in the current second.
   ) const
{
   if (simSec != nullptr) *simSec = simTvSec;
   if (simUSec != nullptr) *simUSec = simTvUSec;
}

// Generates an unique major simulation event ID [1 .. 65535]
unsigned short Simulation::getNewEventID()
{
   return ++eventID;
}

// Generates a unique weapon event ID [1 .. 65535]
unsigned short Simulation::getNewWeaponEventID()
{
   return ++eventWpnID;
}

// Generates a unique ID number for released weapons
unsigned short Simulation::getNewReleasedWeaponID()
{
   return relWpnId++;
};

// Returns the terrain elevation database
const ITerrain* Simulation::getTerrain() const
{
   return terrain;
}

ITerrain* Simulation::getTerrain()
{
   return terrain;
}

// Returns the atmosphere model
IAtmosphere* Simulation::getAtmosphere()
{
   return atmosphere;
}

// Returns the atmospheric model
const IAtmosphere* Simulation::getAtmosphere() const
{
   return atmosphere;
}

// Returns the airport loader
dafif::AirportLoader* Simulation::getAirports()
{
   return airports;
}

// Returns the NAVAID loader
dafif::NavaidLoader* Simulation::getNavaids()
{
   return navaids;
}

// Returns the waypoint loader
dafif::WaypointLoader* Simulation::getWaypoints()
{
   return waypoints;
}

// Returns the data recorder
DataRecorder* Simulation::getDataRecorder()
{
   DataRecorder* p = nullptr;
   Station* sta = getStation();
   if (sta != nullptr) p = sta->getDataRecorder();
   return p;
}

// Our Station
Station* Simulation::getStation()
{
   if (station == nullptr) {
      getStationImp();
   }
   return station;
}

// Our Station (const version)
const Station* Simulation::getStation() const
{
   if (station == nullptr) {
      (const_cast<Simulation*>(this))->getStationImp();
   }
   return station;
}

Station* Simulation::getStationImp()
{
   if (station == nullptr) {
      station = static_cast<Station*>(findContainerByType(typeid(Station)));
      if (station == nullptr && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "simulation::getStationImp(): ERROR, unable to locate the Station class!" << std::endl;
      }
   }
   return station;
}


//------------------------------------------------------------------------------
// Sets the airport loader
//------------------------------------------------------------------------------
bool Simulation::setAirports(dafif::AirportLoader* const p)
{
   if (airports != nullptr) {
      airports->unref();
   }
   airports = p;
   if (airports != nullptr) {
      airports->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// Sets the navaid loader
//------------------------------------------------------------------------------
bool Simulation::setNavaids(dafif::NavaidLoader* const p)
{
   if (navaids != nullptr) {
      navaids->unref();
   }
   navaids = p;
   if (navaids != nullptr) {
      navaids->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// Sets the waypoint loader
//------------------------------------------------------------------------------
bool Simulation::setWaypoints(dafif::WaypointLoader* const p)
{
   if (waypoints != nullptr) {
      waypoints->unref();
   }
   waypoints = p;
   if (waypoints != nullptr) {
      waypoints->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotPlayers() -- set the original player list (make sure we have only
// player type objects with unique names and IDs)
//------------------------------------------------------------------------------
bool Simulation::setSlotPlayers(base::PairStream* const pl)
{
   // Early out if we're just zeroing the player lists
   if (pl == nullptr) {
      origPlayers = nullptr;
      players = nullptr;
      return true;
   }

   bool ok = true;
   unsigned short maxID=0;

   // First, make sure they are all Players.
   {
      base::List::Item* item = pl->getFirstItem();
      while (item != nullptr && ok) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
         item = item->getNext();
         IPlayer* ip = dynamic_cast<IPlayer*>( pair->object() );
         if (ip == nullptr) {
            // Item is NOT a Player
            std::cerr << "simulation::setSlotPlayers: slot: " << *pair->slot() << " is NOT of a Player type!" << std::endl;
            ok = false;
         }
         else {
            // find the max ID# of players with preassigned IDs
            if (ip->getID() > maxID)
               maxID = ip->getID();
         }
      }
   }

   // increment to next available ID
   maxID++;

   // Next, make sure we have unique player names and IDs
   if (ok) {
      // For all players ...
      base::List::Item* item1 = pl->getFirstItem();
      while (item1 != nullptr) {
         base::Pair* pair1 = static_cast<base::Pair*>(item1->getValue());
         item1 = item1->getNext();
         IPlayer* ip1 = static_cast<IPlayer*>(pair1->object());

         // unassigned ID
         if ( (ip1->getID() == 0) && (maxID < 65535) ) {
            ip1->setID(maxID);
            ++maxID;
         }

         base::List::Item* item2 = item1;
         while (item2 != nullptr) {
            base::Pair* pair2 = static_cast<base::Pair*>(item2->getValue());
            IPlayer* ip2 = static_cast<IPlayer*>(pair2->object());

            // unassigned ID
            if ( (ip2->getID() == 0)  && (maxID < 65535) ) {
               ip2->setID(maxID);
               ++maxID;
            }

            if (ip1->getID() == ip2->getID()) {
               std::cerr << "simulation::setSlotPlayers: duplicate player ID: " << ip1->getID() << std::endl;
               ok = false;
            }

            if (*pair1->slot() == *pair2->slot()) {
               std::cerr << "simulation::setSlotPlayers: duplicate player name: " << *pair1->slot() << std::endl;
               ok = false;
            }

            item2 = item2->getNext();
         }

      }
   }

   // Next, set the container pointer, set the player's name
   // and setup the player lists.
   if (ok) {
      base::List::Item* item = pl->getFirstItem();
      while (item != nullptr) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
         item = item->getNext();
         IPlayer* ip = static_cast<IPlayer*>(pair->object());
         ip->container(this);
         ip->setName(*pair->slot());
      }

      // Set the original player list pointer
      origPlayers = pl;

      // Create the new active player list
      base::PairStream* newList( new base::PairStream() );

      // Copy original players to the new list
      if (origPlayers != nullptr) {
         base::safe_ptr<base::PairStream> origPlayerList = origPlayers;
         base::List::Item* item = origPlayerList->getFirstItem();
         while (item != nullptr) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            insertPlayerSort(pair, newList);
            item = item->getNext();
         }
      }

      // Set the active player list pointer
      players = newList;
      newList->unref();
   }

   return ok;
}

//------------------------------------------------------------------------------
// updatePlayerList() -- update the player list ...
//                       1) remove 'deleteRequest' mode players
//                       2) add new players
//------------------------------------------------------------------------------
void Simulation::updatePlayerList()
{
    // ---
    // Do we need to swap player lists?  Only if a player
    // needs to be added or removed ---
    // ---

    // First check for new players ...
    bool yes = newPlayerQueue.isNotEmpty();

    // Second, check for delete requests
    if (!yes) {
        base::safe_ptr<base::PairStream> pl = players;
        base::List::Item* item = pl->getFirstItem();
        while (!yes && item != nullptr) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            IPlayer* p = static_cast<IPlayer*>(pair->object());
            yes = p->isMode(IPlayer::DELETE_REQUEST);
            item = item->getNext();
        }
    }

    if (yes) {

        // Update Required!

        // ---
        // Something old and something new ...
        // ---
        base::safe_ptr<base::PairStream> newList( new base::PairStream() );
        newList->unref();  // 'newList' has it, so unref() from the 'new'

        // ---
        // Copy players to the new list; except 'deleteRequest' mode players
        // ---
        base::safe_ptr<base::PairStream> oldList = players;
        base::List::Item* item = oldList->getFirstItem();
        while (item != nullptr) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            item = item->getNext();
            IPlayer* p = static_cast<IPlayer*>(pair->object());
            if (p->isNotMode(IPlayer::DELETE_REQUEST)) {
                // Add the player to the new list
                newList->put(pair);
            }
            else {
                // Deleting this player: remove us as its container
                // and don't add to the new player list
                p->container(nullptr);

                BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_PLAYER_REMOVED )
                   SAMPLE_1_OBJECT( p )
                END_RECORD_DATA_SAMPLE()
            }
        }

        // ---
        // Add any new players
        // ---
      base::Pair* newPlayer = newPlayerQueue.get();
      while (newPlayer != nullptr) {
            // get the player
            IPlayer* ip = static_cast<IPlayer*>(newPlayer->object());

            BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_NEW_PLAYER )
               SAMPLE_1_OBJECT( ip )
            END_RECORD_DATA_SAMPLE()

/*  DDH
            // TabLogger is deprecated
            if (getAnyEventLogger() != nullptr) {  // EventLogger Deprecated
                TabLogger::TabLogEvent* evt = new TabLogger::LogPlayerData(1, ip); // code 1 for "new" msg
                getAnyEventLogger()->log(evt);
                evt->unref();
            }
*/

            // Set container and name
            ip->container(this);
            ip->setName(*newPlayer->slot());

            // Insert the new player into the new list in sorted order
            insertPlayerSort(newPlayer, newList);

            newPlayer->unref();

         newPlayer = newPlayerQueue.get();
        }

        // ---
        // Swap the lists
        // ---
        players = newList;
    }
}

//------------------------------------------------------------------------------
// addNewPlayer() -- add a new player by name and player object; the new
//                   player is added to the player list at the start of
//                   the next frame.  Returns true of player will be added
//                   or false if there is an error.
//------------------------------------------------------------------------------
bool Simulation::addNewPlayer(base::Pair* const player)
{
    if (player == nullptr) return false;
    player->ref();

    newPlayerQueue.put(player);

    return true;
}

//------------------------------------------------------------------------------
// addNewPlayer() -- add a new player by name and player object; the new
//                   player is added to the player list at the start of
//                   the next frame.  Returns true of player will be added
//                   or false if there is an error.
//------------------------------------------------------------------------------
bool Simulation::addNewPlayer(const char* const playerName, IPlayer* const player)
{
    if (playerName == nullptr || player == nullptr) return false;

    base::Pair* pair = new base::Pair(playerName, player);
    bool ok = addNewPlayer(pair);
    pair->unref();
    return ok;
}

//------------------------------------------------------------------------------
// insertPlayerSort() -- Insert the new player into the new list in sorted order
//------------------------------------------------------------------------------
bool Simulation::insertPlayerSort(base::Pair* const newPlayerPair, base::PairStream* const newList)
{
    newList->ref();

    // create a new base::List::Item to hold the player
    base::List::Item* newItem = new base::List::Item;
    newPlayerPair->ref();
    newItem->value = newPlayerPair;

    // Get the player
    IPlayer* newPlayer = static_cast<IPlayer*>(newPlayerPair->object());

    // Search the new player list and insert into the correct position --
    //  -- sorted by network ID and player ID
    bool inserted = false;
    base::List::Item* refItem = newList->getFirstItem();
    while (refItem != nullptr && !inserted) {
        base::Pair* refPair = static_cast<base::Pair*>(refItem->getValue());
        IPlayer* refPlayer = static_cast<IPlayer*>(refPair->object());

        bool insert = false;
        if (newPlayer->isNetworkedPlayer()) {

            // *** Insert IPlayer -- after local players and lower NIB IDs first
            if (refPlayer->isNetworkedPlayer()) {

               // Get the NIBs
               const INib* nNib = newPlayer->getNib();
               const INib* rNib = refPlayer->getNib();

               // Compare federate names
               int result = std::strcmp(*nNib->getFederateName(), *rNib->getFederateName());
               if (result == 0) {
                  // Same federate name; compare player IDs
                  if (nNib->getPlayerID() > rNib->getPlayerID()) result = +1;
                  else if (nNib->getPlayerID() < rNib->getPlayerID()) result = -1;
               }

               // Insert if the new NIB's IDs are less than the ref NIB's IDs
               insert = (result < 0);
            }
        }
        else {

            // *** Insert a local player -- by player ID and before any IPlayer
            insert = ( (newPlayer->getID() < refPlayer->getID()) || refPlayer->isNetworkedPlayer() );

        }

        if (insert) {
            newList->insert(newItem, refItem);
            inserted = true;
        }

        refItem = refItem->getNext();
    }

    // Hasn't been inserted yet, so add to the tail
    if (!inserted) {
        newList->insert(newItem, nullptr);
    }

    newList->unref();
    return true;
}


//------------------------------------------------------------------------------
// findPlayer() -- Find a player that matches 'id' and 'networkID'
//------------------------------------------------------------------------------
IPlayer* Simulation::findPlayer(const short id, const int netID)
{
   return findPlayerPrivate(id, netID);
}

const IPlayer* Simulation::findPlayer(const short id, const int netID) const
{
   return findPlayerPrivate(id, netID);
}

IPlayer* Simulation::findPlayerPrivate(const short id, const int netID) const
{
    // Quick out
    if (players == nullptr) return nullptr;

    // Find a Player that matches player ID and Sources
    IPlayer* iplayer = nullptr;
    const base::List::Item* item = players->getFirstItem();
    while (iplayer == nullptr && item != nullptr) {
        const base::Pair* pair = static_cast<const base::Pair*>(item->getValue());
        if (pair != nullptr) {
            IPlayer* ip = const_cast<IPlayer*>(static_cast<const IPlayer*>(pair->object()));
            if (ip != nullptr) {
                if (netID > 0) {
                    if ((ip->getID() == id) && (ip->getNetworkID() == netID)) {
                        iplayer = ip;
                    }
                }
                else {
                    if (ip->getID() == id) {
                        iplayer = ip;
                    }
                }
            }
        }
        item = item->getNext();
    }

    return iplayer;
}

//------------------------------------------------------------------------------
// findPlayerByName() -- Find a player by name
//------------------------------------------------------------------------------
IPlayer* Simulation::findPlayerByName(const char* const playerName)
{
   return findPlayerByNamePrivate(playerName);
}

const IPlayer* Simulation::findPlayerByName(const char* const playerName) const
{
   return findPlayerByNamePrivate(playerName);
}

IPlayer* Simulation::findPlayerByNamePrivate(const char* const playerName) const
{
    // Quick out
    if (players == nullptr || playerName == nullptr) return nullptr;

    // Find a Player named 'playerName'
    IPlayer* iplayer = nullptr;
    const base::List::Item* item = players->getFirstItem();
    while (iplayer == nullptr && item != nullptr) {
        const base::Pair* pair = static_cast<const base::Pair*>(item->getValue());
        if (pair != nullptr) {
            IPlayer* ip = const_cast<IPlayer*>(static_cast<const IPlayer*>(pair->object()));
            if (ip != nullptr && ip->isName(playerName)) {
               iplayer = ip;
            }
        }
        item = item->getNext();
    }

    return iplayer;
}

//------------------------------------------------------------------------------
// Data set routines
//------------------------------------------------------------------------------

bool Simulation::setEarthModel(const base::EarthModel* const msg)
{
   if (em != nullptr) {
      em->unref();
      em = nullptr;
   }

   if (msg != nullptr) {
      em = msg->clone();
   }

   return true;
}

bool Simulation::setGamingAreaUseEarthModel(const bool flg)
{
   gaUseEmFlg = flg;
   return true;
}

// Sets Ref latitude
bool Simulation::setRefLatitude(const double v)
{
   bool ok = (v <= 90.0 && v >= -90.0);
   if (ok) {
      // Set the latitude and compute the world matrix
      refLat = v;
      const double r = base::Angle::D2RCC * refLat;
      sinRlat = std::sin(r);
      cosRlat = std::cos(r);
      base::Nav::computeWorldMatrix(refLat, refLon, &wm);
   }
   return ok;
}

// Sets Ref longitude
bool Simulation::setRefLongitude(const double v)
{
   bool ok = (v <= 180.0 && v >= -180.0);
   if (ok) {
      // Set the longitude and compute the world matrix
      refLon = v;
      base::Nav::computeWorldMatrix(refLat, refLon, &wm);
   }
   return ok;
}

// Sets the max range (meters) of the gaming area or zero if there's no limit.
bool Simulation::setMaxRefRange(const double v)
{
   bool ok = (v >= 0);
   if (ok) maxRefRange = v;
   return ok;
}

// Sets the initial simulation time (sec; or less than zero to slave to UTC)
bool Simulation::setInitialSimulationTime(const long time)
{
   simTime0 = time;
   return true;
}

// Increment the cycle counter
void Simulation::incCycle()
{
   cycleCnt++;
}

// Sets the cycle counter
void Simulation::setCycle(const unsigned int c)
{
   cycleCnt = c;
}

// Sets the frame counter
void Simulation::setFrame(const unsigned int f)
{
   frameCnt = f;
}

// Sets the phase counter
void Simulation::setPhase(const unsigned int c)
{
   phaseCnt = c;
}

// Sets the simulation event ID counter
void Simulation::setEventID(unsigned short id)
{
   eventID = id;
}

// Sets the weapon ID event counter
void Simulation::setWeaponEventID(unsigned short id)
{
   eventWpnID = id;
}

//------------------------------------------------------------------------------
// Set Slot routines
//------------------------------------------------------------------------------
bool Simulation::setSlotRefLatitude(const base::LatLon* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setRefLatitude(msg->getDouble());
    }
    return ok;
}

bool Simulation::setSlotRefLatitude(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setRefLatitude(msg->getDouble());
    }
    return ok;
}

bool Simulation::setSlotRefLongitude(const base::LatLon* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setRefLongitude(msg->getDouble());
    }
    return ok;
}

bool Simulation::setSlotRefLongitude(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setRefLongitude(msg->getDouble());
    }
    return ok;
}

bool Simulation::setSlotSimulationTime(const base::Time* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       const long t = static_cast<long>( osg::round(base::Seconds::convertStatic(*msg)) );
       if (t >= -1 && t < (60*60*24)) {
          ok = setInitialSimulationTime(t);
       }
       else {
         std::cerr << "simulation::setSlotSimulationTime(): invalid time of day: " << t << " seconds; use [ 0 .. 86400 ] seconds or -1" << std::endl;
       }
    }
    return ok;
}

bool Simulation::setSlotDay(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const int v = msg->getInt();
      if (v >= 0 && v <= 31) {
         simDay0 = static_cast<unsigned short>(v);
         ok = true;
      }
      else {
         std::cerr << "simulation::setSlotDay(): invalid day: " << v << "; use [ 1 .. 31 ] or zero" << std::endl;
      }
   }
   return ok;
}

bool Simulation::setSlotMonth(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const int v = msg->getInt();
      if (v >= 0 && v <= 12) {
         simMonth0 = static_cast<unsigned short>(v);
         ok = true;
      }
      else {
         std::cerr << "simulation::setSlotMonth(): invalid month: " << v << "; use [ 1 .. 12 ] or zero" << std::endl;
      }
   }
   return ok;
}

bool Simulation::setSlotYear(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const int v = msg->getInt();
      if ((v >= 1970 && v <= 2099) || v == 0) {
         simYear0 = static_cast<unsigned short>(v);
         ok = true;
      }
      else {
         std::cerr << "simulation::setSlotYear(): invalid year: " << v << "; use [ 1970 .. 2100 ] or zero" << std::endl;
      }
   }
   return ok;
}

bool Simulation::setSlotTerrain(ITerrain* const msg)
{
   if (terrain != nullptr) terrain->unref();
   terrain = msg;
   if (terrain != nullptr) terrain->ref();
   return true;
}

bool Simulation::setSlotAtmosphere(IAtmosphere* const msg)
{
   if (atmosphere != nullptr) atmosphere->unref();
   atmosphere = msg;
   if (atmosphere != nullptr) atmosphere->ref();
   return true;
}

bool Simulation::setSlotFirstWeaponId(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= MIN_WPN_ID && v <= 0xffff) {
         relWpnId = static_cast<unsigned short>(v);
         ok = true;
      }
      else {
         std::cerr << "simulation::setSlotFirstWeaponId(): invalid first weapon ID " << v;
         std::cerr << ", use [ " << MIN_WPN_ID << " ... 65535 ]." << std::endl;
      }
   }
   return ok;
}

bool Simulation::setSlotNumTcThreads(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {

      // Max threads is the number of processors assigned to this
      // process minus one, or minimum of one.
      const unsigned short np = base::Thread::getNumProcessors();
      unsigned short maxT = 1;
      if (np > 1) maxT = np - 1;

      const int v = msg->getInt();
      if (v >= 1 && v <= maxT) {
         reqTcThreads = static_cast<unsigned short>(v);
         ok = true;
      }
      else {
         std::cerr << "simulation::setSlotNumTcThreads(): invalid number of threads: " << v;
         std::cerr << "; number of processors = " << np;
         std::cerr << "; use [ 1 ... " << maxT << " ];" << std::endl;
      }
   }
   return ok;
}

bool Simulation::setSlotNumBgThreads(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {

      // Max threads is the number of processors assigned to this
      // process minus one, or minimum of one.
      const unsigned short np = base::Thread::getNumProcessors();
      unsigned short maxT = 1;
      if (np > 1) maxT = np - 1;

      const int v = msg->getInt();
      if (v >= 1 && v <= maxT) {
         reqBgThreads = static_cast<unsigned short>(v);
         ok = true;
      }
      else {
         std::cerr << "simulation::setSlotNumBgThreads(): invalid number of threads: " << v;
         std::cerr << "; number of processors = " << np;
         std::cerr << "; use [ 1 ... " << maxT << " ];" << std::endl;
      }
   }
   return ok;
}

bool Simulation::setSlotGamingAreaRange(const base::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setMaxRefRange( base::Meters::convertStatic(*msg) );
   }
   return ok;
}

bool Simulation::setSlotEarthModel(const base::EarthModel* const msg)
{
   return setEarthModel(msg);
}

bool Simulation::setSlotEarthModel(const base::String* const msg)
{
   bool ok = false;
   if (msg != nullptr && msg->len() > 0) {
      const base::EarthModel* p = base::EarthModel::getEarthModel(*msg);
      if (p != nullptr) {
         // found the earth model
         ok = setEarthModel(p);
      }
      else {
         // Earth model not found by name
         std::cerr << "simulation::setSlotEarthModel() earth model not found: " << *msg << std::endl;
      }
   }
   else {
      // clear the earth model with a empty string
      ok = setEarthModel(nullptr);
   }
   return ok;
}

bool Simulation::setSlotGamingAreaEarthModel(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setGamingAreaUseEarthModel(msg->getBoolean());
   }
   return ok;
}

std::ostream& Simulation::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

   // ref lat/lon position
   {
      double v = getRefLatitude();
      indent(sout,i+j);
      sout << "latitude: ( LatLon";

      sout << " direction: ";
      if (v >= 0) { sout << "n"; }
      else { sout << "s"; v = -v; }

      sout << " degrees: " << v;
      sout << " )" << std::endl;
   }
   {
      double v = getRefLongitude();

      indent(sout,i+j);
      sout << "longitude: ( LatLon";

      sout << " direction: ";
      if (v >= 0) { sout << "e"; }
      else { sout << "w"; v = -v; }

      sout << " degrees: " << v;
      sout << " )" << std::endl;
   }

    if (players != nullptr) {
        indent(sout,i+j);
        sout << "players: {" << std::endl;
        players->serialize(sout,i+j+4,slotsOnly);
        indent(sout,i+j);
        sout << "}" << std::endl;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//=============================================================================
// SimTcThread: Time critical thread
//=============================================================================
IMPLEMENT_SUBCLASS(SimTcThread,"SimTcThread")
EMPTY_SLOTTABLE(SimTcThread)
EMPTY_COPYDATA(SimTcThread)
EMPTY_DELETEDATA(SimTcThread)
EMPTY_SERIALIZER(SimTcThread)

SimTcThread::SimTcThread(base::Component* const parent, const double priority)
      : base::ThreadSyncTask(parent, priority)
{
   STANDARD_CONSTRUCTOR()

   pl0 = nullptr;
   dt0 = 0.0;
   idx0 = 0;
   n0 = 0;
}

void SimTcThread::start(
         base::PairStream* const pl1,
         const double dt1,
         const unsigned int idx1,
         const unsigned int n1
      )
{
   pl0 = pl1;
   dt0 = dt1;
   idx0 = idx1;
   n0 = n1;

   signalStart();
}

unsigned long SimTcThread::userFunc()
{
   // Make sure we've a player list and our index is valid ...
   if (pl0 != nullptr && idx0 > 0 && idx0 <= n0) {
      // then call the Simulation class' update TC player list functions
      Simulation* sim = static_cast<Simulation*>(getParent());
      sim->updateTcPlayerList(pl0, dt0, idx0, n0);
   }

   return 0;
}

//=============================================================================
// SimBgThread: Background thread
//=============================================================================
IMPLEMENT_SUBCLASS(SimBgThread,"SimBgThread")
EMPTY_SLOTTABLE(SimBgThread)
EMPTY_COPYDATA(SimBgThread)
EMPTY_DELETEDATA(SimBgThread)
EMPTY_SERIALIZER(SimBgThread)

SimBgThread::SimBgThread(base::Component* const parent, const double priority)
      : base::ThreadSyncTask(parent, priority)
{
   STANDARD_CONSTRUCTOR()

   pl0 = nullptr;
   dt0 = 0.0;
   idx0 = 0;
   n0 = 0;
}

void SimBgThread::start(
         base::PairStream* const pl1,
         const double dt1,
         const unsigned int idx1,
         const unsigned int n1
      )
{
   pl0 = pl1;
   dt0 = dt1;
   idx0 = idx1;
   n0 = n1;

   signalStart();
}

unsigned long SimBgThread::userFunc()
{
   // Make sure we've a player list and our index is valid ...
   if (pl0 != nullptr && idx0 > 0 && idx0 <= n0) {
      // then call the Simulation class' update TC player list functions
      Simulation* sim = static_cast<Simulation*>(getParent());
      sim->updateBgPlayerList(pl0, dt0, idx0, n0);
   }

   return 0;
}

}
}

