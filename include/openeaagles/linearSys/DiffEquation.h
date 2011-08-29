//------------------------------------------------------------------------------
// DiffEquation class -- Abstract Difference equation
//------------------------------------------------------------------------------
#ifndef __LinearSystem_DiffEquation_H_91F28FCB_CF81_4452_A622_F28CE184F54A__
#define __LinearSystem_DiffEquation_H_91F28FCB_CF81_4452_A622_F28CE184F54A__

#include "openeaagles/linearSys/ScalerFunc.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  DiffEquation
// Base class:  Object -> ScalerFunc -> DiffEquation
//
// Description: Base class for various Discrete z-domain Transfer Functions.  Core
//              engine is a differnece equation that has it's coefficents initialized
//              by the various derived classes.
//
// GUID: {91F28FCB-CF81-4452-A622-F28CE184F54A}
//------------------------------------------------------------------------------
class DiffEquation : public ScalerFunc {
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

#endif      /* __LinearSystem_DiffEquation_H_91F28FCB_CF81_4452_A622_F28CE184F54A__ */
