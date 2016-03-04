//------------------------------------------------------------------------------
// Class: Float
//------------------------------------------------------------------------------
#ifndef __oe_base_Float_H__
#define __oe_base_Float_H__

#include "openeaagles/base/Number.h"

namespace oe {
namespace basic {

//------------------------------------------------------------------------------
// Class:  Float
// Base class:  Object -> Number -> Float
//
// Description:  Class for float numbers.
//
// Factory name: float
//
//
// Public methods: Base class public methods, plus ...
//
//      Float(const LCreal value)
//          Special constructor that initializes the object to 'value'
//
//      Conversion operators: float() and double()
//          Are C++ equivalents.
//
//      Assignment operators: = +=  -=  *=  /=
//          Are C++ equivalents.
//          Division by zero returns a zero, and sends an error message.
//
//      Binary operators: +  -  *  /
//          Are C++ equivalents.
//          Division by zero returns a zero, and sends an error message.
//
//      Comparison operators: ==  !=  <  <=  >  >=
//          Are C++ equivalents.
//
//      Input/Output stream operators: >>  <<
//          Are C++ equivalents.
//------------------------------------------------------------------------------
class Float : public Number {
    DECLARE_SUBCLASS(Float,Number)

public:
    Float()                                         { STANDARD_CONSTRUCTOR() }
    Float(const LCreal value) : Number(value)       { STANDARD_CONSTRUCTOR() }
    Float(const float value)  : Number(value)       { STANDARD_CONSTRUCTOR() }

    operator float() const;
    operator double() const;

    Float& operator=(const LCreal);

    void operator+=(const Float&);
    void operator+=(const LCreal);

    void operator-=(const Float&);
    void operator-=(const LCreal);

    void operator*=(const Float&);
    void operator*=(const LCreal);

    void operator/=(const Float&);
    void operator/=(const LCreal);
};


//------------------------------------------------------------------------------
// Conversion operators:  float() and double()
//------------------------------------------------------------------------------
inline Float::operator float() const
{
    return Number::getFloat();
}

inline Float::operator double() const
{
    return Number::getDouble();
}


//------------------------------------------------------------------------------
// Assignment operator:  =
//------------------------------------------------------------------------------
// Assign value n to val and return pointer
inline Float& Float::operator=(const LCreal n)
{
   val = n;
   return *this;
}


//------------------------------------------------------------------------------
// Addition operators:  + and +=
//------------------------------------------------------------------------------
inline void Float::operator+=(const Float& n)
{
   val += n.val;
}

inline void Float::operator+=(const LCreal n)
{
   val += n;
}

// Add vals of Float instances n1 & n2 and return the sum
inline LCreal operator+(const Float& n1, const Float& n2)
{
    return n1.getReal() + n2.getReal();
}

// Add value n1 to val of Float n2 and return the sum
inline LCreal operator+(const LCreal n1, const Float& n2)
{
    return n1 + n2.getReal();
}

// Add value n2 to val of Float n1 and return the sum
inline LCreal operator+(const Float& n1, const LCreal n2)
{
    return n1.getReal() + n2;
}


//------------------------------------------------------------------------------
// Subtraction operators:  - and -=
//------------------------------------------------------------------------------
inline void Float::operator-=(const Float& n)
{
   val -= n.val;
}

inline void Float::operator-=(const LCreal n)
{
   val -= n;
}

// Subtract the vals of Float instances n1 & n2 and return the difference
inline LCreal operator-(const Float& n1, const Float& n2)
{
    return n1.getReal() - n2.getReal();
}

// Subtract value n1 from val of Float n2 and return the difference
inline LCreal operator-(const LCreal n1, const Float& n2)
{
    return n1 - n2.getReal();
}

// Subtract value n2 from val of Float n1 and return the difference
inline LCreal operator-(const Float& n1, const LCreal n2)
{
    return n1.getReal() - n2;
}


//------------------------------------------------------------------------------
// Multiplication operators:  * and *=
//------------------------------------------------------------------------------
inline void Float::operator*=(const Float& n)
{
   val *= n.val;
}

inline void Float::operator*=(const LCreal n)
{
   val *= n;
}

// Multiply the vals of Float instances n1 & n2 and return the sum
inline LCreal operator*(const Float& n1, const Float& n2)
{
    return n1.getReal() * n2.getReal();
}

// Multiply value n1 to val of Float n2 and return the sum
inline LCreal operator*(const LCreal n1, const Float& n2)
{
    return n1 * n2.getReal();
}

// Multiply value n2 to val of Float n1 and return the sum
inline LCreal operator*(const Float& n1, const LCreal n2)
{
    return n1.getReal() * n2;
}


//------------------------------------------------------------------------------
// Division operators:  / and /=
//------------------------------------------------------------------------------
inline void Float::operator/=(const Float& n)
{
    if (n.val != 0.0f) {
        val /= n.val;
    }
    else {
        std::cerr << "Float::operator/=(): Divide by zero!" << std::endl;
        val = 0.0f;
    }
}

inline void Float::operator/=(const LCreal n)
{
    if (n != 0.0f) {
        val /= n;
    }
    else {
        std::cerr << "Float::operator/=(): Divide by zero!" << std::endl;
        val = 0.0f;
    }
}

// Divide the val of Float instance n1 by Float instance n2 and return
// the quotient, unless Float instance n2 is equal to zero, then return
// a zero and send an error message
inline LCreal operator/(const Float& n1, const Float& n2)
{
    LCreal n = n1.getReal();
    if (n2.getReal() != 0.0f) {
        n /= n2.getReal();
    }
    else {
        std::cerr << "Float::operator/(): Divide by zero!" << std::endl;
        n = 0.0f;
    }
    return n;
}

// Divide val n1 by Float instance n2 and return the quotient, unless
// Float instance n2 is equal to zero, then return a zero and send an
// error message
inline LCreal operator/(const LCreal n1, const Float& n2)
{
    LCreal n = n1;
    if (n2.getReal() != 0.0f) {
        n /= n2.getReal();
    }
    else {
        std::cerr << "Float::operator/(): Divide by zero!" << std::endl;
        n = 0.0f;
    }
    return n;
}

// Divide val of Float instance n1 by val n2 and return the quotient,
// unless val n2 is equal to zero, then return a zero and send an error message
inline LCreal operator/(const Float& n1, const LCreal n2)
{
    LCreal n = n1.getReal();
    if (n2 != 0.0f) {
        n /= n2;
    }
    else {
        std::cerr << "Float::operator/(): Divide by zero!" << std::endl;
        n = 0.0f;
    }
    return n;
}


//------------------------------------------------------------------------------
// Comparison operators:  ==, !=, <, <=, >, and >=
//------------------------------------------------------------------------------

// Return a true or a false after comparing Float n1.val & n2.val
inline bool operator==(const Float& n1, const Float& n2)
{
   return n1.getReal() == n2.getReal();
}

// Return a true or a false after comparing value n1 and Float n2.val
inline bool operator==(const LCreal n1, const Float& n2)
{
   return n1 == n2.getReal();
}

inline bool operator==(const Float& n1, const LCreal n2)
{
   return n1.getReal() == n2;
}

inline bool operator!=(const Float& n1, const Float& n2)
{
   return n1.getReal() != n2.getReal();
}

inline bool operator!=(const LCreal n1, const Float& n2)
{
   return n1 != n2.getReal();
}

inline bool operator!=(const Float& n1, const LCreal n2)
{
   return n1.getReal() != n2;
}

inline bool operator<(const Float& n1, const Float& n2)
{
   return n1.getReal() < n2.getReal();
}

inline bool operator<(const LCreal n1, const Float& n2)
{
   return n1 < n2.getReal();
}

inline bool operator<(const Float& n1, const LCreal n2)
{
   return n1.getReal() < n2;
}

inline bool operator<=(const Float& n1, const Float& n2)
{
   return n1.getReal() <= n2.getReal();
}

inline bool operator<=(const LCreal n1, const Float& n2)
{
   return n1 <= n2.getReal();
}

inline bool operator<=(const Float& n1, const LCreal n2)
{
   return n1.getReal() <= n2;
}

inline bool operator>(const Float& n1, const Float& n2)
{
   return n1.getReal() > n2.getReal();
}

inline bool operator>(const LCreal n1, const Float& n2)
{
   return n1 > n2.getReal();
}

inline bool operator>(const Float& n1, const LCreal n2)
{
   return n1.getReal() > n2;
}

inline bool operator>=(const Float& n1, const Float& n2)
{
   return n1.getReal() >= n2.getReal();
}

inline bool operator>=(const LCreal n1, const Float& n2)
{
   return n1 >= n2.getReal();
}

inline bool operator>=(const Float& n1, const LCreal n2)
{
   return n1.getReal() >= n2;
}

//------------------------------------------------------------------------------
// input stream operator:  >>
//------------------------------------------------------------------------------
inline std::istream& operator>>(std::istream& sin, Float& n)
{
    LCreal tmp = 0;
    sin >> tmp;
    n.setValue(tmp);
    return sin;
}

//------------------------------------------------------------------------------
// output stream operator:  <<
//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& sout, const Float& n)
{
   sout << n.getReal();
   return sout;
}

} // End basic namespace
} // End oe namespace


#endif
