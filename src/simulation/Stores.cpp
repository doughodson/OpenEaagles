// Class Stores

#include "openeaagles/simulation/Stores.h"
#include "openeaagles/simulation/Weapon.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class Stores
//==============================================================================
IMPLEMENT_SUBCLASS(Stores,"Stores")

// Slot table
BEGIN_SLOTTABLE(Stores)
   "numStations",  //  1: Number of stations (less than or equal MAX_STATIONS)
   "stores",      //  2: Our weapons and other external stores
   "selected"     //  3: Selected weapon station number
END_SLOTTABLE(Stores)

// Map slot table to handles 
BEGIN_SLOT_MAP(Stores)
   ON_SLOT( 1, setSlotNumStations,   Basic::Number)
   ON_SLOT( 2, setSlotStores,   Basic::PairStream)
   ON_SLOT( 3, setSlotSelected,   Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Event() map
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Stores)
    ON_EVENT_OBJ( JETTISON_EVENT, onJettisonEvent, Weapon )
    ON_EVENT_OBJ( JETTISON_EVENT, onJettisonEvent, ExternalStore )
END_EVENT_HANDLER()


//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
Stores::Stores()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Stores::initData()
{
   storesList = 0;

   for (unsigned int s = 0; s < MAX_STATIONS; s++) {
      weaponTbl[s] = 0;
      esTbl[s] = 0;
   }
   numWpn = 0;
   numEs = 0;
   ns = 0;
   selected = 0;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Stores::copyData(const Stores& org, const bool cc)
{
   Object::copyData(org);
   if (cc) initData();

   ns = org.ns;
   selected = org.selected;
   setSlotStores(org.storesList); // setSlotStores() clones the stores
}

void Stores::deleteData()
{
   setSlotStores(0);
   setNumberOfStations(0);
}


//------------------------------------------------------------------------------
// Reset()
//------------------------------------------------------------------------------
void Stores::reset()
{
   BaseClass::reset();

   // Reset all of the stores
   Basic::PairStream* stores = getStores();
   if (stores != 0) {
      resetStores(stores);
      stores->unref();
      stores = 0;
   }
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void Stores::process(const LCreal dt)
{
   // check our blocked flags
   updateBlockedFlags();

   BaseClass::process(dt);
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void Stores::updateTC(const LCreal dt)
{
   // Update our non-weapon, external stores, which need to act as
   // active systems attached to our ownship player.
   {
      Basic::PairStream* list = getStores();
      if (list != 0) {
         Basic::List::Item* item = list->getFirstItem();
         while (item != 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            ExternalStore* p = dynamic_cast<ExternalStore*>( pair->object() );
            if (p != 0) p->updateTC(dt);
            item = item->getNext();
         }
         list->unref();
         list = 0;
      }
   }

   BaseClass::updateTC(dt);
}

//------------------------------------------------------------------------------
// updateData() -- update non-time critical stuff here
//------------------------------------------------------------------------------
void Stores::updateData(const LCreal dt)
{
   // Update our non-weapon, external stores, which need to act as
   // active systems attached to our ownship player.
   {
      Basic::PairStream* list = getStores();
      if (list != 0) {
         Basic::List::Item* item = list->getFirstItem();
         while (item != 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            ExternalStore* p = dynamic_cast<ExternalStore*>( pair->object() );
            if (p != 0) p->updateData(dt);
            item = item->getNext();
         }
         list->unref();
         list = 0;
      }
   }

   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Number of stations on the launcher
unsigned int Stores::getNumberOfStations() const
{
   return ns;
}

// Returns the list of our external stores
Basic::PairStream* Stores::getStores()
{
   return storesList.getRefPtr();
}

// Returns the list of our external stores (const version)
const Basic::PairStream* Stores::getStores() const
{
   return storesList.getRefPtr();
}

// Number of weapons on the launcher
unsigned int Stores::getNumberOfWeapons() const
{
   return numWpn;
}

// Returns the number of weapons available for launch
unsigned int Stores::available() const
{
   unsigned int n = 0;
   for (unsigned int s = 1; s <= ns; s++) {
      if ( isWeaponAvailable(s) ) n++;
   }
   return n;
}

// Selected station number or zero if no station is selected
unsigned int Stores::getSelectedStation() const
{
   return selected;
}

// Default weapon availability function
bool Stores::isWeaponAvailable(const unsigned int s) const
{
   // Map 's' to a station array index
   int idx = mapSta2Idx(s);

   // get the weapon
   bool isAvail = false;
   if (idx >= 0 && weaponTbl[idx] != 0) {
      const Weapon* wpn = weaponTbl[idx]->getPointer();

      // Reasons why the weapon may not be available ...
      bool notAvail = wpn->isReleased() || wpn->isBlocked() || wpn->isJettisoned() || wpn->isFailed() || wpn->isHung();

      // and it is if it is not not ;-)
      isAvail = !notAvail;

      wpn->unref();
   }
   return isAvail;
}

// Return a weapon by station (const version)
const Weapon* Stores::getWeapon(const unsigned int s) const
{
   // Map 's' to a station array index
   int idx = mapSta2Idx(s);

   // get the weapon
   const Weapon* wpn = 0;
   if (idx >= 0 && weaponTbl[idx] != 0) {
      wpn = weaponTbl[idx]->getPointer();
   }
   return wpn;
}

// Return a weapon by station (const version)
Weapon* Stores::getWeapon(const unsigned int s)
{
   // Map 's' to a station array index
   int idx = mapSta2Idx(s);

   // get the weapon
   Weapon* wpn = 0;
   if (idx >= 0 && weaponTbl[idx] != 0) {
      wpn = weaponTbl[idx]->getPointer();
   }
   return wpn;
}

// Return a external store by station (const version)
const ExternalStore* Stores::getExternalStore(const unsigned int s) const
{
   // Map 's' to a station array index
   int idx = mapSta2Idx(s);

   const ExternalStore* p = 0;
   if (idx >= 0) p = esTbl[idx].getRefPtr();

   return p;
}

// Return a external store by station (const version)
ExternalStore* Stores::getExternalStore(const unsigned int s)
{
   // Map 's' to a station array index
   int idx = mapSta2Idx(s);

   ExternalStore* p = 0;
   if (idx >= 0) p = esTbl[idx].getRefPtr();

   return p;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Set the number of stations on this launcher
bool Stores::setNumberOfStations(const unsigned int n)
{ 
   ns = n; 
   return true;
}


// Select station number 's'; even if its weapon is not available for
// release.  If 's' is an invalid then the station remains unchanged and
// false is returned.
bool Stores::selectStation(const unsigned int s)
{
   bool ok = false;
   if (s >= 1 && s <= ns) {
      selected = s;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Default function to pre-release a weapon 
//------------------------------------------------------------------------------

// By weapon
Weapon* Stores::prereleaseWeapon(Weapon* const wpn)
{
   Weapon* flyout = 0;

   Player* own = getOwnship();
   if (wpn != 0 && own != 0) {

      // Release the weapon
      wpn->setLaunchVehicle(own);
      flyout = wpn->prerelease();

   }

   return flyout;
}

// By station
Weapon* Stores::prereleaseWeapon(const unsigned int s)
{
   Weapon* flyout = 0;

   Weapon* wpn = getWeapon(s);
   if (wpn != 0) {
      flyout = prereleaseWeapon(wpn);
      wpn->unref();
   }

   return flyout;
}

//------------------------------------------------------------------------------
// Default functions to release a weapon 
//------------------------------------------------------------------------------

// By weapon
Weapon* Stores::releaseWeapon(Weapon* const wpn)
{
   Weapon* flyout = 0;

   Player* own = getOwnship();
   if (wpn != 0 && own != 0) {

      // Release the weapon
      wpn->setLaunchVehicle(own);
      flyout = wpn->release();

   }

   return flyout;
}

// By station
Weapon* Stores::releaseWeapon(const unsigned int s)
{
   Weapon* flyout = 0;

   Weapon* wpn = getWeapon(s);
   if (wpn != 0) {
      flyout = releaseWeapon(wpn);
      wpn->unref();
   }

   return flyout;
}

//------------------------------------------------------------------------------
// Default function to update the weapon blocked flags
//------------------------------------------------------------------------------
void Stores::updateBlockedFlags()
{
}

//------------------------------------------------------------------------------
// Default function to jettison all jettisonable stores
//------------------------------------------------------------------------------
bool Stores::jettisonAll()
{
   // Notify the external stores that we're shutting down
   Basic::PairStream* list = getStores();
   if (list != 0) {
      Basic::List::Item* item = list->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         Basic::Component* p = (Basic::Component*)( pair->object() );
         p->event(JETTISON_EVENT);
         item = item->getNext();
      }
      list->unref();
      list = 0;
   }
   return true;
}

//------------------------------------------------------------------------------
// assignWeaponToStation() -- 
// Station numbers range from 1 to getNumberOfStations()
//------------------------------------------------------------------------------
bool Stores::assignWeaponToStation(const unsigned int s, Weapon* const wpnPtr)
{
   bool ok = false;
   if (s >= 1 && s <= ns) {

      int idx = s-1;

      // Clear previous weapon (if any)
      if (weaponTbl[idx] != 0) {
         numWpn--;
         weaponTbl[idx]->setLauncher(0,0);
         weaponTbl[idx] = 0;
      }

      // Assign the new weapon (if any)
      if (wpnPtr != 0) {
         weaponTbl[idx] = wpnPtr;
         weaponTbl[idx]->setLauncher(this,s);
         numWpn++;
      }

      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// assignExtStoreToStation() -- 
//------------------------------------------------------------------------------
bool Stores::assignExtStoreToStation(const unsigned int s, ExternalStore* const esPtr)
{
   bool ok = false;
   if (s >= 1 && s <= ns) {

      int idx = s-1;

      // Clear previous weapon (if any)
      if (esTbl[idx] != 0) {
         numEs--;
         esTbl[idx] = 0;
      }

      // Assign the new weapon (if any)
      if (esPtr != 0) {
         esTbl[idx] = esPtr;
         numEs++;
      }

      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// resetStores() -- Reset all stores
//------------------------------------------------------------------------------
void Stores::resetStores(Basic::PairStream* const list)
{
   // Reset the external stores
   if (list != 0) {
      Basic::List::Item* item = list->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         Basic::Component* p = (Basic::Component*)( pair->object() );
         p->event(RESET_EVENT);
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------

// Default weapon jettison event handler
bool Stores::onJettisonEvent(Weapon* const wpn)
{
   bool ok = false;
   if (wpn != 0) {

      Basic::PairStream* list = getStores();
      if (list != 0) {

         // First, make sure it's one of ours!
         bool found = false;
         Basic::List::Item* item = list->getFirstItem();
         while (item != 0 && !found) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            found = (wpn == pair->object());  // is it a match?
            item = item->getNext();
         }

         if (found) {
            // Send a jettison event to the weapon
            ok = wpn->event(JETTISON_EVENT);
         }

         list->unref();
         list = 0;
      }
   }
   return ok;
}

// Default external equipment jettison event handler
bool Stores::onJettisonEvent(ExternalStore* const sys)
{
   bool ok = false;
   if (sys != 0) {

      Basic::PairStream* list = getStores();
      if (list != 0) {

         // First, make sure it's one of ours!
         bool found = false;
         Basic::List::Item* item = list->getFirstItem();
         while (item != 0 && !found) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            found = (sys == pair->object());  // is it a match?
            item = item->getNext();
         }

         if (found) {
            // Send a jettison event to the system
            ok = sys->event(JETTISON_EVENT);
         }

         list->unref();
         list = 0;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// Number of station
bool Stores::setSlotNumStations(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 1 && v <= MAX_STATIONS) {
         ok = setNumberOfStations( (unsigned int) v );
      }
      else {
         std::cerr << "Stores::setSlotnumStations() invalid number of stations: " << v;
         std::cerr << "; use [ 1 .. " << MAX_STATIONS << " ]" << std::endl;
      }
   }
   return ok;
}

// Set the stores
bool Stores::setSlotStores(const Basic::PairStream* const msg)
{
   // ---
   // Quick out if the number of stations hasn't been set.
   // ---
   if (ns == 0 && msg != 0) {
      std::cerr << "Stores::setSlotStation() Number of stations is not set!" << std::endl;
      return false;
   }

   // ---
   // Clear the previous stores and assigned weapons
   // ---
   storesList = 0;
   for (unsigned int s = 1; s <= ns; s++) {
      assignWeaponToStation(s,0);
      assignExtStoreToStation(s,0);
   }
   numWpn = 0;
   numEs = 0;

   // ---
   // Quick out if 'msg' is zero 
   // ---
   if (msg == 0) return true;

   bool ok = true;

   // ---
   // Create the new external stores list
   //
   // For all items in the 'msg' list ...
   //   -- Make sure that it's a weapon or other type of external store, and
   //      that it has a valid station number.
   //   -- Clone the store and if it's a weapon then assign it to the station.
   // ---
   Basic::PairStream* newStores = new Basic::PairStream();

   const Basic::List::Item* item = msg->getFirstItem();
   while (item != 0) {

      const Basic::Pair* pair = (Basic::Pair*)(item->getValue());
      const Basic::Component* p = (const Basic::Component*) pair->object();
      if (p != 0) {

         // get the station number from the stores' slot name
         int stationNumber = 0;
         const Basic::Identifier* stationName = pair->slot();
         if (stationName->isInteger()) {
            stationNumber = stationName->getInteger();
         }

         if (stationNumber > 0 && stationNumber <= (int) ns) {

            // check the type of component
            bool isWpn = p->isClassType(typeid(Weapon));
            bool isEE  = p->isClassType(typeid(ExternalStore));

            if ( isWpn || isEE ) {
               // Clone the weapon pair and set us as its container
               Basic::Pair* cpair = (Basic::Pair*) pair->clone();
               Component* cp = (Component*) cpair->object();
               cp->container(this);

               if ( isWpn ) {
                  // Weapon types ...

                  // Assign the weapon to the station
                  Weapon* cwpn = (Weapon*)( cpair->object() );
                  assignWeaponToStation(stationNumber, cwpn);

               }

               if ( isEE ) {
                  // External stores types ...

                  // Assign the external store to the station
                  ExternalStore* cwpn = (ExternalStore*)( cpair->object() );
                  assignExtStoreToStation(stationNumber, cwpn);
               }

               if (cpair != 0) {
                  // Add to the new stores list
                  newStores->put(cpair);
                  cpair->unref(); // the new list has it.
               }
            }
            else {
               std::cerr << "Stores::setSlotStores(): invalid external stores type; use Weapon or Stores classes" << std::endl;
               ok = false;
            }

         }
         else {
            std::cerr << "Stores::setSlotStores(): invalid station number from the store's slot name." << std::endl;
            ok = false;
         }
      }

      item = item->getNext();
   }

   // Make the new stores list the active list
   if (ok && newStores->entries() > 0) {
      storesList = newStores;
   }
   else {
      for (unsigned int s = 1; s <= ns; s++) {
         assignWeaponToStation(s,0);
      }
      numWpn = 0;
   }

   newStores->unref();

   return ok;
}

// Set the selected station number
bool Stores::setSlotSelected(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 1 && v <= (int)ns) {
         ok = selectStation( (unsigned int) v );
      }
      else if (ns == 0) {
         std::cerr << "Stores::setSlotStation() Number of stations is not set!" << std::endl;
      }
      else {
         std::cerr << "Stores::setSlotStation() invalid station number: " << v;
         std::cerr << "; use [ 1 .. " << ns << " ]" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Stores::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Stores::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "numStations: " << getNumberOfStations() << std::endl;

   { // List of external stores
      const Basic::PairStream* list = getStores();
      if (list != 0) {
         indent(sout,i+j);
         sout << "stores: {" << std::endl;
         list->serialize(sout,i+j+4,slotsOnly);
         indent(sout,i+j);
         sout << "}" << std::endl;
         list->unref();
      }
   }

   indent(sout,i+j);
   sout << "selected: " << getSelectedStation() << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // end Simulation namespace
} // end Eaagles namespace
