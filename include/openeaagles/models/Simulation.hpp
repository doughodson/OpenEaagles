
#ifndef __oe_models_Simulation_H__
#define __oe_models_Simulation_H__

#include "openeaagles/simulation/ISimulation.hpp"

namespace oe {

namespace models { class IAtmosphere; }
namespace terrain { class ITerrain; }
namespace dafif { class AirportLoader; class NavaidLoader; class WaypointLoader; }

namespace models {

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
//
//    airportLoader  <dafif::AirportLoader>   ! Airport database (default: nullptr)
//    navaidLoader   <dafif::NavaidLoader>    ! NAVAID database (default: nullptr)
//    waypointLoader <dafif::WaypointLoader>  ! Waypoint database (default: nullptr)
//
//    terrain        <terrain::ITerrain>      ! Terrain elevation database (default: nullptr)
//    atmosphere     <IAtmosphere>            ! Atmosphere
//
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
//    reference point that is computed using flat earth equations (see base::Nav)
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
//    4) See "openeaagles/base/util/system.hpp" for additional time related functions.
//
//
// Environments:
//
//    Current simulation environments include terrain elevation posts, getTerrain(),
//    atmosphere model, getAtmosphere(), and DAFIF navigational aids,
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
class Simulation : public simulation::ISimulation
{
    DECLARE_SUBCLASS(Simulation, simulation::ISimulation)

public:
    Simulation();


    const terrain::ITerrain* getTerrain() const;   // Returns the terrain elevation database

    IAtmosphere* getAtmosphere();                  // Returns the atmosphere model
    const IAtmosphere* getAtmosphere() const;      // Returns the atmosphere model (const version)

    dafif::AirportLoader* getAirports();           // Returns the airport loader
    dafif::NavaidLoader* getNavaids();             // Returns the NAVAID loader
    dafif::WaypointLoader* getWaypoints();         // Returns the waypoint loader

    virtual bool setAirports(dafif::AirportLoader* const p);   // Sets the airport loader
    virtual bool setNavaids(dafif::NavaidLoader* const p);     // Sets the NAVAID loader
    virtual bool setWaypoints(dafif::WaypointLoader* const p); // Sets the waypoint loader

    virtual void updateData(const double dt = 0.0) override;
    virtual void reset() override;

protected:

    terrain::ITerrain* getTerrain();                  // Returns the terrain elevation database

    virtual bool shutdownNotification() override;

private:

   // environmental models
   bool setSlotTerrain(terrain::ITerrain* const msg);
   bool setSlotAtmosphere(IAtmosphere* const msg);

   IAtmosphere*           atmosphere;   // Atmosphere model
   terrain::ITerrain*     terrain;      // Terrain model
   dafif::AirportLoader*  airports;     // Airport loader
   dafif::NavaidLoader*   navaids;      // NAVAID loader
   dafif::WaypointLoader* waypoints;    // Waypoint loader
};

}
}

#endif
