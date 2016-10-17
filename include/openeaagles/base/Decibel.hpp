
#ifndef __oe_base_Decibel_H__
#define __oe_base_Decibel_H__

#include "openeaagles/base/Number.hpp"

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class:  Decibel
//
// Description:  Decibel container class.    db = 10 * LOG10(value)
//               Provides methods for handling and operating with decibels.
//               All member functions handle the number in decibels.
//
// Factory name: dB
//
//
// Public methods: Base class public methods, plus ...
//
//      Decibel(const double value)
//          Special constructor that initializes the object to 'value' dB.
//
//      double getNumber()
//          Convert the value from Decibels into the original units.
//
//      Conversion operators: float() and double()
//          Are C++ equivalents. Values are in decibels.
//
//      Assignment operators: = +=  -=
//          Are C++ equivalents.  Values are in decibels.
//
//      Binary operators: +  -
//          Are C++ equivalents.  Values are in decibels.
//
//      Comparison operators: ==  !=  <  <=  >  >=
//          Are C++ equivalents.  Values are in decibels.
//
//      Input/Output stream operators: >>  <<
//          Are C++ equivalents.  Values are in decibels.
//------------------------------------------------------------------------------
class Decibel : public Number
{
    DECLARE_SUBCLASS(Decibel, Number)

public:
    Decibel(const double value);
    Decibel();

    double getValueDB() const                          { return db; }
    virtual void setValueDB(const double v);

    Decibel& operator=(const double);

    void operator+=(const Decibel&);
    void operator+=(const double);

    void operator-=(const Decibel&);
    void operator-=(const double);

    virtual void setValue(const double nv) override;
    virtual bool setSlotValue(const Number* const svobj) override;

private:
    double db;    // value in dBs
};

//------------------------------------------------------------------------------
// Assignment operator:  =
//------------------------------------------------------------------------------
// Assign value n to val and return pointer
inline Decibel& Decibel::operator=(const double n)
{
   setValueDB(n);
   return *this;
}

//------------------------------------------------------------------------------
// Addition operators:  + and +=
//------------------------------------------------------------------------------
inline void Decibel::operator+=(const Decibel& n)
{
   setValueDB(db + n.getValueDB());
}

inline void Decibel::operator+=(const double n)
{
   setValueDB(db + n);
}

// Add vals of Decibel instances n1 & n2 and return the sum
inline double operator+(const Decibel& n1, const Decibel& n2)
{
    return n1.getValueDB() + n2.getValueDB();
}

// Add value n1 to val of Decibel n2 and return the sum
inline double operator+(const double n1, const Decibel& n2)
{
    return n1 + n2.getValueDB();
}

// Add value n2 to val of Decibel n1 and return the sum
inline double operator+(const Decibel& n1, const double n2)
{
    return n1.getValueDB() + n2;
}


//------------------------------------------------------------------------------
// Subtraction operators:  - and -=
//------------------------------------------------------------------------------
inline void Decibel::operator-=(const Decibel& n)
{
   setValueDB(db - n.getValueDB());
}

inline void Decibel::operator-=(const double n)
{
   setValueDB(db - n);
}

// Subtract the vals of Decibel instances n1 & n2 and return the difference
inline double operator-(const Decibel& n1, const Decibel& n2)
{
    return n1.getValueDB() - n2.getValueDB();
}

// Subtract value n1 from val of Decibel n2 and return the difference
inline double operator-(const double n1, const Decibel& n2)
{
    return n1 - n2.getValueDB();
}

// Subtract value n2 from val of Decibel n1 and return the difference
inline double operator-(const Decibel& n1, const double n2)
{
    return n1.getValueDB() - n2;
}


//------------------------------------------------------------------------------
// Comparison operators:  ==, !=, <, <=, >, and >=
//------------------------------------------------------------------------------

// Return a true or a false after comparing Decibel n1.val & n2.val
inline bool operator==(const Decibel& n1, const Decibel& n2)
{
   return n1.getValueDB() == n2.getValueDB();
}

// Return a true or a false after comparing value n1 and Decibel n2.val
inline bool operator==(const double n1, const Decibel& n2)
{
   return n1 == n2.getValueDB();
}

inline bool operator==(const Decibel& n1, const double n2)
{
   return n1.getValueDB() == n2;
}

inline bool operator!=(const Decibel& n1, const Decibel& n2)
{
   return n1.getValueDB() != n2.getValueDB();
}

inline bool operator!=(const double n1, const Decibel& n2)
{
   return n1 != n2.getValueDB();
}

inline bool operator!=(const Decibel& n1, const double n2)
{
   return n1.getValueDB() != n2;
}

inline bool operator<(const Decibel& n1, const Decibel& n2)
{
   return n1.getValueDB() < n2.getValueDB();
}

inline bool operator<(const double n1, const Decibel& n2)
{
   return n1 < n2.getValueDB();
}

inline bool operator<(const Decibel& n1, const double n2)
{
   return n1.getValueDB() < n2;
}

inline bool operator<=(const Decibel& n1, const Decibel& n2)
{
   return n1.getValueDB() <= n2.getValueDB();
}

inline bool operator<=(const double n1, const Decibel& n2)
{
   return n1 <= n2.getValueDB();
}

inline bool operator<=(const Decibel& n1, const double n2)
{
   return n1.getValueDB() <= n2;
}

inline bool operator>(const Decibel& n1, const Decibel& n2)
{
   return n1.getValueDB() > n2.getValueDB();
}

inline bool operator>(const double n1, const Decibel& n2)
{
   return n1 > n2.getValueDB();
}

inline bool operator>(const Decibel& n1, const double n2)
{
   return n1.getValueDB() > n2;
}

inline bool operator>=(const Decibel& n1, const Decibel& n2)
{
   return n1.getValueDB() >= n2.getValueDB();
}

inline bool operator>=(const double n1, const Decibel& n2)
{
   return n1 >= n2.getValueDB();
}

inline bool operator>=(const Decibel& n1, const double n2)
{
   return n1.getValueDB() >= n2;
}

//------------------------------------------------------------------------------
// input stream operator:  >>
//------------------------------------------------------------------------------
inline std::istream& operator>>(std::istream& sin, Decibel& n)
{
    double tmp = 0;
    sin >> tmp;
    n.setValueDB(tmp);
    return sin;
}

//------------------------------------------------------------------------------
// output stream operator:  <<
//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& sout, const Decibel& n)
{
   sout << n.getValueDB();
   return sout;
}

}
}

#endif
