
#ifndef __oe_models_Simulation_H__
#define __oe_models_Simulation_H__

#include "openeaagles/models/WorldModel.hpp"

namespace oe {
namespace dafif { class AirportLoader; class NavaidLoader; class WaypointLoader; }
namespace terrain { class Terrain; }
namespace models {
class AbstractAtmosphere;

//------------------------------------------------------------------------------
// Class: Simulation
//
// Description: Class to manage the reference position, and other common
//              simulation support components (ground truths, terrain elevation
//              database, etc)
//
// Slots --
//
//    airportLoader  <dafif::AirportLoader>   ! Airport database (default: nullptr)
//    navaidLoader   <dafif::NavaidLoader>    ! NAVAID database (default: nullptr)
//    waypointLoader <dafif::WaypointLoader>  ! Waypoint database (default: nullptr)
//
//    terrain        <terrain:Terrain>        ! Terrain elevation database (default: nullptr)
//    atmosphere     <Atmosphere>             ! Atmosphere
//
//
// Environments:
//
//    Current simulation environments include terrain elevation posts, getTerrain(),
//    atmosphere model, getAtmosphere(), and DAFIF navigational aids,
//    getNavaids(), getAirports() and getWaypoints().
//
// Shutdown:
//
//    At shutdown, the parent object must send a SHUTDOWN_EVENT event to
//    this object, environments and other components.
//
// Factory name: Simulation
//------------------------------------------------------------------------------
class Simulation : public WorldModel
{
    DECLARE_SUBCLASS(Simulation, WorldModel)

public:
    Simulation();

    dafif::AirportLoader* getAirports();           // Returns the airport loader
    dafif::NavaidLoader* getNavaids();             // Returns the NAVAID loader
    dafif::WaypointLoader* getWaypoints();         // Returns the waypoint loader

    const terrain::Terrain* getTerrain() const;    // Returns the terrain elevation database

    AbstractAtmosphere* getAtmosphere();                   // Returns the atmosphere model
    const AbstractAtmosphere* getAtmosphere() const;       // Returns the atmosphere model (const version)

    virtual bool setAirports(dafif::AirportLoader* const p);   // Sets the airport loader
    virtual bool setNavaids(dafif::NavaidLoader* const p);     // Sets the NAVAID loader
    virtual bool setWaypoints(dafif::WaypointLoader* const p); // Sets the waypoint loader

    // environmental models
    bool setSlotTerrain(terrain::Terrain* const msg);
    bool setSlotAtmosphere(AbstractAtmosphere* const msg);

    virtual void updateData(const double dt = 0.0) override;
    virtual void reset() override;

protected:
    terrain::Terrain* getTerrain();                           // Returns the terrain elevation database
    virtual bool shutdownNotification() override;

private:
   void initData();

   dafif::AirportLoader*  airports;     // Airport loader
   dafif::NavaidLoader*   navaids;      // NAVAID loader
   dafif::WaypointLoader* waypoints;    // Waypoint loader

   AbstractAtmosphere* atmosphere;   // Atmosphere model
   terrain::Terrain*   terrain;      // Terrain model

};

}
}

#endif
