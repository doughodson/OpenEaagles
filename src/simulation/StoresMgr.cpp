//------------------------------------------------------------------------------
// classes: StoresMgr, SimpleStoresMgr
//------------------------------------------------------------------------------
#include "openeaagles/simulation/StoresMgr.h"

#include "openeaagles/simulation/Bomb.h"
#include "openeaagles/simulation/Effects.h"
#include "openeaagles/simulation/ExternalStore.h"
#include "openeaagles/simulation/FuelTank.h"
#include "openeaagles/simulation/Guns.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/RfSensor.h"
#include "openeaagles/simulation/Sam.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/TrackManager.h"

#include "openeaagles/basic/Boolean.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class StoresMgr
//==============================================================================
IMPLEMENT_SUBCLASS(StoresMgr,"BaseStoresMgr")
EMPTY_SLOTTABLE(StoresMgr)
EMPTY_SERIALIZER(StoresMgr)

//------------------------------------------------------------------------------
// Event() map
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(StoresMgr)
   ON_EVENT_OBJ(WPN_REL_EVENT,onWpnRelEvent,Basic::Boolean)
   ON_EVENT(WPN_REL_EVENT,onWpnRelEvent)

   ON_EVENT_OBJ(TRIGGER_SW_EVENT,onTriggerSwEvent,Basic::Boolean)
   ON_EVENT(TRIGGER_SW_EVENT,onTriggerSwEvent)

   ON_EVENT(WPN_RELOAD, onWpnReload)
END_EVENT_HANDLER()


//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
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
   weaponsList = 0;
   externalList = 0;
   fuelList = 0;
   gunPtr = 0;

   gunFlg = false;
   mode = NAV;
   masterArm = ARMED;  // default is armed and dangerous
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void StoresMgr::copyData(const StoresMgr& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   gunFlg = org.gunFlg;
   mode = org.mode;
   masterArm = org.masterArm;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void StoresMgr::deleteData()
{
   setSlotStores(0);
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void StoresMgr::process(const LCreal dt)
{
   BaseClass::process(dt);

   // Update the gun
   if (gunPtr != 0) {
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
   Basic::PairStream* list = getStores();
   if (list != 0) {
      Basic::List::Item* item = list->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         Basic::Component* p = (Basic::Component*)( pair->object() );
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
      list->unref();
      list = 0;
   }

   // Clear our stores
   setSlotStores(0);

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
Basic::PairStream* StoresMgr::getWeapons()
{
   return weaponsList.getRefPtr();
}

// Pre-ref()'d list of our weapons (const version)
const Basic::PairStream* StoresMgr::getWeapons() const
{
   return weaponsList.getRefPtr();
}

// Pre-ref()'d list of our external equipment
Basic::PairStream* StoresMgr::getExternalStores()
{
   return externalList.getRefPtr();
}

// Pre-ref()'d list of our external equipment (const version)
const Basic::PairStream* StoresMgr::getExternalStores() const
{
   return externalList.getRefPtr();
}

// Pre-ref()'d list of our external fuel tanks
Basic::PairStream* StoresMgr::getExtFuelTanks()
{
   return fuelList.getRefPtr();
}

// Pre-ref()'d list of our external fuel tanks (const version)
const Basic::PairStream* StoresMgr::getExtFuelTanks() const
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
   return gunFlg && (gunPtr != 0);
}

// Default function to get the current weapon (Pre-ref()'d)
Weapon* StoresMgr::getCurrentWeapon()
{
   // Get the selected station's weapon
   Weapon* wpn = getWeapon();

   if (wpn == 0) {
      // If not found then check to see if the selected station 
      // was really a Stores class object.  If so then ask it
      // for its selected station's weapon
      ExternalStore* es = getExternalStore();
      if (es != 0) {
         Stores* ss = dynamic_cast<Stores*>( es );
         if (ss != 0) wpn = ss->getWeapon();
         es->unref();
      }
   }

   return wpn;
}

const Weapon* StoresMgr::getCurrentWeapon() const
{
   // Get the selected station's weapon
   const Weapon* wpn = getWeapon();

   if (wpn == 0) {
      // If not found then check to see if the selected station 
      // was really a Stores class object.  If so then ask it
      // for its selected station's weapon
      const ExternalStore* es = getExternalStore();
      if (es != 0) {
         const Stores* ss = dynamic_cast<const Stores*>( es );
         if (ss != 0) wpn = ss->getWeapon();
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
   gunFlg = (flg && gunPtr != 0);
   return true;
}

//------------------------------------------------------------------------------
// Empty "simple" get and release functions (derived classes can define these as needed)
//------------------------------------------------------------------------------

Missile* StoresMgr::getNextMissile()               { return 0; }
const Missile* StoresMgr::getNextMissile() const   { return 0; }
Sam* StoresMgr::getNextSam()                       { return 0; }
const Sam* StoresMgr::getNextSam() const           { return 0; }
Bomb* StoresMgr::getNextBomb()                     { return 0; }
const Bomb* StoresMgr::getNextBomb() const         { return 0; }
Chaff* StoresMgr::getNextChaff()                   { return 0; }
const Chaff* StoresMgr::getNextChaff() const       { return 0; }
Flare* StoresMgr::getNextFlare()                   { return 0; }
const Flare* StoresMgr::getNextFlare() const       { return 0; }
Decoy* StoresMgr::getNextDecoy()                   { return 0; }
const Decoy* StoresMgr::getNextDecoy() const       { return 0; }

Missile* StoresMgr::releaseOneMissile()   { return 0; }
Sam* StoresMgr::releaseOneSam()           { return 0; }
Bomb* StoresMgr::releaseOneBomb()         { return 0; }
Chaff* StoresMgr::releaseOneChaff()       { return 0; }
Flare* StoresMgr::releaseOneFlare()       { return 0; }
Decoy* StoresMgr::releaseOneDecoy()       { return 0; }

//------------------------------------------------------------------------------
// Empty event handlers (derived classes can define these as needed)
//------------------------------------------------------------------------------

// Default function to manage the weapon release event
bool StoresMgr::onWpnRelEvent(const Basic::Boolean* const)
{
   return true;
}

// Default function to manage the trigger switch event
bool StoresMgr::onTriggerSwEvent(const Basic::Boolean* const)
{
   return true;
}


//------------------------------------------------------------------------------
// Default weapons reload event handler
//------------------------------------------------------------------------------
bool StoresMgr::onWpnReload()
{
   // Reset the weapons only
   Basic::PairStream* list = getWeapons();
   if (list != 0) {
      resetStores(list);
      list->unref();
      list = 0;
   }
   return true;
}

//------------------------------------------------------------------------------
// Search all of the objects in the main list for objects of 'type' and add
// them to the sublist.  Also check all Stores type objects for any 'type' objects.
//------------------------------------------------------------------------------
void StoresMgr::searchAndAdd(Basic::PairStream* const mainList, const std::type_info& type, Basic::PairStream* sublist)
{
   if (mainList != 0 && sublist != 0) {

      const Basic::List::Item* item = mainList->getFirstItem();
      while (item != 0) {

         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         Basic::Component* p = (Basic::Component*)( pair->object() );

         // Check the type and add to the list
         bool isType = p->isClassType(type);
         if (isType) sublist->put(pair);

         // If this is a Stores object then check its stores for 'type' objects as well
         Stores* sp = dynamic_cast<Stores*>(p);
         if ( sp != 0 ) {
            Basic::PairStream* pstores = sp->getStores();
            searchAndAdd(pstores, type, sublist);
            pstores->unref();
         }

         item = item->getNext();
      }
   }  
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool StoresMgr::setSlotStores(const Basic::PairStream* const msg)
{
   // First let our base class do everything that it needs to.
   BaseClass::setSlotStores(msg);

   // ---
   // Clear all previous stores and assigned weapons
   // ---
   weaponsList = 0;
   externalList = 0;
   fuelList = 0;
   gunPtr = 0;

   // ---
   // Use the stores list that the Stores class just processed.
   Basic::PairStream* stores = getStores();
   if (stores != 0){

      // Create the new weapons list that contains all weapons
      {
         Basic::PairStream* newWeapons = new Basic::PairStream();
         searchAndAdd(stores, typeid(Weapon), newWeapons);
         if (newWeapons->entries() > 0) weaponsList = newWeapons;
         newWeapons->unref();
      }

      // Create the new external stores list that contains all
      // non-weapon, external stores (e.g., fuel tanks, pods, guns)
      {
         Basic::PairStream* newExternal = new Basic::PairStream();
         searchAndAdd(stores, typeid(ExternalStore), newExternal);
         if (newExternal->entries() > 0) externalList = newExternal;
         newExternal->unref();
      }

      // Create the new fuel tank list that contains all fuel tanks
      {
         Basic::PairStream* newFuel = new Basic::PairStream();
         searchAndAdd(stores, typeid(FuelTank), newFuel);
         if (newFuel->entries() > 0) fuelList = newFuel;
         newFuel->unref();
      }

      // Find the primary gun; i.e., the first gun found on our stores
      Basic::List::Item* item = stores->getFirstItem();
      while (item != 0 && gunPtr == 0) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());

         Gun* p = dynamic_cast<Gun*>( pair->object() );
         if (p != 0) gunPtr = p;

         item = item->getNext();
      }

      stores->unref();
      stores = 0;
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
   setSlotStores(0);
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void SimpleStoresMgr::process(const LCreal dt)
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
void SimpleStoresMgr::updateData(const LCreal dt)
{
   BaseClass::updateData(dt);

   // ---
   // Get the current weapon data
   // ---
   {
      Weapon* wpn = (Weapon*) getCurrentWeapon();
      if (wpn != 0) {
         // Weapon ID
         curWpnID = wpn->getWeaponID();

         // Number of this type weapon
         int count = 0;
         Basic::PairStream* list = getStores();
         if (list != 0) {
            const Basic::List::Item* item = list->getFirstItem();
            while (item != 0) {
               const Basic::Pair* pair = (Basic::Pair*) item->getValue();
               if (pair != 0) {
                  const Weapon* s = dynamic_cast<const Weapon*>( pair->object() );
                  if ( s != 0 && s->isMode(Player::INACTIVE) && strcmp(s->getFactoryName(), wpn->getFactoryName()) == 0 ) {
                     count++;
                  }
               }
               item = item->getNext();
            }
            list->unref();
            list = 0;
         }
         nCurWpn = count;

         wpn->unref();
         wpn = 0;
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
Weapon* SimpleStoresMgr::getCurrentWeapon()
{
   Weapon* wpn = 0;
   if ( isWeaponDeliveryMode(A2A) ) {
      wpn = getNextMissile();    // We need a missile
   }
   else {
      wpn = getNextBomb();       // We need a bomb
   }
   return wpn;
}

const Weapon* SimpleStoresMgr::getCurrentWeapon() const
{
   const Weapon* wpn = 0;
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
Missile* SimpleStoresMgr::getNextMissile()               { return getNextMissileImp(); }
const Missile* SimpleStoresMgr::getNextMissile() const   { return ((SimpleStoresMgr*)this)->getNextMissileImp(); }

Missile* SimpleStoresMgr::getNextMissileImp()
{
   Missile* msl = 0;

   Basic::PairStream* list = getWeapons();
   if (list != 0) {

      // find the first free (inactive) missile
      Basic::List::Item* item = list->getFirstItem();
      while (item != 0 && msl == 0) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         Missile* p = dynamic_cast<Missile*>( pair->object() );
         if (p != 0) {
            if (p->isInactive() || p->isReleaseHold()) {
               msl = (Missile*) p->getPointer();
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
Sam* SimpleStoresMgr::getNextSam()              { return getNextSamImp(); }
const Sam* SimpleStoresMgr::getNextSam() const  { return ((SimpleStoresMgr*)this)->getNextSamImp(); }

Sam* SimpleStoresMgr::getNextSamImp()
{
   Sam* msl = 0;

   Basic::PairStream* list = getWeapons();
   if (list != 0) {

      // find the first free (inactive) SAM
      Basic::List::Item* item = list->getFirstItem();
      while (item != 0 && msl == 0) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         Sam* p = dynamic_cast<Sam*>( pair->object() );
         if (p != 0) {
            if (p->isInactive() || p->isReleaseHold()) {
               msl = (Sam*) p->getPointer();
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
Bomb* SimpleStoresMgr::getNextBomb()               { return getNextBombImp(); }
const Bomb* SimpleStoresMgr::getNextBomb() const   { return ((SimpleStoresMgr*)this)->getNextBombImp(); }

Bomb* SimpleStoresMgr::getNextBombImp()
{
   Bomb* bomb = 0;

   Basic::PairStream* list = getWeapons();
   if (list != 0) {

      // find the first free (inactive) bomb
      Basic::List::Item* item = list->getFirstItem();
      while (item != 0 && bomb == 0) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         Bomb* p = dynamic_cast<Bomb*>( pair->object() );
         if (p != 0) {
            if (p->isInactive() || p->isReleaseHold()) {
               bomb = (Bomb*) p->getPointer();
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
Chaff* SimpleStoresMgr::getNextChaff()                { return getNextChaffImp(); }
const Chaff* SimpleStoresMgr::getNextChaff() const    { return ((SimpleStoresMgr*)this)->getNextChaffImp(); }

Chaff* SimpleStoresMgr::getNextChaffImp()
{
   Chaff* chaff = 0;

   Basic::PairStream* list = getWeapons();
   if (list != 0) {

      // find the first free (inactive) chaff bundle
      Basic::List::Item* item = list->getFirstItem();
      while (item != 0 && chaff == 0) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         Chaff* p = dynamic_cast<Chaff*>( pair->object() );
         if (p != 0) {
            if (p->isInactive() || p->isReleaseHold()) {
               chaff = (Chaff*) p->getPointer();
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
Flare* SimpleStoresMgr::getNextFlare()                { return getNextFlareImp(); }
const Flare* SimpleStoresMgr::getNextFlare() const    { return ((SimpleStoresMgr*)this)->getNextFlareImp(); }

Flare* SimpleStoresMgr::getNextFlareImp()
{
   Flare* flare = 0;

   Basic::PairStream* list = getWeapons();
   if (list != 0) {

      // find the first free (inactive) flare
      Basic::List::Item* item = list->getFirstItem();
      while (item != 0 && flare == 0) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         Flare* p = dynamic_cast<Flare*>( pair->object() );
         if (p != 0) {
            if (p->isInactive() || p->isReleaseHold()) {
               flare = (Flare*) p->getPointer();
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
Decoy* SimpleStoresMgr::getNextDecoy()             { return getNextDecoyImp(); }
const Decoy* SimpleStoresMgr::getNextDecoy() const { return ((SimpleStoresMgr*)this)->getNextDecoyImp(); }

Decoy* SimpleStoresMgr::getNextDecoyImp()
{
   Decoy* decoy = 0;

   Basic::PairStream* list = getWeapons();
   if (list != 0) {

      // find the first free (inactive) decoy
      Basic::List::Item* item = list->getFirstItem();
      while (item != 0 && decoy == 0) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         Decoy* p = dynamic_cast<Decoy*>( pair->object() );
         if (p != 0) {
            if (p->isInactive() || p->isReleaseHold()) {
               decoy = (Decoy*) p->getPointer();
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return decoy;
}

// Get the next free missile of type 'missileType'
Missile* SimpleStoresMgr::getSpecificMissile(const Basic::String* const missileType)
{
   Missile* msl = 0;
   if (missileType != 0) {

      Basic::PairStream* list = getWeapons();
      if (list != 0) {

         // Find the first free (inactive) missile of type weaponType
         Basic::List::Item* item = list->getFirstItem();
         while (item != 0 && msl == 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Missile* p = dynamic_cast<Missile*>( pair->object() );
            if (p != 0 && p->isInactive()) {
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
Bomb* SimpleStoresMgr::getSpecificBomb(const Basic::String* const bombType)
{
   Bomb* bomb = 0;
   if (bombType != 0)  {

      Basic::PairStream* list = getWeapons();
      if (list != 0)  {

         // Find the first free (inactive) bomb
         Basic::List::Item* item = list->getFirstItem();
         while (item != 0 && bomb == 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Bomb* p = dynamic_cast<Bomb*>( pair->object() );
            if (p != 0 && p->isInactive()) {
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
Weapon* SimpleStoresMgr::getSpecificWeapon(const std::type_info& type)
{
   Weapon* wpn = 0;
   if (&type != 0) {

      Basic::PairStream* list = getWeapons();
      if (list != 0) {

         // Find the first free (inactive) bomb
         Basic::List::Item* item = list->getFirstItem();
         while (item != 0 && wpn == 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Weapon* p = dynamic_cast<Weapon*>( pair->object() );
            if (p != 0 && p->isInactive() && p->isClassType(type)) {
               p->ref();
               wpn = p;
            }
            item = item->getNext();
         }

         list->unref();
      }

   }
   return wpn;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the release timer
bool SimpleStoresMgr::setWeaponReleaseTimer(const LCreal v)
{
   wpnRelTimer = v;
   return true;
}

//------------------------------------------------------------------------------
// Find the next target to shoot
//------------------------------------------------------------------------------
Track* SimpleStoresMgr::getNextTarget()
{
   Track* trk = 0;

   if (getOwnship() != 0) {

      OnboardComputer* obc = getOwnship()->getOnboardComputer();
      if (obc != 0) {

         // Get the next to shoot
         int n = 0;
         SPtr<Track> track;
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
   Missile* flyout = 0;
   Missile* wpn = getNextMissile();
   if (wpn != 0) {
      flyout = static_cast<Missile*>( releaseWeapon(wpn) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one SAM
//------------------------------------------------------------------------------
Sam* SimpleStoresMgr::releaseOneSam()
{
   Sam* flyout = 0;
   Sam* wpn = getNextSam();
   if (wpn != 0) {
      flyout = static_cast<Sam*>( releaseWeapon(wpn) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one Bomb
//------------------------------------------------------------------------------
Bomb* SimpleStoresMgr::releaseOneBomb()
{
   Bomb* flyout = 0;
   Bomb* wpn = getNextBomb();
   if (wpn != 0) {
      flyout = static_cast<Bomb*>( releaseWeapon(wpn) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one chaff bundle
//------------------------------------------------------------------------------
Chaff* SimpleStoresMgr::releaseOneChaff()
{
   Chaff* flyout = 0;
   Chaff* chaff = getNextChaff();
   if (chaff != 0) {
      flyout = static_cast<Chaff*>( releaseWeapon(chaff) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one flare
//------------------------------------------------------------------------------
Flare* SimpleStoresMgr::releaseOneFlare()
{
   Flare* flyout = 0;
   Flare* flare = getNextFlare();
   if (flare != 0) {
      flyout = static_cast<Flare*>( releaseWeapon(flare) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one decoy
//------------------------------------------------------------------------------
Decoy* SimpleStoresMgr::releaseOneDecoy()
{
   Decoy* flyout = 0;
   Decoy* decoy = getNextDecoy();
   if (decoy != 0) {
      flyout = static_cast<Decoy*>( releaseWeapon(decoy) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Manage the weapon release event
//------------------------------------------------------------------------------
bool SimpleStoresMgr::onWpnRelEvent(const Basic::Boolean* const sw)
{
   // Weapon release follows the switch or by default is true
   bool wpnRel = true;
   if (sw != 0) wpnRel = sw->getBoolean();

   if ( wpnRel && (isWeaponDeliveryMode(A2A) || isWeaponDeliveryMode(A2G)) ) {

      // A/A missiles and A/G bombs only ...

      Weapon* wpn = getCurrentWeapon();
      if (wpn != 0) {

         // release the weapon ---
         //  if successful, returns a pre-ref()'d pointer to the flyout weapon.
         Weapon* flyout = releaseWeapon(wpn);
         if (flyout != 0) {

            if (isWeaponDeliveryMode(A2A)) {

               // ---
               // Setup the guidance ...
               // ---
               Track* trk = getNextTarget();
               if (trk != 0) {
                  Player* tgt = trk->getTarget();

                  TrackManager* tm = 0;
                  RfSensor* sm = flyout->getSensor();
                  if (sm != 0) tm = sm->getTrackManager();

                  if (tm != 0) {
                     // Give a copy of the track to the weapon's track manager
                     Track* newTrk = trk->clone();
                     tm->clearTracksAndQueues();
                     tm->addTrack(newTrk);
                     flyout->setTargetTrack(newTrk,true);
                  }
                  else if (tgt != 0) {
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

         if (flyout != 0) flyout->unref(); // unref() the flyout

         wpn->unref();
         wpn = 0;
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
bool SimpleStoresMgr::onTriggerSwEvent(const Basic::Boolean* const sw)
{
   Gun* g = getGun(); // Get the primary gun
   if (g != 0) {

      // Single Burst?
      bool burst = (sw == 0);

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


} // End Simulation namespace
} // End Eaagles namespace
