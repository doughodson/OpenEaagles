//------------------------------------------------------------------------------
// Class: Integer
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Integer_H__
#define __Eaagles_Basic_Integer_H__

#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  Integer
// Base class:  Object -> Number -> Integer
//
// Description:  Class for integer numbers.
//
// Slots: None
// Events: None
//
//
// Public methods: Base class public methods, plus ...
//
//      Integer(const int value)
//          Special constructor that initializes the object to 'value'
//
//      Conversion operator: int()
//          Is C++ equivalent.
//
//      Assignment operators: =  +=  -=  *=  /=  %=
//          Are C++ equivalents.
//          Division or Modulus by zero returns a zero, and sends an error message.
//
//      Binary operators: +  -  *  /  %
//          Are C++ equivalents.
//          Division or Modulus by zero returns a zero, and sends an error message.
//
//      Comparison operators: ==  !=  <  <=  >  >=
//          Are C++ equivalents.
//
//      Input/Output stream operators: >>  <<
//          Are C++ equivalents.
//------------------------------------------------------------------------------
class Integer : public Number {
    DECLARE_SUBCLASS(Integer,Number)

public:
    Integer(const int num) : Number(double(num)) { STANDARD_CONSTRUCTOR() }
    Integer()                                      { STANDARD_CONSTRUCTOR() }

    operator int() const;

    Integer& operator=(const int);

    void operator+=(const Integer&);
    void operator+=(const int);

    void operator-=(const Integer&);
    void operator-=(const int);

    void operator*=(const Integer&);
    void operator*=(const int);

    void operator/=(const Integer&);
    void operator/=(const int);

    void operator%=(const Integer&);
    void operator%=(const int);
};


//------------------------------------------------------------------------------
// Conversion operator:  int()
//------------------------------------------------------------------------------
inline Integer::operator int() const
{
    return Number::getInt();
}


//------------------------------------------------------------------------------
// Assignment operator:  =
//------------------------------------------------------------------------------
inline Integer& Integer::operator=(const int n)
{
   val = LCreal( n );
   return *this;
}


//------------------------------------------------------------------------------
// Addition operators:  + and +=
//------------------------------------------------------------------------------
inline void Integer::operator+=(const Integer& n)
{
    val += n.val;
}

inline void Integer::operator+=(const int n)
{
    val += n;
}

inline int operator+(const Integer& n1, const Integer& n2)
{
    return int(n1.getInt() + n2.getInt());
}

inline int operator+(const int n1, const Integer& n2)
{
    return int(n1 + n2.getInt());
}

inline int operator+(const Integer& n1, const int n2)
{
    return int(n1.getInt() + n2);
}

//------------------------------------------------------------------------------
// Subtraction operators:  - and -=
//------------------------------------------------------------------------------
inline void Integer::operator-=(const Integer& n)
{
   val -= n.val;
}

inline void Integer::operator-=(const int n)
{
   val -= n;
}

inline int operator-(const Integer& n1, const Integer& n2)
{
    return int(n1.getInt() - n2.getInt());
}

inline int operator-(const int n1, const Integer& n2)
{
    return int(n1 - n2.getInt());
}

inline int operator-(const Integer& n1, const int n2)
{
    return int(n1.getInt() - n2);
}


//------------------------------------------------------------------------------
// Multiply operators:  * and *=
//------------------------------------------------------------------------------
inline void Integer::operator*=(const Integer& n)
{
   val *= n.val;
}

inline void Integer::operator*=(const int n)
{
   val *= n;
}

inline int operator*(const Integer& n1, const Integer& n2)
{
    return int(n1.getInt() * n2.getInt());
}

inline int operator*(const int n1, const Integer& n2)
{
    return int(n1 * n2.getInt());
}

inline int operator*(const Integer& n1, const int n2)
{
    return int(n1.getInt() * n2);
}


//------------------------------------------------------------------------------
// Divison operators:  / and /=
//
// Divison by zero returns a zero, and sends and error message.
//------------------------------------------------------------------------------
inline void Integer::operator/=(const Integer& n)
{
    if (n.val != 0) {
        val /= n.val;
    }
    else {
        std::cerr << "Integer::operator/=(): Divide by zero!" << std::endl;
        val = 0;
    }
}

inline void Integer::operator/=(const int n)
{
    if (n != 0) {
        val /= n;
    }
    else {
        std::cerr << "Integer::operator/=(): Divide by zero!" << std::endl;
        val = 0;
    }
}

inline int operator/(const Integer& n1, const Integer& n2)
{
    int n = int(n1.getInt());
    if (n2.getInt() != 0) {
        n /= int(n2.getInt());
    }
    else {
        std::cerr << "Integer::operator/(): Divide by zero!" << std::endl;
        n = 0;
    }
    return n;
}

inline int operator/(const int n1, const Integer& n2)
{
    int n = n1;
    if (n2.getInt() != 0) {
        n /= int(n2.getInt());
    }
    else {
        std::cerr << "Integer::operator/(): Divide by zero!" << std::endl;
        n = 0;
    }
    return n;
}

inline int operator/(const Integer& n1, const int n2)
{
    int n = int(n1.getInt());
    if (n2 != 0) {
        n /= n2;
    }
    else {
        std::cerr << "Integer::operator/(): Divide by zero!" << std::endl;
        n = 0;
    }
    return n;
}


//------------------------------------------------------------------------------
// Modulus operators:  % and %=
//
// Modulus by zero returns a zero, and sends and error message.
//------------------------------------------------------------------------------
inline void Integer::operator%=(const Integer& n1)
{
    int n = int(val);
    if (int(n1.val) != 0) {
        n %= int(n1.val);
    }
    else {
        std::cerr << "Modulus by zero!" << std::endl;
        n = 0;
    }
    val = LCreal( n );
}

inline void Integer::operator%=(const int n1)
{
    int n = int(val);
    if (n1 != 0) {
        n %= n1;
    }
    else {
        std::cerr << "Modulus by zero!" << std::endl;
        n = 0;
    }
    val = LCreal( n );
}

inline int operator%(const Integer& n1, const Integer& n2)
{
    int n = n1.getInt();
    if (n2.getInt() != 0) {
        n %= n2.getInt();
    }
    else {
        std::cerr << "Modulus by zero!" << std::endl;
        n = 0;
    }
    return n;
}

inline int operator%(const int n1, const Integer& n2)
{
    int n = n1;
    if (n2.getInt() != 0) {
        n %= n2.getInt();
    }
    else {
        std::cerr << "Modulus by zero!" << std::endl;
        n = 0;
    }
    return n;
}

inline int operator%(const Integer& n1, const int n2)
{
    int n = n1.getInt();
    if (n2 != 0) {
        n %= n2;
    }
    else {
        std::cerr << "Modulus by zero!" << std::endl;
        n = 0;
    }
    return n;
}


//------------------------------------------------------------------------------
// Comparison operators:  ==, !=, <, <=, >, and >=
//------------------------------------------------------------------------------
inline bool operator==(const Integer& n1, const Integer& n2)
{
   return n1.getInt() == n2.getInt();
}

inline bool operator==(const int n1, const Integer& n2)
{
   return n1 == n2.getInt();
}

inline bool operator==(const Integer& n1, const int n2)
{
   return n1.getInt() == n2;
}

inline bool operator!=(const Integer& n1, const Integer& n2)
{
   return n1.getInt() != n2.getInt();
}

inline bool operator!=(const int n1, const Integer& n2)
{
   return n1 != n2.getInt();
}

inline bool operator!=(const Integer& n1, const int n2)
{
   return n1.getInt() != n2;
}

inline bool operator<(const Integer& n1, const Integer& n2)
{
   return n1.getInt() < n2.getInt();
}

inline bool operator<(const int n1, const Integer& n2)
{
   return n1 < n2.getInt();
}

inline bool operator<(const Integer& n1, const int n2)
{
   return n1.getInt() < n2;
}

inline bool operator<=(const Integer& n1, const Integer& n2)
{
   return n1.getInt() <= n2.getInt();
}

inline bool operator<=(const int n1, const Integer& n2)
{
   return n1 <= n2.getInt();
}

inline bool operator<=(const Integer& n1, const int n2)
{
   return n1.getInt() <= n2;
}

inline bool operator>(const Integer& n1, const Integer& n2)
{
   return n1.getInt() > n2.getInt();
}

inline bool operator>(const int n1, const Integer& n2)
{
   return n1 > n2.getInt();
}

inline bool operator>(const Integer& n1, const int n2)
{
   return n1.getInt() > n2;
}

inline bool operator>=(const Integer& n1, const Integer& n2)
{
   return n1.getInt() >= n2.getInt();
}

inline bool operator>=(const int n1, const Integer& n2)
{
   return n1 >= n2.getInt();
}

inline bool operator>=(const Integer& n1, const int n2)
{
   return n1.getInt() >= n2;
}

//------------------------------------------------------------------------------
// input stream operator:  >>
//------------------------------------------------------------------------------
inline std::istream& operator>>(std::istream& sin, Integer& n)
{
    int tmp = 0;
    sin >> tmp;
    n.setValue(LCreal(tmp));
    return sin;
}

//------------------------------------------------------------------------------
// output stream operator:  <<
//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& sout, const Integer& n)
{
   sout << n.getInt();
   return sout;
}

} // End Basic namespace
} // End Eaagles namespace

#endif
