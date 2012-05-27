//
// Agent
//
#include "UbfAgent.h"
#include "UbfAction.h"
#include "UbfBehavior.h"
#include "UbfState.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/String.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Station.h"

namespace Eaagles {
namespace Basic {
namespace Ubf {

//==============================================================================
// Class: NewUbf::UbfAgent
// Description: A UbfAgent that manages a component (the "actor") with a behavior (either a player, or a player's component)
//==============================================================================

IMPLEMENT_SUBCLASS(UbfAgent, "UbfAgent")
EMPTY_SERIALIZER(UbfAgent)
EMPTY_COPYDATA(UbfAgent)

// slot table for this class type
BEGIN_SLOTTABLE(UbfAgent)
   "actorPlayerName",                 //  1) The agent's actor - playerName
   "actorComponentName",              //  1) The agent's actor - componentName
   "state",                      //  2) The agent's state object
   "behavior"                    //  3) behavior
END_SLOTTABLE(UbfAgent)

//  mapping of slots to handles
BEGIN_SLOT_MAP(UbfAgent)
   ON_SLOT(1, setSlotActorPlayerName, Basic::String )
   ON_SLOT(2, setSlotActorComponentName, Basic::String )
   ON_SLOT(3, setSlotState,   UbfState)
   ON_SLOT(4, setSlotBehavior, UbfBehavior)
END_SLOT_MAP()

UbfAgent::UbfAgent()
{
   STANDARD_CONSTRUCTOR()
   actorPlayerName   = 0;
   actorComponentName   = 0;
   myActor = 0;
   behavior     = 0;
   state     = 0;
   myStation     = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void UbfAgent::deleteData()
{
   // unref arbiter
   if ( behavior!=0 ) { behavior->unref(); behavior = 0; }
   // unref state
   if ( state!=0 )    { state->unref(); state = 0; }

   actorPlayerName->unref();
   actorComponentName->unref();
   myActor = 0;
}

void UbfAgent::reset()
{
   // behavior is not a full component - it knows that agent is its container, but agent doesn't know that it has behavior components
   if (behavior != 0) {
      behavior->reset();
   }
   
   myActor=0;

   // although state is not explicitly initialized as component, the set state method sets up the component relationship
   // since state is a component, it will get the reset() this way (via the component i/f)
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void UbfAgent::updateData(const LCreal dt)
{
   // update base class stuff first
   BaseClass::updateData(dt);
   controller(dt);
}

void UbfAgent::controller(const LCreal dt)
{
   Basic::Component* actor = getActor();
   if ( (actor!=0) && (getState()!=0) && (getBehavior()!=0) ) {
      
      // update ubf state
      getState()->updateState(actor);

      // generate an action, but allow possibility of no action returned
      UbfAction* action = getBehavior()->genAction(state, dt);
      if (action) {
         action->execute(actor);
         action->unref();
      }
   }
}


void UbfAgent::setBehavior(UbfBehavior* const x)
{
   if (x==0)
      return;
   if (behavior!=0)
      behavior->unref();
   behavior = x;
   behavior->ref();
   behavior->container(this);
}

void UbfAgent::setState(UbfState* const x)
{
   if (x==0)
      return;
   if (state!=0)
      state->unref();
   state = x;
   state->ref();
   state->container(this);
   Basic::Pair* p = new Basic::Pair("", state);
   addComponent(p);
   p->unref();
}

Simulation::Station* UbfAgent::getStation()
{
   if ( myStation==0 ) {
      Simulation::Station* s = dynamic_cast<Simulation::Station*>(findContainerByType(typeid(Simulation::Station)));
      if (s != 0) {
         myStation = s;
      }
   }
   return myStation;
}

Simulation::Simulation* UbfAgent::getSimulation()
{
   Simulation::Simulation* sim = 0;
   Simulation::Station* s = getStation();
   if (s != 0) {
      sim = s->getSimulation();
   }
   return sim;
}

Basic::Component* UbfAgent::getActor()
{
   if (myActor==0) {
      if ( actorPlayerName == 0 && actorComponentName == 0) {
         // our actor is our container
         if (container() != 0) {
            myActor = container();
         }
      }
      else {
         Simulation::Simulation* sim = getSimulation();
         if ( sim!=0 ) {
            Basic::Component* player = sim->findPlayerByName(actorPlayerName->getString());
            if (actorComponentName == 0) {
               // no player component specified, so the player is the actor
               myActor = player;
            }
            else if (player!=0) {
               Basic::Pair* pair = player->findByName(actorComponentName->getString());
               if (pair != 0) {
                  myActor = dynamic_cast<Basic::Component*>( pair->object() );
               }
            }
         }
      }
   }
   return myActor;
}

void UbfAgent::setActorPlayerByName(const char* x)
{
   actorPlayerName = new Basic::String(x);
}
void UbfAgent::setActorComponentByName(const char* x)
{
   actorComponentName = new Basic::String(x);
}
//------------------------------------------------------------------------------
// set slot functions
//------------------------------------------------------------------------------

bool UbfAgent::setSlotActorPlayerName(const Basic::String* const x)
{
   bool ok = false;
   if ( x!=0 ) {
      setActorPlayerByName(x->getString());
      ok = true;
   }
   return ok;
}

bool UbfAgent::setSlotActorComponentName(const Basic::String* const x)
{
   bool ok = false;
   if ( x!=0 ) {
      setActorComponentByName(x->getString());
      ok = true;
   }
   return ok;
}

// Sets the state object for this agent
bool UbfAgent::setSlotState(UbfState* const state)
{
   bool ok = false;
   if (state != 0) {
      setState(state);
      ok = true;
   }
   return ok;
}

bool UbfAgent::setSlotBehavior(UbfBehavior* const x)
{
   bool ok = false;
   if ( x!=0 ) {
      setBehavior(x);
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* UbfAgent::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}



//==============================================================================
// Class: NewUbf::AgentTC
// Description: An Agent that manages a component (the "actor") with a behavior, using TC thread to perform its activity (instead of BG thread)
//==============================================================================

IMPLEMENT_SUBCLASS(AgentTC, "AgentTC")
EMPTY_SLOTTABLE(AgentTC)
EMPTY_CONSTRUCTOR(AgentTC)
EMPTY_SERIALIZER(AgentTC)
EMPTY_COPYDATA(AgentTC)
EMPTY_DELETEDATA(AgentTC)

//------------------------------------------------------------------------------
// updateTC() - implement agent's controller logic in TC thread
//------------------------------------------------------------------------------
void AgentTC::updateTC(const LCreal dt)
{
   // update base class stuff first
   BaseClass::updateTC(dt);
   // since we are a station component, at this point we are always finished with the sim's update TC, which means we have just finished phase 3
   // and are soon to start the new phase 0
   controller(dt);
}

//------------------------------------------------------------------------------
// updateData() - avoid baseclass agent's controller logic in BG thread
//------------------------------------------------------------------------------
void AgentTC::updateData(const LCreal dt)
{
}

} // End Ubf namespace
} // End Basic namespace
} // End Eaagles namespace
