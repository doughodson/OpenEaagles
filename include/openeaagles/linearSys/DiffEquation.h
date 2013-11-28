//------------------------------------------------------------------------------
// Class: DiffEquation
//
// Description: Abstract difference equation
//------------------------------------------------------------------------------
#ifndef __Eaagles_LinearSystem_DiffEquation_H__
#define __Eaagles_LinearSystem_DiffEquation_H__

#include "openeaagles/linearSys/ScalerFunc.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  DiffEquation
//
// Description: Base class for various Discrete z-domain Transfer Functions.  Core
//              engine is a difference equation that has its coefficients initialized
//              by the various derived classes.
//
//------------------------------------------------------------------------------
class DiffEquation : public ScalerFunc
{
    DECLARE_SUBCLASS(DiffEquation,ScalerFunc)

public:
   DiffEquation();
   DiffEquation(const unsigned int rate);

   // ScalerFunc class interface
   virtual LCreal g(const LCreal x);

protected:
   // ScalerFunc class protected interface
   virtual void allocateMemory(unsigned int n);
   virtual void clearMemory();

   // Difference equation coef arrays
   LCreal* pa;    // (a[k] * y[k]) terms; for k = 1 .. n
   LCreal* pb;    // (b[k] * x[k]) terms; for k = 0 .. n

private:
   void initData();  // Initialize our data
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif
