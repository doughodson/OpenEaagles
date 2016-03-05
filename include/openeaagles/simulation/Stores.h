//------------------------------------------------------------------------------
// Classes: Stores
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Stores_H__
#define __oe_simulation_Stores_H__

#include "openeaagles/simulation/ExternalStore.h"

namespace oe {
   namespace base { class Number; class PairStream; }

namespace simulation {
   class Weapon;

//------------------------------------------------------------------------------
// class: Stores
//
// Description: Generic external stores container class (e.g., launcher, rack,
//              ejector, pylon, etc).
//
// Factory name: Stores
// Slots:
//    numStations <Number>             ! Number of stations (less than or equal MAX_STATIONS)
//                                     ! (default: 0)
//
//    stores      <base::PairStream>  ! Our weapons and other external stores (default 0)
//                                     ! -- make sure to set the number of stations first,
//
//    selected    <Number>             ! Selected weapon station number (default: 0)
//
//
// Events:
//    JETTISON_EVENT <Weapon>          ! Jettison this weapon
//    JETTISON_EVENT <ExternalStore>   ! Jettison this external store
//
//
// Notes:
//
//    1) The two primary classes of stores are Weapon objects, which are derived
//    from Player, and ExternalStore objects, which include fuel tanks, avionics
//    pods, guns and other Stores container objects (derived from this class).
//
//    2) Each store is assigned a station number, which must range from 1 to
//    getNumberOfStations(), and which is based on the number assigned as
//    its pair name in the 'stores' slot pair stream.
//
//    3) The number of stations must be set prior to the 'stores' slot.  Use the
//    'numStations', or a derived class that can use setNumberOfStations() from its
//    constructor or initData() function.
//
//    4) The getStores() function returns a pre-ref()'d pointer to the stores lists.
//
//    5) Weapons (see Weapon.h) are derived from the Player class.  They're
//       attached to the launch vehicle, which owns this stores manager.
//       When the weapon is released, it is cloned to create a flyout weapon,
//       which is added to the simulation's player list and becomes an active
//       player.  Weapons include bombs, missiles, effects (e.g., chaff, flares,
//       decoys).
//
//       Weapon players are not updated until they are either pre-released or
//       released, and then only the flyout weapon, which is on the simulation's
//       player list, is being updated.
//
//       During pre-release, the flyout weapon is held in PRE_RELEASE mode and
//       is not a fully ACTIVE player.  It is not directly seen by the other
//       player's sensors or by any interoperability network.  The flyout weapon's
//       systems are active and its sensor can see other players, and other players
//       can see any R/F emissions from the weapon's R/F systems.
//
//    6) External equipment (see ExternalStore.h) includes launchers, guns, fuel
//       tanks, avionics pods, etc.
//
//       This class updates the external equipment using the System's class time
//       critical, updateTC(), function and background function, updateData().
//
//    7) Standard release sequence using releaseWeapon()
//       Simply creates a "flyout" weapon, which will go active after being added
//       to the simulation player list.
//
//    8) Pre-release sequence using prereleaseWeapon()
//       creates a "flyout" weapon, which is added to the simulation player list,
//       but will not go active until either releaseWeapon() or Weapon::release()
//       is called.
//
//    9) The various weapon release functions will return a pre-ref()'d pointer
//       to the flyout weapon, or zero if unable to release a weapon.
//
//   10) The getWeapon() function returns a pointer to the station's "flyout"
//       weapon if it has been pre-released or released, otherwise a pointer
//       to the station's original weapon is returned.
//
//
// EDL Example:
//
//    ( Stores
//       /* The number of stations must be set before the 'stores' slot,
//          or must be set by the derived class' constructor. */
//       numStations:  5
//
//       /* Missiles on station #1 and #5, bombs on #2 and 4, and fuel on #3 */
//       stores: {
//          1: ( Missile ... )
//          2: ( Bomb ... )
//          3: ( FuelTank ... )
//          4: ( Bomb ... )
//          5: ( Missile ... )
//       }
//    )
//
//------------------------------------------------------------------------------
class Stores : public ExternalStore
{
   DECLARE_SUBCLASS(Stores,ExternalStore)

public:
   enum { MAX_STATIONS = 50 };

public:
   Stores();

   unsigned int getNumberOfStations() const;    // Number of stations on the launcher

   base::PairStream* getStores();              // List of external stores (Pre-ref()'d)
   const base::PairStream* getStores() const;  // List of external stores (Pre-ref()'d) (const version)

   unsigned int getNumberOfWeapons() const;     // Number of weapons on the launcher
   virtual unsigned int available() const;      // Number of weapons available for release

   unsigned int getSelectedStation() const;     // Selected station number or zero if no station is selected

   // Returns true if the weapon at station 's' is available, or
   // if 's' is zero then the selected station's availability is used.
   virtual bool isWeaponAvailable(const unsigned int s = 0) const;

   // Returns a pre-ref()'d pointer to the weapon at station 's', or
   // if 's' is zero then the 'selected' station's weapon is returned.
   // When the weapon is in release (or pre-released) mode, then the "flyout"
   // weapon is returned.
   virtual Weapon* getWeapon(const unsigned int s = 0);
   virtual const Weapon* getWeapon(const unsigned int s = 0) const; // const version

   // Returns a pre-ref()'d pointer to the external store at station 's', or
   // if 's' is zero then the 'selected' station's store is returned.
   virtual ExternalStore* getExternalStore(const unsigned int s = 0);
   virtual const ExternalStore* getExternalStore(const unsigned int s = 0) const; // const version

   // Select station number 's'; even if its weapon is not available for
   // release.  If 's' is invalid then the station remains unchanged and
   // false is returned.
   virtual bool selectStation(const unsigned int s);

   // Pre-release the weapon; that is, create the "flyout" and place it on the
   // player list.  Returns a pre-ref()'d pointer to the simulation's flyout
   // or zero if the weapon failed to pre-release.
   virtual Weapon* prereleaseWeapon(Weapon* const wpn);

   // Pre-release the weapon at station 's', or if 's' is zero then release
   // the selected station's weapon.
   virtual Weapon* prereleaseWeapon(const unsigned int s = 0);

   // Release the weapon; returns a pre-ref()'d pointer to the flyout weapon or zero if
   // the weapon failed to pre-release.
   virtual Weapon* releaseWeapon(Weapon* const wpn);

   // Release the weapon at station 's', or if 's' is zero then release
   // the selected weapon.
   virtual Weapon* releaseWeapon(const unsigned int s = 0);

   // Jettison all jettisonable stores
   virtual bool jettisonAll();

   // Event handlers
   virtual bool onJettisonEvent(Weapon* const msg);
   virtual bool onJettisonEvent(ExternalStore* const msg);

   void updateTC(const LCreal dt = 0.0) override;
   void updateData(const LCreal dt = 0.0) override;
   bool event(const int event, base::Object* const obj = nullptr) override;
   void reset() override;

protected:
   // Sets the number of stations on this launcher
   // !!! Derived classes MUST call this function in their constructor. !!!
   virtual bool setNumberOfStations(const unsigned int n);

   virtual void updateBlockedFlags();

   // Assign a weapon to a station
   virtual bool assignWeaponToStation(const unsigned int station, Weapon* const wpnPtr);

   // Assign a external store to a station
   virtual bool assignExtStoreToStation(const unsigned int station, ExternalStore* const esPtr);

   // Sends a Reset Event to all players
   void resetStores(base::PairStream* const list);

   // Slot functions
   virtual bool setSlotNumStations(base::Number* const msg);       // Number of stations
   virtual bool setSlotStores(const base::PairStream* const msg); // (clones the 'msg' list)
   virtual bool setSlotSelected(base::Number* const msg);         // Selected station

   void process(const LCreal dt) override;

private:
   void initData();

   // Full external stores list; set by setSlotStores()
   base::safe_ptr<base::PairStream> storesList;

   // Station tables
   base::safe_ptr<Weapon> weaponTbl[MAX_STATIONS];    // Weapons by station
   unsigned int numWpn;                                // Number of weapons in table

   base::safe_ptr<ExternalStore> esTbl[MAX_STATIONS]; // External store by station
   unsigned int numEs;                                 // Number of external stores in table

   unsigned int ns;                       // Number of Stations
   unsigned int selected;                 // Selection station number [ 1 ... ns ] (or zero if none)

   // Maps station number, or zero for selected, to a
   // stations[] index; returns -1 if invalid
   inline int mapSta2Idx(const unsigned int s) const {
      if (s == 0 && selected > 0 && selected <= ns) return (selected-1);
      else if (s <= ns) return (s-1);
      return -1;
   }
};

} // end simulation namespace
} // end oe namespace

#endif
