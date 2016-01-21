//------------------------------------------------------------------------------
// Class: Pair
//------------------------------------------------------------------------------
#ifndef __oe_Basic_Pair_H__
#define __oe_Basic_Pair_H__

#include "openeaagles/basic/Object.h"
#include "openeaagles/basic/Identifier.h"

namespace oe {
namespace Basic {

//------------------------------------------------------------------------------
// Class: Pair
//
// Description: Slot pair (or named object); i.e., an Identifier/Object pair.
//
//    Used to set object attributes, i.e., 'slots' (see Object::setSlotByName()).
//
//    Used to name objects, since the Object class does not have a name field,
//    and since objects can be in more than one Pair, it can have more than one
//    name (aliases).
//
//
// EDL file syntax:
//      ident: <object>
//
//------------------------------------------------------------------------------
class Pair : public Object {
   DECLARE_SUBCLASS(Pair,Object)

public:
   // Constructor: the slot name and object pointer are both required!
   // -- the object is ref() by this constructor.
   Pair(const char* slot, Object* object);

   Identifier* slot()               { return slotname; } // The slot name
   const Identifier* slot() const   { return slotname; } // The slot name (const version)

   Object* object()                 { return obj; }      // The object
   const Object* object() const     { return obj; }      // The object (const version)

   bool isValid() const override;

private:
   Identifier* slotname;   // Slot name
   Object* obj;            // Object
};

} // End Basic namespace
} // End oe namespace

#endif
