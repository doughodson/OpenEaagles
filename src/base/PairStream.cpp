#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"

#include <cstdlib>

namespace oe {
namespace base {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(PairStream, "PairStream")

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
PairStream::PairStream()
{
    STANDARD_CONSTRUCTOR()
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
EMPTY_COPYDATA(PairStream)
EMPTY_DELETEDATA(PairStream)


//------------------------------------------------------------------------------
// findByType() -- find a Pair by object type
//------------------------------------------------------------------------------
Pair* PairStream::findByType(const std::type_info& type)
{
    Pair* p = nullptr;
    const Item* item = getFirstItem();
    while (item != nullptr && p == nullptr) {
        Pair* pair = const_cast<Pair*>(static_cast<const Pair*>(item->getValue()));
        if (pair->object()->isClassType(type)) p = pair;
        item = item->getNext();
    }
    return p;
}

const Pair* PairStream::findByType(const std::type_info& type) const
{
    const Pair* p = nullptr;
    const Item* item = getFirstItem();
    while (item != nullptr && p == nullptr) {
        const Pair* pair = static_cast<const Pair*>(item->getValue());
        if (pair->object()->isClassType(type)) p = pair;
        item = item->getNext();
    }
    return p;
}


//------------------------------------------------------------------------------
// findByName() -- find a Pair by name
//------------------------------------------------------------------------------
Pair* PairStream::findByName(const char* const slotname)
{
    Pair* p = nullptr;
    if (slotname != nullptr) {
        const Item* item = getFirstItem();
        while (item != nullptr && p == nullptr) {
            Pair* pair = const_cast<Pair*>(static_cast<const Pair*>(item->getValue()));
            if ( *(pair->slot()) == slotname ) p = pair;
            item = item->getNext();
        }
    }
    return p;
}

const Pair* PairStream::findByName(const char* const slotname) const
{
    const Pair* p = nullptr;
    if (slotname != nullptr) {
        const Item* item = getFirstItem();
        while (item != nullptr && p == nullptr) {
            const Pair* pair = static_cast<const Pair*>(item->getValue());
            if ( *(pair->slot()) == slotname ) p = pair;
            item = item->getNext();
        }
    }
    return p;
}


//------------------------------------------------------------------------------
// findName() -- Finds the name associated with an object.
// Returns a copy, so unref() the Identifier when finished.
//------------------------------------------------------------------------------
const Identifier* PairStream::findName(const Object* const obj) const
{
    const Identifier* p = nullptr;
    if (obj != nullptr) {
        const Item* item = getFirstItem();
        while (item != nullptr && p == nullptr) {
            const Pair* pair = static_cast<const Pair*>(item->getValue());
            if (pair->object() == obj) {
                p = static_cast<Identifier*>(pair->slot()->clone());
            }
            item = item->getNext();
        }
    }
    return p;
}


//------------------------------------------------------------------------------
// Comparison operators -- use the List operators
//------------------------------------------------------------------------------
bool PairStream::operator==(const PairStream& stream) const
{
    const List* s1 = this;
    const List* s2 = &stream;
    return *s1 == *s2;
}

bool PairStream::operator!=(const PairStream& stream) const
{
    const List* s1 = this;
    const List* s2 = &stream;
    return *s1 != *s2;
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& PairStream::serialize(std::ostream& sout, const int i, const bool) const
{
    const Item* item = getFirstItem();
    while (item != nullptr) {
        const Pair* pair = static_cast<const Pair*>(item->getValue());
        indent(sout,i);
        pair->serialize(sout,i);
        item = item->getNext();
    }
    return sout;
}

}
}
