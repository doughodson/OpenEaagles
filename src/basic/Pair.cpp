#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/Boolean.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(Pair, "Pair")
EMPTY_SLOTTABLE(Pair)

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
Pair::Pair(const char* slot, Object* object)
{
    STANDARD_CONSTRUCTOR()

    // Set the slot name (already ref() in 'new' constructor)
    slotname = new Identifier(slot);

    // Set the object & ref()
    if (object != 0) {
        obj = object;
        obj->ref();
    }
    else
        obj  = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Pair::copyData(const Pair& pair1, const bool cc)
{
    BaseClass::copyData(pair1);

    // If we're the copy constructor, init the following
    if (cc) {
        slotname = 0;
        obj = 0;
    }

    // unref() any old data
    if (slotname != 0) slotname->unref();
    if (obj != 0) obj->unref();

    // Copy slotname (already ref() by constructor in clone())
    if (pair1.slotname != 0) slotname = (Identifier*) pair1.slotname->clone();
    else slotname = 0;

    // Copy the object (already ref() by constructor in clone())
    if (pair1.obj != 0) {
      obj = pair1.obj->clone();
    }
    else obj = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Pair::deleteData(void)
{
    if (slotname != 0) slotname->unref();
    slotname = 0;

    if (obj != 0) obj->unref();
    obj = 0;
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid Pair
//------------------------------------------------------------------------------
bool Pair::isValid() const
{
    if (!Object::isValid()) return false;
    if (slotname == 0 || obj == 0) return false;
    return slotname->isValid() && obj->isValid();
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Pair::serialize(std::ostream& sout, const int indent, const bool) const
{
   if (slot() != 0 && !slot()->isEmpty()) {
      sout << slot()->getString();
   }
   else {
      sout << "<null>";
   }
   sout << ": ";

   //sout << endl;

   const Object* obj = object();
   if (obj != 0) {
      obj->serialize(sout,indent);
   }
   else sout << "<null>";
   sout << std::endl;

   return sout;
}

} // End Basic namespace
} // End Eaagles namespace

