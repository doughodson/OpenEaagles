
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
//    terrain        <terrain:Terrain>        ! Terrain elevation database (default: nullptr)
//    atmosphere     <Atmosphere>             ! Atmosphere
//
//
// Environments:
//
//    Current simulation environments include terrain elevation posts, getTerrain(),
//    and atmosphere model, getAtmosphere().
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

    const terrain::Terrain* getTerrain() const;            // Returns the terrain elevation database

    AbstractAtmosphere* getAtmosphere();                   // Returns the atmosphere model
    const AbstractAtmosphere* getAtmosphere() const;       // Returns the atmosphere model (const version)

    // environmental models
    bool setSlotTerrain(terrain::Terrain* const msg);
    bool setSlotAtmosphere(AbstractAtmosphere* const msg);

    virtual void reset() override;

protected:
    terrain::Terrain* getTerrain();                        // Returns the terrain elevation database
    virtual bool shutdownNotification() override;

private:
   void initData();

   AbstractAtmosphere* atmosphere;   // Atmosphere model
   terrain::Terrain*   terrain;      // Terrain model

};

}
}

#endif
