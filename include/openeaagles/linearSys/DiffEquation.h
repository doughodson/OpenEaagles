//------------------------------------------------------------------------------
// Class: DiffEquation
//
// Description: Abstract difference equation
//------------------------------------------------------------------------------
#ifndef __oe_linearSys_DiffEquation_H__
#define __oe_linearSys_DiffEquation_H__

#include "openeaagles/linearSys/ScalerFunc.h"

namespace oe {
namespace linearSys {

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

   LCreal g(const LCreal x) override;

protected:
   void allocateMemory(unsigned int n) override;
   void clearMemory() override;

   // Difference equation coef arrays
   LCreal* pa;    // (a[k] * y[k]) terms; for k = 1 .. n
   LCreal* pb;    // (b[k] * x[k]) terms; for k = 0 .. n

private:
   void initData();  // Initialize our data
};

} // End linearSys namespace
} // End oe namespace

#endif
