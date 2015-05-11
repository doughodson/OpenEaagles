
#include "openeaagles/simulation/SimAgent.h"

#include "openeaagles/basic/ubf/Action.h"
#include "openeaagles/basic/ubf/Behavior.h"
#include "openeaagles/basic/ubf/State.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/String.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Station.h"

namespace Eaagles {
namespace Simulation {

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
   ON_SLOT(1, setSlotActorPlayerName, Basic::String )
   ON_SLOT(2, setSlotActorComponentName, Basic::String )
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

Station* SimAgent::getStation()
{
   if ( myStation==nullptr ) {
      Station* s = dynamic_cast<Station*>(findContainerByType(typeid(Station)));
      if (s != nullptr) {
         myStation = s;
      }
   }
   return myStation;
}

Simulation* SimAgent::getSimulation()
{
   Simulation* sim = nullptr;
   Station* s = getStation();
   if (s != nullptr) {
      sim = s->getSimulation();
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
            Basic::Component* player = sim->findPlayerByName(actorPlayerName->getString());
            if (actorComponentName == nullptr) {
               // no player component specified, so the player is the actor
               setActor(player);
            }
            else if (player != nullptr) {
               Basic::Pair* pair = player->findByName(actorComponentName->getString());
               if (pair != nullptr) {
                  setActor(dynamic_cast<Basic::Component*>( pair->object() ));
               }
            }
         }
      }
   }
}

void SimAgent::setActorPlayerByName(const char* x)
{
   actorPlayerName = new Basic::String(x);
}
void SimAgent::setActorComponentByName(const char* x)
{
   actorComponentName = new Basic::String(x);
}
//------------------------------------------------------------------------------
// set slot functions
//------------------------------------------------------------------------------

bool SimAgent::setSlotActorPlayerName(const Basic::String* const x)
{
   bool ok = false;
   if ( x != nullptr ) {
      setActorPlayerByName(x->getString());
      ok = true;
   }
   return ok;
}

bool SimAgent::setSlotActorComponentName(const Basic::String* const x)
{
   bool ok = false;
   if ( x != nullptr ) {
      setActorComponentByName(x->getString());
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* SimAgent::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

} // End Simulation namespace
} // End Eaagles namespace


