
#include "openeaagles/models/Simulation.hpp"

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
   "terrain",        //  1) Terrain elevation database
   "atmosphere",     //  2) Atmospheric model
END_SLOTTABLE(Simulation)

BEGIN_SLOT_MAP(Simulation)
    ON_SLOT( 1, setSlotTerrain,      terrain::Terrain)
    ON_SLOT( 2, setSlotAtmosphere,   AbstractAtmosphere)
END_SLOT_MAP()

EMPTY_SERIALIZER(Simulation)

Simulation::Simulation()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Simulation::initData()
{
   terrain = nullptr;
   atmosphere = nullptr;
}

void Simulation::copyData(const Simulation& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

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

// returns the terrain elevation database
const terrain::Terrain* Simulation::getTerrain() const
{
   return terrain;
}

terrain::Terrain* Simulation::getTerrain()
{
   return terrain;
}

// returns the atmosphere model
AbstractAtmosphere* Simulation::getAtmosphere()
{
   return atmosphere;
}

// returns the atmospheric model
const AbstractAtmosphere* Simulation::getAtmosphere() const
{
   return atmosphere;
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

