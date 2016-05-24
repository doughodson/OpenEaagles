
#include "openeaagles/linearsystem/DiffEquation.h"

namespace oe {
namespace linearsystem {

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
   pa = nullptr;
   pb = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DiffEquation::copyData(const DiffEquation& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // copy data array -- derived classes must have called allocateMemory() prior
   // to calling this copyData() function.
   allocateMemory( org.n );
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
         delete[] pa;  pa = nullptr;
         delete[] pb;  pb = nullptr;
      }

      BaseClass::allocateMemory(n0);

      // Allocate the new space
      if (n0 > 0) {
         pa = new double[n0];
         pb = new double[n0];
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
double DiffEquation::g(const double xn)
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

} // End linearsystem namespace
}
