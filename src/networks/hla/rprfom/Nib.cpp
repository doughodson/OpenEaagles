//------------------------------------------------------------------------------
// Class: RprFom::Nib
//------------------------------------------------------------------------------

#include "openeaagles/networks/hla/rprfom/Nib.h"

#include "openeaagles/networks/hla/rprfom/NetIO.h"
#include "openeaagles/networks/hla/rprfom/RprFom.h"
#include "openeaagles/networks/hla/Ambassador.h"

#include "openeaagles/networks/dis/Ntm.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Signatures.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/Nav.h"
#include "openeaagles/base/NetHandler.h"
#include "openeaagles/base/Number.h"

namespace oe {
namespace hla {
namespace rprfom {

IMPLEMENT_PARTIAL_SUBCLASS(Nib,"Nib")
EMPTY_SLOTTABLE(Nib)
EMPTY_SERIALIZER(Nib)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Nib::Nib(const simulation::NetIO::IoType ioType) : hla::Nib(ioType) 
{
   STANDARD_CONSTRUCTOR()

   baseEntity = 0;
   fireEvent = 0;
   geodPos[0] = 0;
   geodPos[1] = 0;
   geodPos[2] = 0;
   haveWorldLocationFlg = false;
   haveOrientationFlg = false;
   haveEntityTypeFlg = false;
   haveEntityIdFlg = false;

   disKind = 0;
   disDomain = 0;
   disCountry = 0;
   disCategory = 0;
   disSubcategory = 0;
   disSpecific = 0;
   disExtra = 0;

   siteID = 0;
   appID = 0;
}

Nib::Nib(const Nib& org) : hla::Nib(org.getIoType())
{ 
   STANDARD_CONSTRUCTOR()
    copyData(org,true);
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

//------------------------------------------------------------------------------
// copyData() --
//------------------------------------------------------------------------------
void Nib::copyData(const Nib& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) {
   baseEntity = 0;
   }

   // Base entity
   if (org.baseEntity != 0)
      setBaseEntity( (BaseEntity*) org.baseEntity->clone() );
   else
      setBaseEntity(0);

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

//------------------------------------------------------------------------------
// deleteData() --
//------------------------------------------------------------------------------
void Nib::deleteData()
{
    setBaseEntity(0);
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void Nib::setBaseEntity(BaseEntity* const p)
{
   if (baseEntity != 0) baseEntity->unref();
   baseEntity = p;
   if (baseEntity != 0) baseEntity->ref();
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
   simulation::Player* p = getPlayer();

   // ---
   // If we haven't tried to created the IPlayer yet and we have at least position,
   // velocity and orientation, then try to create one
   // ---
   if (p == 0 && isEntityTypeUnchecked() && haveEntityIdFlg && haveEntityTypeFlg && haveWorldLocationFlg && haveOrientationFlg) {
      // Create the player
      p = getNetIO()->createIPlayer(this);
   }

   // ---
   // When we have a player, update it's data from our object's attributes
   // ---
   if (p != 0) {
      // This transfers player data from our basic NIB to the player.
      nib2PlayerState();
      // ... transfer additional data in the future ....
   }
}

}
}
}
