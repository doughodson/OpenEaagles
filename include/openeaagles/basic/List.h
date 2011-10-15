//------------------------------------------------------------------------------
// Class: List
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_List_H__
#define __Eaagles_Basic_List_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:	List
//
// Description: General purpose list of objects.  The linked list and next-
//              previous pointers are maintained using a List::Item structure.
//
// Public members:
//
//      List(float values[], unsigned int nv)
//      List(int values[], unsigned int nv)
//          Constructors; construct a list of 'nv' Float (Integer)
//          objects each containing a value from 'values'.
//          (See also getNumberList())
//
//      unsigned int entries()
//          Returns the number of entries on this list.
//
//      bool isEmpty()
//          Returns true if entries() is zero.
//
//      clear()
//          Clear, or empty, the list
//
//      Object* get()
//          Returns a pointer to the object at the head of this list and
//          the object IS REMOVED from the list.  Empty lists will return
//          null(0).  Ownership of Object is passed to the caller
//          (i.e., this routine does not unref() the object and the
//          caller should not ref() the object).
//
//      put(Object* obj)
//          Put 'obj' at the end of the list.  The Object, obj, is referenced,
//          ref(), by this routine.
//
//      unsigned int getNumberList(double values[], unsigned int max)
//      unsigned int getNumberList(float values[], unsigned int max)
//      unsigned int getNumberList(int values[], unsigned int max)
//          Fills the array, values, with values from the list.  Positions
//          of the objects on the list correspond to the values stored in
//          the array.  List objects that are not of type Number will
//          have a value of zero.  The maximum number of values stored in
//          the array is set by 'max'.  getNumberList() returns the number
//          of values stored in the array.
//
//      Object* getPosition(unsigned int n)
//          Returns a pointer to the n'th object on the list.
//
//      unsigned int find(Object* obj)
//          Finds the object 'obj' in the list and returns the position
//          index.  If the object is not found,zero is returned.
//
//      unsigned int getIndex(Object* obj)
//          Finds the object 'obj' in the list and returns the position
//          index.  If the object is not found, zero is returned.
//
//      addHead(Object* obj)
//      addTail(Object* obj)
//          Adds an object to the head (tail) of this list.
//          The Object is referenced, ref(), by these routines.
//
//      bool remove(Object* obj)
//          Removes 'obj' from this list and true is returned.  If the
//          object 'obj' is not found then false is returned.  The Object
//          is unref() and therefore possibly deleted.
//
//      Object* removeHead()
//      Object* removeTail()
//          Removes the object at the head (tail) of this list.  Ownership of
//          Object is passed to the caller (i.e., these routines do not
//          unref() the object and the caller should not ref() the object).
//
//      operators:   =   ==   !=
//          C++ equivalent operators to copy and compare the List with other lists.
//
// (Note: the following functions are used to manage the List::Items)
//
//      Item* getFirstItem()
//      Item* getLastItem()
//          Return pointers to the first/last List::Item.  The List::Item
//          member fuctions below are used to traverse the list.
//
//      addHead(Item* newItem)
//      addTail(Item* newItem)
//          Adds a List::Item to the first or last positions (head or tail)
//          of the list.
//
//      insert(Item* newItem, Item* refItem)
//          Inserts the new List::Item, newItem, before the reference
//          List::Item, refItem.
//
//      Object* remove(Item* oldItem)
//          Finds and removes the List::Item, oldItem, from the list.
//
//
// List::Item member functions:
//
//     Item* getNext()
//          Returns the next item in the list.
//
//     Item* getPrevious()
//          Returns the previous item in the list.
//
//     Object* getValue()
//          Returns the item's value: a pointer to the Object.
//
//
//  Example of looping through the list:
//
//      List* list = <some list>
//      List::Item* item = list->getFirstItem();
//      while (item != 0) {
//          Object* obj = item->getValue();
//          <... code to use the object ...>
//          item = item->getNext();
//      }
//
//------------------------------------------------------------------------------
class List : public Object {
   DECLARE_SUBCLASS(List,Object)

public:
   struct Item {
      Item() { next = 0; previous = 0; value = 0; }

      Item* getNext()                  { return next; }
      const Item* getNext() const      { return next; }

      Item* getPrevious()              { return previous; }
      const Item* getPrevious() const  { return previous; }

      Object* getValue()               { return value; }
      const Object* getValue() const   { return value; }

      Item* next;                      // Pointer to next entry
      Item* previous;                  // Pointer to previous entry
      Object* value;                   // Data
   };

public:
   List();
   List(const LCreal values[], const unsigned int nv);
   List(const int values[], const unsigned int nv);

   bool operator==(const List& list) const;
   bool operator!=(const List& list) const;

   bool isEmpty() const                            { return (num == 0); }
   unsigned int entries() const                    { return num; }
   void clear();

   Object* get()                                   { return removeHead(); }
   void put(Object* obj)                           { if (obj == 0) return;  addTail(obj); }

   unsigned int getNumberList(double values[], const unsigned int max) const;
   unsigned int getNumberList(float values[], const unsigned int max) const;
   unsigned int getNumberList(int values[], const unsigned int max) const;

   unsigned int find(const Object* const obj) const;

   unsigned int getIndex(const Object* const obj) const;

   Object* getPosition(const unsigned int n)              { return (Object*) getPosition1(n); }
   const Object* getPosition(const unsigned int n) const  { return getPosition1(n); }

   void addHead(Object* const obj);
   void addTail(Object* const obj);

   bool remove(const Object* const obj);
   Object* removeHead();
   Object* removeTail();

   Item* getFirstItem()                { return headP; }
   const Item* getFirstItem() const    { return headP; }

   Item* getLastItem()                 { return tailP; }
   const Item* getLastItem() const     { return tailP; }

   void addHead(Item* newItem);
   void addTail(Item* newItem);
   bool insert(Item* newItem, Item* refItem);
   Object* remove(Item* oldItem);

   // Object interface
   virtual bool isValid() const;

private:
   const Object* getPosition1(const unsigned int n) const;

   Item* headP;            // Pointer to head object
   Item* tailP;            // Pointer to last object

   unsigned int num;       // Number of list objects

};

} // End Basic namespace
} // End Eaagles namespace

#endif
