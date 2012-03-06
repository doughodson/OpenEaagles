#include "openeaagles/basic/List.h"

#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Pair.h"
#include <stdlib.h>

namespace Eaagles {
namespace Basic {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(List,"List")

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
List::List() : headP(0), tailP(0), num(0)
{
    STANDARD_CONSTRUCTOR()
}

List::List(const LCreal values[], const unsigned int nv) : headP(0), tailP(0), num(0)
{
    STANDARD_CONSTRUCTOR()

    // Create Float's for each value and add to the list.
    for (unsigned int i = 0; i < nv; i++) {
        Float* p = new Float(values[i]);
        put(p);
        p->unref(); // ref() by put(), so we can unref().
    }
}

List::List(const int values[], const unsigned int nv) : headP(0), tailP(0), num(0)
{
    STANDARD_CONSTRUCTOR()

    // Create Integer's for each value and add to the list.
    for (unsigned int i = 0; i < nv; i++) {
        Integer* p = new Integer(values[i]);
        put(p);
        p->unref(); // ref() by put(), so we can unref().
    }
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void List::copyData(const List& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        // When called from copy constructor, init these pointers
        headP = 0;
        tailP = 0;
        num = 0;
    }

    // Clear the old list (if any)
    clear();

    // Copy the new list
    const Item* d = org.getFirstItem();
    for ( ; d != 0; d = d->getNext() ) {
        Object* p = d->getValue()->clone();
        if (p != 0) {
            addTail(p);
            p->unref();     // p is ref() by addTail(), so we can unref();
        }
    }
}

void List::deleteData()
{
    clear();
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool List::isValid() const
{
    bool ok = Object::isValid();
    for (const Item* d = headP; ok && d != 0; d = d->getNext() ) {
        const Object* obj = d->getValue();
        if (obj != 0) {
            if (!obj->isValid()) ok = false;
        }
        else ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// clear() -- clear out (or empty) the list
//------------------------------------------------------------------------------
void List::clear()
{
    // Empty out the list ...
    while (!isEmpty()) {
        Object* p = removeHead(); // First remove them
        if (p != 0) {
            p->unref();     // and unref() them
        }
    }
    headP = 0;
    tailP = 0;
    num = 0;
}


//------------------------------------------------------------------------------
// find(Object*) -- find an object on the list
//------------------------------------------------------------------------------
unsigned int List::getIndex(const Object* const obj) const
{
    const Item* p = 0;
    int idx = 0;
    for (const Item* d = headP; d != 0; d = d->getNext() ) {
        idx++;
        if ( obj == d->getValue() )  {
            p = d;
            break;
        }
    }
    return ((p != 0) ? idx : 0);
}

//------------------------------------------------------------------------------
// addHead(Object*) -- Adds an item to the head of the list.
//------------------------------------------------------------------------------
void List::addHead(Object* const obj)
{
    if (obj == 0) return;
    Item* d = new Item;
    d->value = obj;
    obj->ref();
    addHead(d);
}

//------------------------------------------------------------------------------
// addTail(Object*) -- Adds an item to the tail of the list.
//------------------------------------------------------------------------------
void List::addTail(Object* const obj)
{
    if (obj == 0) return;
    Item* d = new Item;
    d->value = obj;
    obj->ref();
    addTail(d);
}

//------------------------------------------------------------------------------
// remove(Object*) -- Removes an object from the list.
//------------------------------------------------------------------------------
bool List::remove(const Object* const obj)
{
    bool ok = false;
    if (obj != 0) {
        Item* d = headP;
        for ( ; d != 0; d = d->getNext() ) {
            if ( obj == d->getValue() )  break;
        }
        if (d != 0) {
           remove(d);
           ok = true;
        } 
    }
    if (ok) obj->unref();
    return ok;
}

//------------------------------------------------------------------------------
// int getNumberList(float* values, int max)
// int getNumberList(int* values, int max)
//      This routine is a method to retrieve an array of Numbers (or
//      Pairs that contain Numbers) from a list.  A maximum
//      of 'max' items from the list are placed in the array 'values'.
//      A zero is placed in 'values' when the item is not a Number.
//      The number of values placed in 'values' is returned by
//      getNumberList().
//------------------------------------------------------------------------------
unsigned int List::getNumberList(double values[], const unsigned int max) const
{
    unsigned int n = 0;
    for (const Item* p = getFirstItem(); p != 0 && n < max; p = p->getNext() ) {
        const Object* p1 = p->getValue();
        const Pair* gp = dynamic_cast<const Pair*>(p1);
        if (gp != 0) {
            // when the item is a Pair, use the object it contains.
            p1 = gp->object();
        }
        const Number* pp = dynamic_cast<const Number*>(p1);
        if (pp != 0) {
            // when we have a number
            values[n++] = pp->getDouble();
        }
        else {
            // when we do not have a number
            values[n++] = 0.0;
        }
    }
    return n;
}

unsigned int List::getNumberList(float values[], const unsigned int max) const
{
    unsigned int n = 0;
    for (const Item* p = getFirstItem(); p != 0 && n < max; p = p->getNext() ) {
        const Object* p1 = p->getValue();
        const Pair* gp = dynamic_cast<const Pair*>(p1);
        if (gp != 0) {
            // when the item is a Pair, use the object it contains.
            p1 = gp->object();
        }
        const Number* pp = dynamic_cast<const Number*>(p1);
        if (pp != 0) {
            // when we have a number
            values[n++] = pp->getFloat();
        }
        else {
            // when we do not have a number
            values[n++] = 0.0f;
        }
    }
    return n;
}


unsigned int List::getNumberList(int values[], const unsigned int max) const
{
    unsigned int n = 0;
    for (const Item* p = getFirstItem(); p != 0 && n < max; p = p->getNext() ) {
        const Object* p1 = p->getValue();
        const Pair* gp = dynamic_cast<const Pair*>(p1);
        if (gp != 0) {
            // when the item is a Pair, use the object it contains.
            p1 = gp->object();
        }
        const Number* pp = dynamic_cast<const Number*>(p1);
        if (pp != 0) {
            // when we have a number
            values[n++] = pp->getInt();
        }
        else {
            // when we do not have a number
            values[n++] = 0;
        }
    }
    return n;
}

//------------------------------------------------------------------------------
// removeHead() -- Removes the item at the head of the list.
//                 (Ownership passed to caller -- does not unref())
//------------------------------------------------------------------------------
Object* List::removeHead()
{
    Object* p = 0;
    if (headP != 0) {
        Item* d = headP;
        headP = headP->next;
        p = d->getValue();
        num--;
        if (headP != 0) headP->previous = 0;
        else tailP = 0;
        delete d;
    }
    return p;
}

//------------------------------------------------------------------------------
// removeTail() -- Removes the item at the tail of the list.
//                 (Ownership passed to caller -- does not unref())
//------------------------------------------------------------------------------
Object* List::removeTail()
{
    Object* p = 0;
    if (tailP != 0) {
        Item *d = tailP;
        tailP = tailP->previous;
        p = d->getValue();
        num--;
        if (tailP != 0) tailP->next = 0;
        else headP = 0;
        delete d;
    }
    return p;
}

//------------------------------------------------------------------------------
// insert(Item*) -- insert a new item before 'refItem'.  If 'refItem' is
//                  null(0), the new item is added to the tail of the list.
//------------------------------------------------------------------------------
bool List::insert(Item* newItem, Item* refItem)
{
    bool ok = true;
    if (refItem != 0) {
        if (refItem == headP) {
            addHead(newItem);
        }
        else {
            newItem->previous = refItem->previous;
            refItem->previous = newItem;
            newItem->previous->next = newItem;
            newItem->next = refItem;
            num++;
        }
    }
    else {
        addTail(newItem);
    }
    return ok;
}

//------------------------------------------------------------------------------
// remove(Item*) -- Removes the Item from the list.
//                 (Ownership passed to caller -- does not unref())
//------------------------------------------------------------------------------
Object* List::remove(List::Item* item)
{
    Object* value = 0;
    if (headP == item)
        value = removeHead();
    else if (tailP == item)
        value = removeTail();
    else if (item != 0) {
        value = item->getValue();
        num--;
        Item* p = item->getPrevious();
        Item* n = item->getNext();
        n->previous = p;
        p->next     = n;
        delete item;
    }
    return value;
}

//------------------------------------------------------------------------------
// addHead(Item*) -- Adds the Item to the head of the list.
//------------------------------------------------------------------------------
void List::addHead(List::Item* item)
{
    item->previous = 0;
    item->next = headP;
    if (headP != 0) headP->previous = item;
    headP = item;
    if (tailP == 0) tailP = item;
    num++;
}

//------------------------------------------------------------------------------
// addTail(Item*) -- Adds the Item to the tail of the list.
//------------------------------------------------------------------------------
void List::addTail(List::Item* item)
{
    item->next = 0;
    item->previous = tailP;
    if (tailP != 0) tailP->next = item;
    tailP = item;
    if (headP == 0) headP = item;
    num++;
}

//------------------------------------------------------------------------------
// operator== 	Returns true if two lists are equal.
//------------------------------------------------------------------------------
bool List::operator==(const List& l) const
{
   if (entries() != l.entries()) return false;

   const Item* tt = getFirstItem();
   const Item* ll = l.getFirstItem();
   while (tt != 0) {
      if (tt->getValue() != ll->getValue()) return false;
      tt = tt->getNext();
      ll = ll->getNext();
   }

   return true;
}

//------------------------------------------------------------------------------
// operator!= 	Returns true if two lists are NOT equal.
//------------------------------------------------------------------------------
bool List::operator!=(const List& l) const
{
    return !(*this == l);
}


//------------------------------------------------------------------------------
// getPosition1() -- Returns the object at the n'th position; w/o next pointer
//------------------------------------------------------------------------------
const Object* List::getPosition1(const unsigned int n) const
{
    if (n < 1 || n > num) return 0;
    unsigned int i = 1;
    const Item* p = getFirstItem();
    while (i < n && p != 0) {
        p = p->getNext();
        i++;
    }
    if (p != 0)
        return p->getValue();
    else
        return 0;
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& List::serialize(std::ostream& sout, const int, const bool) const
{
    std::cout << "{" << std::endl;
    for (const Item* p = getFirstItem(); p != 0; p = p->getNext() ) {
        if (p->getValue() != 0) p->getValue()->serialize(sout,4);
        else std::cout << "<BAD VALUE>" << std::endl;
    }
    std::cout << "}" << std::endl;
    return sout;
}

} // End Basic namespace
} // End Eaagles namespace
