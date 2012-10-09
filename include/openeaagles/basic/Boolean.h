//------------------------------------------------------------------------------
// Class:  Boolean
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Boolean_H__
#define __Eaagles_Basic_Boolean_H__

#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  Boolean
// Base class:  Object -> Number -> Boolean
//
// Description:  Class for boolean numbers
//
// Form name: boolean
//
//
// Public methods: Base class public methods, plus ...
//
//      Boolean(const bool flag)
//          Special constructor that initializes the boolean to 'flag'
//
//      bool()
//          Returns value as 'bool' type
//
//      Operators: ! == != && || << >>
//          C++ equivalent.
//
//------------------------------------------------------------------------------
class Boolean : public Number {
    DECLARE_SUBCLASS(Boolean,Number)

public:
    Boolean(const bool num) : Number(num ? 1.0 : 0.0)     { STANDARD_CONSTRUCTOR() }
    Boolean()                                               { STANDARD_CONSTRUCTOR() }

    operator bool() const;

    Boolean& operator=(const bool);
};

//------------------------------------------------------------------------------
// Conversion operator:  bool()
//------------------------------------------------------------------------------

// Return Value
inline Boolean::operator bool() const
{
    return Number::getBoolean();
}


//------------------------------------------------------------------------------
// NOT operator:  !
//------------------------------------------------------------------------------

// Return the opposite value of the value passed in
inline bool operator!(const Boolean& b)
{
    if (b.getBoolean()) return true;
    else return false;
}


//------------------------------------------------------------------------------
// Assignment operator:  =
//------------------------------------------------------------------------------
inline Boolean& Boolean::operator=(const bool b)
{
    val = (b ? 1.0f : 0.0f);
    return *this;
}


//------------------------------------------------------------------------------
// Equal/not-equal operators:  == and !=
//------------------------------------------------------------------------------

// Return a true if the val's of the two Boolean instances are equal
inline bool operator==(const Boolean& b1, const Boolean& b2)
{
    return b1.getBoolean() == b2.getBoolean();
}

// Return a true if the value b1 is equal to val of Boolean instance b2 
inline bool operator==(const bool b1, const Boolean& b2)
{
    return b1 == b2.getBoolean();
}


// Return a true if the value b2 is equal to val of Boolean instance b1
inline bool operator==(const Boolean& b1, const bool b2)
{
    return b1.getBoolean() == b2;
}

// Return a true if the val's of the two Boolean instances are not equal 
inline bool operator!=(const Boolean& b1, const Boolean& b2)
{
    return b1.getBoolean() != b2.getBoolean();
}

// Return a true if the value b1 is not equal to val of Boolean instance b2
inline bool operator!=(const bool b1, const Boolean& b2)
{
    return b1 != b2.getBoolean();
}

// Return a true if the value b2 is not equal to val of Boolean instance b1
inline bool operator!=(const Boolean& b1, const bool b2)
{
    return b1.getBoolean() != b2;
}


//------------------------------------------------------------------------------
// AND and OR operators:  && and ||
//------------------------------------------------------------------------------

// And the val's of the two Boolean instances 
inline bool operator&&(const Boolean& b1, const Boolean& b2)
{
    return b1.getBoolean() && b2.getBoolean();
}

// And the value b1 to val of Boolean instance b2 
inline bool operator&&(const bool b1, const Boolean& b2)
{
    return b1 && b2.getBoolean();
}

// And the value b2 to val of Boolean instance b1 
inline bool operator&&(const Boolean& b1, const bool b2)
{
    return b1.getBoolean() && b2;
}

// Or the val's of the two Boolean instances 
inline bool operator||(const Boolean& b1, const Boolean& b2)
{
    return b1.getBoolean() || b2.getBoolean();
}

// Or the value b1 to val of Boolean instance b2 
inline bool operator||(const bool b1, const Boolean& b2)
{
    return b1 || b2.getBoolean();
}

// Or the value b2 to val of Boolean instance b1 
inline bool operator||(const Boolean& b1, const bool b2)
{
    return b1.getBoolean() || b2;
}


//------------------------------------------------------------------------------
// input stream operator:  >>
//------------------------------------------------------------------------------
inline std::istream& operator>>(std::istream& sin, Boolean& n)
{
    LCreal tmp;
    sin >> tmp;
    if (tmp != 0.0f) tmp = 1.0f;
    else tmp = 0.0f;
    n.setValue(tmp);
    return sin;
}

//------------------------------------------------------------------------------
// output stream operator:  <<
//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& sout, const Boolean& n)
{
    sout << n.getBoolean();
    return sout;
}

} // End Basic namespace
} // End Eaagles namespace

#endif
