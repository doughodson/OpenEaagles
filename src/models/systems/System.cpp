
#include "openeaagles/models/systems/System.hpp"

#include "openeaagles/models/players/Player.hpp"

#include "openeaagles/simulation/Simulation.hpp"

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(System, "System")


BEGIN_SLOTTABLE(System)
   "powerSwitch",    //  1) Power switch position ("OFF", "STBY", "ON") (default: "ON")
END_SLOTTABLE(System)

// Map slot table to handles
BEGIN_SLOT_MAP(System)
   ON_SLOT( 1, setSlotPowerSwitch, base::String)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Event() map
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(System)
    ON_EVENT_OBJ(KILL_EVENT,killedNotification,Player)
    ON_EVENT(KILL_EVENT,killedNotification)
END_EVENT_HANDLER()

System::System() : ownship(nullptr)
{
   STANDARD_CONSTRUCTOR()

   pwrSw = PWR_ON;   // Default: power is ON
   ownship = nullptr;
}

void System::copyData(const System& org, const bool)
{
   BaseClass::copyData(org);

   // Don't copy ownship, we'll need to reacquire it.
   ownship = nullptr;

   pwrSw = org.pwrSw;
}

void System::deleteData()
{
   ownship = nullptr;
}

//------------------------------------------------------------------------------
// isFrozen() -- checks both the system's freeze flag and its ownship's freeze flag
//------------------------------------------------------------------------------
bool System::isFrozen() const
{
   bool frz = BaseClass::isFrozen();
   if (!frz && ownship != nullptr) frz = ownship->isFrozen();
   return frz;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void System::reset()
{
   // We're nothing without an ownship ...
   if (ownship == nullptr && getOwnship() == nullptr) return;

   BaseClass::reset();
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void System::updateData(const double dt)
{
   // We're nothing without an ownship ...
   if (ownship == nullptr && getOwnship() == nullptr) return;

   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void System::updateTC(const double dt0)
{
   // We're nothing without an ownship ...
   if (ownship == nullptr && getOwnship() == nullptr) return;

   // ---
   // Delta time
   // ---

   // real or frozen?
   double dt = dt0;
   if (isFrozen()) dt = 0.0;

   // Delta time for methods that are running every fourth phase
   double dt4 = dt * 4.0f;

   // ---
   // Four phases per frame
   // ---
   simulation::Simulation* sim = ownship->getSimulation();
   if (sim == nullptr) return;

   switch (sim->phase()) {

      case 0 : // Frame0 --- Dynamics method
         dynamics(dt4);
         break;

      case 1 : // Frame1 --- Transmit method
         transmit(dt4);
         break;

      case 2 : // Frame2 --- Receive method
         receive(dt4);
         break;

      case 3 : // Frame3 --- Process method
         process(dt4);
         break;
   }

   // ---
   // Last, update our base class
   // and use 'dt' because if we're frozen then so are our subcomponents.
   // ---
   BaseClass::updateTC(dt);
}

//------------------------------------------------------------------------------
// Default phase callbacks
//------------------------------------------------------------------------------
void System::dynamics(const double)
{
}

void System::transmit(const double)
{
}

void System::receive(const double)
{
}

void System::process(const double)
{
}

//------------------------------------------------------------------------------
// killedNotification() -- Default killed notification handler
//------------------------------------------------------------------------------
bool System::killedNotification(Player* const p)
{
   // Just let all of our subcomponents know that we were just killed
   base::PairStream* subcomponents = getComponents();
   if(subcomponents != nullptr) {
      for (base::List::Item* item = subcomponents->getFirstItem(); item != nullptr; item = item->getNext()) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
         base::Component* sc = static_cast<base::Component*>(pair->object());
         sc->event(KILL_EVENT, p);
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }
   return true;
}

//-----------------------------------------------------------------------------
// Get (access) functions
//-----------------------------------------------------------------------------

// Returns a pointer to the main Simulation class
simulation::Simulation* System::getSimulation()
{
   simulation::Simulation* p = nullptr;
   if (ownship != nullptr) p = ownship->getSimulation();
   return p;
}

// Returns a pointer to the main Simulation class (const version)
const simulation::Simulation* System::getSimulation() const
{
   const simulation::Simulation* p = nullptr;
   if (ownship != nullptr) p = ownship->getSimulation();
   return p;
}

// Returns the system's master power switch setting (see power enumeration)
unsigned int System::getPowerSwitch() const
{
   return pwrSw;
}

// Returns a pointer to our ownship player
Player* System::getOwnship()
{
   if (ownship == nullptr) findOwnship();
   return ownship;
}

// Returns a pointer to our ownship player (const version)
const Player* System::getOwnship() const
{
   if (ownship == nullptr) {
      (const_cast<System*>(this))->findOwnship();
   }
   return ownship;
}

//-----------------------------------------------------------------------------
// Set functions
//-----------------------------------------------------------------------------

// Sets the system's master power switch setting (see power enumeration)
bool System::setPowerSwitch(const unsigned int p)
{
   pwrSw = p;
   return true;
}

// find our ownship
bool System::findOwnship()
{
   if (ownship == nullptr) {
      ownship = static_cast<Player*>(findContainerByType( typeid(Player) ));
   }

   return (ownship != nullptr);
}

//-----------------------------------------------------------------------------
// Set functions
//-----------------------------------------------------------------------------
bool System::setSlotPowerSwitch(const base::String* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      if (*msg == "OFF" || *msg == "off") ok = setPowerSwitch(PWR_OFF);
      else if (*msg == "STBY" || *msg == "stby") ok = setPowerSwitch(PWR_STBY);
      else if (*msg == "ON" || *msg == "on") ok = setPowerSwitch(PWR_ON);
   }
   return ok;
}

std::ostream& System::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   // Power switch (if greater than PWR_LAST then our derived class should handle this)
   if (getPowerSwitch() < PWR_LAST) {
      indent(sout,i+j);
      sout << "powerSwitch: " ;
      switch (getPowerSwitch()) {
         case PWR_OFF : sout << "OFF"; break;
         case PWR_STBY : sout << "STBY"; break;
         case PWR_ON : sout << "ON"; break;
      }
      sout << std::endl;
   }

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

}
}
