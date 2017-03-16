
#include "openeaagles/models/player/SamVehicles.hpp"

#include "openeaagles/models/player/Sam.hpp"
#include "openeaagles/models/system/StoresMgr.hpp"

#include "openeaagles/base/List.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/osg/Matrixd"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Distances.hpp"
#include "openeaagles/base/units/Times.hpp"

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(SamVehicle, "SamVehicle")
EMPTY_SERIALIZER(SamVehicle)
EMPTY_DELETEDATA(SamVehicle)

//------------------------------------------------------------------------------
// Local parameters
//------------------------------------------------------------------------------
static const double DEFAULT_MAX_LAUNCH_RANGE = 40000.0f;     // Default max launch range (meters)
static const double DEFAULT_MIN_LAUNCH_RANGE =  2000.0f;     // Default min launch range (meters)

BEGIN_SLOTTABLE(SamVehicle)
    "minLaunchRange",         // 1: Min launch range (base::Distance)
    "maxLaunchRange",         // 2: Max launch range (base::Distance)
END_SLOTTABLE(SamVehicle)

// Map slot table to handles
BEGIN_SLOT_MAP(SamVehicle)
    ON_SLOT(1, setSlotMinLaunchRange,    base::Distance)
    ON_SLOT(2, setSlotMaxLaunchRange,    base::Distance)
END_SLOT_MAP()

SamVehicle::SamVehicle()
{
   STANDARD_CONSTRUCTOR()
   static base::String generic("GenericSamSite");
   setType(&generic);

   maxMslRng = DEFAULT_MAX_LAUNCH_RANGE;
   minMslRng = DEFAULT_MIN_LAUNCH_RANGE;
}

void SamVehicle::copyData(const SamVehicle& org, const bool)
{
   BaseClass::copyData(org);

   maxMslRng = org.maxMslRng;
   minMslRng = org.minMslRng;
   numMsl = org.numMsl;
}

//------------------------------------------------------------------------------
// updateData() -- Update background stuff
//------------------------------------------------------------------------------
void SamVehicle::updateData(const double dt)
{
   BaseClass::updateData(dt);

   // ---
   // Count the number of available SAM missiles
   // ---
   unsigned int count = 0;
   const StoresMgr* sm = getStoresManagement();
   if (sm != nullptr) {

      // We have a stores manager -- but do we have any available SAMs?
      const base::PairStream* stores = sm->getStores();
      if (stores != nullptr) {
         const base::List::Item* item = stores->getFirstItem();
         while (item != nullptr) {
            const auto pair = static_cast<const base::Pair*>(item->getValue());
            if (pair != nullptr) {
               const auto sam = dynamic_cast<const Sam*>( pair->object() );
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

double SamVehicle::getMaxLaunchRange() const
{
   return maxMslRng;
}

double SamVehicle::getMinLaunchRange() const
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
bool SamVehicle::setMaxLaunchRange(const double rng)
{
   maxMslRng = rng;
   return true;
}

// Min Launch Range        (meters)
bool SamVehicle::setMinLaunchRange(const double rng)
{
   minMslRng = rng;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// maxLaunchRange: Max launch range (base::Distance)
bool SamVehicle::setSlotMaxLaunchRange(const base::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setMaxLaunchRange( base::Meters::convertStatic( *msg ) );
   }
   return ok;
}

// minLaunchRange: Min launch range (base::Distance)
bool SamVehicle::setSlotMinLaunchRange(const base::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setMinLaunchRange( base::Meters::convertStatic( *msg ) );
   }
   return ok;
}

}
}
