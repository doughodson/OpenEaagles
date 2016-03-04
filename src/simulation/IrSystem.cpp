//==============================================================================
// Class: IrSystem
//==============================================================================

#include "openeaagles/simulation/IrSystem.h"

#include "openeaagles/simulation/IrSeeker.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Tdb.h"

#include "openeaagles/base/Number.h"
#include "openeaagles/base/PairStream.h"

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(IrSystem,"IrSystem")
EMPTY_SERIALIZER(IrSystem)

// Slot table
BEGIN_SLOTTABLE(IrSystem)
   "seekerName",        //  1: Name of the requested Seeker  (basic::String)
   "disableQueries",    //  2: Disable sending queries packets flag (default: false)
END_SLOTTABLE(IrSystem)

//  Map slot table
BEGIN_SLOT_MAP(IrSystem)
   ON_SLOT(1,  setSlotSeekerName,  basic::String)
   ON_SLOT(2,  setSlotDisableQueries,  basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
IrSystem::IrSystem() : seeker(nullptr) , seekerName(nullptr)
{
   STANDARD_CONSTRUCTOR()

   disableQueries = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IrSystem::copyData(const IrSystem& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      seeker = nullptr;
      seekerName = nullptr;
   }

   disableQueries = org.disableQueries;

   // No seeker yet
   setSeeker(nullptr);
   basic::String* p = const_cast<basic::String*>(static_cast<const basic::String*>(org.getSeekerName()));
   setSlotSeekerName( p );
}


//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void IrSystem::deleteData()
{
   setSeeker(nullptr);
   setSlotSeekerName(nullptr);
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool IrSystem::shutdownNotification()
{
   setSeeker(nullptr);
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void IrSystem::reset()
{
   // FAB - sensor needs to know its seeker without waiting for updateData
   if (getSeeker() == nullptr && getSeekerName() != nullptr && getOwnship() != nullptr) {
      // We have a name of the seeker, but not the seeker itself
      const char* name = *getSeekerName();

      // Get the named seeker from the player's list of gimbals
      IrSeeker* p = dynamic_cast<IrSeeker*>( getOwnship()->getGimbalByName(name) );
      if (p != nullptr) {
         setSeeker( p );
      }

      if (getSeeker() == nullptr) {
         // The assigned seeker was not found!
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSystem::update() ERROR -- seeker: " << name << ", was not found!" << std::endl;
         }
         setSlotSeekerName(nullptr);
      }
   }

   BaseClass::reset();
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void IrSystem::updateData(const LCreal dt)
{
   // ---
   // Do we need to find the seeker?
   // ---
   if (getSeeker() == nullptr && getSeekerName() != nullptr && getOwnship() != nullptr) {
      // We have a name of the seeker, but not the seeker itself
      const char* name = *getSeekerName();

      // Get the named seeker from the player's list of gimbals
      IrSeeker* p = dynamic_cast<IrSeeker*>( getOwnship()->getGimbalByName(name) );
      if (p != nullptr) {
         setSeeker( p );
         // FAB - not needed - esp if multiple sensors on a seeker.
         //getSeeker()->setSystem(this);
      }

      if (getSeeker() == nullptr) {
         // The assigned seeker was not found!
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSystem::update() ERROR -- seeker: " << name << ", was not found!" << std::endl;
         }
         setSlotSeekerName(nullptr);
      }
   }

   // ---
   // Process our players of interest
   // ---
   processPlayersOfInterest();

   // ---
   // Our base class methods
   // ---
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// Process players of interest -- This will work with the function in Gimbal to create
// a filtered list of players that we plan to send emission packets to.
//------------------------------------------------------------------------------
void IrSystem::processPlayersOfInterest()
{
   // ---
   // Do we have a seeker?
   // ---
   if (getSeeker() != nullptr) {
      basic::PairStream* poi = nullptr;
      Simulation* sim = getSimulation();
      if ( sim != nullptr && !areQueriesDisabled() )
         poi = getSimulation()->getPlayers();

      // Pass our players of interest to the seeker for processing
      getSeeker()->processPlayersOfInterest(poi);

      if (poi != nullptr) { poi->unref(); poi = nullptr; }
   }
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// isQuerying() -- Returns true if the IR system is currently observing targets
bool IrSystem::isQuerying() const
{
    // Default: if we're enabled and have a seeker, we're transmitting.
    return ( !areQueriesDisabled() && getSeeker() != nullptr && getOwnship() != nullptr );
}

// Returns true if sending query packets has been disabled
bool IrSystem::areQueriesDisabled() const
{
   return disableQueries;
}

// Pointer to the seeker model, or zero (0) if none
IrSeeker* IrSystem::getSeeker()
{
   return seeker;
}

// Pointer to the seeker model, or zero (0) if none
const IrSeeker* IrSystem::getSeeker() const
{
   return seeker;
}

// Name of the seeker model, or zero (0) if none
const basic::String* IrSystem::getSeekerName() const
{
   return seekerName;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Disables/enables sending the R/F emissions packets
bool IrSystem::setDisableQueriesFlag(const bool b)
{
   disableQueries = b;
   return true;
}

// setSeeker() -- set the seeker
bool IrSystem::setSeeker(IrSeeker* const p)
{
   if (seeker != nullptr) {
      seeker->unref();
   }
   seeker = p;
   if (seeker != nullptr) {
      seeker->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// Slot Functions  (return 'true' if the slot was set, else 'false')
//------------------------------------------------------------------------------

// seekerName: IrSeeker name  (basic::String)
bool IrSystem::setSlotSeekerName(basic::String* const p)
{
   if (seekerName != nullptr) {
      seekerName->unref();
   }
   seekerName = p;
   if (seekerName != nullptr) {
      seekerName->ref();
   }
   return true;
}

// setSlotDisableQueries() -- sets the disable sending emissions flag
bool IrSystem::setSlotDisableQueries(basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setDisableQueriesFlag( msg->getBoolean() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
basic::Object* IrSystem::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End simulation namespace
} // End oe namespace

