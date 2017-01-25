
#ifndef __oe_models_Simulation_H__
#define __oe_models_Simulation_H__

#include "openeaagles/models/WorldModel.hpp"

namespace oe {
namespace terrain { class Terrain; }
namespace models {
class AbstractAtmosphere;

//------------------------------------------------------------------------------
// Class: Simulation
//
// Description: A world model including enviromental aspects such as
//              terrain elevation and atmospherics
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

    const terrain::Terrain* getTerrain() const;            // returns the terrain elevation database

    AbstractAtmosphere* getAtmosphere();                   // returns the atmosphere model
    const AbstractAtmosphere* getAtmosphere() const;       // returns the atmosphere model (const version)

    // environmental models
    bool setSlotTerrain(terrain::Terrain* const msg);
    bool setSlotAtmosphere(AbstractAtmosphere* const msg);

    virtual void reset() override;

protected:
    terrain::Terrain* getTerrain();                        // returns the terrain elevation database
    virtual bool shutdownNotification() override;

private:
   void initData();

   AbstractAtmosphere* atmosphere;   // atmosphere model
   terrain::Terrain* terrain;        // terrain model
};

}
}

#endif
