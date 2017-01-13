
#include "openeaagles/models/Simulation.hpp"

#include "openeaagles/dafif/AirportLoader.hpp"
#include "openeaagles/dafif/NavaidLoader.hpp"
#include "openeaagles/dafif/WaypointLoader.hpp"

#include "openeaagles/models/environment/AbstractAtmosphere.hpp"
#include "openeaagles/terrain/Terrain.hpp"

#include "openeaagles/base/EarthModel.hpp"
#include "openeaagles/base/Identifier.hpp"
#include "openeaagles/base/LatLon.hpp"
#include "openeaagles/base/Nav.hpp"

#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Distances.hpp"
#include "openeaagles/base/units/Times.hpp"
#include "openeaagles/base/Statistic.hpp"
#include "openeaagles/base/util/system.hpp"

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(Simulation, "Simulation")

BEGIN_SLOTTABLE(Simulation)
   "airportLoader",  //  1) Airport database
   "navaidLoader",   //  2) NAVAID database
   "waypointLoader", //  3) Waypoint database
   "terrain",        //  4) Terrain elevation database
   "atmosphere",     //  5) Atmospheric model
END_SLOTTABLE(Simulation)

BEGIN_SLOT_MAP(Simulation)
    ON_SLOT( 1, setAirports,         dafif::AirportLoader)
    ON_SLOT( 2, setNavaids,          dafif::NavaidLoader)
    ON_SLOT( 3, setWaypoints,        dafif::WaypointLoader)
    ON_SLOT( 4, setSlotTerrain,      terrain::Terrain)
    ON_SLOT( 5, setSlotAtmosphere,   AbstractAtmosphere)
END_SLOT_MAP()

EMPTY_SERIALIZER(Simulation)

Simulation::Simulation()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Simulation::initData()
{
   airports = nullptr;
   navaids = nullptr;
   waypoints = nullptr;
   terrain = nullptr;
   atmosphere = nullptr;
}

void Simulation::copyData(const Simulation& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   const dafif::AirportLoader* apLoader = org.airports;
   setAirports( const_cast<dafif::AirportLoader*>(static_cast<const dafif::AirportLoader*>(apLoader)) );

   const dafif::NavaidLoader* naLoader = org.navaids;
   setNavaids( const_cast<dafif::NavaidLoader*>(static_cast<const dafif::NavaidLoader*>(naLoader)) );

   const dafif::WaypointLoader* wpLoader = org.waypoints;
   setWaypoints( const_cast<dafif::WaypointLoader*>(static_cast<const dafif::WaypointLoader*>(wpLoader)) );

   if (org.terrain != nullptr) {
      terrain::Terrain* copy = org.terrain->clone();
      setSlotTerrain( copy );
      copy->unref();
   }
   else {
      setSlotTerrain(nullptr);
   }

   if (org.atmosphere != nullptr) {
      AbstractAtmosphere* copy = org.atmosphere->clone();
      setSlotAtmosphere( copy );
      copy->unref();
   }
   else {
      setSlotAtmosphere(nullptr);
   }
}

void Simulation::deleteData()
{
   setSlotAtmosphere( nullptr );
   setSlotTerrain( nullptr );
   setAirports( nullptr );
   setNavaids( nullptr );
   setWaypoints( nullptr );
}

void Simulation::reset()
{
   BaseClass::reset();

   // ---
   // First time reset of terrain database will load the data
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
}

bool Simulation::shutdownNotification()
{
   // ---
   // Shutdown our baseclass, which will notify our components
   // ---
   BaseClass::shutdownNotification();

   // ---
   // Tell the environments ...
   // ---
   if (atmosphere != nullptr) atmosphere->event(SHUTDOWN_EVENT);
   if (terrain != nullptr) terrain->event(SHUTDOWN_EVENT);

   return true;
}

void Simulation::updateData(const double dt)
{
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

   BaseClass::updateData(dt);
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

// Returns the terrain elevation database
const terrain::Terrain* Simulation::getTerrain() const
{
   return terrain;
}

terrain::Terrain* Simulation::getTerrain()
{
   return terrain;
}

// Returns the atmosphere model
AbstractAtmosphere* Simulation::getAtmosphere()
{
   return atmosphere;
}

// Returns the atmospheric model
const AbstractAtmosphere* Simulation::getAtmosphere() const
{
   return atmosphere;
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

bool Simulation::setSlotTerrain(terrain::Terrain* const msg)
{
   if (terrain != nullptr) terrain->unref();
   terrain = msg;
   if (terrain != nullptr) terrain->ref();
   return true;
}

bool Simulation::setSlotAtmosphere(AbstractAtmosphere* const msg)
{
   if (atmosphere != nullptr) atmosphere->unref();
   atmosphere = msg;
   if (atmosphere != nullptr) atmosphere->ref();
   return true;
}

}
}

