//------------------------------------------------------------------------------
// Classes: StoresMgr, SimpleStoresMgr
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_StoresMgr_H__
#define __Eaagles_Simulation_StoresMgr_H__

#include "openeaagles/simulation/Stores.h"

namespace Eaagles {
   namespace Basic {
      class Boolean;
      class PairStream;
   }

namespace Simulation {
class Bomb;
class Chaff;
class Decoy;
   class ExternalStore;
class Flare;
class Gun;
class Missile;
class Player;
class Sam;
class Track;
class Weapon;

//------------------------------------------------------------------------------
// Class: StoresMgr
//
// Description: Base class for stores management systems.  Manages the player's
//              external stores, to include both weapons and external equipment.
//              A player may be airborne, land-based, water-based or space-based.
//
// Form name: BaseStoresMgr
//
// Events:              
//    WPN_REL_EVENT  <Basic::Boolean>    ! Weapon release switch with position
//    WPN_REL_EVENT  <none>              ! Weapon release switch (single shot)
//    WPN_RELOAD     <none>              ! Request reload of weapons 
//
// Notes:
//
//    1) The getWeapons(), getExternalStores() and getExtFuelTanks() functions
//       return pre-ref()'d pointers to the lists.
//
//    2) getGun() returns the primary gun model, if any (i.e., first gun found
//       on our primary stores list).  Additional gun models, if any, can be
//       found on the external equipment list.
//
//    3) There are several simple "get next" or "release next" functions
//       that get or release the next bomb, missile, SAM, chaff, flare,
//       or decoy and which are provided as a simple StoreMgr interface for
//       default functions in other simulation components.  This class will
//       only return zero(0) for these functions.  Derived classes should
//       define these functions as required (e.g., SimpleStoresMgr class).
//
//    4) This class is one of the "top level" systems attached to a Player
//       class (see Player.h).
//
//------------------------------------------------------------------------------
class StoresMgr : public Stores  
{
    DECLARE_SUBCLASS(StoresMgr,Stores)

public:
    enum Mode { NAV, A2A, A2G, NUM_MODES };
    enum ArmMode { SAFE, ARMED, SIM, NUM_ARM_MODES };

public:
   StoresMgr();

   Basic::PairStream* getWeapons();                    // List of all weapons (Pre-ref()'d)
   const Basic::PairStream* getWeapons() const;        // List of all weapons (Pre-ref()'d) (const version)

   Basic::PairStream* getExternalStores();             // List of all external equipment (Pre-ref()'d)
   const Basic::PairStream* getExternalStores() const; // List of all external equipment (Pre-ref()'d) (const version)

   Basic::PairStream* getExtFuelTanks();               // List of all external fuel tanks (Pre-ref()'d)
   const Basic::PairStream* getExtFuelTanks() const;   // List of all external fuel tanks (Pre-ref()'d) (const version)

   Gun* getGun();                                      // The primary gun
   const Gun* getGun() const;                          // The primary gun (const version)

   virtual bool isMasterArm(const unsigned int mode) const;  // Is this the master arm mode?
   virtual unsigned int getMasterArm() const;                // Returns the master arm mode

   virtual bool isWeaponDeliveryMode(const unsigned int mode) const;  // Is this the delivery mode?
   virtual unsigned int getWeaponDeliveryMode() const;                // Returns the delivery mode

   virtual bool isGunSelected() const;                 // Returns true when gun is selected
   virtual bool isWeaponReleased() const;              // Are weapons being released?

   virtual Weapon* getCurrentWeapon();                 // The current weapon (Pre-ref()'d)
   virtual const Weapon* getCurrentWeapon() const;     // The current weapon (Pre-ref()'d) (const version)

   virtual bool setMasterArm(const unsigned int newMode);        // Sets the master arm mode
   virtual bool setWeaponDeliveryMode(const unsigned int mode);  // Sets the weapon delivery mode
   virtual bool setGunSelected(const bool flg);                  // Selects the gun

   // The following are simple weapon access functions; all return pref-ref()'d
   // pointers to a weapon or zero if no weapon is available.
   virtual Missile* getNextMissile();                 // Simple get next missile function (Pre-ref()'d)
   virtual const Missile* getNextMissile() const;     // Simple get next missile function (Pre-ref()'d) (const version)
   virtual Sam* getNextSam();                         // Simple get next SAM function  (Pre-ref()'d)
   virtual const Sam* getNextSam() const;             // Simple get next SAM function  (Pre-ref()'d) (const version)
   virtual Bomb* getNextBomb();                       // Simple get next bomb function  (Pre-ref()'d)
   virtual const Bomb* getNextBomb() const;           // Simple get next bomb function  (Pre-ref()'d) (const version)
   virtual Chaff* getNextChaff();                     // Simple get next chaff bundle function  (Pre-ref()'d)
   virtual const Chaff* getNextChaff() const;         // Simple get next chaff bundle function  (Pre-ref()'d) (const version)
   virtual Flare* getNextFlare();                     // Simple get next flare function  (Pre-ref()'d)
   virtual const Flare* getNextFlare() const;         // Simple get next flare function  (Pre-ref()'d) (const version)
   virtual Decoy* getNextDecoy();                     // Simple get next decoy function  (Pre-ref()'d)
   virtual const Decoy* getNextDecoy() const;         // Simple get next decoy function  (Pre-ref()'d) (const version)

   // The following are simple release functions; all return pref-ref()'d
   // pointers to the flyout weapon or zero if no weapon was released.
   virtual Missile* releaseOneMissile();              // Simple missile release function (Pre-ref()'d)
   virtual Sam* releaseOneSam();                      // Simple surface to air (SAM) missile release function (Pre-ref()'d)
   virtual Bomb* releaseOneBomb();                    // Simple bomb release function (Pre-ref()'d)
   virtual Chaff* releaseOneChaff();                  // Simple chaff bundle release function (Pre-ref()'d)
   virtual Flare* releaseOneFlare();                  // Simple flare release function (Pre-ref()'d)
   virtual Decoy* releaseOneDecoy();                  // Simple decoy release function (Pre-ref()'d)

   // Event handlers
   virtual bool onWpnRelEvent(const Basic::Boolean* const sw = 0);
   virtual bool onTriggerSwEvent(const Basic::Boolean* const sw = 0);
   virtual bool onWpnReload();

   // Component Interface
   virtual bool event(const int event, Basic::Object* const obj = 0);

protected:
   // Stores class interface; protected slot functions
   virtual bool setSlotStores(const Basic::PairStream* const msg); // (clones the 'msg' list)

   // System class interface; phase() callbacks
   virtual void process(const LCreal dt);     // Phase 3

   // Basic::Component class interface; protected functions
   virtual bool shutdownNotification();

private:
   void initData();
   void searchAndAdd(Basic::PairStream* const, const std::type_info&, Basic::PairStream*);

   SPtr<Basic::PairStream> weaponsList;  // Weapon list; set by setSlotStores()
   SPtr<Basic::PairStream> externalList; // External equipment list; set by setSlotStores()
   SPtr<Basic::PairStream> fuelList;     // External fuel tank list; set by setSlotStores()
   SPtr<Gun> gunPtr;                     // Gun model; set by setSlotStores()

   bool   gunFlg;          // Gun has been selected
   unsigned int   mode;               // Weapon delivery mode
   unsigned int masterArm;         // Master arming mode
};

//------------------------------------------------------------------------------
// Class: SimpleStoresMgr
// Description: Simple (example) external stores management systems
//
// Form name: StoresMgr
//
// StoresMgr Events:
//    WPN_REL_EVENT     ! Weapon release switch (see StoresMgr)
//
// Notes:
//    1) The various weapon release functions will return a pre-ref()'d pointer
//       to the flyout weapon, or zero if unable to release a weapon.
//
//------------------------------------------------------------------------------
class SimpleStoresMgr : public StoresMgr  
{
    DECLARE_SUBCLASS(SimpleStoresMgr,StoresMgr)

public:
   SimpleStoresMgr();

   int getCurrentWeaponID() const;                       // Current weapon ID number
   int getNumCurrentWeapons() const;                     // Number of weapons with current weapon ID (e.g. # MK84 bombs on the platform)

   // Get the first missile of type weaponType from our weapons list (Pre-ref()'d)
   virtual Missile* getSpecificMissile(const Basic::String* const missileType);

   // Get the first bomb of type weaponType from our weapons list (Pre-ref()'d)
   virtual Bomb* getSpecificBomb(const Basic::String* const bombType);

   // Get the first weapon by type from our weapons list (Pre-ref()'d)
   virtual Weapon* getSpecificWeapon(const std::type_info& type);

   // Get the target track
   virtual Track* getNextTarget();

   virtual bool setWeaponReleaseTimer(const LCreal v);   // Sets the release timer

   // StoresMgr class events
   virtual bool onWpnRelEvent(const Basic::Boolean* const sw = 0);
   virtual bool onTriggerSwEvent(const Basic::Boolean* const sw = 0);

   // StoresMgr class functions
   virtual Weapon* getCurrentWeapon();
   virtual const Weapon* getCurrentWeapon() const;
   virtual bool isWeaponReleased() const;
   virtual Missile* getNextMissile();
   virtual const Missile* getNextMissile() const;
   virtual Sam* getNextSam();
   virtual const Sam* getNextSam() const;
   virtual Bomb* getNextBomb();
   virtual const Bomb* getNextBomb() const;
   virtual Chaff* getNextChaff();
   virtual const Chaff* getNextChaff() const;
   virtual Flare* getNextFlare();
   virtual const Flare* getNextFlare() const;
   virtual Decoy* getNextDecoy();
   virtual const Decoy* getNextDecoy() const;
   virtual Missile* releaseOneMissile();
   virtual Sam* releaseOneSam();
   virtual Bomb* releaseOneBomb();
   virtual Chaff* releaseOneChaff();
   virtual Flare* releaseOneFlare();
   virtual Decoy* releaseOneDecoy();

   // Component Interface
   virtual void updateData(const LCreal dt = 0.0f);

protected:
   // System class interface; phase() callbacks
   virtual void process(const LCreal dt);     // Phase 3

private:
   void initData();

   Missile* getNextMissileImp();        // First available missile from our weapons list (Pre-ref()'d) (const version)
   Sam* getNextSamImp();                // First available SAM from our weapons list (Pre-ref()'d) (const version)
   Bomb* getNextBombImp();              // First available bomb from our weapons list (Pre-ref()'d) (const version)
   Chaff* getNextChaffImp();            // First available chaff bundle from our weapons list (Pre-ref()'d) (const version)
   Flare* getNextFlareImp();            // First available flare from our weapons list (Pre-ref()'d) (const version)
   Decoy* getNextDecoyImp();            // First available decoy from our weapons list (Pre-ref()'d) (const version)

   LCreal wpnRelTimer;     // Weapon released timer
   int    curWpnID;        // Current Weapon ID
   int    nCurWpn;         // Number of weapons of the current type
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
