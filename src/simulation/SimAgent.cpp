//
// Agent
//
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

//==============================================================================
// Class: SimAgent
// Description: A UbfAgent that manages a component (the "actor") with a behavior (either a player, or a player's component)
//==============================================================================

IMPLEMENT_SUBCLASS(SimAgent, "SimAgent")
EMPTY_SERIALIZER(SimAgent)
EMPTY_COPYDATA(SimAgent)

// slot table for this class type
BEGIN_SLOTTABLE(SimAgent)
   "actorPlayerName",                 //  1) The agent's actor - playerName
   "actorComponentName"              //  1) The agent's actor - componentName
END_SLOTTABLE(SimAgent)

//  mapping of slots to handles
BEGIN_SLOT_MAP(SimAgent)
   ON_SLOT(1, setSlotActorPlayerName, Basic::String )
   ON_SLOT(2, setSlotActorComponentName, Basic::String )
END_SLOT_MAP()

SimAgent::SimAgent()
{
   STANDARD_CONSTRUCTOR()
   actorPlayerName   = 0;
   actorComponentName   = 0;
   myStation     = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SimAgent::deleteData()
{
   actorPlayerName->unref();
   actorComponentName->unref();
}

Station* SimAgent::getStation()
{
   if ( myStation==0 ) {
      Station* s = dynamic_cast<Station*>(findContainerByType(typeid(Station)));
      if (s != 0) {
         myStation = s;
      }
   }
   return myStation;
}

Simulation* SimAgent::getSimulation()
{
   Simulation* sim = 0;
   Station* s = getStation();
   if (s != 0) {
      sim = s->getSimulation();
   }
   return sim;
}

// finds our actor during reset() processing
void SimAgent::initActor()
{
   if (getActor() == 0 ) {
      if (actorPlayerName == 0) {
         // not correctly specified as a SimAgent, try baseClass ?
         BaseClass::initActor();
      }
      else {
         Simulation* sim = getSimulation();
         if ( sim!=0 ) {
            Basic::Component* player = sim->findPlayerByName(actorPlayerName->getString());
            if (actorComponentName == 0) {
               // no player component specified, so the player is the actor
               setActor(player);
            }
            else if (player!=0) {
               Basic::Pair* pair = player->findByName(actorComponentName->getString());
               if (pair != 0) {
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
   if ( x!=0 ) {
      setActorPlayerByName(x->getString());
      ok = true;
   }
   return ok;
}

bool SimAgent::setSlotActorComponentName(const Basic::String* const x)
{
   bool ok = false;
   if ( x!=0 ) {
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
