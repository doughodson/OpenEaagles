//------------------------------------------------------------------------------
// ScalerFunc class -- Abstract scaler function
//------------------------------------------------------------------------------
#ifndef __LinearSystem_ScalerFunc_H_FF9998A9_A1EC_487a_9FDD_934A46797364__
#define __LinearSystem_ScalerFunc_H_FF9998A9_A1EC_487a_9FDD_934A46797364__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
   namespace Basic {
      class Frequency;
      class Number;
   }

namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  ScalerFunc
// Base class:  Object -> ScalerFunc
//
// Description: Abstract base class for various scaler functions
//             (e.g., z-domain difference equations, sample&hold, limits, etc).
//
// Notes:
//
// 1) Each iteration is handled by the g() member function.  We're derived from
//    Object and not Component, so we're not using updateTC(), updateData() or
//    reset().  The simple g() function, which you must call at the master rate
//    and with the current X(n) input, will produce the current Y(n) output.
//
// 2) For g() to generate a valid Y(n) output, the object must be able to return
//    true from 'isValid()' and must be called at the master rate.
//
// 3) By default, the system starts at zero.  Use X0 and Y0 to initialize all of
//    the previous (N-k) values of X and Y.
//
// Slots:
//    rate  <Frequency>  Master rate
//    rate  <Number>     Master rate Hz 
//    x0    <Number>     Initial (previous) input value: X(0) (default: 0)
//    y0    <Number>     Initial (previous) output value: Y(0) (default: 0)
//
// GUID: {FF9998A9-A1EC-487a-9FDD-934A46797364}
//------------------------------------------------------------------------------
class ScalerFunc : public Basic::Object {
    DECLARE_SUBCLASS(ScalerFunc,Basic::Object)

public:
   ScalerFunc();
   ScalerFunc(const unsigned int rate);

   // one iteration of the system:  Y(n) = g(X(n))
   virtual LCreal g(const LCreal x1) =0;

   LCreal getX0() const          { return x0; }       // Returns X(0)
   LCreal getY0() const          { return y0; }       // Returns Y
   unsigned int getRate() const  { return rate; } // hz

   // Sets the inital X value, X(0), and all past X values, X(-1), X(-2) ,, X(-k+1)
   virtual bool setX0(const LCreal v);

   // Sets the inital Y value, Y(0), and all past Y values, Y(-1), Y(-2) ,, Y(-k+1)
   virtual bool setY0(const LCreal v);

   // Sets the master update rate (hz)
   virtual bool setRate(const unsigned int v);

   // Set slot functions
   virtual bool setSlotRate(const Basic::Frequency* const msg);
   virtual bool setSlotRate(const Basic::Number* const msg);
   virtual bool setSlotX0(const Basic::Number* const msg);
   virtual bool setSlotY0(const Basic::Number* const msg);

   // Returns true if we're valid (ie, all required parameters have been set)
   virtual bool isValid() const;

protected:
   // Allocate memory arrays -- removes old arrays, if any
   virtual void allocateMemory(unsigned int n);

   // Clear the memory arrays
   virtual void clearMemory();

   // Initialize this control/filter
   virtual void initialize();

   unsigned int n;   // Number coefficents (order)
   LCreal* px;       // X input array
   LCreal* py;       // Y output array

   // Parameters and initial values
   unsigned int rate;
   LCreal x0;
   LCreal y0;

private:
   void initData();  // Initialize our data
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif      /* __LinearSystem_ScalerFunc_H_FF9998A9_A1EC_487a_9FDD_934A46797364__ */
