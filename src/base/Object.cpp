//------------------------------------------------------------------------------
// Object class
//------------------------------------------------------------------------------

#include "openeaagles/base/Object.h"

#include <cctype>
#include <cstdlib>
#include <cstring>

namespace oe {
namespace base {

// ---
// Table of registered classes
//    Note: class Object is pre-registered as the first class
// ---
const Object::_Static* Object::classes[MAX_CLASSES] = { &Object::_static };
unsigned int Object::numClasses = 1;

// ---
// Object's static member data
// ---
Object::_Static Object::_static(0, typeid(Object).name(), "Object", &Object::slottable, nullptr);

// ---
// Object's SlotTable
//    Note: Keep our slot table empty.  There are several light weight classes derived
//    from Object (e.g., the classes for colors, numbers and units) that expect their
//    slots to be the first slots -- starting at slot number 1.
// ---
const char* Object::slotnames[] = { "" };
const int Object::nslots = 0;
const SlotTable Object::slottable(nullptr, 0);


//------------------------------------------------------------------------------
// Standard object stuff -- derived classes used macro IMPLEMENT_SUBCLASS, see macros.h
//------------------------------------------------------------------------------

// Constructor
Object::Object()
{
   STANDARD_CONSTRUCTOR()
   refCount = 1;    // (start out ref() by the creator)
   semaphore = 0;
   enbMsgBits = (MSG_ERROR | MSG_WARNING);
   disMsgBits = 0;
}

Object::Object(const Object& org)
{
   STANDARD_CONSTRUCTOR()
   copyData(org,true);
}

// Destructor
Object::~Object()
{
   STANDARD_DESTRUCTOR()
}

Object& Object::operator=(const Object& org)
{
    if (this != &org) copyData(org);
    return *this;
}

// Clone
Object* Object::clone() const
{
   return new Object(*this);
}

// Check class type
bool Object::isClassType(const std::type_info& type) const
{
    if ( type == typeid(Object) ) return true;
    else return false;
}

// Check factory name
bool Object::isFactoryName(const char name[]) const
{
    if (name == nullptr) return false;
    if ( std::strcmp(_static.fname, name) == 0 )  return true;
    else return false;
}

// Copy object data -- derived classes should call
// BaseClass::copyData() first and then copy their data.
void Object::copyData(const Object& org, const bool cc)
{
    slotTable = org.slotTable;
    enbMsgBits = org.enbMsgBits;
    disMsgBits = org.disMsgBits;
    if (cc) {
       refCount = 1;    // (start out ref() by the creator)
       semaphore = 0;
    }
}

// Delete object data -- derived classes should delete
// or unref() their own data
void Object::deleteData()
{
}

// set slots by index
bool Object::setSlotByIndex(const int, Object* const)
{
    // We have no slots, so we shouldn't ever be here!
    return false;
}

// get slots by index
Object* Object::getSlotByIndex(const int)
{
    // We have no slots, so we shouldn't ever be here!
    return nullptr;
}

// get factory name
const char* Object::getFactoryName()
{
    return _static.fname;
}

// get class name
const char* Object::getClassName()
{
    return _static.cname;
}

// get slot table
const SlotTable& Object::getSlotTable()
{
   return slottable;
}

//------------------------------------------------------------------------------
// slotName2Index() -- returns the index of the slot named 'slotname'
//------------------------------------------------------------------------------
int Object::slotName2Index(const char* const slotname) const
{
   int slotindex = 0;

   // No 'slotname' then no slot index
   if (slotname == nullptr) {
      return slotindex;
   }

   // How many slots do we have
   int n = slotTable->n();

   // a) check if 'slotname' is a number (e.g., "12")
   bool isNum = true;
   for (int i = 0; isNum && slotname[i] != '\0'; i++) {
      if ( !std::isdigit(slotname[i]) ) isNum = false;
   }

   // b) convert 'slotname' to a slot index
   if (isNum) {
      // when the slotname is just a number (e.g., "12")
      int j = std::atoi(slotname);
      if (j > 0 && j <= n) slotindex = j;
   }
   else {
      // when the 'slotname' is a name (e.g., "some-slot")
      slotindex = slotTable->index(slotname);
      if (slotindex <= 0)
      std::cerr << "slot not found: " << slotname << std::endl;
   }

    return slotindex;
}


//------------------------------------------------------------------------------
// setSlotByName() -- set the value of slot 'slotname' to 'obj'  Returns
//                 true if the slot and object were processed; returns
//                 false if there was an error.
//------------------------------------------------------------------------------
bool Object::setSlotByName(const char* const slotname, Object* const obj)
{
    bool ok = false;
    if (obj == nullptr) return ok;
    int slotindex = slotName2Index(slotname);
    if (slotindex > 0) ok = setSlotByIndex(slotindex,obj);
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByName() -- Returns a pointer to the slot named 'slotname'.
//------------------------------------------------------------------------------
Object* Object::getSlotByName(const char* const slotname)
{
    Object* obj = nullptr;
    int slotindex = slotName2Index(slotname);
    if (slotindex > 0) obj = getSlotByIndex(slotindex);
    return obj;
}

//------------------------------------------------------------------------------
// slotIndex2Name() -- returns the name of the slot at 'slotindex'
//------------------------------------------------------------------------------
const char* Object::slotIndex2Name(const int slotindex) const
{
    return  slotTable->name(slotindex);
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid object
//------------------------------------------------------------------------------
bool Object::isValid() const
{
    return (refCount > 0);
}

//------------------------------------------------------------------------------
// Message types
//------------------------------------------------------------------------------

bool Object::isMessageEnabled(const unsigned short msgType) const
{
   bool enabled = false;

   if (msgType != 0) {
      if ((msgType & MSG_ERROR) != 0) {
         // Error messages are always enabled
         enabled = true;
      }
      else {
         // enabled if any bits match
         enabled = (msgType & enbMsgBits) != 0;
      }
   }
   return enabled;
}

bool Object::isMessageDisabled(const unsigned short msgType) const
{
   bool disabled = false;

   if (msgType != 0) {
      if ((msgType & MSG_ERROR) != 0) {
         // Error messages are not disabled
         disabled = false;
      }
      else {
         // disabled if any bits match
         disabled = (msgType & disMsgBits) != 0;
      }
   }

   return disabled;
}

bool Object::enableMessageTypes(const unsigned short msgTypeBits)
{
   // Set the enabled bits
   enbMsgBits |= msgTypeBits;

   // Clear the disabled bits
   disMsgBits &= ~msgTypeBits;

   return true;
}

bool Object::disableMessageTypes(const unsigned short msgTypeBits)
{
   // Set the disabled bits
   disMsgBits |= msgTypeBits;

   // Clear the enabled bits
   enbMsgBits &= ~msgTypeBits;

   return true;
}

//------------------------------------------------------------------------------
// Writes out the class list
//------------------------------------------------------------------------------
void Object::writeClassList(std::ostream& sout)
{
   sout << "Number of classes: " << numClasses << std::endl;
   for (unsigned int i = 0; i < numClasses; i++) {
      sout
         << classes[i]->classIndex
         << ": " << classes[i]->fname
         << "(" << classes[i]->count
         << "/" << classes[i]->mc
         << "/" << classes[i]->tc
         << ") => " << classes[i]->cname
         << std::endl;
   }
}

//------------------------------------------------------------------------------
// Register a new class type
//------------------------------------------------------------------------------
unsigned int Object::registerClass(const _Static* const p)
{
   unsigned int idx = 0;
   if (numClasses < MAX_CLASSES) {
      classes[numClasses] = p;
      idx = numClasses;
      numClasses++;
   }
   return idx;
}

//------------------------------------------------------------------------------
// indent() -- indent the output stream by 'ii' spaces
//------------------------------------------------------------------------------
void Object::indent(std::ostream& sout, const int ii) const
{
    for (int i = 0; i < ii; i++) sout << " ";
}


//------------------------------------------------------------------------------
// serialize() -- Sends the contents of an object (important stuff anyway) to
//            the output stream, sout.  Each output line can be indented
//            by 'indent' spaces.  If slotsOnly is true, then only the
//            slots are printed.
//------------------------------------------------------------------------------
std::ostream& Object::serialize(std::ostream& sout, const int, const bool) const
{
   return sout;
}

//==============================================================================
// struct  Object::_Static
//==============================================================================

Object::_Static::_Static(
      const unsigned int ci,
      const char* const cn,
      const char* const fn,
      const SlotTable* const p,
      const _Static* const bs
   ) : classIndex(ci), cname(cn), fname(fn), st(p), bstatic(bs), count(0), mc(0), tc(0)
{
}

Object::_Static& Object::_Static::operator=(const _Static&)
{
   return *this;
}

//------------------------------------------------------------------------------
// Get the Object _Static member
//------------------------------------------------------------------------------
const Object::_Static* Object::getStatic()
{
    return &_static;
}

} // End base namespace
} // End oe namespace

