
#include "openeaagles/interop/rprfom/Nib.hpp"

#include "openeaagles/interop/rprfom/NetIO.hpp"
#include "openeaagles/interop/rprfom/RprFom.hpp"
#include "openeaagles/interop/hla/Ambassador.hpp"

#include "openeaagles/interop/dis/Ntm.hpp"

#include "openeaagles/models/players/AirVehicle.hpp"
#include "openeaagles/models/players/Missile.hpp"
#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/Signatures.hpp"

#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/String.hpp"
#include "openeaagles/base/network/NetHandler.hpp"
#include "openeaagles/base/Number.hpp"

namespace oe {
namespace rprfom {

IMPLEMENT_PARTIAL_SUBCLASS(Nib, "Nib")
EMPTY_SLOTTABLE(Nib)
EMPTY_SERIALIZER(Nib)

Nib::Nib(const interop::NetIO::IoType ioType) : hla::Nib(ioType) 
{
   STANDARD_CONSTRUCTOR()
}

Nib::Nib(const Nib& org) : hla::Nib(org.getIoType())
{ 
   STANDARD_CONSTRUCTOR()
   copyData(org, true);
}

Nib::~Nib()
{
   deleteData();
}

Nib& Nib::operator=(const Nib& org)
{
   deleteData();
   copyData(org,false);
   return *this;
}

Nib* Nib::clone() const
{
   return new Nib(*this);
}

void Nib::copyData(const Nib& org, const bool)
{
   BaseClass::copyData(org);

   // Base entity
   if (org.baseEntity != nullptr)
      setBaseEntity( (BaseEntity*) org.baseEntity->clone() );
   else
      setBaseEntity(nullptr);

   // other stuff
   fireEvent = org.fireEvent;
   geodPos[0] = org.geodPos[0];
   geodPos[1] = org.geodPos[1];
   geodPos[2] = org.geodPos[2];
   haveWorldLocationFlg = org.haveWorldLocationFlg;
   haveOrientationFlg = org.haveOrientationFlg;
   haveEntityTypeFlg = org.haveEntityTypeFlg;
   haveEntityIdFlg = org.haveEntityIdFlg;

   disKind = org.disKind;
   disDomain = org.disDomain;
   disCountry = org.disCountry;
   disCategory = org.disCategory;
   disSubcategory = org.disSubcategory;
   disSpecific = org.disSpecific;
   disExtra = org.disExtra;

   siteID = org.siteID;
   appID = org.appID;
}

void Nib::deleteData()
{
    setBaseEntity(nullptr);
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void Nib::setBaseEntity(BaseEntity* const p)
{
   if (baseEntity != nullptr) baseEntity->unref();
   baseEntity = p;
   if (baseEntity != nullptr) baseEntity->ref();
}

void Nib::setWeaponFireEvent(const unsigned short e)
{
   fireEvent = e;
}

/// Set (DIS) Entity type enumerations
bool Nib::setEntityType(
       const unsigned char k,       // Kind
       const unsigned char d,       // Domain
       const unsigned short cc,     // Country code
       const unsigned char c,       // Category
       const unsigned char sc,      // Subcategory
       const unsigned char sp,      // Specific
       const unsigned char e        // Extra
    )
{
   // Some simple validity checks
   bool ok = (k < dis::NetIO::NUM_ENTITY_KINDS) || (d < dis::NetIO::MAX_ENTITY_DOMAINS);

   if (ok) {
      disKind = k;    
      disDomain = d;
      disCountry = cc;
      disCategory = c;
      disSubcategory = sc;
      disSpecific = sp;
      disExtra = e;
   }

   return ok;
}

//------------------------------------------------------------------------------
// isPlayerStateUpdateRequiredForOutput() -- check to see if an update is required
//------------------------------------------------------------------------------
bool Nib::isPlayerStateUpdateRequired(const double curExecTime)
{
    // Check the update required flags first ...
    bool required = 
      isAttributeUpdateRequired(NetIO::SPATIAL_AI) ||
      isAttributeUpdateRequired(NetIO::ENTITY_TYPE_AI) ||
      isAttributeUpdateRequired(NetIO::ENTITY_IDENTIFIER_AI) ||
      isAttributeUpdateRequired(NetIO::FORCE_IDENTIFIER_AI);

    // Check the base class next ...
    if (!required) {
        required = BaseClass::isPlayerStateUpdateRequired(curExecTime);
    }

    return required;
}


//------------------------------------------------------------------------------
// updateTheIPlayer() -- (Input support)
//   Called by our processInputList() to it's time to update the EAAGLES's
//   networked player.
//------------------------------------------------------------------------------
void Nib::updateTheIPlayer()
{
   models::Player* p = getPlayer();

   // ---
   // If we haven't tried to created the IPlayer yet and we have at least position,
   // velocity and orientation, then try to create one
   // ---
   if (p == nullptr && isEntityTypeUnchecked() && haveEntityIdFlg && haveEntityTypeFlg && haveWorldLocationFlg && haveOrientationFlg) {
      // Create the player
      p = getNetIO()->createIPlayer(this);
   }

   // ---
   // When we have a player, update it's data from our object's attributes
   // ---
   if (p != nullptr) {
      // This transfers player data from our basic NIB to the player.
      nib2PlayerState();
      // ... transfer additional data in the future ....
   }
}

}
}
