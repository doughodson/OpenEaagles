// Route

#include "openeaagles/simulation/Route.h"

#include "openeaagles/simulation/Actions.h"
#include "openeaagles/simulation/Navigation.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Steerpoint.h"

#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/LatLon.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(Route,"Route")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Route)
    "to",               // 1) Initial "TO" steerpoint: by name (Basic::Identifier) or index (Basic::Number)
    "autoSequence",     // 2) Auto sequence flag
    "autoSeqDistance",  // 3) Distance to auto sequence    (Basic::Distance)
    "wrap",             // 4) Route wrap flag (wrap back to the beginning when past the end)
END_SLOTTABLE(Route)

// Map slot table to handles 
BEGIN_SLOT_MAP(Route)
    ON_SLOT(1,setSlotTo,Basic::Identifier)
    ON_SLOT(1,setSlotTo,Basic::Number)
    ON_SLOT(2,setSlotAutoSequence,Basic::Number)
    ON_SLOT(3,setSlotAutoSeqDistance,Basic::Distance)
	ON_SLOT(3,setSlotAutoSeqDistance,Basic::Number)
    ON_SLOT(4,setSlotWrap,Basic::Number)
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

    to = 0;
    initToStptName = 0;

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
        to = 0;
        initToStptName = 0;
    }

    to = 0; // find it using 'initToStptName' or 'initToStptIdx'
    
    Basic::String* n = 0;
    if (org.initToStptName != 0) n = (Basic::String*) org.initToStptName->clone();
    initToStptName = n;

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
   directTo((unsigned int) 0);
   initToStptName = 0;
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
   directTo((unsigned int) 0);
   Basic::PairStream* steerpoints = getComponents();
   if (steerpoints != 0) {

      // First try to find by name
      if (initToStptName != 0) {
         directTo(*initToStptName);
      }

      // Next try to find by index
      else if (initToStptIdx != 0) {
         directTo(initToStptIdx);
      }

      // We still don't have a 'to' steerpoint, then just use the first one
      if (to == 0) {
         directTo(1);
      }

      steerpoints->unref();
      steerpoints = 0;
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
void Route::updateData(const LCreal dt)
{
   BaseClass::updateData(dt);

   const Navigation* nav = (const Navigation*) findContainerByType(typeid(Navigation));
   if (nav != 0) {
      computeSteerpointData(dt,nav);
      autoSequencer(dt,nav);
   }
}

//------------------------------------------------------------------------------
// Compute nav steering data for each steerpoint.
//------------------------------------------------------------------------------
void Route::computeSteerpointData(const LCreal, const Navigation* const nav)
{
   if (nav != 0) {
      Basic::PairStream* steerpoints = getComponents();
      if (steerpoints != 0) {

         // Until we pass the 'to' steerpoint, the 'from' pointer will be
         // null(0) and the steerpoint's compute() function will compute
         // direct-to the steerpoint.  After the 'to' steerpoint, the 'from'
         // pointer will help compute each from-to leg of the route.
         Steerpoint* from = 0;

         Basic::List::Item* item = steerpoints->getFirstItem();
         while (item != 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Steerpoint* stpt = (Steerpoint*)( pair->object() );
            stpt->compute(nav,from);
            if (pair == to || from != 0) from = stpt;
            item = item->getNext();
         }

         steerpoints->unref();
         steerpoints = 0;
      }
   }
}

//------------------------------------------------------------------------------
// Auto Sequence through Steerpoints
//------------------------------------------------------------------------------
void Route::autoSequencer(const LCreal, const Navigation* const nav)
{
   if (isAutoSequence() && to != 0 && nav != 0) {
      Steerpoint* toSP = (Steerpoint*) (to->object());
      if (toSP->getDistNM() <= autoSeqDistNM) {
         // We're within range of the steerpoint, so compute our relative
         // to see if we just passed it.
         double rbrg = Basic::Angle::aepcdDeg(toSP->getTrueBrgDeg() - nav->getHeadingDeg());
         if ( std::fabs(rbrg) >= 90.0f) {
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
   // find and start the current 'to' steerpont action 
   // ---
   Player* own = (Player*) findContainerByType(typeid(Player));
   if (to != 0 && own != 0) {
      Steerpoint* toSP = (Steerpoint*) (to->object());
      Action* toAction = toSP->getAction();
      if (toAction != 0) {
         OnboardComputer* obc = own->getOnboardComputer();
         if (obc != 0) obc->triggerAction(toAction);
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
    const Basic::PairStream* steerpoints = getComponents();
    if (steerpoints != 0) {
        unsigned int n = steerpoints->entries();
        unsigned int idx = stptIdx + 1;
        if (idx > n) idx = (wrap ? 1 : n);
        ok = directTo(idx);
        steerpoints->unref();
        steerpoints = 0;
    }
    return ok;
}

bool Route::decStpt()
{
    bool ok = false;
    const Basic::PairStream* steerpoints = getComponents();
    if (steerpoints != 0) {
        unsigned int n = steerpoints->entries();
        unsigned int idx = stptIdx - 1;
        if (idx < 1) idx = (wrap ? n : 1);
        ok = directTo(idx);
        steerpoints->unref();
        steerpoints = 0;
    }
    return ok;
}

//------------------------------------------------------------------------------
// Get the current 'to' steerpoint --
//------------------------------------------------------------------------------

const Steerpoint* Route::getSteerpointImp() const
{
    Steerpoint* p = 0;
    if (to != 0) {
        p = (Steerpoint*) to->object();
    }
    return p;
}

const char* Route::getSteerpointName() const
{
    const char* name = 0;
    if (to != 0) {
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
    Basic::PairStream* steerpoints = getComponents();
    if (steerpoints != 0 && stpt != 0) {
        // When we have steerpoints (components) and a steerpoint to switch to ...
        Basic::Pair* sp = findSteerpoint(stpt);
        if (sp != 0) {
            to = sp;
            stptIdx = steerpoints->getIndex(sp);
            ok = true;
        }
        steerpoints->unref();
        steerpoints = 0;
    }

    else if (stpt == 0) {
      to = 0;
      stptIdx = 0;
      ok = true;
    }

    return ok;
}

bool Route::directTo(const char* const name)
{
    bool ok = false;
    Basic::PairStream* steerpoints = getComponents();
    if (steerpoints != 0 && name != 0) {
        // When we have steerpoints (components) and a name of a steerpoint
        Basic::Pair* sp = findSteerpoint(name);
        if (sp != 0) {
            to = sp;
            stptIdx = steerpoints->getIndex(sp);
            ok = true;
        }
        steerpoints->unref();
        steerpoints = 0;
    }

    else if (name == 0) {
      to = 0;
      stptIdx = 0;
      ok = true;
    }

    return ok;
}

bool Route::directTo(const unsigned int idx)
{
    bool ok = false;

    if (idx != 0) {
        Basic::Pair* sp = findSteerpoint(idx);
        if (sp != 0) {
            to = sp;
            stptIdx = idx;
            ok = true;
        }
    }

    else if (idx == 0) {
      to = 0;
      stptIdx = 0;
      ok = true;
    }

    return ok;
}


//------------------------------------------------------------------------------
// findSteerpoint() -- Find a steerpoint (pair)
//------------------------------------------------------------------------------

const Basic::Pair* Route::findSteerpointImp(const Steerpoint* const stpt) const
{
    const Basic::Pair* sp = 0;
    const Basic::PairStream* steerpoints = getComponents();
    if (steerpoints != 0 && stpt != 0) {
        const Basic::List::Item* item = steerpoints->getFirstItem();
        while (item != 0 && sp == 0) {
            const Basic::Pair* pair = (const Basic::Pair*)(item->getValue());
            const Steerpoint* p = (const Steerpoint*)(pair->object());
            if (stpt == p) sp = pair;
            item = item->getNext();
        }
        steerpoints->unref();
        steerpoints = 0;
    }
    return sp;
}

const Basic::Pair* Route::findSteerpointImp(const char* const name) const
{
    const Basic::Pair* sp = 0;
    const Basic::PairStream* steerpoints = getComponents();
    if (steerpoints != 0 && name != 0) {
        sp = steerpoints->findByName(name);
        steerpoints->unref();
        steerpoints = 0;
    }
    return sp;
}

const Basic::Pair* Route::findSteerpointImp(const unsigned int idx) const
{
    const Basic::Pair* sp = 0;
    const Basic::PairStream* steerpoints = getComponents();
    if (steerpoints != 0) {
        sp = steerpoints->getPosition(idx);
        steerpoints->unref();
        steerpoints = 0;
    }
    return sp;
}

//------------------------------------------------------------------------------
// getSteerpoints() -- Get the route we're flying to (starting at 'to')
//------------------------------------------------------------------------------
unsigned int Route::getSteerpoints(SPtr<Steerpoint>* const stptList, const unsigned int max)
{
    unsigned int i = 0;
    Basic::PairStream* steerpoints = getComponents();
    if (stptList != 0 && max > 0 && steerpoints != 0) {
    
        // Find our 'to' steerpoint
        bool found = false;
        Basic::List::Item* item = steerpoints->getFirstItem();
        while (item != 0 && !found) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            found = (pair == to);
            if (!found) {
                item = item->getNext();
            }
        }

        // Get the route we're flying 'to'
        while (item != 0 && i < max) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Steerpoint* p = dynamic_cast<Steerpoint*>(pair->object());
            if (p != 0) {
                stptList[i++] = p;
            }
            item = item->getNext();
        }
        steerpoints->unref();
        steerpoints = 0;
    }
    return i;
}

//------------------------------------------------------------------------------
// getAllSteerpoints() -- Get all of the steerpoints in the route
//------------------------------------------------------------------------------
unsigned int Route::getAllSteerpoints(SPtr<Steerpoint>* const stptList, const unsigned int max)
{
    unsigned int i = 0;
    Basic::PairStream* steerpoints = getComponents();
    if (stptList != 0 && max > 0 && steerpoints != 0) {
        Basic::List::Item* item = steerpoints->getFirstItem();
        while (item != 0 && i < max) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Steerpoint* p = dynamic_cast<Steerpoint*>(pair->object());
            if (p != 0) {
                stptList[i++] = p;
            }
            item = item->getNext();
        }
        steerpoints->unref();
        steerpoints = 0;
    }
    return i;
}

//------------------------------------------------------------------------------
// insertSteerpoint() -
//------------------------------------------------------------------------------
bool Route::insertSteerpoint(Steerpoint* const newStpt, const unsigned int pos)
{
    bool ok = false;
    unsigned int num = getNumberOfSteerpoints();
    // make a new character string
    char numString[10];

    // this tells us the number of the next steerpoint to be created in the slot list
    sprintf(numString,"%i",(num+1));

    // now we have the slot name, which is the next number in the steerpoint list
    // now create a new pair, and if we have a component list, add it to it, if
    // not, then create a new component list
    Basic::Pair* p = new Basic::Pair(numString, newStpt);
    if (p != 0) {

        // We're its container
        newStpt->container(this);

        // Get our steerpoints
        Basic::PairStream* steerpoints = getComponents();

        // now we have to add it to our component list
        if (steerpoints != 0 && num != 0) {

            // Copy the current steerpoint list
            Basic::PairStream* tempList = new Basic::PairStream();
            {
               Basic::List::Item* item = steerpoints->getFirstItem();
               while (item != 0) {
                  Basic::Pair* pair = (Basic::Pair*) item->getValue();
                  tempList->put(pair);
                  item = item->getNext();
               }
            }

            if (pos == -1) {
               tempList->addHead(p);
               ok = true;
            }

            else if (pos == 0 || pos == (num+1)) {
               tempList->addTail(p);
               ok = true;
            }

            else if (pos > 0 && pos <= num) {

                // count to our position, then insert it
                unsigned int counter = 1;
                Basic::List::Item* item = tempList->getFirstItem();
                while (counter < pos && item != 0) {
                    item = item->getNext();
                    counter++;
                }

                // now we should have the reference pair at the 'pos' position
                if (item != 0) {
                    Basic::List::Item* newItem = new Basic::List::Item;
                    newItem->value = p;
                    // insert 'newItem' just before 'item'
                    ok = tempList->insert(newItem, item);
                }
            }

            // swap our current steerpoint (components) list for this new one
            if (ok) BaseClass::processComponents(tempList,typeid(Steerpoint));

            tempList->unref();
            tempList = 0;

            steerpoints->unref();
            steerpoints = 0;
        }

        // if we have no components, we need to start a new component list
        else if (pos <= 1) {
            BaseClass::processComponents(0,typeid(Steerpoint),p);
            ok = true;
        }

    }

    // ---
    // if we were going nowhere, force a direct-to 
    // ---
    if (ok) {
       if (to != 0) {
         Steerpoint* sp = (Steerpoint*) to->object();
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
bool Route::replaceAllSteerpoints(Basic::PairStream* const newSteerpointList, unsigned int newStptIdx)
{
   bool ok = false;

   if (newSteerpointList != 0) {

      BaseClass::processComponents(newSteerpointList,typeid(Steerpoint));

      // Try to force a 'Direct to' the new 'stptIdx' or default to stpt #1 
      directTo((unsigned int) 0);
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
   Basic::PairStream* steerpoints = getComponents();
   BaseClass::processComponents(steerpoints,typeid(Steerpoint),0,sp);
   steerpoints->unref(); steerpoints = 0;

   // When we just deleted our current 'to' steerpoint,
   // force a new 'direct to' using stptIdx
   if (p == sp) {
      unsigned int idx = stptIdx;
      directTo((unsigned int) 0);
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
   BaseClass::processComponents(0,typeid(Steerpoint));

   // No steerpoints, so we're going nowhere
   directTo((unsigned int) 0);

   return true;
}

//------------------------------------------------------------------------------
// processComponets() -- process our components; make sure they are all of
//	type Steerpoint (or derived); tell them that we are their container
//------------------------------------------------------------------------------
void Route::processComponents(
      Basic::PairStream* const list,
      const std::type_info&,
      Basic::Pair* const add,
      Basic::Component* const remove
   )
{
   BaseClass::processComponents(list,typeid(Steerpoint),add,remove);
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Route::setSlotTo(const Basic::Identifier* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initToStptName = msg;
        ok = true;
    }
    return ok;
}

bool Route::setSlotTo(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initToStptIdx = msg->getInt();
        ok = true;
    }
    return ok;
}

bool Route::setSlotAutoSequence(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        autoSeq = msg->getBoolean();
        ok = true;
    }
    return ok;
}

bool Route::setSlotAutoSeqDistance(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
        autoSeqDistNM = Basic::NauticalMiles::convertStatic(*msg);
        ok = true;
    }
    return ok;
}
bool Route::setSlotAutoSeqDistance(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        // we are assuming nautical miles here
        autoSeqDistNM = msg->getReal();
        ok = true;
    }
    return ok;
}

bool Route::setSlotWrap(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        wrap = msg->getBoolean();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Route::getSlotByIndex(const int si)
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
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    if (initToStptName != 0) {
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

} // End Simulation namespace
} // End Eaagles namespace
