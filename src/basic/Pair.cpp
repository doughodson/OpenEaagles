
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
    if (object != nullptr) {
        obj = object;
        obj->ref();
    }
    else {
        obj  = nullptr;
    }
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Pair::copyData(const Pair& pair1, const bool cc)
{
    BaseClass::copyData(pair1);

    // If we're the copy constructor, init the following
    if (cc) {
        slotname = nullptr;
        obj = nullptr;
    }

    // unref() any old data
    if (slotname != nullptr) {
       slotname->unref();
    }

    if (obj != nullptr) {
       obj->unref();
    }

    // Copy slotname (already ref() by constructor in clone())
    if (pair1.slotname != nullptr) {
       slotname = static_cast<Identifier*>(pair1.slotname->clone());
    }
    else {
       slotname = nullptr;
    }

    // Copy the object (already ref() by constructor in clone())
    if (pair1.obj != nullptr) {
      obj = pair1.obj->clone();
    }
    else {
       obj = nullptr;
    }
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Pair::deleteData(void)
{
    if (slotname != nullptr) slotname->unref();
    slotname = nullptr;

    if (obj != nullptr) obj->unref();
    obj = nullptr;
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid Pair
//------------------------------------------------------------------------------
bool Pair::isValid() const
{
    if (!Object::isValid()) return false;
    if (slotname == nullptr || obj == nullptr) return false;
    return slotname->isValid() && obj->isValid();
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Pair::serialize(std::ostream& sout, const int indent, const bool) const
{
   if (slot() != nullptr && !slot()->isEmpty()) {
      sout << slot()->getString();
   }
   else {
      sout << "<null>";
   }
   sout << ": ";

   //sout << endl;

   const Object* obj = object();
   if (obj != nullptr) {
      obj->serialize(sout,indent);
   }
   else sout << "<null>";
   sout << std::endl;

   return sout;
}

} // End Basic namespace
} // End Eaagles namespace

