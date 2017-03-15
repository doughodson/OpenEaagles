
#include "openeaagles/otw/Otm.hpp"

#include "openeaagles/models/players/Player.hpp"

#include "openeaagles/base/Identifier.hpp"
#include "openeaagles/base/Number.hpp"

namespace oe {
namespace otw {

IMPLEMENT_SUBCLASS(Otm, "Otm")
EMPTY_SERIALIZER(Otm)

BEGIN_SLOTTABLE(Otm)
   "factoryName",       // 1) Reference factory name
   "typeName",          // 2) Reference type name
   "modelTypeId",       // 3) OTW entity type ID number
END_SLOTTABLE(Otm)

BEGIN_SLOT_MAP(Otm)
    ON_SLOT(1, setSlotRefFactoryName, base::Identifier)
    ON_SLOT(2, setSlotRefTypeName, base::String)
    ON_SLOT(3, setSlotTypeId,      base::Number)
END_SLOT_MAP()

Otm::Otm()
{
   STANDARD_CONSTRUCTOR()
}

void Otm::copyData(const Otm& org, const bool)
{
   BaseClass::copyData(org);

   setTypeId( org.typeId );
   setSlotRefFactoryName( org.refFactoryName );
   setSlotRefTypeName( org.refTypeName );
}

void Otm::deleteData()
{
   setTypeId( 0 );
   setSlotRefFactoryName( nullptr );
   setSlotRefTypeName( nullptr );
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the OTW entity type number
bool Otm::setTypeId(const unsigned int newType)
{
   typeId = newType;
   return true;
}

// Sets the OTW entity type number
bool Otm::setSlotTypeId(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const int i = msg->getInt();
      if (i >= 0) {
         ok = setTypeId( static_cast<unsigned int>(i) );
      }
   }
   return ok;
}

// Sets the player's factory name
bool Otm::setSlotRefFactoryName(const base::Identifier* const msg)
{
   refFactoryName = msg;
   return true;
}

// Sets the player's type name
bool Otm::setSlotRefTypeName(const base::String* const msg)
{
   refTypeName = msg;
   return true;
}

//------------------------------------------------------------------------------
// isMatchingPlayerType() -- Returns true if the factory & type names match
//------------------------------------------------------------------------------
bool Otm::isMatchingPlayerType(const models::Player* const p) const
{
   bool match = false;
   if (p != nullptr && refFactoryName != nullptr) {
      // first match the factory name --
      if (p->isFactoryName( *refFactoryName ) ) {

         // The factory names match!

         // By default we match, unless we have both type names to match,
         match = true;

         // Do we have both type names?
         const base::String* ptype = p->getType();
         if ( refTypeName != nullptr && ptype != nullptr) {

            // Then compare at most the length of our reference type name ...
            match = std::strncmp( ptype->getString(), refTypeName->getString(), refTypeName->len() ) == 0;

         }
      }
   }
   return match;
}

}
}
