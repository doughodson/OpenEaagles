
#include "openeaagles/linearSys/LimitFunc.h"

namespace oe {
namespace linearSys {

//==============================================================================
// Class LimitFunc
//==============================================================================
IMPLEMENT_SUBCLASS(LimitFunc,"LimitFunc")
EMPTY_SLOTTABLE(LimitFunc)
EMPTY_SERIALIZER(LimitFunc)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
LimitFunc::LimitFunc()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

LimitFunc::LimitFunc(const LCreal ll, const LCreal uu) : ScalerFunc()
{
   STANDARD_CONSTRUCTOR()

   initData();

   lower = ll;
   upper = uu;

   initialize();
}

//------------------------------------------------------------------------------
// initData() -- init member data
//------------------------------------------------------------------------------
void LimitFunc::initData()
{
   lower = 0;
   upper = 0;

   allocateMemory(ORDER);
   clearMemory();
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void LimitFunc::copyData(const LimitFunc& org, const bool cc)
{
   if (cc) {
      // allocate memory before calling BaseClass::copyData()
      allocateMemory(ORDER);
   }

   BaseClass::copyData(org);

   lower = org.lower;
   upper = org.upper;

   initialize();
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void LimitFunc::deleteData()
{
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool LimitFunc::isValid() const
{
   // Valid if we have a master rate and a sample rate
   bool valid = (
         n == ORDER &&
         lower <= upper
      );

   return valid && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// g() is one iteration of the difference equation.
//------------------------------------------------------------------------------
LCreal LimitFunc::g(const LCreal xn)
{
   if (isValid()) {

      // Current input
      px[0] = xn;

      // LimitFunc the input
      LCreal tmp = xn;

      if (tmp > upper) tmp = upper;
      else if (tmp < lower) tmp = lower;

      py[0] = tmp;

      return py[0];
   }
   else {
      // If invalid transfer function, just return the input value
      return xn;
   }
}

//------------------------------------------------------------------------------
// initialize the system
//------------------------------------------------------------------------------
void LimitFunc::initialize()
{
   BaseClass::initialize();

   if (isValid()) {
      // Init the past values
      px[0] = x0;
      py[0] = y0;
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool LimitFunc::setLowerLimit(const LCreal v)
{
   lower = v;
   initialize();
   return true;
}

bool LimitFunc::setUpperLimit(const LCreal v)
{
   upper = v;
   initialize();
   return true;
}

} // End linearSys namespace
} // End oe namespace
