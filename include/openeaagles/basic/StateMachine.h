//------------------------------------------------------------------------------
// Class: StateMachine
//------------------------------------------------------------------------------
#ifndef __oe_Basic_StateMachine_H__
#define __oe_Basic_StateMachine_H__

#include "openeaagles/basic/Component.h"

namespace oe {
namespace Basic {
   class PairStream;

//------------------------------------------------------------------------------
// Class: StateMachine
// Description: Abstract State Machine class
//
//    Your finite state machines are implemented as derived classes.  States are
//    identified by unique state numbers and are implemented using "state functions"
//    or other state machines.  The state functions implement the state's actions,
//    check input conditions, determine transitions, and perform the transition
//    actions.  A "state table" is used to map state numbers to these state functions
//    and macros (see macros.h) are used to define the state table.
//
// Slots:
//    stateMachines   <PairStream>  ! List of state machines that are used to
//                                  ! implement our states. (default: 0)
//
// Events:
//    ON_ENTRY    ! Entering the state machine
//    ON_RETURN   ! Returning from a (sub) state machine.
//    ON_EXIT     ! Exiting the state machine
//
//
// State numbers
//
//    Each state is identified by a state number, getState().  The state
//    machine is initialized to INVALID_STATE.  With reset(), the state
//    machine will transition to INIT_STATE with the previous state set
//    to INVALID_STATE.  All other state numbers are user defined and
//    are mapped to the state functions using the state table.
//
//
// State functions:
//
//    All state functions take a single argument for delta time in seconds.
//    There are three types of user defined state functions:
//
//       1) The 'pre-state' function, preStateProc(), is a user defined, state
//          independent function that is called just prior to the state's function.
//          It performs the common front-end code required by the state machine
//          (e.g., gathering inputs).
//
//       2) The state function is a user defined, state dependent function
//          that performs the state's actions, processes inputs, and executes
//          transitions.  The state functions are mapped to state numbers by
//          the "state table" using macros similar to the 'event' mapping tables
//          in the Component class (see macros.h).
//
//       3) The 'post-state' function, postStateProc(), is a user define, state
//          independent function that is called just after the state's function.
//          It performs the common back-end code required by the state machine
//          (e.g., sending outputs).
//
//
// State table:
//
//    Use the following macros (see macros.h) to build the "state table".
//
//       BEGIN_STATE_TABLE(Foo)
//          -- Start of the state table for the 'Foo' state machine class.
//
//       STATE_MACH(stateNum, childSM)
//          -- maps the state number, stateNum, to the child state machine, childSM
//
//       STATE_FUNC(stateNum, stateFunc)
//          -- maps the state number, stateNum, to the state function, stateFunc
//
//       ANY_STATE_FUNC(stateFunc)
//          -- maps all state numbers to the state function, stateFunc.
//          If used, use with caution and only at the end of the state table since
//          it'll match all state numbers.
//
//       END_STATE_TABLE()
//          -- End of the state table.
//
//
// One "step" of the state machine:
//
//    One iteration, or step, of the state machine is performed by the step()
//    function, which is called by the updateTC() function.  For each iteration,
//    the step() function will
//       (1) call the 'pre-state' function, preStateProc(),
//       (2) call the state's function via the state table, or the state machine
//           assigned to handle the state, and
//       (3) call the 'post-state' function, postStateProc().
//
//
// State transition control:
//
//    There are four functions that are used by the state functions to request
//    a transition from one state to another state.
//
//       1) next(), which sequences to the next state in the "state table"
//
//       2) goTo(), which jumps to a requested state by number.
//
//       3) call(), which jumps to a requested state number and the previous
//          state and substate numbers are pushed on the "state stack".
//
//       4) rtn(), which pops the new state and substate numbers from the
//          "state stack" (i.e., returning from a call)
//
//    The new state can access the previous state number using getPreviousState().
//
//    The previous state can pass an argument to the new state.  This argument can
//    be accessed by the new state function using getArgument().
//
// State modes:
//
//    Since the state functions are called every frame by step() and since we
//    could remain in a given state for a period of time, there are three state
//    modes that are used by the state function.
//
//       NEW_STATE   -- First pass mode; first time the function is being
//                      called for this state.
//
//       RTN_STATE   -- Same as NEW_STATE except we're returning from a state
//                      that was previously call()'d by this state function.
//
//       HOLD_STATE  -- Holding in this state;
//
//    For the NEW_STATE and RTN_STATE modes, use getPreviousState() to get the
//    previous state number and use getArgument() to get an optional argument
//    passed to us by the previous state.
//
//
// Substate numbers:
//
//    Each state has an internal substate number that is used as needed
//    by the state function getSubstate() to sequence internal tasks.  The state
//    transition functions next(), goTo() and call() set the substate number
//    to INIT_STATE, and rtn() will pop the substate number from the "state
//    stack".
//
//       nextSubstate() -- sequences to the next substate number
//       goToSubstate() -- jumps to a substate number.
//
//
// Using a child state machine in place of the state's function.
//
//    Use the 'stateMachines' slot to attach any number of child state machines
//    to this state machine and then using the state table macro, STATE_MACH(),
//    to assign the state number of one of the child state machine by name.
//    The step() function will call the child state machine's tcFrame().
//
//    There are four functions that are used by the child state machine to request
//    the parent state machine to transition from one state to another state.
//
//       1) nextState(); go to the next parent state in order
//
//       2) goToState(); go to the new parent state
//
//       3) callState(); calls another of the parent states
//
//       4) rtnState(); returns to the parent's calling state from the "called" state
//
//    As the state transitions, the following events are sent to the child state
//    machine that handles the state.
//
//       ON_ENTRY    -- Entering this state
//       ON_RETURN   -- Returning to this state from a callState() state
//       ON_EXIT     -- Leaving this state
//
//
// Example "state table", which is defined in the state machine's *.cpp file.
//
//    BEGIN_STATE_TABLE(MyStateMachine)
//       STATE_FUNC( INIT_STATE, stateFunc00)   // initial state; calls state function stateFunc00()
//       STATE_MACH( 1, "sm01")                 // State #1; uses state machine "sm01"
//       STATE_FUNC( 2, stateFunc02)            // State #2; calls state function stateFunc02();
//       STATE_FUNC( 3, stateFunc03)            // State #3; calls state function stateFunc03();
//       STATE_MACH( 4, "sm02")                 // State #4; uses state machine "sm02"
//       STATE_FUNC( 9, stateFunc99)            // State #9; Final state function
//    END_STATE_TABLE()
//
//
// Example EDL input
//
//    ( MyStateMachine // Main state machine
//       stateMachines: {  // child state machines
//          sm01: ( MyStateMachine01 )
//          sm02: ( MyStateMachine02 )
//       }
//    )
//
//
// Examples of state machines can be found in the OpenEaagles
// example application 'testStateMach'.
//------------------------------------------------------------------------------
class StateMachine : public Component {
   DECLARE_SUBCLASS(StateMachine,Component)

public:
   static const unsigned short INIT_STATE = 0;         // Standard initial state
   static const unsigned short INVALID_STATE = 0xFFFF; // Invalid state number

public:
   StateMachine();

   // Current state numbers
   unsigned short getState() const     { return state; }
   unsigned short getSubstate() const  { return substate; }

   void updateData(const LCreal dt = 0.0) override;
   void updateTC(const LCreal dt = 0.0) override;
   bool event(const int event, Object* const obj = nullptr) override;
   void reset() override;

protected:
   enum Mode { HOLD_STATE, NEW_STATE, RTN_STATE };
   enum StateTableCode { CURR_STATE, TEST_STATE, FIND_NEXT_STATE };

   // Current mode
   Mode getMode() const                { return mode; }

   // Current argument
   Object* getArgument()               { return arg; }
   const Object* getArgument() const   { return arg; }

   // Previous state number
   unsigned short getPreviousState() const   { return pState; }

   // One step of this state machine (called from updateTC())
   virtual void step(const LCreal dt);

   // User function called just before the state specific function
   virtual void preStateProc(const LCreal dt);

   // User function called just after the state specific function
   virtual void postStateProc(const LCreal dt);

   // ---
   // State table ---
   //
   //    1) Must be implemented by our derived classes using the BEGIN_STATE_TABLE(),
   //       END_STATE_TABLE(), STATE_FUNC and STATE_MACH macros.
   //
   //    2) The state specific functions mapped by the STATE_FUNC() macro must
   //       have a single argument of type LCreal for delta time (see the preStateProc()
   //       and postStateProc() functions)
   // ---
   virtual unsigned short stateTable(
         const unsigned short cstate,
         const StateTableCode code,
         const LCreal dt=0
      ) =0;


   // ---
   // Transition functions -- these control movement between our states
   // ---

   // Go to the next state in order, while passing an optional argument, 'arg'
   bool next(Object* const arg = 0);

   // Go to the new state, 'newState', while passing an optional argument, 'arg'
   bool goTo(const unsigned short newState, Object* const arg = 0);

   // Call the 'newState', while passing an optional argument, 'arg'
   bool call(const unsigned short newState, Object* const arg = 0);

   // Return to the calling state, while returning an optional argument, 'arg'
   bool rtn(Object* const arg = 0);


   // ---
   // Substate Transition functions -- these control movement between our substates
   // ---

   // Goto the next substate
   bool nextSubstate();

   // Goto the 'newSubstate' substate number
   bool goToSubstate(const unsigned short newSubstate);


   // ---
   // Parent State Machine's transition functions -- these control movement
   // between our parent state machine's states.
   // ---

   // Go to the next parent state in order, while passing an optional argument, 'arg'
   bool nextState(Object* const arg = 0);

   // Go to the new parent state, 'newState', while passing an optional argument, 'arg'
   bool goToState(const unsigned short newState, Object* const arg = 0);

   // Call the parent state 'newState', while passing an optional argument, 'arg'
   bool callState(const unsigned short newState, Object* const arg = 0);

   // Return to the parent's calling state, while returning an optional argument, 'arg'
   bool rtnState(Object* const arg = 0);

   // ---
   // Event handlers
   // ---
   virtual bool onEntry(Object* const msg = 0);
   virtual bool onReturn(Object* const msg = 0);
   virtual bool onExit();

   // ---
   // State machine list functions
   // ---
   Pair* findStMachByName(const char* const name);
   Pair* findStMachByType(const std::type_info& type);
   bool setStMach(const char* const name, const StateTableCode code);

   // Slot function
   bool setSlotStateMachines(const PairStream* const msg);

private:
   void initData();

   // Next state
   unsigned short nState;     // Next state number
   unsigned short nSubstate;  // Next substate number
   safe_ptr<Object> nArg;         // Next argument
   Mode nMode;                // Next mode

   // Current state
   unsigned short state;      // Current state number
   unsigned short substate;   // Current substate number
   Mode mode;                 // Current mode
   safe_ptr<Object> arg;          // Current argument
   StateMachine* stMach;      // Current state's state machine object
   Identifier* stMachName;    // Current state's state machine name

   // Previous state
   unsigned short pState;     // Previous state number
   unsigned short pSubstate;  // Previous substate number
   StateMachine* pStMach;     // Previous state's state machine object

   // State stack and stack pointer
   static const unsigned short STACK_SIZE = 10;
   unsigned short stateStack[STACK_SIZE];    // State number stack
   unsigned short substateStack[STACK_SIZE]; // Substate number stack
   unsigned short sp;                        // Stack pointer

   // List of state machines
   safe_ptr<PairStream> stMachList;
};

} // End Basic namespace
} // End oe namespace

#endif
