//------------------------------------------------------------------------------
// Class: ExternalStore
//------------------------------------------------------------------------------
#ifndef __ExternalStore_H_61E93E3C_90E2_405a_B2E3_9E7BA25F6683__
#define __ExternalStore_H_61E93E3C_90E2_405a_B2E3_9E7BA25F6683__

#include "openeaagles/simulation/System.h"

namespace Eaagles {
   namespace Basic { class String; }

namespace Simulation {

//------------------------------------------------------------------------------
// Class: ExternalStore
//
// Description: Base class for non-Weapon class, external stores.  These may
//              include bomb racks, missile launchers, fuel tanks, avionics pods,
//              and gun pods.
//
// Form name: ExternalStore
// Slots:
//    type         <Basic::String>  ! Type string (default: 0)
//    jettisonable <Boolean>        ! Store can be jettisoned (default: true )
//
// Events:
//      JETTISON_EVENT      Jettison event
//
// Notes:
//    1) ExternalStore class objects are usually updated by a parent Stores class
//       object during time-critical phases and the background thread.
//
//    2) ExternalStore objects are not released, but may be jettisoned. If the object
//       if jettisoned, it will no longer be updated.
//
// GUID: {61E93E3C-90E2-405a-B2E3-9E7BA25F6683}
//------------------------------------------------------------------------------
class ExternalStore : public System
{
   DECLARE_SUBCLASS(ExternalStore,System)

public:
   ExternalStore();

   const Basic::String* getType() const;

   bool isJettisonable() const;  // True if the weapon can be jettisoned
   bool isJettisoned() const;    // True if the weapon has been jettisoned

   // Event handler(s)
   virtual bool onJettisonEvent();

   // Basic::Component interface
   virtual bool event(const int event, Basic::Object* const obj = 0);
   virtual void reset();

protected:
   bool setJettisonable(const bool f);    // Sets the jettison enable flag

   // Slot function(s)
   virtual bool setSlotType(const Basic::String* const msg);
   virtual bool setSlotJettisonable(Basic::Number* const p);

private:
   void initData();

   SPtr<const Basic::String> type; // Type string for external store
   bool        canJettison;     // Weapon can be jettisoned.
   bool        jettisoned;      // Weapon has been jettisoned.
};

} // end Simulation namespace
} // end Eaagles namespace

#endif // __ExternalStore_H_61E93E3C_90E2_405a_B2E3_9E7BA25F6683__
