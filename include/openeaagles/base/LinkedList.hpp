
#ifndef __oe_base_LinkedList_H__
#define __oe_base_LinkedList_H__

#include "openeaagles/base/Object.hpp"

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: LinkedList<T>
//
// Description: Manages a linked list of pointers to reference counted
//              (i.e., Object-based) instances.
//
// Public members:
//
//      LinkedList(double values[], unsigned int nv)
//      LinkedList(int values[], unsigned int nv)
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
//      <T>* get()
//          Returns a pointer to the object at the head of this list and
//          the object IS REMOVED from the list.  Empty lists will return
//          nullptr.  Ownership of Object is passed to the caller
//          (i.e., this routine does not unref() the object and the
//          caller should not ref() the object).
//
//      put(<T>* obj)
//          Put 'obj' at the end of the list.  The Object, obj, is referenced,
//          ref(), by this routine.
//
//      <T>* getPosition(unsigned int n)
//          Returns a pointer to the n'th object on the list.
//
//      unsigned int getIndex(<T>* obj)
//          Finds the object 'obj' in the list and returns the position
//          index.  If the object is not found, zero is returned.
//
//      unsigned int find(<T>* obj)
//          ### removed -- use getIndex() ###
//
//      addHead(<T>* obj)
//      addTail(<T>* obj)
//          Adds an object to the head (tail) of this list.
//          The Object is referenced, ref(), by these routines.
//
//      bool remove(<T>* obj)
//          Removes 'obj' from this list and true is returned.  If the
//          object 'obj' is not found then false is returned.  The Object
//          is unref() and therefore possibly deleted.
//
//      <T>* removeHead()
//      <T>* removeTail()
//          Removes the object at the head (tail) of this list.  Ownership of
//          Object is passed to the caller (i.e., these routines do not
//          unref() the object and the caller should not ref() the object).
//
//      operators:   =   ==   !=
//          C++ equivalent operators to copy and compare the List with other lists.
//
// (Note: the following functions are used to manage the LinkedList::Items)
//
//      Item* getFirstItem()
//      Item* getLastItem()
//          Return pointers to the first/last LinkedList::Item.  The List::Item
//          member functions below are used to traverse the list.
//
//      addHead(Item* newItem)
//      addTail(Item* newItem)
//          Adds a List::Item to the first or last positions (head or tail)
//          of the list.
//
//      insert(Item* newItem, Item* refItem)
//          Inserts the new LinkedList::Item, newItem, before the reference
//          LinkedList::Item, refItem.
//
//      <T>* remove(Item* oldItem)
//          Finds and removes the LinkedList::Item, oldItem, from the list.
//
//
// LinkedList::Item member functions:
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
//      while (item != nullptr) {
//          <T>* obj = item->getValue();
//          <... code to use the object ...>
//          item = item->getNext();
//      }
//
//------------------------------------------------------------------------------
template <class T>
class LinkedList : public Object
{

//-------------------------------------------------------------------------------------

//   DECLARE_SUBCLASS(LinkedList<T>, Object)

//    typedef BaseType BaseClass;                                                                                            
    public: LinkedList(const LinkedList& org);                                                                                  
    public: virtual ~LinkedList();                                                                                              
    public: virtual LinkedList* clone() const override;                                                                           
    public: LinkedList& operator=(const LinkedList& org);                                                                           
    protected: void copyData(const LinkedList& org, const bool cc = false);                                                       
    protected: void deleteData();                                                                                               
    public: virtual bool isClassType(const std::type_info& type) const override;                                                
    private: static ::oe::base::MetaObject metaObject;                                                                                     
    protected: static const ::oe::base::MetaObject* getMetaObject();                                                                                
    public: static const char* getFactoryName();                                                                                
    public: virtual bool isFactoryName(const char name[]) const override;                                                       
    protected: virtual bool setSlotByIndex(const int slotindex, ::oe::base::Object* const obj) override;                        
    public: static const ::oe::base::SlotTable& getSlotTable();                                                                 
    protected: static const ::oe::base::SlotTable slottable;                                                                    
    private: static const char* slotnames[];                                                                                    
    private: static const int nslots;                                                                                           
    public: virtual std::ostream& serialize(std::ostream& sout, const int i = 0, const bool slotsOnly = false) const override;  

//-------------------------------------------------------------------------------------


public:

   struct Item {
      Item() = default;

      Item* getNext()                  { return next; }
      const Item* getNext() const      { return next; }

      Item* getPrevious()              { return previous; }
      const Item* getPrevious() const  { return previous; }

      T* getValue()                    { return value; }
      const T* getValue() const        { return value; }

      Item* next {};                   // Pointer to next entry
      Item* previous {};               // Pointer to previous entry
      T* value {};                     // Data
   };

public:
   LinkedList();

   bool operator==(const LinkedList<T>& list) const;
   bool operator!=(const LinkedList<T>& list) const;

   bool isEmpty() const                            { return (num == 0); }
   unsigned int entries() const                    { return num; }
   void clear();

   T* get()                                   { return removeHead(); }
   void put(T* obj)                           { if (obj == nullptr) return;  addTail(obj); }

   unsigned int getIndex(const T* const obj) const;

   T* getPosition(const unsigned int n)              { return const_cast<T*>(getPosition1(n)); }
   const T* getPosition(const unsigned int n) const  { return getPosition1(n); }

   void addHead(T* const obj);
   void addTail(T* const obj);

   bool remove(const T* const obj);
   T* removeHead();
   T* removeTail();

   Item* getFirstItem()                { return headP; }
   const Item* getFirstItem() const    { return headP; }

   Item* getLastItem()                 { return tailP; }
   const Item* getLastItem() const     { return tailP; }

   void addHead(Item* newItem);
   void addTail(Item* newItem);
   bool insert(Item* newItem, Item* refItem);
   T* remove(Item* oldItem);

   virtual bool isValid() const override;

private:
   const T* getPosition1(const unsigned int n) const;

   Item* headP {};            // Pointer to head object
   Item* tailP {};            // Pointer to last object

   unsigned int num {};       // Number of list objects
};

//-------------------------------------------------------------------------------------

//#define IMPLEMENT_SUBCLASS(ThisType, FACTORYNAME)  BELOW

                                    
template <class T>
    ::oe::base::MetaObject LinkedList<T>::metaObject(                                               
      typeid(LinkedList).name(), "LinkedList",                   
        &LinkedList::slottable, Object::getMetaObject()                                   
    );                                                                                 


template <class T>
    const ::oe::base::MetaObject* LinkedList<T>::getMetaObject() { return &metaObject; }                


template <class T>
    const char* LinkedList<T>::getFactoryName() { return metaObject.getFactoryName(); }                   

template <class T>
    bool LinkedList<T>::isFactoryName(const char name[]) const                              
    {                                                                                  
        if (name == nullptr) return false;                                             
        if ( std::strcmp(metaObject.getFactoryName(), name) == 0 )  return true;                      
        else return LinkedList<T>::Object::isFactoryName(name);                          
    }                                                                                  

template <class T>
    const ::oe::base::SlotTable& LinkedList<T>::getSlotTable()  { return slottable; }       

template <class T>
    bool LinkedList<T>::isClassType(const std::type_info& type) const                       
    {                                                                                  
        if ( type == typeid(LinkedList<T>) ) return true;                                   
        else return LinkedList<T>::Object::isClassType(type);                            
    }                                                                                  

template <class T>
    LinkedList<T>::~LinkedList() {                                                            
        STANDARD_DESTRUCTOR()                                                          
    }                                                                                

template <class T>
    LinkedList<T>* LinkedList<T>::clone() const                                                
    {                                                                                
        return new LinkedList<T>(*this);                                                  
    }                                                                                

template <class T>
    LinkedList<T>::LinkedList(const LinkedList<T>& org) : Object()                            
    {                                                                                
        STANDARD_CONSTRUCTOR()                                                       
        copyData(org,true);                                                          
    }                                                                                

template <class T>
    LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& org)                               
    {                                                                                
        if (this != &org) copyData(org,false);                                       
        return *this;                                               
    }

//#define EMPTY_SLOTTABLE(ThisType) BELOW

template <class T>
    const char* LinkedList<T>::slotnames[] = { "" };

template <class T>
    const int LinkedList<T>::nslots = 0;                                                        

template <class T>
    const ::oe::base::SlotTable LinkedList<T>::slottable(0, 0, Object::getSlotTable());      

template <class T>
    bool LinkedList<T>::setSlotByIndex(const int si, ::oe::base::Object* const obj)             
    {                                                                                      
        return Object::setSlotByIndex(si,obj);                                          
    }


//-------------------------------------------------------------------------------------

template <class T>
LinkedList<T>::LinkedList() : headP(nullptr), tailP(nullptr), num(0)
{
    STANDARD_CONSTRUCTOR()
}

template <class T>
void LinkedList<T>::copyData(const LinkedList<T>& org, const bool cc)
{
//    BaseClass::copyData(org);   // DDH!!!!
    Object::copyData(org);   // DDH!!!!

    if (cc) {
        headP = nullptr;
        tailP = nullptr;
        num = 0;
    }

    // Clear the old list (if any)
    clear();

    // Copy the new list
    const Item* d = org.getFirstItem();
    for ( ; d != nullptr; d = d->getNext() ) {
        T* p = d->getValue()->clone();
        if (p != nullptr) {
            addTail(p);
            p->unref();     // p is ref() by addTail(), so we can unref();
        }
    }
}

template <class T>
void LinkedList<T>::deleteData()
{
    clear();
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
template <class T>
bool LinkedList<T>::isValid() const
{
    bool ok = Object::isValid();
    for (const Item* d = headP; ok && d != nullptr; d = d->getNext() ) {
        const Object* obj = d->getValue();
        if (obj != nullptr) {
            if (!obj->isValid()) ok = false;
        }
        else ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// clear() -- clear out (or empty) the list
//------------------------------------------------------------------------------
template <class T>
void LinkedList<T>::clear()
{
    // Empty out the list ...
    while (!isEmpty()) {
        T* p = removeHead(); // First remove them
        if (p != nullptr) {
            p->unref();     // and unref() them
        }
    }
    headP = nullptr;
    tailP = nullptr;
    num = 0;
}

//------------------------------------------------------------------------------
// find(T*) -- find an object on the list
//------------------------------------------------------------------------------
template <class T>
unsigned int LinkedList<T>::getIndex(const T* const obj) const
{
    const Item* p = nullptr;
    int idx = 0;
    for (const Item* d = headP; d != nullptr; d = d->getNext() ) {
        idx++;
        if ( obj == d->getValue() )  {
            p = d;
            break;
        }
    }
    return ((p != nullptr) ? idx : 0);
}

//------------------------------------------------------------------------------
// addHead(T*) -- Adds object to the head of the list.
//------------------------------------------------------------------------------
template <class T>
void LinkedList<T>::addHead(T* const obj)
{
    if (obj == nullptr) return;
    const auto d = new Item;
    d->value = obj;
    obj->ref();
    addHead(d);
}

//------------------------------------------------------------------------------
// addTail(T*) -- Adds object to the tail of the list.
//------------------------------------------------------------------------------
template <class T>
void LinkedList<T>::addTail(T* const obj)
{
    if (obj == nullptr) return;
    const auto d = new Item;
    d->value = obj;
    obj->ref();
    addTail(d);
}

//------------------------------------------------------------------------------
// remove(T*) -- Removes object from the list.
//------------------------------------------------------------------------------
template <class T>
bool LinkedList<T>::remove(const T* const obj)
{
    bool ok = false;
    if (obj != nullptr) {
        Item* d = headP;
        for ( ; d != nullptr; d = d->getNext() ) {
            if ( obj == d->getValue() )  break;
        }
        if (d != nullptr) {
           remove(d);
           ok = true;
        }
    }
    if (ok) obj->unref();
    return ok;
}

//------------------------------------------------------------------------------
// removeHead() -- Removes the item at the head of the list.
//                 (Ownership passed to caller -- does not unref())
//------------------------------------------------------------------------------
template <class T>
T* LinkedList<T>::removeHead()
{
    T* p = nullptr;
    if (headP != nullptr) {
        Item* d = headP;
        headP = headP->next;
        p = d->getValue();
        num--;
        if (headP != nullptr) headP->previous = nullptr;
        else tailP = nullptr;
        delete d;
    }
    return p;
}

//------------------------------------------------------------------------------
// removeTail() -- Removes the item at the tail of the list.
//                 (Ownership passed to caller -- does not unref())
//------------------------------------------------------------------------------
template <class T>
T* LinkedList<T>::removeTail()
{
    T* p = nullptr;
    if (tailP != nullptr) {
        Item *d = tailP;
        tailP = tailP->previous;
        p = d->getValue();
        num--;
        if (tailP != nullptr) tailP->next = nullptr;
        else headP = nullptr;
        delete d;
    }
    return p;
}

//------------------------------------------------------------------------------
// insert(Item*) -- insert a new item before 'refItem'.  If 'refItem' is
//                  nullptr, the new item is added to the tail of the list.
//------------------------------------------------------------------------------
template <class T>
bool LinkedList<T>::insert(Item* newItem, Item* refItem)
{
    bool ok = true;
    if (refItem != nullptr) {
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
template <class T>
T* LinkedList<T>::remove(Item* item)
{
    T* value = nullptr;
    if (headP == item)
        value = removeHead();
    else if (tailP == item)
        value = removeTail();
    else if (item != nullptr) {
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
template <class T>
void LinkedList<T>::addHead(Item* item)
{
    item->previous = nullptr;
    item->next = headP;
    if (headP != nullptr) headP->previous = item;
    headP = item;
    if (tailP == nullptr) tailP = item;
    num++;
}

//------------------------------------------------------------------------------
// addTail(Item*) -- Adds the Item to the tail of the list.
//------------------------------------------------------------------------------
template <class T>
void LinkedList<T>::addTail(Item* item)
{
    item->next = nullptr;
    item->previous = tailP;
    if (tailP != nullptr) tailP->next = item;
    tailP = item;
    if (headP == nullptr) headP = item;
    num++;
}

//------------------------------------------------------------------------------
// operator==    Returns true if two lists are equal.
//------------------------------------------------------------------------------
template <class T>
bool LinkedList<T>::operator==(const LinkedList<T>& l) const
{
   if (entries() != l.entries()) return false;

   const Item* tt = getFirstItem();
   const Item* ll = l.getFirstItem();
   while (tt != nullptr) {
      if (tt->getValue() != ll->getValue()) return false;
      tt = tt->getNext();
      ll = ll->getNext();
   }
   return true;
}

//------------------------------------------------------------------------------
// operator!=    Returns true if two lists are NOT equal.
//------------------------------------------------------------------------------
template <class T>
bool LinkedList<T>::operator!=(const LinkedList<T>& l) const
{
    return !(*this == l);
}

//------------------------------------------------------------------------------
// getPosition1() -- Returns the object at the n'th position; w/o next pointer
//------------------------------------------------------------------------------
template <class T>
const T* LinkedList<T>::getPosition1(const unsigned int n) const
{
    if (n < 1 || n > num) return nullptr;
    unsigned int i = 1;
    const Item* p = getFirstItem();
    while (i < n && p != nullptr) {
        p = p->getNext();
        i++;
    }
    if (p != nullptr)
        return p->getValue();
    else
        return nullptr;
}

template <class T>
std::ostream& LinkedList<T>::serialize(std::ostream& sout, const int, const bool) const
{
    std::cout << "{" << std::endl;
    for (const Item* p = getFirstItem(); p != nullptr; p = p->getNext() ) {
        if (p->getValue() != nullptr) p->getValue()->serialize(sout,4);
        else std::cout << "<BAD VALUE>" << std::endl;
    }
    std::cout << "}" << std::endl;
    return sout;
}

}
}

#endif

