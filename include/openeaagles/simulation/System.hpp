
#ifndef __oe_simulation_System_H__
#define __oe_simulation_System_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
   namespace base { class String; }

namespace simulation {

class Player;
class Simulation;

//------------------------------------------------------------------------------
// Class: System
//
// Description: Base classes for all simulation systems that can be attached
//              to a player.
//
// Factory name: System
// Slots:
//    powerSwitch    <base::String>   ! Power switch position ("OFF", "STBY", "ON") (default: "ON")
//
//
// Events:
//    KILL_EVENT        ()       Process a (we were just) killed events
//
//
// Notes:
//
//    1) Time critical phase Callbacks -- The following virtual member functions
//       are to be overridden by derived systems, as needed.  Each function is
//       called one per frame during the corresponding phase.
//
//      virtual void dynamics(const double dt);    // Phase 0
//      virtual void transmit(const double dt);    // Phase 1
//      virtual void receive(const double dt);     // Phase 2
//      virtual void process(const double dt);     // Phase 3
//
//    2) Power switch enums --
//       a) Current switch enums are PWR_OFF, PWR_STBY and PWR_ON.
//       b) PWR_LAST is use by derived systems to expand the power switch list
//             enum {
//                PWR_NEW1 = BaseClass::PWR_LAST,
//                PWR_NEW2,
//                PWR_LAST
//             };
//       c) derived systems can extend the 'powerSwitch' slot, setSlotPowerSwitch(),
//          for additional switch positions.
//
//    3) Systems inherit the SHUTDOWN_EVENT event from Component.  So, make sure
//       to use this to shutdown threads and network connections that you may
//       have created, and to unref() objects that can't wait until deleteData()
//       (e.g., circular references).  (see Component::shutdownNotification())
//
//------------------------------------------------------------------------------
class System : public base::Component
{
   DECLARE_SUBCLASS(System, base::Component)

public:
   // Power switch enumerations (can be expanded by derived classes)
   enum {
      PWR_OFF,
      PWR_STBY,
      PWR_ON,        // On, Normal, Operate, etc.
      PWR_LAST       // Hook for subclasses to expand
   };

public:
   System();

   virtual unsigned int getPowerSwitch() const;          // Returns the system's master power switch setting (see power enumeration)
   virtual bool setPowerSwitch(const unsigned int p);    // Sets the system's master power switch setting (see power enumeration)

   virtual Player* getOwnship();                         // Returns a pointer to our ownship player
   virtual const Player* getOwnship() const;             // Returns a pointer to our ownship player (const version)

   virtual Simulation* getSimulation();                  // Returns a pointer to the Simulation model
   virtual const Simulation* getSimulation() const;      // Returns a pointer to the Simulation model (const version)

   // Event handler(s)
   virtual bool killedNotification(Player* const killedBy = 0); // Killed (KILL_EVENT) event handler

   virtual void updateData(const double dt = 0.0) override;
   virtual void updateTC(const double dt = 0.0) override;
   virtual bool event(const int event, base::Object* const obj = nullptr) override;
   virtual void reset() override;
   virtual bool isFrozen() const override;

protected:
   // Slot function(s)
   virtual bool setSlotPowerSwitch(const base::String* const msg);

   // Time critical phase callbacks --
   // --- to be used by the derived classes, as needed
   virtual void dynamics(const double dt);    // Phase 0
   virtual void transmit(const double dt);    // Phase 1
   virtual void receive(const double dt);     // Phase 2
   virtual void process(const double dt);     // Phase 3

private:
   bool findOwnship();

   Player* ownship;      // Our player (not ref()'d because the own player owns us).
   unsigned int pwrSw;   // System's master power switch
};

}
}

#endif
