//------------------------------------------------------------------------------
// Class: Simulation
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Simulation_H__
#define __Eaagles_Simulation_Simulation_H__

#include "openeaagles/basic/Component.h"
#include "openeaagles/basic/safe_queue.h"

namespace Eaagles {
   namespace Basic { class Distance; class EarthModel; class LatLon; class Pair; class Time; class Terrain; }
   namespace Dafif { class AirportLoader; class NavaidLoader; class WaypointLoader; }

namespace Simulation {
   class DataRecorder;
   class IrAtmosphere;
   class Player;
   class SimBgThread;
   class SimTcThread;
   class Station;

//------------------------------------------------------------------------------
// Class: Simulation
//
// Description: General class to manage the player list, the simulation and
//              executive times, the reference position, and other common
//              simulation support components (ground truths, terrain elevation
//              database, etc)
//
//
// Factory name: Simulation
// Slots --
//    players        <Basic::PairStream>     ! Local player list (Basic::PairStream of Player) (default: 0)
//
//    latitude       <Basic::LatLon>         ! Reference (gaming area) latitude (default: 0.0)
//                   <Basic::Number>         ! Reference (gaming area) latitude (deg)
//
//    longitude      <Basic::LatLon>         ! reference (gaming area) longitude (default 0.0)
//                   <Basic::Number>         ! reference (gaming area) longitude (deg)
//
//    gamingAreaRange <Basic::Distance>      ! Max valid range of the simulation's gaming area or zero for unlimited
//                                           !   default: zero -- unlimited range
//
//    earthModel     <Basic::EarthModel>     ! Earth model for geodetic lat/lon (default is WGS-84)
//                   <Basic::Identifier>     ! Earth model by name (see EarthModel.h)
//
//    gamingAreaUseEarthModel <Basic::Boolean> ! If true, use the 'earthModel' or its WGS-84 default for flat
//                                           ! earth projections between geodetic lat/lon and the gaming
//                                           ! area's NED coordinates.  Otherwise, use a standard spherical
//                                           ! earth with a radius of Nav::ERAD60. (default: false)
//
//    simulationTime <Basic::Time>           ! Initial simulated time since midnight (UTC) (second),
//                                           ! or -1 to use current time of day (default: -1)
//
//    day            <Basic::Number>         ! Initial simulated day of month [ 1 .. 31 ],
//                                           ! or zero to use current day (default: 0)
//
//    month          <Basic::Number>         ! Initial simulated month [ 1 .. 12 ],
//                                           ! or zero to use current month (default: 0)
//
//    year           <Basic::Number>         ! Initial simulated year [ 1970 .. 2100 ],
//                                           ! or zero to use current year (default: 0)
//
//    airportLoader  <Dafif::AirportLoader>  ! Airport database (default: 0)
//    navaidLoader   <Dafif::NavaidLoader>   ! NAVAID database (default: 0)
//    waypointLoader <Dafif::WaypointLoader> ! Waypoint database (default: 0)
//
//    terrain        <Basic::Terrain>        ! Terrain elevation database (default: 0)
//    irAtmosphere   <IrAtmosphere>          ! Atmosphere database for IR algorithms (default: 0)
//
//    firstWeaponId  <Basic::Number>         ! First Released Weapon ID; [ 10001 ... 65535 ] (default: 10001)
//
//    numTcThreads   <Basic::Number>         ! Number of T/C threads to use with the player list
//                                           !   default: 1 -- no additional threads)
//                                           !   range: [ 1 .. (#CPUs-1) ]; minimum of one
//
//    numBgThreads   <Basic::Number>         ! Number of background threads to use with the player list
//                                           !   default: 1 -- no additional threads)
//                                           !   range: [ 1 .. (#CPUs-1) ]; minimum of one
//
//
// The player list
//
//    a) is managed by the background thread
//
//    b) You can request a new player to be added to the player list by
//       using addNewPlayer(), however the player will not be added until
//       updatePlayerList() is run in the background thread.
//
//    c) You can request a player to be removed from the list by setting
//       the player's mode to DELETE_REQUEST.  Again, the player will not
//       actually be removed until updatePlayerList() is run in the
//       background thread.
//
//    d) During reset(), the player list is restored to the initial players
//       that were defined by the 'players' slot, plus any active networked
//       players.
//
//    e) The Player list is sorted by networdID (zero being local) and then by
//       player ID, therefore all local players are located at the beginning of
//       the list.
//
//    f) To ensure a stable player list throughout the time-critical and background
//       frames, the player list is ref() before and unref() after the player list
//       is traversed in both the updateTC() and updateData() functions.
//
//    g) You can find players on the list by Player ID [plus Net ID], findPlayer(),
//       or by name using findPlayerByName().
//
//
// Gaming area reference point:
//
//    The reference latitude and longitude is the center of the simulation's
//    gaming area.  Each player has a position, [ x y z ] NED in meters, from this
//    reference point that is computed using flat earth equations (see Basic::Nav)
//    and using the cosine of this reference latitude.  The NED coordinate system's
//    'down' is perpendicular to this tangent plane.
//
//       double getRefLatitude()
//       double getRefLongitude()
//          Return the reference point (degrees)
//
//       const osg::Matrixd& getWorldMat()
//          Returns the world transformation matrix;
//          earth (NED) <== > Earth Centered, Earth Fixed (ECEF);
//          where the NED XY plane is tangent to and centered at
//          our reference point.
//
//                Matrix M = getWorldMat() = Rz[-lon] * Ry[90+lat]
//                Usage:
//                   Vned  = M * Vecef
//                   Vecef = Vned * M;
//
//
// Cycles, frames and phases:
//
//    Each call to updateTC() by its manager (e.g., a Station class) is one
//    frame, and there are 16 frames, which are numbered 0 to 15, to one cycle.
//
//    Each frame is broken into 4 phases, which are numbered 0 to 3, and are used
//    to process dynamics, transmit sensor queries (e.g., emissions), receive sensor
//    queries, and information or control processing, respectively.  These phases
//    allow for the synchronized flow of data between players within a given frame.
//    The player list is traversed for each phase, therefore the player list is
//    traversed 4 times the core frame rate (e.g., 50 Hz frame rate is 200Hz phase
//    rate).
//
//    Use cycle(), frame() and phase() to get the current values, and use getExecCounter()
//    to get the total number of phases since the start of the exec.
//
//
// Multiple time critical and background threads:
//
//    Use the 'numTcThreads' and 'numBgThreads' slots, you can request the number of
//    threads to traverse the player list.  These threads will each process a subset
//    of players.  The T/C threads rejoin at the end of each phase (see phases above).
//
//    There is overhead with managing threads, so this is effective only with
//    a larger number of players.  The trade off point is dependent on the
//    complexity of the players and the speed of your computer system, so you
//    may need to do a little experimenting on your system.
//
//    These threads will be very CPU bound, so having more threads than CPUs is
//    very ineffective.  And to be nice, ...
//
//       Rule: Can not request more threads than the number of CPUs minus one.
//             This leaves a CPU for the operating system, other applications
//             and our other threads.
//
//
// Time and Date:
//
//    All time and date values are updated only at the start of each frame.
//
//    1) Computer System Time of Day:
//       -- Use getSysTimeOfDay() to get the current computer system's time of
//       day in seconds since midnight, UTC.
//
//    2) Executive time:
//       -- Use getExecTimeSec() to get the time in seconds since the start of
//       the application.
//
//    3) Simulated Time and Date:
//       a) Use getSimTimeOfDay() to get the simulated time of day in seconds
//       since midnight, UTC.
//
//       b) Use getSimTimeValues() to get the two time values ...
//          1) The whole seconds since midnight (00:00:00), January 1, 1970
//          2) The number of microseconds in the current second.
//
//       By default, the simulated time and date are slaved to the computer
//       system's time and date.  Use the slots 'simulationTime', 'day',
//       'month' and 'year' to override the computer system's time and/or
//       date with your own simulated time and/or date.
//
//    4) See "openeaagles/basic/support.h" for additional get time and time
//       conversion functions.
//
//
// Environments:
//
//    Current simulation environments include terrain elevation posts, getTerrain(),
//    IR atmosphere model, getIrAtmosphere(), and DAFIF navigational aids,
//    getNavaids(), getAirports() and getWaypoints().
//
//
// Event IDs:
//
//    There are several player events that need to be identified uniquely
//    within the simulation -- usually because of interoperability networks.
//       a) To identify major simulation events, use getNewEventID()
//       b) To identify weapon release/detonation events, use getNewWeaponEventID()
//       c) To uniquely set player IDs for newly released weapons, use getNewReleasedWeaponID()
//
//
// Shutdown:
//
//    At shutdown, the parent object must send a SHUTDOWN_EVENT event to
//    this object, which will send it to all players, environments and
//    other components.
//
//------------------------------------------------------------------------------
class Simulation : public Basic::Component
{
    DECLARE_SUBCLASS(Simulation,Basic::Component)

public:
   // Minimum released weapon ID
   static const unsigned short MIN_WPN_ID = 10001;

   // Size of the new player queue; ie, the max number
   // of new players accepted per background frame
   static const int MAX_NEW_PLAYERS = 1000;

public:
    Simulation();

    Basic::PairStream* getPlayers();               // Returns the player list; pre-ref()'d
    const Basic::PairStream* getPlayers() const;   // Returns the player list; pre-ref()'d (const version)

    double getRefLatitude() const;                 // Returns the reference latitude (degs)
    double getRefLongitude() const;                // Returns the reference longitude (degs)
    double getSinRefLat() const;                   // Returns the sine of the reference latitude
    double getCosRefLat() const;                   // Returns the cosine of the reference latitude
    double getMaxRefRange() const;                 // Max valid range (meters) of the gaming area or zero if there's no limit.
    const osg::Matrixd& getWorldMat() const;       // World transformation matrix
                                                   //    ECEF <==> NED
                                                   //       where the NED XY plane is tangent at our ref point
                                                   //    matrix = Rz[-lon] * Ry[90+lat]
                                                   //    Usage:
                                                   //       Vned  = M * Vecef
                                                   //       Vecef = Vned * M;

    const Basic::EarthModel* getEarthModel() const; // Returns a pointer to the EarthModel
                                                   // (default: if zero we're using Basic::EarthModel::wgs84)

    bool isGamingAreaUsingEarthModel() const;      // Gaming area using the earth model?

    unsigned int cycle() const;                    // Cycle counter; each cycle represents 16 frames.
    unsigned int frame() const;                    // Frame counter [0 .. 15]; each frame represents a call to our updateTC()
    unsigned int phase() const;                    // Phase counter [0 .. 3]; frames are divide into 4 phases to help
                                                   // the sequencing of data within a  frame.
                                                   //    { 0::dynamics, 1::transmit, 2::receive, 3::process }

    unsigned int getExecCounter() const;           // Executive counter (R/T phases since start)
    double getExecTimeSec() const;                 // Executive time; time since start of simulation (seconds)
    double getSysTimeOfDay() const;                // Computer system's time of day; UTC (seconds since midnight)
    double getSimTimeOfDay() const;                // Simulated run time of day; UTC (seconds since midnight);

    void getSimTimeValues(                         // Simulated time (UTC) values, where ...
       unsigned long* const simSec,                //    The whole seconds since midnight (00:00:00), January 1, 1970
       unsigned long* const simUSec) const;        //    The number of microseconds in the current second.

    // Unique event and weapon IDs
    unsigned short getNewEventID();                // Generates an unique major simulation event ID [1 .. 65535]
    unsigned short getNewWeaponEventID();          // Generates a unique weapon event ID [1 .. 65535]
    unsigned short getNewReleasedWeaponID();       // Generates a unique ID number for released weapons

    const Basic::Terrain* getTerrain() const;      // Returns the terrain elevation database

    IrAtmosphere* getIrAtmosphere();               // Returns the atmosphere database for IR algorithms
    const IrAtmosphere* getIrAtmosphere() const;   // Returns the atmosphere database for IR algorithms (const version)

    Dafif::AirportLoader* getAirports();           // Returns the airport loader
    Dafif::NavaidLoader* getNavaids();             // Returns the NAVAID loader
    Dafif::WaypointLoader* getWaypoints();         // Returns the waypoint loader

    DataRecorder* getDataRecorder();               // Returns the data recorder

    Station* getStation();                         // Returns our Station
    const Station* getStation() const;             // Returns our Station (const version)

    Player* findPlayer(const short id, const int netID = 0);             // Find a player by player (and network) ID
    const Player* findPlayer(const short id, const int netID = 0) const; // Find a player by player (and network) ID (const version)

    Player* findPlayerByName(const char* const playerName);             // Find a player by name
    const Player* findPlayerByName(const char* const playerName) const; // Find a player by name (const version)

    virtual bool addNewPlayer(const char* const playerName, Player* const player); // Add a new player
    virtual bool addNewPlayer(Basic::Pair* const player);                          // Add a new player (pair: name, player)

    virtual bool setInitialSimulationTime(const long time);    // Sets the initial simulated time (sec; or less than zero to slave to UTC)

    virtual bool setAirports(Dafif::AirportLoader* const p);   // Sets the airport loader
    virtual bool setNavaids(Dafif::NavaidLoader* const p);     // Sets the NAVAID loader
    virtual bool setWaypoints(Dafif::WaypointLoader* const p); // Sets the waypoint loader

    void updateTC(const LCreal dt = 0.0) override;
    void updateData(const LCreal dt = 0.0) override;
    void reset() override;

public:
    void updateTcPlayerList(
       Basic::PairStream* const playerList,
       const LCreal dt,
       const unsigned int idx,
       const unsigned int n
    );

    void updateBgPlayerList(
       Basic::PairStream* const playerList,
       const LCreal dt,
       const unsigned int idx,
       const unsigned int n
    );

protected:
    virtual void updatePlayerList();                  // Updates the current player list
    bool setSlotPlayers(Basic::PairStream* const msg);

    Basic::Terrain* getTerrain();                     // Returns the terrain elevation database

    virtual bool setEarthModel(const Basic::EarthModel* const msg); // Sets our earth model
    virtual bool setGamingAreaUseEarthModel(const bool flg);

    virtual bool setRefLatitude(const double v);      // Sets Ref latitude
    virtual bool setRefLongitude(const double v);     // Sets Ref longitude
    virtual bool setMaxRefRange(const double v);      // Sets the max range (meters) of the gaming area or zero if there's no limit.

    virtual void incCycle();                          // Increments the cycle counter
    virtual void setCycle(const unsigned int c);      // Sets the cycle counter
    virtual void setFrame(const unsigned int f);      // Sets the frame counter
    virtual void setPhase(const unsigned int c);      // Sets the phase counter

    virtual void setEventID(unsigned short id);       // Sets the simulation event ID counter
    virtual void setWeaponEventID(unsigned short id); // Sets the weapon ID event counter

    void printTimingStats() override;
    bool shutdownNotification() override;

private:
   void initData();
   Station* getStationImp();

   bool insertPlayerSort(Basic::Pair* const newPlayer, Basic::PairStream* const newList);
   Player* findPlayerPrivate(const short id, const int netID) const;
   Player* findPlayerByNamePrivate(const char* const playerName) const;

   bool setSlotRefLatitude(const Basic::LatLon* const msg);
   bool setSlotRefLatitude(const Basic::Number* const msg);
   bool setSlotRefLongitude(const Basic::LatLon* const msg);
   bool setSlotRefLongitude(const Basic::Number* const msg);
   bool setSlotSimulationTime(const Basic::Time* const msg);
   bool setSlotDay(const Basic::Number* const msg);
   bool setSlotMonth(const Basic::Number* const msg);
   bool setSlotYear(const Basic::Number* const msg);
   bool setSlotTerrain(Basic::Terrain* const msg);
   bool setSlotIrAtmosphere(IrAtmosphere* const msg);
   bool setSlotFirstWeaponId(const Basic::Number* const msg);
   bool setSlotNumTcThreads(const Basic::Number* const msg);
   bool setSlotNumBgThreads(const Basic::Number* const msg);
   bool setSlotGamingAreaRange(const Basic::Distance* const msg);
   bool setSlotEarthModel(const Basic::EarthModel* const msg);
   bool setSlotEarthModel(const Basic::String* const msg);
   bool setSlotGamingAreaEarthModel(const Basic::Number* const msg);

   Basic::safe_ptr<Basic::PairStream> players;     // Main player list (sorted by network and player IDs)
   Basic::safe_ptr<Basic::PairStream> origPlayers; // Original player list

   bool loggedHeadings;          // set true once headings have been added to output file

   // Our Earth Model, or default to using Basic::EarthModel::wgs84 if zero
   const Basic::EarthModel* em;

   double  refLat;               // Reference (center of gaming area) latitude (deg)
   double  refLon;               // Reference (center of gaming area) longitude (deg)
   double  sinRlat;              // Sine of ref latitude
   double  cosRlat;              // Cosine of ref latitude
   double  maxRefRange;          // Max valid range (meters) of the gaming area or zero if there's no limit.
   bool gaUseEmFlg;              // Gaming area using earth model projections
   osg::Matrixd wm;              // World transformation matrix:
                                 //    Local tangent plane (NED) <==> Earth Centered, Earth Fixed (ECEF)
                                 //    Usage:
                                 //       ecef = wm; * earthNED
                                 //       earthNED  = ecef * wm;

   unsigned int  cycleCnt;       // Real-Time Cycle Counter (Cycles consist of Frames)
   unsigned int  frameCnt;       // Real-Time Frame Counter (Frames consist of Phases)
   unsigned int  phaseCnt;       // Real-Time Phase Counter

   double execTime;              // Executive time (seconds since start of application )

   double pcTime;                // Computer system UTC time of day (seconds since midnight)
   unsigned long pcTvSec;        // Computer system UTC time value: Whole seconds since midnight (00:00:00), January 1, 1970
   unsigned long pcTvUSec;       // Computer system UTC time value: Microseconds

   double simTime;               // Simulated UTC time of day (seconds since midnight)
   unsigned long simTvSec;       // Simulated UTC time value: Whole seconds since midnight (00:00:00), January 1, 1970
   unsigned long simTvUSec;      // Simulated UTC time value: Microseconds
   bool simTimeSlaved;           // Simulated time is slaved to the computer time

   long simTime0;                // Initial time of day since midnight (default: -1.0, which slaves to UTC time)
   unsigned short simDay0;       // Initial day of the month [ 1 .. 31 ] (default: 0, which slaves to UTC time)
   unsigned short simMonth0;     // Initial month [ 1 .. 12 ] (default: 0, which slaves to UTC time)
   unsigned short simYear0;      // Initial year YYYY (default: 0, which slaves to UTC time)

   unsigned short eventID;       // Major event ID
   unsigned short eventWpnID;    // Weapon event ID
   unsigned short relWpnId;      // Current released weapon ID

   Basic::safe_queue<Basic::Pair*> newPlayerQueue;   // Queue of new players

   IrAtmosphere*          irAtmosphere; // Atmosphere data for IR algorithms
   Basic::Terrain*        terrain;      // Terrain data
   Dafif::AirportLoader*  airports;     // Airport loader
   Dafif::NavaidLoader*   navaids;      // NAVAID loader
   Dafif::WaypointLoader* waypoints;    // Waypoint loader
   Station*               station;      // The Station that owns us (not ref()'d)

   // Time critical thread pool
   static const unsigned short MAX_TC_THREADS = 32;
   SimTcThread* tcThreads[MAX_TC_THREADS]; // Thread pool; 'numTcThreads' threads
   unsigned int reqTcThreads;              // Requested number of threads
   unsigned int numTcThreads;              // Number of threads in pool; should be (reqTcThreads - 1)
   bool tcThreadsFailed;                   // Failed to create threads.

   // Background thread pool
   static const unsigned short MAX_BG_THREADS = 32;
   SimBgThread* bgThreads[MAX_BG_THREADS]; // Thread pool; 'reqBgThreads' threads
   unsigned int reqBgThreads;              // Requested number of threads
   unsigned int numBgThreads;              // Number of threads in pool; should be (reqBgThreads - 1)
   bool bgThreadsFailed;                   // Failed to create threads.
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
