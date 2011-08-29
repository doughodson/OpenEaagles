
#include "openeaagles/linearSys/DiffEquation.h"

namespace Eaagles {
namespace LinearSystem {

//==============================================================================
// Class DiffEquation
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(DiffEquation,"DiffEquation")
EMPTY_SLOTTABLE(DiffEquation)
EMPTY_SERIALIZER(DiffEquation)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
DiffEquation::DiffEquation()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

DiffEquation::DiffEquation(const unsigned int r) : ScalerFunc(r)
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// initData() -- init member data
//------------------------------------------------------------------------------
void DiffEquation::initData()
{
   pa = 0;
   pb = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DiffEquation::copyData(const DiffEquation& org, const bool)
{
   BaseClass::copyData(org);

   // copy data array -- drived classes must have called allocateMemory() prior
   // to calling this copyData() function.
   for (unsigned int i = 0; i < n; i++) {
      pa[i] = org.pa[i];
      pb[i] = org.pb[i];
   }
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void DiffEquation::deleteData()
{
}

//------------------------------------------------------------------------------
// Allocate memory arrays and free any old arrays
//------------------------------------------------------------------------------
void DiffEquation::allocateMemory(unsigned int n0)
{
   if (n0 != n) {
      // Free the old memory arrays
      if (n > 0) {
         delete[] pa;  pa = 0;
         delete[] pb;  pb = 0;
      }

      BaseClass::allocateMemory(n0);

      // Allocate the new space
      if (n0 > 0) {
         pa = new LCreal[n0];
         pb = new LCreal[n0];
      }
   }
}

//------------------------------------------------------------------------------
// Clear the memory arrays
//------------------------------------------------------------------------------
void DiffEquation::clearMemory()
{
   for (unsigned int i = 0; i < n; i++) {
      pa[i] = 0;
      pa[i] = 0;
   }
}

//------------------------------------------------------------------------------
// g() is one iteration of the difference equation.
//------------------------------------------------------------------------------
LCreal DiffEquation::g(const LCreal xn)
{
   if (isValid()) {

      // Shift (age) the past input and output values
      for (unsigned int k = (n-1); k > 0; k--) {
         px[k] = px[k-1];
         py[k] = py[k-1];
      }

      // Current input
      px[0] = xn;

      // The difference equation ... compute the current output
      py[0] = 0;
      for (unsigned int k = 1; k < n; k++) {
         // past outputs
         py[0] += pa[k]*py[k];
      }
      for (unsigned int k = 0; k < n; k++) {
         // Inputs
         py[0] += pb[k]*px[k];
      }

      return py[0];
   }
   else {
      // If invalid transfer function, just return the input value
      return xn;
   }
}

} // End LinearSystem namespace
} // End Eaagles namespace
