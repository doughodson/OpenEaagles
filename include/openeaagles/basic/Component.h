//------------------------------------------------------------------------------
// Class: Component
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Component_H__
#define __Eaagles_Basic_Component_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic {

class Identifier;
class Logger;
class Number;
class Pair;
class PairStream;
class Statistic;
class String;

//------------------------------------------------------------------------------
// Class: Component
//
// Description: Base class for all OpenEaagles components.  Components provide
//              a common interface for time critical and non-time critical tasks,
//              and for passing event messages.  Any component can be a container
//              for a list of components.
//
// Factory name: Component
// Slots:
//    components           <Component>  ! Single child component (default: 0)
//                         <PairStream> ! List of child components.
//
//    select               <String>     ! Selects a child component by name (default: 0)
//                         <Number>     ! Selects a child component by index (default: 0)
//
//    logger               <Logger>     ! Set the event logger for this component (default: 0)
//
//    enableTimingStats    <Number>     ! Enable/disable the timing statistics for updateTC() (default: 0)
//
//    printTimingStats     <Number>     ! Enable/disable the printing of the timing statistics (default: false)
//
//    freeze               <Number>     ! Freeze flag: true(1)- frozen, false(0)- unfrozen; (default: false)
//
//    enableMessageType    <Identifier> ! Enable message type { WARNING, INFO, DEBUG, DATA, USER } (default: MSG_ERROR | MSG_WARNING)
//                         <Number>     ! Enable message type by number (e.g., 0x0100)
//
//    disableMessageType   <Identifier> ! Disable message type { WARNING, INFO, DEBUG, DATA, USER }
//                         <Number>     ! Disable message type by number (e.g., 0x0100) (default: 0)
//
// Events:
//    SELECT         (String)  Select a child component by name.
//    SELECT         (Number)  Select a child component by index.
//    RESET_EVENT    ()        Reset the component.
//    FREEZE_EVENT   ()        Freeze this component.
//    FREEZE_EVENT   (Boolean) Freeze this component if arg is true.
//    SHUTDOWN_EVENT ()        Shutdown this component.
//
//
//
// Simulation support functions:
//
//       updateTC(LCreal dt)
//          Time-Critical update -- This routine will be called by our tcFrame()
//          at a steady rate of 1/dt, where 'dt' is the  delta time in seconds
//          between calls.  Derived classes will provide updateTC() routines,
//          as needed.
//
//       tcFrame(LCreal dt)
//          Time-Critical Frame -- This routine will be called by our container
//          at a steady rate of 1/dt, where 'dt' is the  delta time in seconds
//          between calls.  The component time statistics are computed by this
//          function (see slots 'enableTimingStats' and 'printTimingStats').
//
//       updateData(LCreal dt)
//          Non-time-critical (i.e., background) update of the component, where
//          'dt' is the delta time in seconds between calls.  Derived classes
//          will provide updateData() routines, as needed.
//
//       bool isFrozen()
//       freeze(bool flag)
//          Gets/Sets our freeze flag.  When the freeze flag is set, delta time is
//          typically set to zero in updateTC() and updateData().  The freeze flag
//          can be preset using the 'freeze' slot.
//
//       reset()
//          Sets this component to its initial conditions.
//
//       bool isShutdown()
//          True if the component is shutting down or already shutdown. (e.g., received
//          a SHUTDOWN_EVENT event).
//
//       bool isNotShutdown()
//          True if the component is not shutting down, or shut down.
//
//
// Components and Containers:
//
//    Using a PairStream (see PairStream.h), a component can contain a list of
//    named components, and can be contained in a container's component list,
//    therefore creating a component tree.  All functions return zero if the
//    container or component is not found.  The components are initially set
//    using the 'components' slot, and the list can be modified with the
//    addComponent() and processComponents() functions.
//
//       Component* container()
//          Pointer to our container (i.e., we are a component of our container).
//
//       Component* findContainerByType(type_info& foo)
//          Going up the component tree, finds the first of our containers that
//          is of class type 'foo'.
//
//       Component* container(Component* p)
//          Sets our container pointer; returns 'p'
//
//       PairStream* getComponents()
//          Returns a ref()'d pointer to our list of components.
//
//       unsigned int getNumberOfComponents()
//          Returns the number of child components
//
//       Pair* findByName(char* name)
//          Finds one of our components by 'name'; returns a pointer to the
//          component Pair.  The following are rules for locating components
//          by name.
//
//                xxx      -- Look for 'xxx' as the name of one of our children
//                            components then our grandchildren components, etc.
//
//                .xxx     -- Only look for 'xxx' as the name of one of our
//                            children components.
//
//                xxx.yyy  -- complex name; look for 'xxx' as the name of one
//                            of our children components then our grandchildren
//                            components, etc. and if found, do a look for the
//                            name '.yyy' as one of 'xxx's components.
//
//       Pair* findByType(type_info& type)
//          Going down the component tree, finds one of our components by type;
//          returns a pointer to the component Pair.
//
//       Pair* findByIndex(int index)
//          Finds a child component by list 'index' number; returns a pointer
//          to the child component Pair.
//             -- Component index number is one based
//
//       Identifier* findNameOfComponent(Component* p)
//          Returns a ref()'d pointer to an Identifier that contains the name of
//          components 'p'; checking our children first then the grandchildren, etc.
//
//             -- For children components, a clone of the name (Identifier) is returned.
//             -- For grandchildren, a new Identifier is created containing the full name
//                of the component (i.e., "xxx.yyy.zzz").
//
//       select(String* name)
//          Selects a component for processing.  By default, all of our components
//          are called by updateTC() and updateData().  If a component is selected,
//          only the selected component will be updated.  A component can be pre-
//          selected by name or index number using the 'select' slot.
//
//       select(Number* index)
//          See "select(String* name)" description
//
//       bool isComponentSelected()
//          Returns true if a subcomponent was selected by index or name using
//          the 'select()' function above (even if the selected index or name
//          wasn't found).
//
//       Component* getSelectedComponent()
//          Returns a pointer to the selected component, or zero if no component
//          is selected or the selected component wasn't found.
//
//
// Events:
//
//    Components can send an event token, along with an optional Object based argument,
//    to any other component.  Events are passed by calling the receiving component's
//    event() function.  Event() can be called from an updateTC() routines, so the
//    event tokens must be processed quickly, similar to interrupt handlers.
//
//    Standard event tokens are defined in the file, eventTokens.h, which is included
//    by this Component class and are therefore within the Eaagles::Basic::Component
//    namespace.  There is a companion file, eventTokens.epp, which can be included
//    as part of the Eaagles Description Language (EDL) input files.
//
//    The event() function can be implemented using the BEGIN_EVENT_HANDLER()
//    and END_EVENT_HANDLER() macros.  Along with the macros ON_EVENT(), ON_EVENT_OBJ(),
//    ON_ANYKEY() and ON_ANYKEY_OBJ(), these macros are used to build an event dispatch
//    table.  Components will typically provide functions, "event handlers", that will
//    process the individual event tokens.
//
//          BEGIN_EVENT_HANDLER(Foo)
//
//             // The function onSelect() handles the 'SELECT' token if passed with
//             // a Number type argument
//             ON_EVENT_OBJ( SELECT, onSelect, Number)
//
//             // The function onUpdateValue() handles the 'UPDATE_VALUE' token if
//             // passed with a String type argument
//             ON_EVENT_OBJ( UPDATE_VALUE, onUpdateValue, String)
//
//             // The function onF1Key() handles the 'RESET_EVENT' token.  Any argument
//             // that may have been passed is ignored.
//             ON_EVENT( F1_KEY, onF1Key )
//
//          END_EVENT_HANDLER()
//
//    The event handlers are usually 'virtual' functions that can be overridden and
//    handled by a derived class.
//
//    The event handlers will take either a single argument, which matches the type
//    in the ON_EVENT_OBJ macro, or no argument if using just ON_EVENT.  The event
//    handlers will return a boolean which will be 'true' if the event is processed,
//    or 'false' if the event was not processed.
//
//    Events that are not processed are passed up to the 'BaseClass' class until reaching
//    this Component class.  'Key' events (see eventTokens.h) that are not processed by
//    this Component class are passed up to the container object.
//
//
// send() functions and SendData
//
//    Using the send() functions, a container component can send an event token,
//    along with an optional argument, to one of its components named 'id'.
//    The send() functions pass the token and argument to 'id' using the event()
//    function.  As with event(), send() will return true if the event was
//    processed.
//
//    Send() locates the receiving component, 'id', from our components list,
//    and it uses the SendData structure to save the pointer to the receiving
//    component.
//
//    For 'int', 'float', 'double', 'bool' and 'char*' type arguments, send()
//    will create the proper type Object to pass to the event() function.
//    The SendData structure is used to save the previous argument value.
//    With all subsequent send()'s, the previous argument value is compared to
//    the current argument value, and the event() is sent only if the values
//    have changed.
//
//
// Data Logger:
//    You can attach a data logger, slot 'logger', to any component (see Logger.h).
//    A logger can be used to collect data at major simulation events (e.g., weapon
//    released), which are not to be confused with the event tokens and event()
//    functions described above.
//
//       Logger* getEventLogger()
//          getEventLogger() returns the data logger assigned to this component.
//
//       Logger* getAnyEventLogger()
//          getAnyEventLogger() will start at this component and searches up
//          the component tree until a logger is found.
//
//       In either case, zero is returned if no logger is found.
//
//
// Object class' message functions
//
//    The Object class' isMessageEnabled() function has been extended to search
//    up the component tree for explicitly enabled or disabled messages.  Using
//    the 'enableMessageType' and 'disableMessageType' slots, or the functions
//    below, a container component can affect all of its child component's
//    messages, unless a child explicitly enables or disables their own message
//    bits.
//
//      bool isMessageEnabled(const unsigned short msgType)
//          Same as Object, plus if the message bit hasn't been explicitly
//          enabled or disabled in this component, it'll check our container's
//          bits as well.
//
//      bool enableMessageTypes(const unsigned short msgTypeBits)
//      bool disableMessageTypes(const unsigned short msgTypeBits)
//          Since isMessageEnabled() is also checking our container's message
//          bits then setting our bits it will therefore affect our components
//          that haven't explicitly enabled or disabled their messages.
//------------------------------------------------------------------------------
class Component : public Object
{
   DECLARE_SUBCLASS(Component, Object)

public:
   // Standard Event Tokens
   #include "openeaagles/basic/eventTokens.h"

   // SendData class used by the send() member functions
   class SendData {
      public:  SendData()   { obj = 0; past = 0; }
      public:  ~SendData()  { empty(); }
      public:  void empty();
      public:  Component* getObject(Component* p, const char* const id, const int n = 0);
      public:  void setObject(Component* p);
      public:  Object* getValue(const int value);
      public:  Object* getValue(const float value);
      public:  Object* getValue(const double value);
      public:  Object* getValue(const char* const value);
      public:  Object* getValue(const bool value);
      private: Component* obj;   // Object to send to
      private: Object* past;     // Old value
   };

public:
   Component();

   Component* container()                          { return containerPtr; }
   const Component* container() const              { return containerPtr; }
   Component* findContainerByType(const std::type_info& type);
   const Component* findContainerByType(const std::type_info& type) const;
   Component* container(Component* const p)      { return (containerPtr = p); }

   unsigned int getNumberOfComponents() const;

   PairStream* getComponents();
   const PairStream* getComponents() const;
   virtual bool addComponent(Pair* const p);

   virtual Pair* findByName(const char* const slotname);                // find a component by its name
   virtual const Pair* findByName(const char* const slotname) const;

   virtual Pair* findByIndex(const int slotindex);
   virtual const Pair* findByIndex(const int slotindex) const;

   virtual Pair* findByType(const std::type_info& type);
   virtual const Pair* findByType(const std::type_info& type) const;

   bool isComponentSelected() const                { return (selection != nullptr); }
   Component* getSelectedComponent()               { return selected; }
   const Component* getSelectedComponent() const   { return selected; }

   virtual const Identifier* findNameOfComponent(const Component* const p) const;

   virtual void updateTC(const LCreal dt = 0.0);
   virtual void updateData(const LCreal dt = 0.0);
   void tcFrame(const LCreal dt = 0.0);

   virtual bool isFrozen() const;
   virtual bool isNotFrozen() const;
   virtual void freeze(const bool fflag);
   virtual void reset();

   bool isShutdown() const    { return shutdown; }
   bool isNotShutdown() const { return !shutdown; }

   // ---
   // Sends the 'event' token with an optional argument 'obj' to this component.
   // Returns true if the 'event' has received and processed this component.
   // Typically implemented using the event macros (see above).
   // ---
   virtual bool event(const int event, Object* const obj = nullptr);

   // ---
   // Send the 'event' message to our component named 'id' with an optional
   // argument, 'value',  The SendData structure maintains the n-1 value
   // and the pointer to our component.  Send() returns true if the 'event'
   // has been received and used.
   // ---
   bool send(const char* const id, const int event);
   bool send(const char* const id, const int event, const int value, SendData& sd);
   bool send(const char* const id, const int event, const float value, SendData& sd);
   bool send(const char* const id, const int event, const double value, SendData& sd);
   bool send(const char* const id, const int event, const char* const value, SendData& sd);
   bool send(const char* const id, const int event, const bool value, SendData& sd);
   bool send(const char* const id, const int event, Object* const value, SendData& sd);

   // ---
   // Sends the 'event' message to 'n' components with 'n' arguments from the
   // 'value' arrays.  This requires an array of 'n' SendData structures to
   // maintain the n-1 values and component pointers.  The component 'id' is
   // used in a format for sprintf() to determine the 'n' individual components.
   //    Example:
   //       n = 4 and id = 'test%02d'
   //    gives component id's = { test01 test02 test03 test04 }
   // ---
   bool send(const char* const id, const int event, const int value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, const float value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, const double value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, const char* const value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, const bool value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, Object* const value[], SendData sd[], const int n);

   // Timing-Critical Statistics (managed by the tcFrame() function)
   const Statistic* getTimingStats() const           { return timingStats; }
   bool isTimingStatsEnabled() const                 { return (timingStats != 0); }
   bool isTimingStatsPrintEnabled() const            { return (pts && isTimingStatsEnabled()); }
   virtual bool setTimingStatsEnabled(const bool b);
   virtual bool setPrintTimingStats(const bool b);

   // Event (data) logger functions
   Logger* getEventLogger();
   Logger* getAnyEventLogger();
   virtual bool setEventLogger(Logger* const logger);

   // Slot functions
   virtual bool setSlotComponent(PairStream* const multiple);        // Sets the components list
   virtual bool setSlotComponent(Component* const single);           // Sets a single component
   virtual bool setSlotEnableTimingStats(const Number* const num);   // Sets the timing enabled flag
   virtual bool setSlotPrintTimingStats(const Number* const num);    // Sets the print timing stats flag
   virtual bool setSlotFreeze(const Number* const num);              // Sets the freeze flag
   virtual bool setSlotEventLogger(Logger* const logger);            // Sets the Event logger
   virtual bool setSlotEnableMsgType(const Identifier* const msg);   // Enables message types by name
   virtual bool setSlotEnableMsgType(const Number* const msg);       // Enables message types by bit
   virtual bool setSlotDisableMsgType(const Identifier* const msg);  // Disables message types by name
   virtual bool setSlotDisableMsgType(const Number* const msg);      // Disables message types by bit

   bool isMessageEnabled(const unsigned short msgType) const override;

protected:
   virtual void printTimingStats();         // Print statistics on component timing
   virtual bool shutdownNotification();     // We're shutting down
   virtual bool onEventReset();             // Reset event handler

   virtual bool setSelectionName(const Object* const s); // Name (or number) of component to select
   virtual bool select(const String* const name);        // Select component by name
   virtual bool select(const Number* const num);         // Select component by number

   // processComponents() -- process our new components list;
   //   -- Add the components from the input list, 'list', to a new list
   //      make sure they are all of class Component (or derived from it)
   //      tell them that we are their container
   //   -- Add an optional component to the end of the new list
   //   -- Swap our 'components' list with this new list
   //   -- Handle component selection.
   virtual void processComponents(
         PairStream* const list,       // Source list of components
         const std::type_info& filter, // Type filter
         Pair* const add = 0,          // Optional pair to add
         Component* const remove = 0   // Optional component to remove
      );

private:
   safe_ptr<PairStream> components; // Child components
   Component* containerPtr;         // We are a component of this container

   Component* selected;             // Selected child (process only this one)
   Object*    selection;            // Name of selected child

   safe_ptr<Logger> elog;           // Our event logger
   safe_ptr<Logger> elog0;          // Event logger from slots
   Statistic* timingStats;          // Timing statistics
   bool pts;                        // Print timing statistics
   bool frz;                        // Freeze flag -- true if this component is frozen
   bool shutdown;                   // True if this component is being (or has been) shutdown
};

} // End Basic namespace
} // End Eaagles namespace


#endif
