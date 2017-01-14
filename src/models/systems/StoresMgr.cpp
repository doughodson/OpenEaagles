//------------------------------------------------------------------------------
// classes: StoresMgr, SimpleStoresMgr
//------------------------------------------------------------------------------
#include "openeaagles/models/systems/StoresMgr.hpp"

#include "openeaagles/models/players/Bomb.hpp"
#include "openeaagles/models/players/Effects.hpp"

#include "openeaagles/models/systems/ExternalStore.hpp"
#include "openeaagles/models/systems/FuelTank.hpp"
#include "openeaagles/models/systems/Guns.hpp"
#include "openeaagles/models/players/Missile.hpp"
#include "openeaagles/models/systems/OnboardComputer.hpp"
#include "openeaagles/models/systems/RfSensor.hpp"
#include "openeaagles/models/players/Sam.hpp"
#include "openeaagles/models/Track.hpp"
#include "openeaagles/models/systems/TrackManager.hpp"

#include "openeaagles/base/Boolean.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include <cstring>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(StoresMgr, "BaseStoresMgr")
EMPTY_SLOTTABLE(StoresMgr)
EMPTY_SERIALIZER(StoresMgr)

BEGIN_EVENT_HANDLER(StoresMgr)
   ON_EVENT_OBJ(WPN_REL_EVENT,onWpnRelEvent,base::Boolean)
   ON_EVENT(WPN_REL_EVENT,onWpnRelEvent)

   ON_EVENT_OBJ(TRIGGER_SW_EVENT,onTriggerSwEvent,base::Boolean)
   ON_EVENT(TRIGGER_SW_EVENT,onTriggerSwEvent)

   ON_EVENT(WPN_RELOAD, onWpnReload)
END_EVENT_HANDLER()

StoresMgr::StoresMgr()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// initData() -- init our member data
//------------------------------------------------------------------------------
void StoresMgr::initData()
{
   weaponsList = nullptr;
   externalList = nullptr;
   fuelList = nullptr;
   gunPtr = nullptr;

   gunFlg = false;
   mode = NAV;
   masterArm = ARMED;  // default is armed and dangerous
}

void StoresMgr::copyData(const StoresMgr& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   gunFlg = org.gunFlg;
   mode = org.mode;
   masterArm = org.masterArm;
}

void StoresMgr::deleteData()
{
   setSlotStores(nullptr);
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void StoresMgr::process(const double dt)
{
   BaseClass::process(dt);

   // Update the gun
   if (gunPtr != nullptr) {
      gunPtr->setGunArmed( isGunSelected() && isMasterArm(ARMED) );
      gunPtr->tcFrame(dt);
   }
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool StoresMgr::shutdownNotification()
{
   // Notify the external stores that we're shutting down
   base::PairStream* list = getStores();
   if (list != nullptr) {
      base::List::Item* item = list->getFirstItem();
      while (item != nullptr) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
         base::Component* p = static_cast<base::Component*>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
      list->unref();
      list = nullptr;
   }

   // Clear our stores
   setSlotStores(nullptr);

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Is master arm mode == 'mode'
bool StoresMgr::isMasterArm(const unsigned int mode) const
{
   return (masterArm == mode);
}

// Returns the master arm mode
unsigned int StoresMgr::getMasterArm() const
{
   return masterArm;
}

// Is the delivery mode == 'testMode'
bool StoresMgr::isWeaponDeliveryMode(const unsigned int testMode) const
{
   return (mode == testMode);
}

// Returns the delivery mode
unsigned int StoresMgr::getWeaponDeliveryMode() const
{
   return mode;
}

// Pre-ref()'d list of our weapons
base::PairStream* StoresMgr::getWeapons()
{
   return weaponsList.getRefPtr();
}

// Pre-ref()'d list of our weapons (const version)
const base::PairStream* StoresMgr::getWeapons() const
{
   return weaponsList.getRefPtr();
}

// Pre-ref()'d list of our external equipment
base::PairStream* StoresMgr::getExternalStores()
{
   return externalList.getRefPtr();
}

// Pre-ref()'d list of our external equipment (const version)
const base::PairStream* StoresMgr::getExternalStores() const
{
   return externalList.getRefPtr();
}

// Pre-ref()'d list of our external fuel tanks
base::PairStream* StoresMgr::getExtFuelTanks()
{
   return fuelList.getRefPtr();
}

// Pre-ref()'d list of our external fuel tanks (const version)
const base::PairStream* StoresMgr::getExtFuelTanks() const
{
   return fuelList.getRefPtr();
}

// Get the gun model
Gun* StoresMgr::getGun()
{
   return gunPtr;
}

// Get the gun model (const version)
const Gun* StoresMgr::getGun() const
{
   return gunPtr;
}

// Are weapons are being released (default function)
bool StoresMgr::isWeaponReleased() const
{
   return false;
}

// Is the gun selected
bool StoresMgr::isGunSelected() const
{
   return gunFlg && (gunPtr != nullptr);
}

// Default function to get the current weapon (Pre-ref()'d)
AbstractWeapon* StoresMgr::getCurrentWeapon()
{
   // Get the selected station's weapon
   AbstractWeapon* wpn = getWeapon();

   if (wpn == nullptr) {
      // If not found then check to see if the selected station
      // was really a Stores class object.  If so then ask it
      // for its selected station's weapon
      ExternalStore* es = getExternalStore();
      if (es != nullptr) {
         const auto ss = dynamic_cast<Stores*>( es );
         if (ss != nullptr) wpn = ss->getWeapon();
         es->unref();
      }
   }

   return wpn;
}

const AbstractWeapon* StoresMgr::getCurrentWeapon() const
{
   // Get the selected station's weapon
   const AbstractWeapon* wpn = getWeapon();

   if (wpn == nullptr) {
      // If not found then check to see if the selected station
      // was really a Stores class object.  If so then ask it
      // for its selected station's weapon
      const ExternalStore* es = getExternalStore();
      if (es != nullptr) {
         const auto ss = dynamic_cast<const Stores*>( es );
         if (ss != nullptr) wpn = ss->getWeapon();
         es->unref();
      }
   }

   return wpn;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the master arming mode
bool StoresMgr::setMasterArm(const unsigned int newMode)
{
   masterArm = newMode;
   return true;
}

// Sets the weapon delivery mode
bool StoresMgr::setWeaponDeliveryMode(const unsigned int newMode)
{
   mode = newMode;
   return true;
}

// Selects the gun
bool StoresMgr::setGunSelected(const bool flg)
{
   gunFlg = (flg && gunPtr != nullptr);
   return true;
}

//------------------------------------------------------------------------------
// Empty "simple" get and release functions (derived classes can define these as needed)
//------------------------------------------------------------------------------

Missile* StoresMgr::getNextMissile()               { return nullptr; }
const Missile* StoresMgr::getNextMissile() const   { return nullptr; }
Sam* StoresMgr::getNextSam()                       { return nullptr; }
const Sam* StoresMgr::getNextSam() const           { return nullptr; }
Bomb* StoresMgr::getNextBomb()                     { return nullptr; }
const Bomb* StoresMgr::getNextBomb() const         { return nullptr; }
Chaff* StoresMgr::getNextChaff()                   { return nullptr; }
const Chaff* StoresMgr::getNextChaff() const       { return nullptr; }
Flare* StoresMgr::getNextFlare()                   { return nullptr; }
const Flare* StoresMgr::getNextFlare() const       { return nullptr; }
Decoy* StoresMgr::getNextDecoy()                   { return nullptr; }
const Decoy* StoresMgr::getNextDecoy() const       { return nullptr; }

Missile* StoresMgr::releaseOneMissile()   { return nullptr; }
Sam* StoresMgr::releaseOneSam()           { return nullptr; }
Bomb* StoresMgr::releaseOneBomb()         { return nullptr; }
Chaff* StoresMgr::releaseOneChaff()       { return nullptr; }
Flare* StoresMgr::releaseOneFlare()       { return nullptr; }
Decoy* StoresMgr::releaseOneDecoy()       { return nullptr; }

//------------------------------------------------------------------------------
// Empty event handlers (derived classes can define these as needed)
//------------------------------------------------------------------------------

// Default function to manage the weapon release event
bool StoresMgr::onWpnRelEvent(const base::Boolean* const)
{
   return true;
}

// Default function to manage the trigger switch event
bool StoresMgr::onTriggerSwEvent(const base::Boolean* const)
{
   return true;
}


//------------------------------------------------------------------------------
// Default weapons reload event handler
//------------------------------------------------------------------------------
bool StoresMgr::onWpnReload()
{
   // Reset the weapons only
   base::PairStream* list = getWeapons();
   if (list != nullptr) {
      resetStores(list);
      list->unref();
      list = nullptr;
   }
   return true;
}

//------------------------------------------------------------------------------
// Search all of the objects in the main list for objects of 'type' and add
// them to the sublist.  Also check all Stores type objects for any 'type' objects.
//------------------------------------------------------------------------------
void StoresMgr::searchAndAdd(base::PairStream* const mainList, const std::type_info& type, base::PairStream* sublist)
{
   if (mainList != nullptr && sublist != nullptr) {

      const base::List::Item* item = mainList->getFirstItem();
      while (item != nullptr) {

         const auto pair = static_cast<const base::Pair*>(item->getValue());
         const auto p = static_cast<const base::Component*>(pair->object());

         // Check the type and add to the list
         bool isType = p->isClassType(type);
         if (isType) sublist->put(const_cast<base::Pair*>(pair));

         // If this is a Stores object then check its stores for 'type' objects as well
         const auto sp = dynamic_cast<const Stores*>(p);
         if ( sp != nullptr ) {
            const base::PairStream* pstores = sp->getStores();
            if (pstores != nullptr) {
               searchAndAdd(const_cast<base::PairStream*>(pstores), type, sublist);
               pstores->unref();
            }
         }

         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool StoresMgr::setSlotStores(const base::PairStream* const msg)
{
   // First let our base class do everything that it needs to.
   BaseClass::setSlotStores(msg);

   // ---
   // Clear all previous stores and assigned weapons
   // ---
   weaponsList = nullptr;
   externalList = nullptr;
   fuelList = nullptr;
   gunPtr = nullptr;

   // ---
   // Use the stores list that the Stores class just processed.
   base::PairStream* stores = getStores();
   if (stores != nullptr){

      // Create the new weapons list that contains all weapons
      {
         const auto newWeapons = new base::PairStream();
         searchAndAdd(stores, typeid(AbstractWeapon), newWeapons);
         if (newWeapons->entries() > 0) weaponsList = newWeapons;
         newWeapons->unref();
      }

      // Create the new external stores list that contains all
      // non-weapon, external stores (e.g., fuel tanks, pods, guns)
      {
         const auto newExternal = new base::PairStream();
         searchAndAdd(stores, typeid(ExternalStore), newExternal);
         if (newExternal->entries() > 0) externalList = newExternal;
         newExternal->unref();
      }

      // Create the new fuel tank list that contains all fuel tanks
      {
         const auto newFuel = new base::PairStream();
         searchAndAdd(stores, typeid(FuelTank), newFuel);
         if (newFuel->entries() > 0) fuelList = newFuel;
         newFuel->unref();
      }

      // Find the primary gun; i.e., the first gun found on our stores
      base::List::Item* item = stores->getFirstItem();
      while (item != nullptr && gunPtr == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Gun*>(pair->object());
         if (p != nullptr) gunPtr = p;
         item = item->getNext();
      }

      stores->unref();
      stores = nullptr;
   }

   return true;
}



//==============================================================================
// Class SimpleStoresMgr
//==============================================================================
IMPLEMENT_SUBCLASS(SimpleStoresMgr,"StoresMgr")
EMPTY_SLOTTABLE(SimpleStoresMgr)
EMPTY_SERIALIZER(SimpleStoresMgr)


//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
SimpleStoresMgr::SimpleStoresMgr()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// initData() -- init our member data
//------------------------------------------------------------------------------
void SimpleStoresMgr::initData()
{
   wpnRelTimer = 0.0;
   curWpnID = 0;
   nCurWpn = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SimpleStoresMgr::copyData(const SimpleStoresMgr& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   wpnRelTimer = org.wpnRelTimer;
   curWpnID = org.curWpnID;
   nCurWpn = org.nCurWpn;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SimpleStoresMgr::deleteData()
{
   setSlotStores(nullptr);
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void SimpleStoresMgr::process(const double dt)
{
   BaseClass::process(dt);

   // Weapon released timer
   if (wpnRelTimer > 0.0) {
      // decrease timer to zero
      wpnRelTimer -= dt;
   }
}

//------------------------------------------------------------------------------
// updateData() -- update non-time critical stuff here
//------------------------------------------------------------------------------
void SimpleStoresMgr::updateData(const double dt)
{
   BaseClass::updateData(dt);

   // ---
   // Get the current weapon data
   // ---
   {
      auto wpn = static_cast<AbstractWeapon*>(getCurrentWeapon());
      if (wpn != nullptr) {
         // Weapon ID
         curWpnID = wpn->getWeaponID();

         // Number of this type weapon
         int count = 0;
         base::PairStream* list = getStores();
         if (list != nullptr) {
            const base::List::Item* item = list->getFirstItem();
            while (item != nullptr) {
               const base::Pair* pair = static_cast<const base::Pair*>(item->getValue());
               if (pair != nullptr) {
                  const auto s = dynamic_cast<const AbstractWeapon*>( pair->object() );
                  if ( s != nullptr && s->isMode(Player::INACTIVE) && std::strcmp(s->getFactoryName(), wpn->getFactoryName()) == 0 ) {
                     count++;
                  }
               }
               item = item->getNext();
            }
            list->unref();
            list = nullptr;
         }
         nCurWpn = count;

         wpn->unref();
         wpn = nullptr;
      }
      else {
         curWpnID = 0;
         nCurWpn = 0;
      }
   }

}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Default function to return the current weapon (Pre-ref()'d)
AbstractWeapon* SimpleStoresMgr::getCurrentWeapon()
{
   AbstractWeapon* wpn = nullptr;
   if ( isWeaponDeliveryMode(A2A) ) {
      wpn = getNextMissile();    // We need a missile
   }
   else {
      wpn = getNextBomb();       // We need a bomb
   }
   return wpn;
}

const AbstractWeapon* SimpleStoresMgr::getCurrentWeapon() const
{
   const AbstractWeapon* wpn = nullptr;
   if ( isWeaponDeliveryMode(A2A) ) {
      wpn = getNextMissile();    // We need a missile
   }
   else {
      wpn = getNextBomb();       // We need a bomb
   }
   return wpn;
}

// Get the current weapon ID
int SimpleStoresMgr::getCurrentWeaponID() const
{
   return curWpnID;
}

// Get the number of weapons of the current type
int SimpleStoresMgr::getNumCurrentWeapons() const
{
   return nCurWpn;
}

// Are weapons are being released
bool SimpleStoresMgr::isWeaponReleased() const
{
   return (wpnRelTimer > 0.0);
}

// ---
// Default function to get the next free missile in our stores (Pre-ref()'d)
// ---
Missile* SimpleStoresMgr::getNextMissile()
{
   return getNextMissileImp();
}

const Missile* SimpleStoresMgr::getNextMissile() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextMissileImp();
}

Missile* SimpleStoresMgr::getNextMissileImp()
{
   Missile* msl = nullptr;

   base::PairStream* list = getWeapons();
   if (list != nullptr) {

      // find the first free (inactive) missile
      base::List::Item* item = list->getFirstItem();
      while (item != nullptr && msl == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Missile*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               msl = static_cast<Missile*>(p->getPointer());
            }
         }
         item = item->getNext();
      }
      list->unref();
   }

   return msl;
}

// ---
// Default function to get the next free SAM in our stores (Pre-ref()'d)
// ---
Sam* SimpleStoresMgr::getNextSam()
{
   return getNextSamImp();
}

const Sam* SimpleStoresMgr::getNextSam() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextSamImp();
}

Sam* SimpleStoresMgr::getNextSamImp()
{
   Sam* msl = nullptr;

   base::PairStream* list = getWeapons();
   if (list != nullptr) {

      // find the first free (inactive) SAM
      base::List::Item* item = list->getFirstItem();
      while (item != nullptr && msl == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Sam*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               msl = static_cast<Sam*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return msl;
}

// ---
// Default function to get the next free bomb in our stores (Pre-ref()'d)
// ---
Bomb* SimpleStoresMgr::getNextBomb()
{
   return getNextBombImp();
}

const Bomb* SimpleStoresMgr::getNextBomb() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextBombImp();
}

Bomb* SimpleStoresMgr::getNextBombImp()
{
   Bomb* bomb = nullptr;

   base::PairStream* list = getWeapons();
   if (list != nullptr) {

      // find the first free (inactive) bomb
      base::List::Item* item = list->getFirstItem();
      while (item != nullptr && bomb == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Bomb*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               bomb = static_cast<Bomb*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return bomb;
}

// ---
// Default function to get the next free chaff bundle in our stores (Pre-ref()'d)
// ---
Chaff* SimpleStoresMgr::getNextChaff()
{
   return getNextChaffImp();
}

const Chaff* SimpleStoresMgr::getNextChaff() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextChaffImp();
}

Chaff* SimpleStoresMgr::getNextChaffImp()
{
   Chaff* chaff = nullptr;

   base::PairStream* list = getWeapons();
   if (list != nullptr) {

      // find the first free (inactive) chaff bundle
      base::List::Item* item = list->getFirstItem();
      while (item != nullptr && chaff == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Chaff*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               chaff = static_cast<Chaff*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return chaff;
}

// ---
// Default function to get the next free flare in our stores (Pre-ref()'d)
// ---
Flare* SimpleStoresMgr::getNextFlare()
{
   return getNextFlareImp();
}

const Flare* SimpleStoresMgr::getNextFlare() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextFlareImp();
}

Flare* SimpleStoresMgr::getNextFlareImp()
{
   Flare* flare = nullptr;

   base::PairStream* list = getWeapons();
   if (list != nullptr) {

      // find the first free (inactive) flare
      base::List::Item* item = list->getFirstItem();
      while (item != nullptr && flare == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Flare*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               flare = static_cast<Flare*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return flare;
}

// ---
// Default function to get the next decoy from our stores (Pre-ref()'d)
// ---
Decoy* SimpleStoresMgr::getNextDecoy()
{
   return getNextDecoyImp();
}

const Decoy* SimpleStoresMgr::getNextDecoy() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextDecoyImp();
}

Decoy* SimpleStoresMgr::getNextDecoyImp()
{
   Decoy* decoy = nullptr;

   base::PairStream* list = getWeapons();
   if (list != nullptr) {

      // find the first free (inactive) decoy
      base::List::Item* item = list->getFirstItem();
      while (item != nullptr && decoy == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Decoy*>( pair->object() );
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               decoy = static_cast<Decoy*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return decoy;
}

// Get the next free missile of type 'missileType'
Missile* SimpleStoresMgr::getSpecificMissile(const base::String* const missileType)
{
   Missile* msl = nullptr;
   if (missileType != nullptr) {

      base::PairStream* list = getWeapons();
      if (list != nullptr) {

         // Find the first free (inactive) missile of type weaponType
         base::List::Item* item = list->getFirstItem();
         while (item != nullptr && msl == nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto p = dynamic_cast<Missile*>(pair->object());
            if (p != nullptr && p->isInactive()) {
               // Ok, we have a missile, but is it the type we want?
               if (*p->getType() == *missileType) {
                  p->ref();
                  msl = p;
               }
            }
            item = item->getNext();
         }

         list->unref();
      }

   }
   return msl;
}

// Get the next free bomb of type 'bombType'
Bomb* SimpleStoresMgr::getSpecificBomb(const base::String* const bombType)
{
   Bomb* bomb = nullptr;
   if (bombType != nullptr)  {

      base::PairStream* list = getWeapons();
      if (list != nullptr)  {

         // Find the first free (inactive) bomb
         base::List::Item* item = list->getFirstItem();
         while (item != nullptr && bomb == nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto p = dynamic_cast<Bomb*>(pair->object());
            if (p != nullptr && p->isInactive()) {
               // Ok, we have a bomb, but is it the type we want?
               if (*p->getType() == *bombType) {
                  p->ref();
                  bomb = p;
               }
            }
            item = item->getNext();
         }

         list->unref();
      }

   }
   return bomb;
}

// Get the next free weapon of this 'type'
AbstractWeapon* SimpleStoresMgr::getSpecificWeapon(const std::type_info& type)
{
   AbstractWeapon* wpn = nullptr;
   base::PairStream* list = getWeapons();
   if (list != nullptr) {
      // Find the first free (inactive) bomb
      base::List::Item* item = list->getFirstItem();
      while (item != nullptr && wpn == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<AbstractWeapon*>(pair->object());
         if (p != nullptr && p->isInactive() && p->isClassType(type)) {
            p->ref();
            wpn = p;
         }
         item = item->getNext();
      }
      list->unref();
   }
   return wpn;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the release timer
bool SimpleStoresMgr::setWeaponReleaseTimer(const double v)
{
   wpnRelTimer = v;
   return true;
}

//------------------------------------------------------------------------------
// Find the next target to shoot
//------------------------------------------------------------------------------
Track* SimpleStoresMgr::getNextTarget()
{
   Track* trk = nullptr;

   if (getOwnship() != nullptr) {

      OnboardComputer* obc = getOwnship()->getOnboardComputer();
      if (obc != nullptr) {

         // Get the next to shoot
         int n = 0;
         base::safe_ptr<Track> track;
         n = obc->getShootList(&track,1);
         if (n > 0) trk = track;

      }
   }
   return trk;
}

//------------------------------------------------------------------------------
// Release one Missile
//------------------------------------------------------------------------------
Missile* SimpleStoresMgr::releaseOneMissile()
{
   Missile* flyout = nullptr;
   Missile* wpn = getNextMissile();
   if (wpn != nullptr) {
      flyout = static_cast<Missile*>( releaseWeapon(wpn) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one SAM
//------------------------------------------------------------------------------
Sam* SimpleStoresMgr::releaseOneSam()
{
   Sam* flyout = nullptr;
   Sam* wpn = getNextSam();
   if (wpn != nullptr) {
      flyout = static_cast<Sam*>( releaseWeapon(wpn) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one Bomb
//------------------------------------------------------------------------------
Bomb* SimpleStoresMgr::releaseOneBomb()
{
   Bomb* flyout = nullptr;
   Bomb* wpn = getNextBomb();
   if (wpn != nullptr) {
      flyout = static_cast<Bomb*>( releaseWeapon(wpn) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one chaff bundle
//------------------------------------------------------------------------------
Chaff* SimpleStoresMgr::releaseOneChaff()
{
   Chaff* flyout = nullptr;
   Chaff* chaff = getNextChaff();
   if (chaff != nullptr) {
      flyout = static_cast<Chaff*>( releaseWeapon(chaff) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one flare
//------------------------------------------------------------------------------
Flare* SimpleStoresMgr::releaseOneFlare()
{
   Flare* flyout = nullptr;
   Flare* flare = getNextFlare();
   if (flare != nullptr) {
      flyout = static_cast<Flare*>( releaseWeapon(flare) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one decoy
//------------------------------------------------------------------------------
Decoy* SimpleStoresMgr::releaseOneDecoy()
{
   Decoy* flyout = nullptr;
   Decoy* decoy = getNextDecoy();
   if (decoy != nullptr) {
      flyout = static_cast<Decoy*>( releaseWeapon(decoy) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Manage the weapon release event
//------------------------------------------------------------------------------
bool SimpleStoresMgr::onWpnRelEvent(const base::Boolean* const sw)
{
   // Weapon release follows the switch or by default is true
   bool wpnRel = true;
   if (sw != nullptr) wpnRel = sw->getBoolean();

   if ( wpnRel && (isWeaponDeliveryMode(A2A) || isWeaponDeliveryMode(A2G)) ) {

      // A/A missiles and A/G bombs only ...

      AbstractWeapon* wpn = getCurrentWeapon();
      if (wpn != nullptr) {

         // release the weapon ---
         //  if successful, returns a pre-ref()'d pointer to the flyout weapon.
         AbstractWeapon* flyout = releaseWeapon(wpn);
         if (flyout != nullptr) {

            if (isWeaponDeliveryMode(A2A)) {

               // ---
               // Setup the guidance ...
               // ---
               Track* trk = getNextTarget();
               if (trk != nullptr) {
                  Player* tgt = trk->getTarget();

                  TrackManager* tm = nullptr;
                  RfSensor* sm = flyout->getSensor();
                  if (sm != nullptr) tm = sm->getTrackManager();

                  if (tm != nullptr) {
                     // Give a copy of the track to the weapon's track manager
                     Track* newTrk = trk->clone();
                     tm->clearTracksAndQueues();
                     tm->addTrack(newTrk);
                     flyout->setTargetTrack(newTrk,true);
                  }
                  else if (tgt != nullptr) {
                     // No sensor -- fake it and just track the target
                     flyout->setTargetPlayer(tgt,true);
                  }
                  else {
                     // no target track
                  }

                  trk->setWeaponReleased(true);
               }

            }


            // Set the weapon release timer
            setWeaponReleaseTimer(1.0);
         }

         if (flyout != nullptr) flyout->unref(); // unref() the flyout

         wpn->unref();
         wpn = nullptr;
      }

   }
   else {
      setWeaponReleaseTimer(0);
   }

   return true;
}


//------------------------------------------------------------------------------
// Manage the trigger switch event
//------------------------------------------------------------------------------
bool SimpleStoresMgr::onTriggerSwEvent(const base::Boolean* const sw)
{
   Gun* g = getGun(); // Get the primary gun
   if (g != nullptr) {

      // Single Burst?
      bool burst = (sw == nullptr);

      // Firing?
      bool fire = false;
      if ( isWeaponDeliveryMode(A2A) || isWeaponDeliveryMode(A2G) ) {
         if ( burst ) fire = true;
         else fire = sw->getBoolean();
      }

      // Pass the control to the gun
      g->fireControl(fire, burst);
   }

   return true;
}


}
}
