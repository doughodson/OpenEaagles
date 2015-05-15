//******************************************************************************
// Component
//******************************************************************************

#include "openeaagles/basic/Component.h"

#include "openeaagles/basic/Boolean.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Logger.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Statistic.h"
#include "openeaagles/basic/String.h"

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...

#if(_MSC_VER>=1400)   // VC8+
 #pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(Component,"Component")

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Component)
    "components",          // 1) Children components                                  (PairStream)
    "select",              // 2) Manage only this child component (component idx)     (Number)
    "logger",              // 3) Event logger                                         (Logger)
    "enableTimingStats",   // 4) Enable/disable the timing statistics for updateTC()  (Number) (default: false)
    "printTimingStats",    // 5) Enable/disable the printing of the timing statistics (Number) (default: false)
    "freeze",              // 6) Freeze flag: true(1), false(0); default: false       (Number) (default: false)
    "enableMessageType",   // 7) Enable message type { WARNING INFO DEBUG USER DATA }
    "disableMessageType"   // 8) Disable message type { WARNING INFO DEBUG USER DATA }
END_SLOTTABLE(Component)

// Map slot table to handles
BEGIN_SLOT_MAP(Component)
    ON_SLOT( 1, setSlotComponent, PairStream)
    ON_SLOT( 1, setSlotComponent, Component)
    ON_SLOT( 2, select, Number)
    ON_SLOT( 2, select, String)
    ON_SLOT( 3, setSlotEventLogger, Logger)
    ON_SLOT( 4, setSlotEnableTimingStats, Number)
    ON_SLOT( 5, setSlotPrintTimingStats, Number)
    ON_SLOT( 6, setSlotFreeze, Number)
    ON_SLOT( 7, setSlotEnableMsgType, Identifier)
    ON_SLOT( 7, setSlotEnableMsgType, Number)
    ON_SLOT( 8, setSlotDisableMsgType, Identifier)
    ON_SLOT( 8, setSlotDisableMsgType, Number)
END_SLOT_MAP()

// Event Table
BEGIN_EVENT_HANDLER(Component)
    ON_EVENT_OBJ(SELECT,select,Number)
    ON_EVENT_OBJ(SELECT,select,String)
    ON_EVENT(RESET_EVENT, onEventReset )
    ON_EVENT(SHUTDOWN_EVENT,shutdownNotification)
    ON_EVENT_OBJ(FREEZE_EVENT, setSlotFreeze, Number )

    // *** Special handling of the end of the EVENT table ***
    // Pass only key events up to our container
    if (_event <= MAX_KEY_EVENT && container() != nullptr) {
        _used = container()->event(_event,_obj);
    }
    return _used;
}


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Component::Component()
{
   STANDARD_CONSTRUCTOR()

   // Child components and our container
   components = nullptr;
   containerPtr = nullptr;

   // Nothing selected
   selected = nullptr;
   selection = nullptr;

   elog = nullptr;       // No event logger
   elog0 = nullptr;

   timingStats = nullptr;
   pts = false;

   frz = false;    // We're not frozen
   shutdown = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Component::copyData(const Component& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      components = nullptr;
      containerPtr = nullptr;
      selected = nullptr;
      selection = nullptr;
      elog = nullptr;
      elog0 = nullptr;
      timingStats = nullptr;
      shutdown = false;
   }

   // Copy event logger
   const Logger* p = org.elog;
   elog = const_cast<Logger*>(p);
   if (org.elog0 != nullptr)
      elog0 = static_cast<Logger*>(org.elog0->clone());
   else
      elog0 = nullptr;

   // Copy selection
   setSelectionName(org.selection);
   selected = nullptr;

   // Copy child components
   const PairStream* oc = org.components.getRefPtr();
   if (oc != nullptr) {
      PairStream* tmp = static_cast<PairStream*>(oc->clone());
      oc->unref();
      processComponents(tmp, typeid(Component));
      tmp->unref();
   }
   else
      components = nullptr;

   // Timing statistics
   if (timingStats != nullptr) timingStats->unref();
   timingStats = nullptr;
   if (org.timingStats != nullptr) {
      timingStats = static_cast<Statistic*>(org.timingStats->clone());
   }
   pts = org.pts;

   // Our container
   containerPtr = nullptr;             // Copied doesn't mean contained in the same container!

   frz = org.frz;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Component::deleteData()
{
    // just in case our components haven't heard, we're shutting down!
    shutdownNotification();

    // Delete component selection
    setSelectionName(nullptr);
    selected = nullptr;

    // Delete list of components
    components = nullptr;

    if (timingStats != nullptr) {
       timingStats->unref();
       timingStats = nullptr;
    }
}

//------------------------------------------------------------------------------
// Message types
//------------------------------------------------------------------------------

bool Component::isMessageEnabled(const unsigned short msgType) const
{
   // Start with our own object's "is enabled" check
   bool enabled = BaseClass::isMessageEnabled(msgType);

   // If we were not enabled, and we were not explicitly disabled, and if
   // we have a container, then see if our container has this message enabled.
   if ( !enabled && !isMessageDisabled(msgType) && containerPtr != nullptr) {
      enabled = containerPtr->isMessageEnabled(msgType);
   }
   return enabled;
}

//------------------------------------------------------------------------------
// Freeze status and control
//------------------------------------------------------------------------------
bool Component::isFrozen() const
{
   return frz;
}

bool Component::isNotFrozen() const
{
   return !isFrozen();
}

void Component::freeze(const bool fflag)
{
   frz = fflag;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Component::reset()
{
   PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
        if (selection != nullptr) {
            // When we've selected only one
            if (selected != nullptr) selected->reset();
        }
        else {
            // When we should reset them all
            List::Item* item = subcomponents->getFirstItem();
            while (item != nullptr) {
                Pair* pair = static_cast<Pair*>(item->getValue());
                Component* obj = static_cast<Component*>(pair->object());
                obj->reset();
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }

    // Reset the log file
    if (elog0 != nullptr) {
        elog0->reset();
    }
}

//------------------------------------------------------------------------------
// tcFrame() -- Main time-critical frame
//------------------------------------------------------------------------------
void Component::tcFrame(const LCreal dt)
{
   // ---
   // Collect start time
   // ---
   double tcStartTime = 0.0;
   if (isTimingStatsEnabled()) {
      #if defined(WIN32)
         LARGE_INTEGER fcnt;
         QueryPerformanceCounter(&fcnt);
         tcStartTime = static_cast<double>( fcnt.QuadPart );
      #else
         tcStartTime = getComputerTime();
      #endif
   }

   // ---
   // Execute one time-critical frame
   // ---
   this->updateTC(dt);

   // ---
   // Process timing data
   // ---
   if (isTimingStatsEnabled()) {

      double dtime = 0;    // Delta time in MS
      #if defined(WIN32)
         LARGE_INTEGER cFreq;
         QueryPerformanceFrequency(&cFreq);
         double freq = static_cast<double>( cFreq.QuadPart );
         LARGE_INTEGER fcnt;
         QueryPerformanceCounter(&fcnt);
         double endCnt = static_cast<double>( fcnt.QuadPart );
         double dcnt = endCnt - tcStartTime;
         dtime = (dcnt/freq) * 1000.0;
      #else
         dtime = (getComputerTime() - tcStartTime) * 1000.0;
      #endif
      timingStats->sigma(dtime); // Time in MS

      if (isTimingStatsPrintEnabled()) {
         printTimingStats();
      }

   }
}

//------------------------------------------------------------------------------
// printTimingStats() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Component::printTimingStats()
{
   std::cout << "timing(" << this << "): dt=" << timingStats->value() << ", ave=" << timingStats->mean() << ", max=" << timingStats->maxValue() << std::endl;
}

//------------------------------------------------------------------------------
// updateTC() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Component::updateTC(const LCreal dt)
{
    // Update all my children
    PairStream* subcomponents = getComponents();
    if (subcomponents != nullptr) {
        if (selection != nullptr) {
            // When we've selected only one
            if (selected != nullptr) selected->tcFrame(dt);
        }
        else {
            // When we should update them all
            List::Item* item = subcomponents->getFirstItem();
            while (item != nullptr) {
                Pair* pair = static_cast<Pair*>(item->getValue());
                Component* obj = static_cast<Component*>( pair->object() );
                obj->tcFrame(dt);
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }

    // Update our log file
    if (elog0 != nullptr) {
        elog0->tcFrame(dt);
    }
}


//------------------------------------------------------------------------------
// updateData() -- Update non-time critical (background) stuff here
//------------------------------------------------------------------------------
void Component::updateData(const LCreal dt)
{
    // Update all my children
    PairStream* subcomponents = getComponents();
    if (subcomponents != nullptr) {
        if (selection != nullptr) {
            // When we've selected only one
            if (selected != nullptr) selected->updateData(dt);
        }
        else {
            // When we should update them all
            List::Item* item = subcomponents->getFirstItem();
            while (item != nullptr) {
                Pair* pair = static_cast<Pair*>(item->getValue());
                Component* obj = static_cast<Component*>(pair->object());
                obj->updateData(dt);
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }

    // Update our log file
    if (elog0 != nullptr) {
        elog0->updateData(dt);
    }
}

//------------------------------------------------------------------------------
// getComponents() -- returns a ref()'d pointer to our list of components;
//                    need to unref() when completed.
//------------------------------------------------------------------------------
PairStream* Component::getComponents()
{
   return components.getRefPtr();
}

const PairStream* Component::getComponents() const
{
   return components.getRefPtr();
}

//------------------------------------------------------------------------------
// getNumberOfComponents() -- returns the number of components
//------------------------------------------------------------------------------
unsigned int Component::getNumberOfComponents() const
{
   unsigned int n = 0;
   const PairStream* subcomponents = components.getRefPtr();
   if (subcomponents != nullptr) {
      n = subcomponents->entries();
      subcomponents->unref();
      subcomponents = nullptr;
   }
   return n;
}

//------------------------------------------------------------------------------
// onEventReset() -- Reset event handler
//------------------------------------------------------------------------------
bool Component::onEventReset()
{
   this->reset();
   return true;
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Default shutdown
//------------------------------------------------------------------------------
bool Component::shutdownNotification()
{
   // Tell all of our components
   PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      List::Item* item = subcomponents->getFirstItem();
      while (item != nullptr) {
         Pair* pair = static_cast<Pair*>(item->getValue());
         Component* p = static_cast<Component*>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }

   // And tell the logger
   if (elog != nullptr) elog->event(SHUTDOWN_EVENT);

   shutdown = true;
   return shutdown;
}


//------------------------------------------------------------------------------
// findContainerByType() -- find a container of ours by type
//------------------------------------------------------------------------------
Component* Component::findContainerByType(const std::type_info& type)
{
   Component* p = nullptr;
   if (container() != nullptr) {
      if ( container()->isClassType(type) )
        p = container();
      else
        p = container()->findContainerByType(type);
   }
   return p;
}

const Component* Component::findContainerByType(const std::type_info& type) const
{
   const Component* p = nullptr;
   if (container() != nullptr) {
      if ( container()->isClassType(type) )
        p = container();
      else
        p = container()->findContainerByType(type);
   }
   return p;
}


//------------------------------------------------------------------------------
// findByName() -- find one of our components by slotname
//
//  Forms of slotname:
//      xxx     -- simple name, look for 'xxx' as one of our components
//                 or one of our components' components.
//      xxx.yyy -- complex name, look for 'xxx' as one of our
//                 components and '.yyy' as one of our components'
//                 components.
//      .yyy    -- hard name, look for 'yyy' only as one of our
//                 components.
//------------------------------------------------------------------------------
const Pair* Component::findByName(const char* const slotname) const
{
    const Pair* q = nullptr;
    const PairStream* subcomponents = getComponents();
    if (subcomponents != nullptr) {

        const char* name = slotname;
        if (slotname[0] == '.') name++;      // remove '.' from hard names

        // Copy the name up to a possible period.
        char fname[128];
        int i = 0;
        while (name[i] != '\0' && name[i] != '.') {
            fname[i] = name[i];
            i++;
        }
        fname[i] = '\0';

        // Is this a complex name? (xxx.yyy)
        if (name[i] == '.') {
            // When it is a complex name ...

            // Find a component named 'xxx' (we copied 'xxx' to fname above)
            const Pair* q1 = subcomponents->findByName(fname);

            // Found it?
            if (q1 != nullptr) {
                // Check its components for 'yyy'
                const Component* gobj = static_cast<const Component*>(q1->object());
                q = gobj->findByName(&name[i]);
            }

        }
        else
            // When it's a simple name ...
            q = subcomponents->findByName(name);


        // Did we find it?
        if (q == nullptr && slotname[0] != '.') {
            // No, its not one of our components and its not a hard name,
            //  so check our components' components
            const List::Item* item = subcomponents->getFirstItem();
            while (item != nullptr && q == nullptr) {
                const Pair* p = static_cast<const Pair*>(item->getValue());
                const Component* obj = static_cast<const Component*>(p->object());
                q = obj->findByName(slotname);
                item = item->getNext();
            }
        }

        subcomponents->unref();
        subcomponents = nullptr;
    }
    return q;
}

Pair* Component::findByName(const char* const slotname)
{
   const Component* cThis = this;
   const Pair* p = cThis->findByName(slotname);
   return const_cast<Pair*>(p);
}


//------------------------------------------------------------------------------
// findByIndex() -- find component one of our components by slot index
//------------------------------------------------------------------------------
const Pair* Component::findByIndex(const int slotindex) const
{
   const Pair* p = nullptr;

   const PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      p = subcomponents->getPosition(slotindex);
      subcomponents->unref();
      subcomponents = nullptr;
   }

   return p;
}

Pair* Component::findByIndex(const int slotindex)
{
   Pair* p = nullptr;

   PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      p = subcomponents->getPosition(slotindex);
      subcomponents->unref();
      subcomponents = nullptr;
   }

   return p;
}

//------------------------------------------------------------------------------
// findByType() -- find one of our components by type (our children first
//                 then grandchildren).
//------------------------------------------------------------------------------
const Pair* Component::findByType(const std::type_info& type) const
{
    const Pair* q = nullptr;
    const PairStream* subcomponents = getComponents();
    if (subcomponents != nullptr) {
        q = subcomponents->findByType(type);
        const List::Item* item = subcomponents->getFirstItem();
        while (item != nullptr && q == nullptr) {
            const Pair* p = static_cast<const Pair*>(item->getValue());
            const Component* obj = static_cast<const Component*>(p->object());
            q = obj->findByType(type);
            item = item->getNext();
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }
    return q;
}

Pair* Component::findByType(const std::type_info& type)
{
   const Component* cThis = this;
   const Pair* p = cThis->findByType(type);
   return const_cast<Pair*>(p);
}


//------------------------------------------------------------------------------
// findNameOfComponent() --
//   1) Returns a pointer to an Identifier that contains the name of components 'p'
//      (our children first then grandchildren).  For grandchildren, a full name is
//      created to a component (i.e., "xxx.yyy.zzz", see findByName()).
//   2) Unref() the Identifier when finished.
//   3) Zero(0) is returned if 'p' is not found.
//------------------------------------------------------------------------------
const Identifier* Component::findNameOfComponent(const Component* const p) const
{
    const Identifier* name = nullptr;
    const PairStream* subcomponents = getComponents();
    if (subcomponents != nullptr) {

        // First check our component list ..
        name = subcomponents->findName(p);

        if (name == nullptr) {

            // Not found, so check our children's components ...

            const List::Item* item = subcomponents->getFirstItem();
            while (item != nullptr && name == nullptr) {
                const Pair* pair = static_cast<const Pair*>(item->getValue());
                const Component* child = static_cast<const Component*>(pair->object());
                const Identifier* name0 = child->findNameOfComponent(p);
                if (name0 != nullptr) {
                    // Found it, so prefix it with our child's name and
                    // return the full name.
                    Identifier* fullname = static_cast<Identifier*>(pair->slot()->clone());
                    *fullname += ".";
                    *fullname += name0->getString();
                    name = fullname;
                    name0->unref();
                }
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }
    return name;
}

//------------------------------------------------------------------------------
// getEventLogger() -- Returns a pointer to the logger assigned to this component
//------------------------------------------------------------------------------
Logger* Component::getEventLogger()
{
    return elog;
}

//------------------------------------------------------------------------------
// getAnyEventLogger() -- Returns a pointer to the logger assigned to this
//  component or to one of its containers.
//------------------------------------------------------------------------------
Logger* Component::getAnyEventLogger()
{
    if (elog == nullptr && containerPtr != nullptr) {
        elog = containerPtr->getAnyEventLogger();
    }
    return elog;
}

//------------------------------------------------------------------------------
// setEventLogger() -- Sets the event logger
//  -- may be set by a container or as a slot (setSlotEventLogger())
//------------------------------------------------------------------------------
bool Component::setEventLogger(Logger* const logger)
{
    if (elog != nullptr) elog->container(nullptr);
    elog = logger;
    if (elog != nullptr) elog->container(this);
    return true;
}

//------------------------------------------------------------------------------
// addComponent() -- Add a new component to our list of components
//------------------------------------------------------------------------------
bool Component::addComponent(Pair* const p)
{
   PairStream* subcomponents = getComponents();
   processComponents(subcomponents, typeid(Component), p);
   if (subcomponents != nullptr) subcomponents->unref();
   return true;
}


//------------------------------------------------------------------------------
// processComponents() -- process our new components list;
//   -- Add the components from the input list, 'list', to a new list, 'newList'
//      make sure they are all of type Component (or derived from it)
//      tell them that we are their container
//   -- Add an optional component to the end of the new list
//   -- Swap our 'components' list with the new list, newList
//   -- Handle selections.
//------------------------------------------------------------------------------
void Component::processComponents(
      PairStream* const list,
      const std::type_info& filter,
      Pair* const add,
      Component* const remove
   )
{
   PairStream* oldList = components.getRefPtr();

   // ---
   // Our dynamic_cast (see below) already filters on the Component class
   // ---
   bool skipFilter = false;
   if (filter == typeid(Component)) {
      skipFilter = true;
   }

   // ---
   // Create a new list, copy (filter) the component pairs and set their container pointers
   // ---
   PairStream* newList = new PairStream();
   if (list != nullptr) {

      // Add the (filtered) components to the new list and set their container
      List::Item* item = list->getFirstItem();
      while (item != nullptr) {
         Pair* pair = static_cast<Pair*>(item->getValue());
         Component* cp = dynamic_cast<Component*>( pair->object() );
         if ( cp != nullptr && cp != remove && (skipFilter || cp->isClassType(filter)) ) {
            newList->put(pair);
            cp->container(this);
         }
         else if ( cp != nullptr && cp == remove ) {
            cp->container(nullptr);
         }
         item = item->getNext();
      }

   }

   // ---
   // Add the optional component
   // ---
   if (add != nullptr) {
      Component* cp = dynamic_cast<Component*>( add->object() );
      if ( cp != nullptr && (skipFilter || cp->isClassType(filter)) ) {
         newList->put(add);
         cp->container(this);
      }
   }

   // ---
   // Swap lists
   // ---
   components = newList;
   newList->unref();

   // ---
   // Anything selected?
   // ---
   if (selection != nullptr) {
      if (selection->isClassType(typeid(String))) {
            String* str = new String(*(static_cast<String*>(selection)));
            select(str);
            str->unref();
      }
      else {
            Integer* num = new Integer((static_cast<Number*>(selection))->getInt());
            select(num);
            num->unref();
      }
   }

   if (oldList != nullptr) {
      oldList->unref();
   }
}

//------------------------------------------------------------------------------
// setSelectionName() -- Name (or number) of component to selected
//------------------------------------------------------------------------------
bool Component::setSelectionName(const Object* const s)
{
   if (selection != nullptr) selection->unref();
   selection = nullptr;
   if (s != nullptr) {
      selection = s->clone();
   }
   return true;
}

//------------------------------------------------------------------------------
// select() -- select one of our components, using String or Number
//------------------------------------------------------------------------------
bool Component::select(const String* const name)
{
    bool ok = true;
    selected = nullptr;
    setSelectionName(nullptr);
    if (name != nullptr) {
        setSelectionName(name);
        Pair* p = findByName(*name);
        if (p != nullptr) selected = static_cast<Component*>(p->object());
        else {
            std::cerr << "Component::select: name not found!"  << std::endl;
            ok = false;
        }
    }
    return ok;
}

bool Component::select(const Number* const num)
{
    bool ok = true;
    selected = nullptr;
    setSelectionName(nullptr);
    if (num != nullptr) {
        setSelectionName(num);
        Pair* p = findByIndex(num->getInt());
        if (p != nullptr) {
           selected = static_cast<Component*>(p->object());
        }
        else {
           std::cerr << "Component::select: index out of range; num = " << num->getInt() << std::endl;
           ok = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setTimingStatsEnabled() -- enable/disable timing statistics
//------------------------------------------------------------------------------
bool Component::setTimingStatsEnabled(const bool b)
{
   if (b) {
      // Enable timing statistics by creating a statistics object
      if (timingStats != nullptr) {
         // Already have one, just clear it
         timingStats->clear();
      }
      else {
         timingStats = new Statistic();
      }
   }
   else {
      // Disable the timing statistics
      if (timingStats != nullptr) {
         // We disable it by getting rid of it.
         timingStats->unref();
         timingStats = nullptr;
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// setPrintTimingStats() -- enable/disable print the timing statistics
//------------------------------------------------------------------------------
bool Component::setPrintTimingStats(const bool b)
{
   pts = b;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// setSlotEnableTimingStats() -- slot to enable/disable the timing statistics
bool Component::setSlotEnableTimingStats(const Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setTimingStatsEnabled(num->getBoolean());
   }
   return ok;
}

// setSlotPrintTimingStats() -- slot to enable/disable printing the timing statistics
bool Component::setSlotPrintTimingStats(const Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setPrintTimingStats(num->getBoolean());
   }
   return ok;
}

// setSlotFreeze() -- slot to set/clear the freeze flag
bool Component::setSlotFreeze(const Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      freeze(num->getBoolean());
      ok = true;
   }
   return ok;
}

// setSlotComponent() -- Sets a pairstream
bool Component::setSlotComponent(PairStream* const multiple)
{
   // Process the new components list and swap
   processComponents(multiple, typeid(Component));
   return true;
}

// setSlotComponent() -- Sets a single component
bool Component::setSlotComponent(Component* const single)
{
   // When a only one component ... make it a PairStream
   PairStream* pairStream = new PairStream();
   Pair* pair = new Pair("1", single);
   pairStream->put( pair );
   pair->unref();

   // Process the new components list and swap
   processComponents(pairStream, typeid(Component));
   pairStream->unref();

   return true;
}

// setSlotEventLogger() -- Sets the event logger slot
bool Component::setSlotEventLogger(Logger* const logger)
{
    elog0 = logger;     // Save for the print routine
    return setEventLogger(logger);
}

// enableMessageType --- Enable message type { WARNING INFO DEBUG DATA USER }
bool Component::setSlotEnableMsgType(const Identifier* const msg)
{
   bool ok = false;
   if (msg != nullptr) {

      const Identifier* p = msg;

      if (*p == "WARNING")    ok = enableMessageTypes(MSG_WARNING);
      else if (*p == "INFO")  ok = enableMessageTypes(MSG_INFO);
      else if (*p == "DEBUG") ok = enableMessageTypes(MSG_DEBUG);
      else if (*p == "USER")  ok = enableMessageTypes(MSG_USER);
      else if (*p == "DATA")  ok = enableMessageTypes(MSG_DATA);
      else {
         if (isMessageEnabled(MSG_ERROR)) {
             std::cerr << "Object::setSlotEnableMsgType(): unknown message type: " << *p;
             std::cerr << "; use: { WARNING INFO DEBUG USER DATA }" << std::endl;
         }
      }

   }
   return ok;
}

// enableMessageType --- Enable message type by number (e.g., 0x0100)
bool Component::setSlotEnableMsgType(const Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = enableMessageTypes( static_cast<unsigned short>(msg->getInt()) );
   }
   return ok;
}

// disableMessageType --- Disable message type { WARNING INFO DEBUG DATA USER }
bool Component::setSlotDisableMsgType(const Identifier* const msg)
{
   bool ok = false;
   if (msg != nullptr) {

      const Identifier* p = msg;

      if (*p == "WARNING")    ok = disableMessageTypes(MSG_WARNING);
      else if (*p == "INFO")  ok = disableMessageTypes(MSG_INFO);
      else if (*p == "DEBUG") ok = disableMessageTypes(MSG_DEBUG);
      else if (*p == "USER")  ok = disableMessageTypes(MSG_USER);
      else if (*p == "DATA")  ok = disableMessageTypes(MSG_DATA);
      else {
         if (isMessageEnabled(MSG_ERROR)) {
             std::cerr << "Object::disableMessageTypes(): unknown message type: " << *p;
             std::cerr << "; use: { WARNING INFO DEBUG USER DATA }" << std::endl;
         }
      }

   }
   return ok;
}

// disableMessageType --- Disable message type by number (e.g., 0x0100)
bool Component::setSlotDisableMsgType(const Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = disableMessageTypes( static_cast<unsigned short>(msg->getInt()) );
   }
   return ok;
}

//------------------------------------------------------------------------------
// send() -- various support routines that send event messages to components.
//           Return true of the message was received and used.
//------------------------------------------------------------------------------

// Send an event message to component 'id'
bool Component::send(const char* const id, const int event)
{
    bool val = false;
    Pair* p = findByName(id);
    if (p != nullptr) {
        Component* g = static_cast<Component*>(p->object());
        val = g->event(event);
    }
    return val;
}


// Send an event message with an int value to component 'id'
bool Component::send(const char* const id, const int event, const int value, SendData& sd)
{
   bool val = false;
   Object* vv = sd.getValue(value);
   if (vv != nullptr) {
      Component* g = sd.getObject(this,id);
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// Send an event message with a float value to component 'id'
bool Component::send(const char* const id, const int event, const float value, SendData& sd)
{
   bool val = false;
   Object* vv = sd.getValue(static_cast<LCreal>(value));
   if (vv != nullptr) {
      Component* g = sd.getObject(this,id);
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// Send an event message with a double value to component 'id'
bool Component::send(const char* const id, const int event, const double value, SendData& sd)
{
   bool val = false;
   Object* vv = sd.getValue(static_cast<LCreal>(value));
   if (vv != nullptr) {
      Component* g = sd.getObject(this,id);
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// Send an event message with a character string to component 'id'
bool Component::send(const char* const id, const int event, const char* const value, SendData& sd)
{
   bool val = false;
   Object* vv = sd.getValue(value);
   if (vv != nullptr) {
      Component* g = sd.getObject(this,id);
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// Send an event message with a boolean to component 'id'
bool Component::send(const char* const id, const int event, const bool value, SendData& sd)
{
   bool val = false;
   Object* vv = sd.getValue(value);
   if (vv != nullptr) {
      Component* g = sd.getObject(this,id);
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// Send an event message with an Object value to component 'id'
bool Component::send(const char* const id, const int event, Object* const value, SendData& sd)
{
    // we don't check past values here, because it would be tedious and more overhead
    // to go through each object and see if any data has changed.  So we take a smaller
    // hit just by sending the data through every time.
    bool val = false;
    if (value != nullptr) {
      Component* g = sd.getObject(this,id);
      if (g != nullptr) val = g->event(event, value);
   }
   return val;
}


// ---
// Send 'n' event messages with int type values to components.  The 'id' is used
// with std::sprintf() to create the component names.  Example: n = 4, id="test%02d"
// will generate component names test01, test02, test03 and test04.
// ---
bool Component::send(const char* const id, const int event, const int value[], SendData sd[], const int n)
{
   bool val = false;
   for (int i = 0; i < n; i++) {
      Object* vv = sd[i].getValue(value[i]);
      if (vv != nullptr) {
         Component* g = sd[i].getObject(this,id,(i+1));
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

// ---
// Send 'n' event messages with float type values to components.  The 'id' is used
// with std::sprintf() to create the component names.  Example: n = 4, id="test%02d"
// will generate component names test01, test02, test03 and test04.
// ---
bool Component::send(const char* const id, const int event, const float value[], SendData sd[], const int n)
{
   bool val = false;
   for (int i = 0; i < n; i++) {
      Object* vv = sd[i].getValue(value[i]);
      if (vv != nullptr) {
         Component* g = sd[i].getObject(this,id,(i+1));
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

// ---
// Send 'n' event messages with char type values to components.  The 'id' is used
// with std::sprintf() to create the component names.  Example: n = 4, id="test%02d"
// will generate component names test01, test02, test03 and test04.
// ---
bool Component::send(const char* const id, const int event, const double value[], SendData sd[], const int n)
{
   bool val = false;
   for (int i = 0; i < n; i++) {
      Object* vv = sd[i].getValue(value[i]);
      if (vv != nullptr) {
         Component* g = sd[i].getObject(this,id,(i+1));
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

bool Component::send(const char* const id, const int event, const bool value[], SendData sd[], const int n)
{
   bool val = false;
   for (int i = 0; i < n; i++) {
      Object* vv = sd[i].getValue(value[i]);
      if (vv != nullptr) {
         Component* g = sd[i].getObject(this,id,(i+1));
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

bool Component::send(const char* const id, const int event, const char* const value[], SendData sd[], const int n)
{
   bool val = false;
   for (int i = 0; i < n; i++) {
      Object* vv = sd[i].getValue(value[i]);
      if (vv != nullptr) {
         Component* g = sd[i].getObject(this,id,(i+1));
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

bool Component::send(const char* const id, const int event, Object* const value[], SendData sd[], const int n)
{
   bool val = false;
   for (int i = 0; i < n; i++) {
      if (value != nullptr) {
         Component* g = sd[i].getObject(this,id,(i+1));
         if (g != nullptr) val = g->event(event,value[i]);
      }
   }
   return val;
}



//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Object* Component::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Component::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        //indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    // List of components
    const PairStream* subcomponents = getComponents();
    if (subcomponents != nullptr) {
        indent(sout,i+j);
        sout << "components: {" << std::endl;
        subcomponents->serialize(sout,i+j+4,slotsOnly);
        indent(sout,i+j);
        sout << "}" << std::endl;
        subcomponents->unref();
        subcomponents = nullptr;
    }

    // Selected component
    if (selection != nullptr) {
        indent(sout,i+j);
        sout << "select: ";
        String* str = dynamic_cast<String*>(selection);
        if (str != nullptr) {
            sout << *str;
        }
        else {
            Number* num = dynamic_cast<Number*>(selection);
            if (num != nullptr) sout << num->getInt();
        }
        sout << std::endl;
    }

    // Event logger
    if (elog0 != nullptr) {
        indent(sout,i+j);
        sout << "logger: " << std::endl;
        elog0->serialize(sout,i+j,slotsOnly);
    }

    // enableTimingStats
    if (isTimingStatsEnabled()) {
        indent(sout,i+j);
        sout << "enableTimingStats: " << isTimingStatsEnabled() << std::endl;
    }

    // printTimingStats
    if (isTimingStatsPrintEnabled()) {
        indent(sout,i+j);
        sout << "printTimingStats: " << isTimingStatsPrintEnabled() << std::endl;
    }

    // Freeze
    if (isFrozen()) {
        indent(sout,i+j);
        sout << "freeze: " << isFrozen() << std::endl;
    }

    {
      unsigned short bits = getMessageEnableBits();

      // Message enable bits (only if anything other than just ERROR is set)
      if (bits != MSG_ERROR) {

         // Standard
         if ( (bits & MSG_WARNING) != 0 ) {
            indent(sout,i+j);
            sout << "enableMessageType:  WARNING" << std::endl;
         }
         if ( (bits & MSG_INFO) != 0 ) {
            indent(sout,i+j);
            sout << "enableMessageType:  INFO" << std::endl;
         }
         if ( (bits & MSG_DEBUG) != 0 ) {
            indent(sout,i+j);
            sout << "enableMessageType:  DEBUG" << std::endl;
         }
         if ( (bits & MSG_DATA) != 0 ) {
            indent(sout,i+j);
            sout << "enableMessageType:  DATA" << std::endl;
         }
         if ( (bits & MSG_USER) != 0 ) {
            indent(sout,i+j);
            sout << "enableMessageType:  USER" << std::endl;
         }

         // Non-standard
         if ( (bits & ~MSG_STD_ALL) != 0 ) {
            indent(sout,i+j);
            sout << "enableMessageType: " << std::hex << (bits & ~MSG_STD_ALL) << std::dec << std::endl;
         }
      }
    }

    {
      unsigned short bits = getMessageDisableBits();

      // Message disable bits (only if any are set)
      if (bits != 0) {

         // Standard
         if ( (bits & MSG_WARNING) != 0 ) {
            indent(sout,i+j);
            sout << "disableMessageType:  WARNING" << std::endl;
         }
         if ( (bits & MSG_INFO) != 0 ) {
            indent(sout,i+j);
            sout << "disableMessageType:  INFO" << std::endl;
         }
         if ( (bits & MSG_DEBUG) != 0 ) {
            indent(sout,i+j);
            sout << "disableMessageType:  DEBUG" << std::endl;
         }
         if ( (bits & MSG_DATA) != 0 ) {
            indent(sout,i+j);
            sout << "disableMessageType:  DATA" << std::endl;
         }
         if ( (bits & MSG_USER) != 0 ) {
            indent(sout,i+j);
            sout << "disableMessageType:  USER" << std::endl;
         }

         // Non-standard
         if ( (bits & ~MSG_STD_ALL) != 0 ) {
            indent(sout,i+j);
            sout << "disableMessageType: " << std::hex << (bits & ~MSG_STD_ALL) << std::dec << std::endl;
         }
      }
    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}


//==============================================================================
// class Component::SendData
//==============================================================================

// empty() the SendData structure
void Component::SendData::empty()
{
   obj = nullptr;
   if (past != nullptr) past->unref();
   past = nullptr;
}


// setObject() -- Set which object (component) we're sending to
void Component::SendData::setObject(Component* p)
{
   obj = p;
}


// getObject() -- Get/Find the object (component) we're sending to
Component* Component::SendData::getObject(Component* gobj, const char* const id, const int n)
{
    // Did we already find the object?
    if (obj == nullptr) {
        // No, then try to find it among our components ...
        Pair* p = nullptr;
        if (n <= 0) {
            // When n is zero (or less) just use 'id' as
            // the name when finding the object.
            p = gobj->findByName(id);
        }
        else {
            // ---
            // When n is greater than zero, use 'id' as a format
            // with 'n' to get the name.  Great for items in rows
            // or columns.
            //   Example:   n = 5
            //             id = xxx.%d
            //      gives name = xxx.5
            // ---
            char name[128];
            std::sprintf(name,id,n);
            p = gobj->findByName(name);
        }
        if (p != nullptr) obj = static_cast<Component*>(p->object());
    }
    return obj;
}

// getValue() -- get an object containing the int value to send
// or null(0) if the value hasn't changed.
Object* Component::SendData::getValue(const int value)
{
    Integer* num = dynamic_cast<Integer*>(past);
    if (num == nullptr) {
        if (past != nullptr) past->unref();
        past = new Integer(value);
        return past;
    }
    if (*num != value) {
        *num = value;
        return num;
    }
    else return nullptr;
}

// getValue() -- get an object containing the real value to send
// or null(0) if the value hasn't changed.
Object* Component::SendData::getValue(const float value)
{
    Float* num = dynamic_cast<Float*>(past);
    if (num == nullptr) {
        if (past != nullptr) past->unref();
        past = new Float(value);
        return past;
    }
    if (*num != static_cast<LCreal>(value)) {
        *num = value;
        return num;
    }
    else return nullptr;
}

Object* Component::SendData::getValue(const double value)
{
    Number* num = dynamic_cast<Number*>(past);
    if (num == nullptr) {
        if (past != nullptr) past->unref();
        past = new Float(value);
        return past;
    }
    if (num != nullptr && num->getDouble() != value) {
        num->setValue(value);
        return num;
    }
    else return nullptr;
}


// getValue() -- get an object containing the char string to send
// or null(0) if the value hasn't changed.
Object* Component::SendData::getValue(const char* const value)
{
    // get our past string
    String* str = dynamic_cast<String*>(past);
    if (str == nullptr) {
        if (past != nullptr) past->unref();
        past = new String(value);
        return past;
    }

    // Compare our new value to our past string.
    if (value != nullptr) {
        if (*str != value) {
            *str = value;
            return str;
        }
        else return nullptr;
    }

    // When our value is a null string, check if the past string was null
    else {
        if ( !str->isEmpty() ) {
            str->empty();
            return str;
        }
        else return nullptr;
    }
}

// getValue() -- get an object containing the boolean value to send
// or null(0) if the value hasn't changed.
Object* Component::SendData::getValue(const bool value)
{
    Boolean* num = dynamic_cast<Boolean*>(past);
    if (num == nullptr) {
        if (past != nullptr) past->unref();
        past = new Boolean(value);
        return past;
    }
    if (*num != value) {
        *num = value;
        return num;
    }
    else return nullptr;
}

} // End Basic namespace
} // End Eaagles namespace

