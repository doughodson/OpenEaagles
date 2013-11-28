//------------------------------------------------------------------------------
// Class: Stack
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Stack_H__
#define __Eaagles_Basic_Stack_H__

#include "openeaagles/basic/List.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class: Stack
// Base class:  Object -> List -> Stack
//
// Description: Stacks of objects.
//
//
// Public methods: Base class public methods, plus ...
//
//      push(Object* object)
//          Pushes the 'object' to the top of the stack.
//
//      Object* pop()
//          Pops the object off the top of the stack.  Empty stacks
//          will return null(0).
//
//------------------------------------------------------------------------------
class Stack : public List {
    DECLARE_SUBCLASS(Stack,List)

public:
    Stack();

    int operator==(const Stack& list) const;
    int operator!=(const Stack& list) const;

    void push(Object* object);
    Object* pop();
};


inline Stack::Stack() : List()
{
}


inline void Stack::push(Object* object)
{
    List::addHead(object);
}

inline Object* Stack::pop()
{
    return List::removeHead();
}


inline int Stack::operator==(const Stack& list) const
{
    const List* s1 = this;
    const List* s2 = &list;
    return *s1 == *s2;
}


inline int Stack::operator!=(const Stack& list) const
{
    const List* s1 = this;
    const List* s2 = &list;
    return *s1 != *s2;
}

} // End Basic namespace
} // End Eaagles namespace

#endif
