//------------------------------------------------------------------------------
// Class:  Number
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Number_H__
#define __Eaagles_Basic_Number_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class: Number
//
// Description:  Base class for number objects (Float, Integer, Boolean, 
//               Decibel and LatLon), numerical operators (Add, Subtract,
//               Multiply and Divide), and various units (see basic/units/*.h)
//
// Slots:
//     value  <Number>  ! Sets the value of this number. (default: 0)
//
//
// Public methods:
//
//    Number(const LCreal value)
//       Special constructor that initializes the number to 'value'
//
//    LCreal getReal()
//    double getDouble()
//    float getFloat()
//    int getInt()
//    bool getBoolean()
//       Return the value of the Number object as a LCreal, double,
//       float, integer, or bool, respectively, regardless of the derived
//       class type of the object.
//
//------------------------------------------------------------------------------
class Number : public Object {
   DECLARE_SUBCLASS(Number,Object)

public:
   Number()                         { val = 0.0;   STANDARD_CONSTRUCTOR() }
   Number(const int value)          { val = static_cast<double>(value); STANDARD_CONSTRUCTOR() }
   Number(const Integer64 value)    { val = static_cast<double>(value); STANDARD_CONSTRUCTOR() }
   Number(const bool value)         { val = (value ? 1.0 : 0.0); STANDARD_CONSTRUCTOR() }
   Number(const LCreal value)       { val = value; STANDARD_CONSTRUCTOR() }
   Number(const float value)        { val = value; STANDARD_CONSTRUCTOR() }

   LCreal getReal() const           { return static_cast<LCreal>(val); }
   double getDouble() const         { return val; }
   float getFloat() const           { return static_cast<float>(val); }
   int getInt() const               { return static_cast<int>(val); }
   Integer64 getInt64() const       { return static_cast<Integer64>(val); }
   bool getBoolean() const          { return (val != 0.0); }

   virtual void setValue(const double nv)   { val = nv; }

   virtual bool setSlotValue(const Number* const svobj);

protected:
   double val;   // value of this object
};

} // End Basic namespace
} // End Eaagles namespace

#endif
