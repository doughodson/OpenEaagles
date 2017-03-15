
#include "openeaagles/base/StateMachine.hpp"

#include "openeaagles/base/Identifier.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(StateMachine, "AbstractStateMachine")

BEGIN_SLOTTABLE(StateMachine)
    "stateMachines",  // 1: List of state machine objects (StateMachine class)
END_SLOTTABLE(StateMachine)

BEGIN_SLOT_MAP(StateMachine)
    ON_SLOT(1, setSlotStateMachines, PairStream)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(StateMachine)

    ON_EVENT_OBJ(ON_ENTRY, onEntry, Object)     // always check w/Object first
    ON_EVENT(ON_ENTRY, onEntry)

    ON_EVENT_OBJ(ON_RETURN, onReturn, Object)   // always check w/Object first
    ON_EVENT(ON_RETURN, onReturn)

    ON_EVENT(ON_EXIT, onExit)

   // If our current state is controlled by another StateMachine then
   // see if this StateMachine will handled this event.
   if (stMach != nullptr && !_used) _used = stMach->event(_event,_obj);
END_EVENT_HANDLER()

StateMachine::StateMachine()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void StateMachine::initData()
{
   stateStack.fill(INVALID_STATE);
   substateStack.fill(INVALID_STATE);
}

void StateMachine::copyData(const StateMachine& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setSlotStateMachines(org.stMachList);
}

void StateMachine::deleteData()
{
   setStMach(nullptr,CURR_STATE);
   setSlotStateMachines(nullptr);
}

// -----------------------------------------------------------------
// reset() -- Resets the state machine
// -----------------------------------------------------------------
void StateMachine::reset()
{
   BaseClass::reset();

   // Reset our state machine list
   if (stMachList != nullptr) {
      List::Item* item = stMachList->getFirstItem();
      while (item != nullptr) {
         const auto p = static_cast<Pair*>(item->getValue());
         const auto q = static_cast<Component*>(p->object());
         q->reset();
         item = item->getNext();
      }
   }

   // Goto our RESET state
   state = INVALID_STATE;
   substate = INVALID_STATE;
   stMach = nullptr;
   stMachName = nullptr;
   arg = nullptr;

   goTo(INIT_STATE);
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void StateMachine::updateData(const double dt)
{
   if (stMach != nullptr) stMach->updateData(dt);
   BaseClass::updateData(dt);
}

// -----------------------------------------------------------------
// updateTC() -- one iteration step of the state machine
// -----------------------------------------------------------------
void StateMachine::updateTC(const double dt)
{
   // Step state machine
   step(dt);

   // BaseClass
   BaseClass::updateTC(dt);
}

// -----------------------------------------------------------------
// One iteration step of the state machine
// -----------------------------------------------------------------
void StateMachine::step(const double dt)
{
   // ---
   // 1) If requested, step into a new state
   // ---
   if (nMode == NEW_STATE) {
      pState = state;
      state = nState;
      arg = nArg;
      mode = NEW_STATE;
   }
   else if (nMode == RTN_STATE) {
      pState = state;
      state = nState;
      arg = nArg;
      mode = RTN_STATE;
   }
   else { // (nMode == HOLD_STATE)
      pState = state;
      arg = nullptr;
      mode = HOLD_STATE;
   }
   nState = INVALID_STATE;
   nArg = nullptr;
   nMode = HOLD_STATE;

   // always step the substate
   pSubstate = substate;
   substate = nSubstate;


   // ---
   // 2) common pre-state function
   //
   preStateProc(dt);


   // ---
   // 3) process the state table, which calls state specific function
   //    or sets this state's child state machine, 'stMach'.
   // ---
   {
      stateTable(state, CURR_STATE, dt);

      // Send state exit events to the previous state's state machine
      if (pStMach != nullptr) {
         pStMach->event(ON_EXIT);
         pStMach = nullptr;
      }

      if (stMach != nullptr) {
         if (mode == NEW_STATE) {
            // Send state entry events
            stMach->event(ON_ENTRY);
         }
         else if (mode == RTN_STATE) {
            // Send returning events
            stMach->event(ON_RETURN);
         }

         // Step this state's child state machine
         stMach->tcFrame(dt);
      }
   }

   // ---
   // 4) common post-state function
   // ---
   postStateProc(dt);
}

// -----------------------------------------------------------------
// Transition functions -- these control movement between our states
// -----------------------------------------------------------------

bool StateMachine::next(Object* const arg)
{
   bool ok = false;
   unsigned short newState = stateTable(state, FIND_NEXT_STATE);
   if (newState != INVALID_STATE) {
      nState = newState;
      nSubstate = INIT_STATE;
      nArg = arg;
      nMode = NEW_STATE;
      ok = true;
   }
   return ok;
}

bool StateMachine::goTo(const unsigned short newState, Object* const arg)
{
   bool ok = false;
   unsigned short test = stateTable(newState, TEST_STATE);
   if (test != INVALID_STATE) {
      nState = newState;
      nSubstate = INIT_STATE;
      nArg = arg;
      nMode = NEW_STATE;
      ok = true;
   }
   return ok;
}

bool StateMachine::call(const unsigned short newState, Object* const arg)
{
   bool ok = false;
   if (sp > 0) {
      unsigned short test = stateTable(newState, TEST_STATE);
      if (test != INVALID_STATE) {
         stateStack[--sp] = state;
         substateStack[sp] = substate;
         nState = newState;
         nSubstate = INIT_STATE;
         nArg = arg;
         nMode = NEW_STATE;
         ok = true;
      }
   }
   return ok;
}

bool StateMachine::rtn(Object* const arg)
{
   bool ok = false;
   if (sp < STACK_SIZE) {
      nSubstate = substateStack[sp];
      nState = stateStack[sp++];
      nArg = arg;
      nMode = RTN_STATE;
      ok = true;
   }
   return ok;
}


// -----------------------------------------------------------------
// Substate Transition functions -- these control movement between our substates
// -----------------------------------------------------------------

bool StateMachine::nextSubstate()
{
   nSubstate = (substate+1);
   return true;
}

bool StateMachine::goToSubstate(const unsigned short newSubstate)
{
   nSubstate = newSubstate;
   return true;
}


// -----------------------------------------------------------------
// Parent State Machine's transition functions -- these control movement
// between our parent state machine's states.
// -----------------------------------------------------------------

bool StateMachine::nextState(Object* const arg)
{
   bool ok = false;
   const auto parent = dynamic_cast<StateMachine*>( container() );
   if (parent != nullptr) {
      ok = parent->next(arg);
   }
   return ok;
}

bool StateMachine::goToState(const unsigned short newState, Object* const arg)
{
   bool ok = false;
   const auto parent = dynamic_cast<StateMachine*>( container() );
   if (parent != nullptr) {
      ok = parent->goTo(newState,arg);
   }
   return ok;
}

bool StateMachine::callState(const unsigned short newState, Object* const arg)
{
   bool ok = false;
   const auto parent = dynamic_cast<StateMachine*>( container() );
   if (parent != nullptr && sp > 0) {
      ok = parent->call(newState,arg);
      if (ok) {
         stateStack[--sp] = state;
         substateStack[sp] = substate;
      }
   }
   return ok;
}

bool StateMachine::rtnState(Object* const arg)
{
   bool ok = false;
   const auto parent = dynamic_cast<StateMachine*>( container() );
   if (parent != nullptr) {
      ok = parent->rtn(arg);
   }
   return ok;
}


// -----------------------------------------------------------------
// Default event handlers
// -----------------------------------------------------------------
bool StateMachine::onEntry(Object* const msg)
{
   nState = INIT_STATE;
   nArg = msg;
   nMode = NEW_STATE;
   nSubstate = INIT_STATE;
   return true;
}

bool StateMachine::onExit()
{
   state = INVALID_STATE;
   stMach = nullptr;
   mode = HOLD_STATE;
   arg = nullptr;

   pState = INVALID_STATE;
   pStMach = nullptr;

   return true;
}

bool StateMachine::onReturn(Object* const msg)
{
   // Try to return to our calling state
   bool ok = rtn(msg);

   if (!ok) {
      // or just goto the reset state
      nState = INIT_STATE;
      nArg = msg;
      nMode = RTN_STATE;
      nSubstate = INIT_STATE;
   }
   return true;
}

// -----------------------------------------------------------------
// Default procedure that's called by stepState just before
// the state specific procedure
// -----------------------------------------------------------------
void StateMachine::preStateProc(const double)
{
}

// -----------------------------------------------------------------
// Default procedure that's called by stepState just after
// the state specific procedure
// -----------------------------------------------------------------
void StateMachine::postStateProc(const double)
{
}

//------------------------------------------------------------------------------
// findStMachByName(), findStMachByType() -- find a state machine
//------------------------------------------------------------------------------
Pair* StateMachine::findStMachByName(const char* const name)
{
    Pair* p = nullptr;
    if (stMachList != nullptr && name != nullptr) p = stMachList->findByName(name);
    return p;
}


Pair* StateMachine::findStMachByType(const std::type_info& type)
{
    Pair* p = nullptr;
    if (stMachList != nullptr) p = stMachList->findByType(type);
    return p;
}

// -----------------------------------------------------------------
// Sets the state machine 'name' or zero to clear.  Return true
// if successful. If the 'code' is CHK_CUR_STATE or GET_NEXT_STATE
// then we do NOT actually set the stMach, but we only check to
// see if the stMach 'name' exists.
// -----------------------------------------------------------------
bool StateMachine::setStMach(const char* const name, const StateTableCode code)
{
   bool ok = false;

   if (code == CURR_STATE) {

      // Current state is now also the previous state
      StateMachine* oldStMach = stMach;

      // First, check to see if they're asking for the same state
      // as our current state.
      ok = (stMachName == nullptr && name == nullptr);
      if (!ok && stMachName != nullptr && name != nullptr) {
         ok = *stMachName == name;
      }

      // When they're not the same then we make a switch ...
      if (!ok) {
         if (name != nullptr) {
            Pair* p = findStMachByName(name);
            if (p != nullptr) {
               stMach = static_cast<StateMachine*>(p->object());
               stMachName = p->slot();
               ok = true;
            }
         }
         else {
            // 'name' is null, so set the new state to null.
            stMach = nullptr;
            stMachName = nullptr;
            ok = true;
         }
         if (ok) {
            pStMach = oldStMach;
         }
      }

   }

   else {
      // -- only need to look to see if this named state machine exists.
      ok = (findStMachByName(name) != nullptr);
   }

   return ok;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool StateMachine::setSlotStateMachines(const PairStream* const msg)
{
   // First remove the old list; and make sure we tell the old stMachList
   // that we're no longer their container.
   if (stMachList != nullptr) {
      List::Item* item = stMachList->getFirstItem();
      while (item != nullptr) {
         const auto p = static_cast<Pair*>(item->getValue());
         const auto q = static_cast<Component*>(p->object());
         q->container(nullptr);
         item = item->getNext();
      }
      stMachList = nullptr;
   }

   // Build a new list containing only StateMachine class (or derived) objects
   if (msg != nullptr) {
      const auto newList = new PairStream();

      // For each object in the list; if it's a StateMachine (or derived from) then
      // clone the object and add it to the new list.
      const List::Item* item = msg->getFirstItem();
      while (item != nullptr) {
         const auto p = static_cast<const Pair*>(item->getValue());
         const auto q = dynamic_cast<const StateMachine*>(p->object());

         if (q != nullptr) {
            const auto cp = static_cast<Pair*>(p->clone());
            const auto cq = static_cast<StateMachine*>(cp->object());
            cq->container(this);
            newList->put(cp);
         }
         else {
            std::cerr << "StateMachine::setSlotStateMachines(): " << *p->slot() << " is not a StateMachine!" << std::endl;
         }

         item = item->getNext();
      }

      // Set the pointer to the new stMach list
      stMachList = newList;
   }

   return true;
}

std::ostream& StateMachine::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    if (stMachList != nullptr) {
        indent(sout,i+j);
        sout << "stateMachines: {" << std::endl;
        stMachList->serialize(sout,i+j+4,slotsOnly);
        indent(sout,i+j);
        sout << "}" << std::endl;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}

