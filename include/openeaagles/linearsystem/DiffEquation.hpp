
#ifndef __oe_linearsystem_DiffEquation_H__
#define __oe_linearsystem_DiffEquation_H__

#include "openeaagles/linearsystem/ScalerFunc.hpp"

namespace oe {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class:  DiffEquation
//
// Description: Abstract difference equation
// Description: Base class for various Discrete z-domain Transfer Functions.  Core
//              engine is a difference equation that has its coefficients initialized
//              by the various derived classes.
//
//------------------------------------------------------------------------------
class DiffEquation : public ScalerFunc
{
    DECLARE_SUBCLASS(DiffEquation, ScalerFunc)

public:
   DiffEquation();
   DiffEquation(const unsigned int rate);

   virtual double g(const double x) override;

protected:
   virtual void allocateMemory(unsigned int n) override;
   virtual void clearMemory() override;

   // Difference equation coef arrays
   double* pa;    // (a[k] * y[k]) terms; for k = 1 .. n
   double* pb;    // (b[k] * x[k]) terms; for k = 0 .. n

private:
   void initData();  // Initialize our data
};

}
}

#endif
