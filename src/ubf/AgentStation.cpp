
#include "openeaagles/ubf/AgentStation.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

#include "openeaagles/ubf/Agent.h"

namespace Eaagles {
namespace Ubf {

IMPLEMENT_SUBCLASS(AgentStation, "UbfAgentStation")
EMPTY_SERIALIZER(AgentStation)
EMPTY_COPYDATA(AgentStation)
EMPTY_DELETEDATA(AgentStation)

// slot table for this class type
BEGIN_SLOTTABLE(AgentStation)
   "agents"                    //  1) list of all agents
END_SLOTTABLE(AgentStation)

//  mapping of slots to handles
BEGIN_SLOT_MAP(AgentStation)
   ON_SLOT(1, setSlotAgents, Basic::PairStream)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AgentStation::AgentStation()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// reset() -- reset all the agents 
//------------------------------------------------------------------------------
void AgentStation::reset()
{
   // setup ownship player pointer
   setOwnshipByName( getOwnshipName()->getString() );

   // reset our baseclass 
   BaseClass::reset();

   // reset() all our agents
   if (agents != 0) {
      Basic::List::Item* item = agents->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         Agent* agent = (Agent*)( pair->object() );
         agent->reset();
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// updateTC() -- update agents time critical task
//------------------------------------------------------------------------------
void AgentStation::updateTC(const LCreal dt)
{
   // update base classes
   BaseClass::updateTC(dt);

   // updateData() on all our agents
   if (agents != 0) {
      Basic::List::Item* item = agents->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         Agent* agent = (Agent*)( pair->object() );
         agent->tcFrame(dt);
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// updateData() -- update agents background task
//------------------------------------------------------------------------------
void AgentStation::updateData(const LCreal dt)
{
   // First update the simulation
   BaseClass::updateData(dt);

   // updateTC() on all our agents
   if (agents != 0) {
      Basic::List::Item* item = agents->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         Agent* agent = (Agent*)( pair->object() );
         agent->updateData(dt);
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

bool AgentStation::setSlotAgents(Basic::PairStream* const al)
{
   // Early out if we're just zeroing the agent list
   if (al == 0) {
      agents = 0;
      return true;
   }
   bool ok = true;

   // First, make sure they are all agents
   {
      Basic::List::Item* item = al->getFirstItem();
      while (item != 0 && ok) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         item = item->getNext();
         Agent* agent = dynamic_cast<Agent*>( pair->object() );
         if (agent == 0) {
            // Item is NOT an agent
            std::cerr << "setSlotAgents: slot: " << *pair->slot() << " is NOT of a Agent type!" << std::endl;
            ok = false;
         }
      }
   }

   // next, set the container pointer
   if (ok) {
      Basic::List::Item* item = al->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         item = item->getNext();
         Agent* agent = (Agent*)( pair->object() );
         agent->container(this);
      }
   }

   // set the agent list pointer
   agents = al;

   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* AgentStation::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

}
}

