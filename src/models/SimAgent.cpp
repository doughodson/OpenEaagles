
#include "openeaagles/models/SimAgent.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/Simulation.hpp"

#include "openeaagles/simulation/Station.hpp"

#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/String.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: SimAgent
//
// Description: An Agent that manages a component (the "actor") with a behavior
// (either a player, or a player's component)
//------------------------------------------------------------------------------

IMPLEMENT_SUBCLASS(SimAgent, "SimAgent")
EMPTY_SERIALIZER(SimAgent)
EMPTY_COPYDATA(SimAgent)

// slot table for this class type
BEGIN_SLOTTABLE(SimAgent)
   "actorPlayerName",                 //  1) The agent's actor - playerName
   "actorComponentName"               //  1) The agent's actor - componentName
END_SLOTTABLE(SimAgent)

//  mapping of slots to handles
BEGIN_SLOT_MAP(SimAgent)
   ON_SLOT(1, setSlotActorPlayerName, base::String )
   ON_SLOT(2, setSlotActorComponentName, base::String )
END_SLOT_MAP()

SimAgent::SimAgent()
{
   STANDARD_CONSTRUCTOR()
   actorPlayerName = nullptr;
   actorComponentName = nullptr;
   myStation = nullptr;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SimAgent::deleteData()
{
   if (actorPlayerName!=nullptr) {
      actorPlayerName->unref();
      actorPlayerName=nullptr;
   }
   if (actorComponentName!=nullptr) {
      actorComponentName->unref();
      actorComponentName=nullptr;
   }
}

simulation::Station* SimAgent::getStation()
{
   if ( myStation==nullptr ) {
      const auto s = dynamic_cast<simulation::Station*>(findContainerByType(typeid(simulation::Station)));
      if (s != nullptr) {
         myStation = s;
      }
   }
   return myStation;
}

Simulation* SimAgent::getSimulation()
{
   Simulation* sim = nullptr;
   simulation::Station* s = getStation();
   if (s != nullptr) {
      sim = dynamic_cast<Simulation*>(s->getSimulation());
   }
   return sim;
}

// finds our actor during reset() processing
void SimAgent::initActor()
{
   if (getActor() == nullptr ) {
      if (actorPlayerName == nullptr) {
         // not correctly specified as a SimAgent, try baseClass ?
         BaseClass::initActor();
      }
      else {
         Simulation* sim = getSimulation();
         if ( sim != nullptr ) {
            base::Component* player = sim->findPlayerByName(actorPlayerName->getString());
            if (actorComponentName == nullptr) {
               // no player component specified, so the player is the actor
               setActor(player);
            }
            else if (player != nullptr) {
               base::Pair* pair = player->findByName(actorComponentName->getString());
               if (pair != nullptr) {
                  setActor(dynamic_cast<base::Component*>( pair->object() ));
               }
            }
         }
      }
   }
}

void SimAgent::setActorPlayerByName(const char* x)
{
   actorPlayerName = new base::String(x);
}
void SimAgent::setActorComponentByName(const char* x)
{
   actorComponentName = new base::String(x);
}
//------------------------------------------------------------------------------
// set slot functions
//------------------------------------------------------------------------------

bool SimAgent::setSlotActorPlayerName(const base::String* const x)
{
   bool ok = false;
   if ( x != nullptr ) {
      setActorPlayerByName(x->getString());
      ok = true;
   }
   return ok;
}

bool SimAgent::setSlotActorComponentName(const base::String* const x)
{
   bool ok = false;
   if ( x != nullptr ) {
      setActorComponentByName(x->getString());
      ok = true;
   }
   return ok;
}

}
}


