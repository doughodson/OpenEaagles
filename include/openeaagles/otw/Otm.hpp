
#ifndef __oe_otw_Otm_H__
#define __oe_otw_Otm_H__

#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/safe_ptr.hpp"

namespace oe {

namespace base { class Identifier; class Number; class String; }

namespace simulation { class IPlayer; }

namespace otw {

//------------------------------------------------------------------------------
// Class: Otm
//
// Description: OTW-Type-Mapper (OTM) --
//              Maps player's factory name and type string to a OTW entity type ID.
//
// Factory name: Otm
// Slots:
//     factoryName   <Identifier>     ! Reference factory name (default: 0)
//     typeName      <String>         ! Reference type name (default: 0)
//     modelTypeId   <Number>         ! Entity type ID number (unsigned int) (default: 0)
//
// Examples:
//    ( Otm  Aircraft  "B-1B"          203 )
//    ( Otm  Aircraft  "F-14"          309 )
//    ( Otm  Aircraft     modelTypeId: 302 )       // Default aircraft type
//
//
// Notes:
//   1) If typeName isn't given, only the factoryName will be used.
//   2) Derived class (OTW unique) can use 'modelTypeId' or add their own IDs
//
//
// Notes on comparing player types -- isMatchingPlayerType()
//
//    1) Derived classes and more specific type should be checked first
//       (see #2 & #5 below -- we get matches with the base classes and
//        the more general type names)
//
//    2) Factory names will also match base class factory names
//
//    3) Factory names are compared first and then the type names are compared
//
//    4) It's considered a match if the factory names match and no type name as given.
//
//    5) For type names, we compare at most the length of our type name ...
//          (e.g., Test player's "F-16C" would match our "F-16C"
//          (e.g., Test player's "F-16C" would match our general "F-16" type)
//          (e.g., Test player's general "F-16" type would not match our "F-16C")
//
//------------------------------------------------------------------------------
class Otm : public base::Object
{
    DECLARE_SUBCLASS(Otm, base::Object)

public:
    Otm();

    unsigned int getTypeId() const                                        { return typeId; }      // OTW entity type ID number
    virtual bool setTypeId(const unsigned int newType);                   // Sets the OTW entity type number
    virtual bool setSlotTypeId(const base::Number* const msg);            // Sets the OTW entity type number

    const base::Identifier* getRefFactoryName() const                       { return refFactoryName; } // Reference factory name
    virtual bool setSlotRefFactoryName(const base::Identifier* const msg);  // Sets the Reference factory name

    const base::String* getRefTypeName() const                           { return refTypeName; } // Reference type name
    virtual bool setSlotRefTypeName(const base::String* const msg);      // Sets the Reference type name

    // True if player's factory & type names match our reference factory and type names.
    virtual bool isMatchingPlayerType(const simulation::IPlayer* const p) const;

private:
    base::safe_ptr<const base::Identifier> refFactoryName;    // Reference factory name
    base::safe_ptr<const base::String> refTypeName;           // Reference type name (e.g., "F-16C", "T-71"
    unsigned int typeId;                                      // OTW entity type ID number
};

}
}

#endif
