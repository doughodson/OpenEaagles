#include "openeaagles/simulation/SamVehicles.h"

#include "openeaagles/simulation/Sam.h"
#include "openeaagles/simulation/StoresMgr.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// class SamVehicle
//==============================================================================
IMPLEMENT_SUBCLASS(SamVehicle,"SamVehicle")
EMPTY_SERIALIZER(SamVehicle)

//------------------------------------------------------------------------------
// Local parameters
//------------------------------------------------------------------------------
static const LCreal DEFAULT_MAX_LAUNCH_RANGE = 40000.0f;     // Default max launch range (meters)
static const LCreal DEFAULT_MIN_LAUNCH_RANGE =  2000.0f;     // Default min launch range (meters)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(SamVehicle)
    "minLaunchRange",         // 1: Min launch range (Basic::Distance)
    "maxLaunchRange",         // 2: Max launch range (Basic::Distance)
END_SLOTTABLE(SamVehicle)

// Map slot table to handles
BEGIN_SLOT_MAP(SamVehicle)
    ON_SLOT(1, setSlotMinLaunchRange,    Basic::Distance)
    ON_SLOT(2, setSlotMaxLaunchRange,    Basic::Distance)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SamVehicle::SamVehicle()
{
   STANDARD_CONSTRUCTOR()
   static Basic::String generic("GenericSamSite");
   setType(&generic);

   maxMslRng = DEFAULT_MAX_LAUNCH_RANGE;
   minMslRng = DEFAULT_MIN_LAUNCH_RANGE;
   numMsl = 0;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void SamVehicle::copyData(const SamVehicle& org, const bool)
{
   BaseClass::copyData(org);

   maxMslRng = org.maxMslRng;
   minMslRng = org.minMslRng;
   numMsl = org.numMsl;
}

void SamVehicle::deleteData()
{
}

//------------------------------------------------------------------------------
// updateData() -- Update background stuff
//------------------------------------------------------------------------------
void SamVehicle::updateData(const LCreal dt)
{
   BaseClass::updateData(dt);

   // ---
   // Count the number of available SAM missiles
   // ---
   unsigned int count = 0;
   const StoresMgr* sm = getStoresManagement();
   if (sm != nullptr) {

      // We have a stores manager -- but do we have any available SAMs?
      const Basic::PairStream* stores = sm->getStores();
      if (stores != nullptr) {
         const Basic::List::Item* item = stores->getFirstItem();
         while (item != nullptr) {
            const Basic::Pair* pair = static_cast<const Basic::Pair*>(item->getValue());
            if (pair != nullptr) {
               const Sam* sam = dynamic_cast<const Sam*>( pair->object() );
               if (sam != nullptr) {
                  // We have a SAM that hasn't been launched
                  if (sam->isInactive()) count++;
               }
            }
            item = item->getNext();
         }
         stores->unref();
         stores = nullptr;
      }

   }
   numMsl = count;
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------
unsigned int SamVehicle::getNumberOfMissiles() const
{
   return numMsl;
}

LCreal SamVehicle::getMaxLaunchRange() const
{
   return maxMslRng;
}

LCreal SamVehicle::getMinLaunchRange() const
{
   return minMslRng;
}

bool SamVehicle::isLauncherReady() const
{
   // We're ready if we have missiles
   return (getNumberOfMissiles() > 0);
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Max Launch Range        (meters)
bool SamVehicle::setMaxLaunchRange(const LCreal rng)
{
   maxMslRng = rng;
   return true;
}

// Min Launch Range        (meters)
bool SamVehicle::setMinLaunchRange(const LCreal rng)
{
   minMslRng = rng;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// maxLaunchRange: Max launch range (Basic::Distance)
bool SamVehicle::setSlotMaxLaunchRange(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setMaxLaunchRange( Basic::Meters::convertStatic( *msg ) );
   }
   return ok;
}

// minLaunchRange: Min launch range (Basic::Distance)
bool SamVehicle::setSlotMinLaunchRange(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setMinLaunchRange( Basic::Meters::convertStatic( *msg ) );
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* SamVehicle::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End Simulation namespace
} // End Eaagles namespace
