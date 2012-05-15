//
// Agent
//

#include "openeaagles/ubf/Agent.h"

#include "openeaagles/basic/String.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Station.h"

#include "openeaagles/ubf/Behavior.h"
#include "openeaagles/ubf/State.h"
#include "openeaagles/ubf/Action.h"

namespace Eaagles {
namespace Ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(Agent, "UbfAgent")
EMPTY_SERIALIZER(Agent)
EMPTY_COPYDATA(Agent)

// slot table for this class type
BEGIN_SLOTTABLE(Agent)
   "ownship",                    //  1) The agents ownship (player)
   "behavior"                    //  2) behavior
END_SLOTTABLE(Agent)

//  mapping of slots to handles
BEGIN_SLOT_MAP(Agent)
   ON_SLOT(1, setSlotOwnshipName, Basic::String )
   ON_SLOT(2, setSlotBehavior, Behavior)
END_SLOT_MAP()

Agent::Agent()
{
   STANDARD_CONSTRUCTOR()

   myStation     = 0;
   ownshipName   = 0;

   behavior = 0;
   state = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Agent::deleteData()
{
   // unref arbiter
   if ( behavior!=0 ) { behavior->unref(); behavior = 0; }
   // unref state
   if ( state!=0 )    { state->unref(); state = 0; }
}

//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void Agent::updateData(const LCreal dt)
{
   // update base class stuff first
   BaseClass::updateData(dt);

   controller(dt);
}

void Agent::setOwnshipByName(const char* x)
{
   ownshipName = new Basic::String(x);
}

//------------------------------------------------------------------------------
// Simulation access functions
//------------------------------------------------------------------------------
Simulation::Station* Agent::getStation()
{
   if ( myStation==0 ) {
      Simulation::Station* s = dynamic_cast<Simulation::Station*>(findContainerByType(typeid(Simulation::Station)));
      if (s != 0) {
         myStation = s;
      }
   }
   return myStation;
}

Simulation::Simulation* Agent::getSimulation()
{
   Simulation::Simulation* sim = 0;
   Simulation::Station* s = getStation();
   if (s != 0) {
      sim = s->getSimulation();
   }
   return sim;
}

Simulation::Player* Agent::getOwnship()
{
   Simulation::Player* p = 0;
   Simulation::Simulation* sim = getSimulation();
   if ( sim!=0 && (ownshipName!=0) ) {
      p = sim->findPlayerByName(ownshipName->getString());
   }
   return p;
}

//------------------------------------------------------------------------------
// controller - generic controller
//------------------------------------------------------------------------------
void Agent::controller(const LCreal dt)
{
   Simulation::Player* ownship = getOwnship();
   if ( (ownship!=0) && (state!=0) && (behavior!=0) ) {
      // update ubf state
      state->updateState(ownship);
      // generate an action
      Action* action = behavior->genAction(state, dt);
      //
      executeAction(action);
      //
      action->unref();
   }
}

void Agent::setBehavior(Behavior* const x)
{
   if (x==0)
      return;
   if (behavior!=0)
      behavior->unref();
   behavior = x;
   behavior->ref();
}

void Agent::setState(State* const x)
{
   if (x==0)
      return;
   if (state!=0)
      state->unref();
   state = x;
   state->ref();
}

//------------------------------------------------------------------------------
// set slot functions
//------------------------------------------------------------------------------
bool Agent::setSlotOwnshipName(const Basic::String* const x)
{
   bool ok = false;
   if ( x!=0 ) {
      setOwnshipByName(x->getString());
      ok = true;
   }
   return ok;
}

bool Agent::setSlotBehavior(Behavior* const x)
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
Basic::Object* Agent::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

}
}
