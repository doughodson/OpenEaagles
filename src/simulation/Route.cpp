// Route

#include "openeaagles/simulation/Route.h"

#include "openeaagles/simulation/Actions.h"
#include "openeaagles/simulation/Navigation.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Steerpoint.h"

#include "openeaagles/base/Identifier.h"
#include "openeaagles/base/LatLon.h"
#include "openeaagles/base/List.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Distances.h"
#include "openeaagles/base/units/Times.h"

#include <cstdio>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(Route,"Route")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Route)
    "to",               // 1) Initial "TO" steerpoint: by name (base::Identifier) or index (base::Number)
    "autoSequence",     // 2) Auto sequence flag
    "autoSeqDistance",  // 3) Distance to auto sequence    (base::Distance)
    "wrap",             // 4) Route wrap flag (wrap back to the beginning when past the end)
END_SLOTTABLE(Route)

// Map slot table to handles
BEGIN_SLOT_MAP(Route)
    ON_SLOT(1,setSlotTo,base::Identifier)
    ON_SLOT(1,setSlotTo,base::Number)
    ON_SLOT(2,setSlotAutoSequence,base::Number)
    ON_SLOT(3,setSlotAutoSeqDistance,base::Distance)
    ON_SLOT(3,setSlotAutoSeqDistance,base::Number)
    ON_SLOT(4,setSlotWrap,base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Event() map
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Route)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Route::Route()
{
    STANDARD_CONSTRUCTOR()

    to = nullptr;
    initToStptName = nullptr;

    initToStptIdx = 0;
    stptIdx = 0;
    autoSeqDistNM = 2.0;
    autoSeq = true;
    wrap = true;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Route::copyData(const Route& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        to = nullptr;
        initToStptName = nullptr;
    }

    to = nullptr; // find it using 'initToStptName' or 'initToStptIdx'

    {
        base::String* n = nullptr;
        if (org.initToStptName != nullptr) n = org.initToStptName->clone();
        initToStptName = n;
        if (n != nullptr) n->unref();  // safe_ptr<> has it
    }

    initToStptIdx = org.initToStptIdx;
    stptIdx = org.stptIdx;
    autoSeqDistNM = org.autoSeqDistNM;
    autoSeq = org.autoSeq;
    wrap = org.wrap;
}


//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Route::deleteData()
{
   directTo(static_cast<unsigned int>(0));
   initToStptName = nullptr;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Route::reset()
{
   BaseClass::reset();

   // ---
   // reset the initial 'to' steerpoint
   // ---
   directTo(static_cast<unsigned int>(0));
   base::PairStream* steerpoints = getComponents();
   if (steerpoints != nullptr) {

      // First try to find by name
      if (initToStptName != nullptr) {
         directTo(*initToStptName);
      }

      // Next try to find by index
      else if (initToStptIdx != 0) {
         directTo(initToStptIdx);
      }

      // We still don't have a 'to' steerpoint, then just use the first one
      if (to == nullptr) {
         directTo(1);
      }

      steerpoints->unref();
      steerpoints = nullptr;
   }
}

//------------------------------------------------------------------------------
// getNumberOfSteerpoints() -- returns the number of components (stpts) in our
// list
//------------------------------------------------------------------------------
unsigned int Route::getNumberOfSteerpoints() const
{
    return getNumberOfComponents();
}

//------------------------------------------------------------------------------
// updateData() -- update Non-time critical stuff here
//------------------------------------------------------------------------------
void Route::updateData(const double dt)
{
   BaseClass::updateData(dt);

   const Navigation* nav = static_cast<const Navigation*>(findContainerByType(typeid(Navigation)));
   if (nav != nullptr) {
      computeSteerpointData(dt,nav);
      autoSequencer(dt,nav);
   }
}

//------------------------------------------------------------------------------
// Compute nav steering data for each steerpoint.
//------------------------------------------------------------------------------
void Route::computeSteerpointData(const double, const Navigation* const nav)
{
   if (nav != nullptr) {
      base::PairStream* steerpoints = getComponents();
      if (steerpoints != nullptr) {

         // Until we pass the 'to' steerpoint, the 'from' pointer will be
         // null(0) and the steerpoint's compute() function will compute
         // direct-to the steerpoint.  After the 'to' steerpoint, the 'from'
         // pointer will help compute each from-to leg of the route.
         Steerpoint* from = nullptr;

         base::List::Item* item = steerpoints->getFirstItem();
         while (item != nullptr) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            Steerpoint* stpt = static_cast<Steerpoint*>(pair->object());
            stpt->compute(nav,from);
            if (pair == to || from != nullptr) from = stpt;
            item = item->getNext();
         }

         steerpoints->unref();
         steerpoints = nullptr;
      }
   }
}

//------------------------------------------------------------------------------
// Auto Sequence through Steerpoints
//------------------------------------------------------------------------------
void Route::autoSequencer(const double, const Navigation* const nav)
{
   if (isAutoSequence() && to != nullptr && nav != nullptr) {
      Steerpoint* toSP = static_cast<Steerpoint*>(to->object());
      if (toSP->getDistNM() <= autoSeqDistNM) {
         // We're within range of the steerpoint, so compute our relative
         // to see if we just passed it.
         const double rbrg = base::Angle::aepcdDeg(toSP->getTrueBrgDeg() - nav->getHeadingDeg());
         if ( std::fabs(rbrg) >= 90.0) {
            // We're within range and we're going away from it, so ...
            triggerAction();
            incStpt();
         }
      }
   }
}

//------------------------------------------------------------------------------
// trigger the 'to' steerpoint's action (if any)
//------------------------------------------------------------------------------
void Route::triggerAction()
{
   // ---
   // find and start the current 'to' steerpoint action
   // ---
   Player* own = static_cast<Player*>(findContainerByType(typeid(Player)));
   if (to != nullptr && own != nullptr) {
      Steerpoint* toSP = static_cast<Steerpoint*>(to->object());
      Action* toAction = toSP->getAction();
      if (toAction != nullptr) {
         OnboardComputer* obc = own->getOnboardComputer();
         if (obc != nullptr) obc->triggerAction(toAction);
      }
   }
}

//------------------------------------------------------------------------------
// Set auto sequence mode on/off
//------------------------------------------------------------------------------
bool Route::setAutoSequence(const bool flg)
{
   autoSeq = flg;
   return true;
}

//------------------------------------------------------------------------------
// Set wrap around mode on/off
//------------------------------------------------------------------------------
bool Route::setWrapEnable(const bool flg)
{
   wrap = flg;
   return true;
}

//------------------------------------------------------------------------------
// Auto sequence distance (NM)
//------------------------------------------------------------------------------
bool Route::setAutoSeqDistance(const double nm)
{
   autoSeqDistNM = nm;
   return true;
}

//------------------------------------------------------------------------------
// Change steerpoints
//------------------------------------------------------------------------------
bool Route::incStpt()
{
    bool ok = false;
    const base::PairStream* steerpoints = getComponents();
    if (steerpoints != nullptr) {
        unsigned int n = steerpoints->entries();
        unsigned int idx = stptIdx + 1;
        if (idx > n) idx = (wrap ? 1 : n);
        ok = directTo(idx);
        steerpoints->unref();
        steerpoints = nullptr;
    }
    return ok;
}

bool Route::decStpt()
{
    bool ok = false;
    const base::PairStream* steerpoints = getComponents();
    if (steerpoints != nullptr) {
        unsigned int n = steerpoints->entries();
        unsigned int idx = stptIdx - 1;
        if (idx < 1) idx = (wrap ? n : 1);
        ok = directTo(idx);
        steerpoints->unref();
        steerpoints = nullptr;
    }
    return ok;
}

//------------------------------------------------------------------------------
// Get the current 'to' steerpoint --
//------------------------------------------------------------------------------

const Steerpoint* Route::getSteerpointImp() const
{
    const Steerpoint* p = nullptr;
    if (to != nullptr) {
        p = static_cast<const Steerpoint*>(to->object());
    }
    return p;
}

const char* Route::getSteerpointName() const
{
    const char* name = nullptr;
    if (to != nullptr) {
        name = *to->slot();
    }
    return name;
}


//------------------------------------------------------------------------------
// directTo() -- Change to this steerpoint
//------------------------------------------------------------------------------
bool Route::directTo(const Steerpoint* const stpt)
{
    bool ok = false;
    base::PairStream* steerpoints = getComponents();
    if (steerpoints != nullptr && stpt != nullptr) {
        // When we have steerpoints (components) and a steerpoint to switch to ...
        base::Pair* sp = findSteerpoint(stpt);
        if (sp != nullptr) {
            to = sp;
            stptIdx = steerpoints->getIndex(sp);
            ok = true;
        }
    }

    else if (stpt == nullptr) {
      to = nullptr;
      stptIdx = 0;
      ok = true;
    }

    if (steerpoints != nullptr) {
       steerpoints->unref();
       steerpoints = nullptr;
    }

    return ok;
}

bool Route::directTo(const char* const name)
{
    bool ok = false;
    base::PairStream* steerpoints = getComponents();
    if (steerpoints != nullptr && name != nullptr) {
        // When we have steerpoints (components) and a name of a steerpoint
        base::Pair* sp = findSteerpoint(name);
        if (sp != nullptr) {
            to = sp;
            stptIdx = steerpoints->getIndex(sp);
            ok = true;
        }
    }

    else if (name == nullptr) {
      to = nullptr;
      stptIdx = 0;
      ok = true;
    }

    if (steerpoints != nullptr) {
       steerpoints->unref();
       steerpoints = nullptr;
    }

    return ok;
}

bool Route::directTo(const unsigned int idx)
{
    bool ok = false;

    if (idx != 0) {
        base::Pair* sp = findSteerpoint(idx);
        if (sp != nullptr) {
            to = sp;
            stptIdx = idx;
            ok = true;
        }
    }

    else if (idx == 0) {
      to = nullptr;
      stptIdx = 0;
      ok = true;
    }

    return ok;
}


//------------------------------------------------------------------------------
// findSteerpoint() -- Find a steerpoint (pair)
//------------------------------------------------------------------------------

const base::Pair* Route::findSteerpointImp(const Steerpoint* const stpt) const
{
    const base::Pair* sp = nullptr;
    const base::PairStream* steerpoints = getComponents();
    if (steerpoints != nullptr && stpt != nullptr) {
        const base::List::Item* item = steerpoints->getFirstItem();
        while (item != nullptr && sp == nullptr) {
            const base::Pair* pair = static_cast<const base::Pair*>(item->getValue());
            const Steerpoint* p = static_cast<const Steerpoint*>(pair->object());
            if (stpt == p) sp = pair;
            item = item->getNext();
        }
    }

    if (steerpoints != nullptr) {
       steerpoints->unref();
       steerpoints = nullptr;
    }

    return sp;
}

const base::Pair* Route::findSteerpointImp(const char* const name) const
{
    const base::Pair* sp = nullptr;
    const base::PairStream* steerpoints = getComponents();
    if (steerpoints != nullptr && name != nullptr) {
        sp = steerpoints->findByName(name);
    }

    if (steerpoints != nullptr) {
       steerpoints->unref();
       steerpoints = nullptr;
    }

    return sp;
}

const base::Pair* Route::findSteerpointImp(const unsigned int idx) const
{
    const base::Pair* sp = nullptr;
    const base::PairStream* steerpoints = getComponents();
    if (steerpoints != nullptr) {
        sp = steerpoints->getPosition(idx);
        steerpoints->unref();
        steerpoints = nullptr;
    }
    return sp;
}

//------------------------------------------------------------------------------
// getSteerpoints() -- Get the route we're flying to (starting at 'to')
//------------------------------------------------------------------------------
unsigned int Route::getSteerpoints(base::safe_ptr<Steerpoint>* const stptList, const unsigned int max)
{
    unsigned int i = 0;
    base::PairStream* steerpoints = getComponents();
    if (stptList != nullptr && max > 0 && steerpoints != nullptr) {

        // Find our 'to' steerpoint
        bool found = false;
        base::List::Item* item = steerpoints->getFirstItem();
        while (item != nullptr && !found) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            found = (pair == to);
            if (!found) {
                item = item->getNext();
            }
        }

        // Get the route we're flying 'to'
        while (item != nullptr && i < max) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            Steerpoint* p = dynamic_cast<Steerpoint*>(pair->object());
            if (p != nullptr) {
                stptList[i++] = p;
            }
            item = item->getNext();
        }
    }

    if (steerpoints != nullptr) {
       steerpoints->unref();
       steerpoints = nullptr;
    }

    return i;
}

//------------------------------------------------------------------------------
// getAllSteerpoints() -- Get all of the steerpoints in the route
//------------------------------------------------------------------------------
unsigned int Route::getAllSteerpoints(base::safe_ptr<Steerpoint>* const stptList, const unsigned int max)
{
    unsigned int i = 0;
    base::PairStream* steerpoints = getComponents();
    if (stptList != nullptr && max > 0 && steerpoints != nullptr) {
        base::List::Item* item = steerpoints->getFirstItem();
        while (item != nullptr && i < max) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            Steerpoint* p = dynamic_cast<Steerpoint*>(pair->object());
            if (p != nullptr) {
                stptList[i++] = p;
            }
            item = item->getNext();
        }
    }

    if (steerpoints != nullptr) {
       steerpoints->unref();
       steerpoints = nullptr;
    }

    return i;
}

//------------------------------------------------------------------------------
// insertSteerpoint() -
//------------------------------------------------------------------------------
bool Route::insertSteerpoint(Steerpoint* const newStpt, const int pos)
{
    bool ok = false;
    unsigned int num = getNumberOfSteerpoints();
    // make a new character string
    char numString[10];

    // this tells us the number of the next steerpoint to be created in the slot list
    std::sprintf(numString,"%i", static_cast<int>(num+1));

    // now we have the slot name, which is the next number in the steerpoint list
    // now create a new pair, and if we have a component list, add it to it, if
    // not, then create a new component list
    base::Pair* p = new base::Pair(numString, newStpt);
    if (p != nullptr) {

        // We're its container
        newStpt->container(this);

        // Get our steerpoints
        base::PairStream* steerpoints = getComponents();

        // now we have to add it to our component list
        if (steerpoints != nullptr && num != 0) {

            // Copy the current steerpoint list
            base::PairStream* tempList = new base::PairStream();
            {
               base::List::Item* item = steerpoints->getFirstItem();
               while (item != nullptr) {
                  base::Pair* pair = (base::Pair*) item->getValue();
                  tempList->put(pair);
                  item = item->getNext();
               }
            }

            if (pos == -1) {
               tempList->addHead(p);
               ok = true;
            }

            else if (pos == 0 || pos > num) {
               tempList->addTail(p);
               ok = true;
            }

            else if (pos > 0 && pos <= static_cast<int>(num)) {

                // count to our position, then insert it
                int counter = 1;
                base::List::Item* item = tempList->getFirstItem();
                while (counter < pos && item != nullptr) {
                    item = item->getNext();
                    counter++;
                }

                // now we should have the reference pair at the 'pos' position
                if (item != nullptr) {
                    base::List::Item* newItem = new base::List::Item;
                    newItem->value = p;
                    p->ref();
                    // insert 'newItem' just before 'item'
                    ok = tempList->insert(newItem, item);
                }
            }

            // swap our current steerpoint (components) list for this new one
            if (ok) {
               base::Component::processComponents(tempList,typeid(Steerpoint));
            }

            tempList->unref();
            tempList = nullptr;

        }

        // if we have no components, we need to start a new component list
        else {
            base::Component::processComponents(nullptr, typeid(Steerpoint), p);
            ok = true;
        }

        // Unref the original steerpoint list
        if (steerpoints != nullptr) {
            steerpoints->unref();
            steerpoints = nullptr;
        }

        p->unref();  // Our component list has it now.
    }

    // ---
    // Call directTo() to reset the steerpoint index, or if we were going nowhere
    // then go direct-to steerpoint one.
    // ---
    if (ok) {
       if (to != nullptr) {
         Steerpoint* sp = static_cast<Steerpoint*>(to->object());
         directTo(sp);
       }
       else {
         directTo(1);
       }
    }

    return ok;

}

//------------------------------------------------------------------------------
// Replace the complete steerpoint list with a new one
//------------------------------------------------------------------------------
bool Route::replaceAllSteerpoints(base::PairStream* const newSteerpointList, unsigned int newStptIdx)
{
   bool ok = false;

   if (newSteerpointList != nullptr) {

      base::Component::processComponents(newSteerpointList, typeid(Steerpoint));

      // Try to force a 'Direct to' the new 'stptIdx' or default to stpt #1
      directTo(static_cast<unsigned int>(0));
      if ( !directTo(newStptIdx) ) directTo(1);

      ok = true;
   }

   return ok;
}

//------------------------------------------------------------------------------
// deleteSteerpoint() - goes through and deletes the steerpoint if there is a match
//------------------------------------------------------------------------------
bool Route::deleteSteerpoint(Steerpoint* const sp)
{
   // get a pointer to our current 'to' steerpoint
   const Steerpoint* p = getSteerpoint();

   // remove the steerpoint
   base::PairStream* steerpoints = getComponents();
   base::Component::processComponents(steerpoints,typeid(Steerpoint),nullptr,sp);
   if (steerpoints != nullptr) {
      steerpoints->unref();
      steerpoints = nullptr;
   }

   // When we just deleted our current 'to' steerpoint,
   // force a new 'direct to' using stptIdx
   if (p == sp) {
      unsigned int idx = stptIdx;
      directTo(static_cast<unsigned int>(0));
      while ( !directTo(idx) && idx > 0 ) idx--;
   }

   // Otherwise just force a new 'direct to' using our current
   // 'to' steerpoint to make sure our stptIdx is correct.
   else {
      directTo(p);
   }

   return true;
}

//------------------------------------------------------------------------------
// deleteAllSteerpoints() - deletes all of the steerpoints
//------------------------------------------------------------------------------
bool Route::deleteAllSteerpoints()
{
   // This will create a new null(0) steerpoint (components) list
   base::Component::processComponents(nullptr, typeid(Steerpoint));

   // No steerpoints, so we're going nowhere
   directTo(static_cast<unsigned int>(0));

   return true;
}

//------------------------------------------------------------------------------
// processComponents() -- process our components; make sure they are all of
//    type Steerpoint (or derived); tell them that we are their container
//------------------------------------------------------------------------------
void Route::processComponents(
      base::PairStream* const list,
      const std::type_info&,
      base::Pair* const add,
      base::Component* const remove
   )
{
   base::Component::processComponents(list, typeid(Steerpoint), add, remove);
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Route::setSlotTo(const base::Identifier* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initToStptName = msg;
        ok = true;
    }
    return ok;
}

bool Route::setSlotTo(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initToStptIdx = msg->getInt();
        ok = true;
    }
    return ok;
}

bool Route::setSlotAutoSequence(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        autoSeq = msg->getBoolean();
        ok = true;
    }
    return ok;
}

bool Route::setSlotAutoSeqDistance(const base::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        autoSeqDistNM = base::NauticalMiles::convertStatic(*msg);
        ok = true;
    }
    return ok;
}
bool Route::setSlotAutoSeqDistance(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        // we are assuming nautical miles here
        autoSeqDistNM = msg->getReal();
        ok = true;
    }
    return ok;
}

bool Route::setSlotWrap(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        wrap = msg->getBoolean();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* Route::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Route::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{

    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    if (initToStptName != nullptr) {
        indent(sout, i+j);
        sout << "to: " << *initToStptName << std::endl;
    }

    // auto sequence
    indent(sout, i+j);
    sout << "autoSequence: " << (isAutoSequence() ? "true" : "false") << std::endl;

    // auto sequence distance
    indent(sout, i+j);
    sout << "autoSeqDistance: ( NauticalMiles " << getAutoSeqDistance() << " )" << std::endl;

    // wrap
    indent(sout, i+j);
    sout << "wrap: " << (isWrapEnabled() ? "true" : "false") << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End simulation namespace
} // End oe namespace
