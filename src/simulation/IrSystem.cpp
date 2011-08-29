//==============================================================================
// Class: IrSystem
//==============================================================================

#include "openeaagles/simulation/IrSystem.h"

#include "openeaagles/simulation/IrSeeker.h"
#include "openeaagles/simulation/OnboardComputer.h" 
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Tdb.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(IrSystem,"IrSystem")
EMPTY_SERIALIZER(IrSystem)

// Slot table
BEGIN_SLOTTABLE(IrSystem)
   "seekerName",        //  1: Name of the requested Seeker  (Basic::String)
   "disableQueries",    //  2: Disable sending queries packets flag (default: false)
END_SLOTTABLE(IrSystem)

//  Map slot table 
BEGIN_SLOT_MAP(IrSystem)
   ON_SLOT(1,  setSlotSeekerName,  Basic::String)
   ON_SLOT(2,  setSlotDisableQueries,  Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
IrSystem::IrSystem() : seeker(0) , seekerName(0)
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
      seeker = 0;
      seekerName = 0;
   }

   disableQueries = org.disableQueries;

   // No seeker yet
   setSeeker(0);
   Basic::String* p = (Basic::String*) org.getSeekerName();
   setSlotSeekerName( p );
}

    
//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void IrSystem::deleteData()
{
   setSeeker(0);
   setSlotSeekerName(0);
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void IrSystem::reset()
{
   // FAB - sensor needs to know its seeker without waiting for updateData
   if (getSeeker() == 0 && getSeekerName() != 0 && getOwnship() != 0) {
      // We have a name of the seeker, but not the seeker itself
      const char* name = *getSeekerName();
      
      // Get the named seeker from the player's list of gimbals
      IrSeeker* p = dynamic_cast<IrSeeker*>( getOwnship()->getGimbalByName(name) );
      if (p != 0) {
         setSeeker( p );
      }
      
      if (getSeeker() == 0) {
         // The assigned seeker was not found!
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSystem::update() ERROR -- seeker: " << name << ", was not found!" << std::endl;
         }
         setSlotSeekerName(0);
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
   if (getSeeker() == 0 && getSeekerName() != 0 && getOwnship() != 0) {
      // We have a name of the seeker, but not the seeker itself
      const char* name = *getSeekerName();
      
      // Get the named seeker from the player's list of gimbals
      IrSeeker* p = dynamic_cast<IrSeeker*>( getOwnship()->getGimbalByName(name) );
      if (p != 0) {
         setSeeker( p );
         // FAB - not needed - esp if multiple sensors on a seeker.
		 //getSeeker()->setSystem(this);
      }
      
      if (getSeeker() == 0) {
         // The assigned seeker was not found!
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSystem::update() ERROR -- seeker: " << name << ", was not found!" << std::endl;
         }
         setSlotSeekerName(0);
      }
   }

   // ---
   // Process our player's of interest
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
   if (getSeeker() != 0) {
      Basic::PairStream* poi = 0;
      Simulation* sim = getSimulation();
      if ( sim != 0 && !areQueriesDisabled() ) poi = getSimulation()->getPlayers();

      // Pass our players of interest to the seeker for processing
      getSeeker()->processPlayersOfInterest(poi);

      if (poi != 0) { poi->unref(); poi = 0; }
   }
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// isQuerying() -- Returns true if the IR system is currently observing targets
bool IrSystem::isQuerying() const
{
    // Default: if we're enabled and have a seeker, we're transmitting.
    return ( !areQueriesDisabled() && getSeeker() != 0 && getOwnship() != 0 );
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
const Basic::String* IrSystem::getSeekerName() const
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
   if (seeker != 0) {
      seeker->unref();
   }
   seeker = p;
   if (seeker != 0) {
      seeker->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// Slot Functions  (return 'true' if the slot was set, else 'false')
//------------------------------------------------------------------------------

// seekerName: IrSeeker name  (Basic::String)
bool IrSystem::setSlotSeekerName(Basic::String* const p)
{
   if (seekerName != 0) {
      seekerName->unref();
   }
   seekerName = p;
   if (seekerName != 0) {
      seekerName->ref();
   }
   return true;
}

// setSlotDisableQueries() -- sets the disable sending emissions flag
bool IrSystem::setSlotDisableQueries(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setDisableQueriesFlag( msg->getBoolean() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* IrSystem::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End Simulation namespace
} // End Eaagles namespace

