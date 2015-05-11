
#include "openeaagles/linearSys/FirstOrderTf.h"

namespace Eaagles {
namespace LinearSystem {

//==============================================================================
// Class FirstOrderTf
//==============================================================================
IMPLEMENT_SUBCLASS(FirstOrderTf,"FirstOrderTf")
EMPTY_SLOTTABLE(FirstOrderTf)
EMPTY_SERIALIZER(FirstOrderTf)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
FirstOrderTf::FirstOrderTf()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

FirstOrderTf::FirstOrderTf(const unsigned int ir,
         const LCreal in1, const LCreal in2,
         const LCreal id1, const LCreal id2
         ) : DiffEquation(ir)

{
   STANDARD_CONSTRUCTOR()

   initData();

   n1 = in1;
   n2 = in2;
   d1 = id1;
   d2 = id2;

   initialize();
}

//------------------------------------------------------------------------------
// initData() -- init member data
//------------------------------------------------------------------------------
void FirstOrderTf::initData()
{
   n1 = 0;
   n2 = 0;
   d1 = 0;
   d2 = 0;

   allocateMemory(ORDER);
   clearMemory();

   initialize();
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void FirstOrderTf::copyData(const FirstOrderTf& org, const bool cc)
{
   if (cc) {
      allocateMemory(ORDER);
   }

   BaseClass::copyData(org);

   n1 = org.n1;
   n2 = org.n2;
   d1 = org.d1;
   d2 = org.d2;

   initialize();
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void FirstOrderTf::deleteData()
{
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool FirstOrderTf::isValid() const
{
   unsigned int orderN = 0;
   if (n1 != 0) orderN = 1;

   unsigned int orderD = 0;
   if (d1 != 0) orderD = 1;

   // Valid if we have a update rate and a value for the denominator, and the
   // order of the denominator is greater than or equal the numerator.
   bool valid = (
         n == ORDER &&
         rate > 0 &&
         (d1 != 0 || d2 != 0) &&
         (orderD >= orderN)
      );

   return valid && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// initialize the system
//------------------------------------------------------------------------------
void FirstOrderTf::initialize()
{
   BaseClass::initialize();

   if (isValid()) {
      const LCreal T = 1.0f / static_cast<LCreal>(rate);
      pa[0] = 0;
      pa[1] = -(T*d2 - 2.0f*d1)/(T*d2 + 2.0f*d1);
      pb[0] =  (T*n2 + 2.0f*n1)/(T*d2 + 2.0f*d1);
      pb[1] =  (T*n2 - 2.0f*n1)/(T*d2 + 2.0f*d1);
      for (unsigned int k = 0; k < n; k++) {
         px[k] = x0;
         py[k] = y0;
      }

      if (isMessageEnabled(MSG_DEBUG)) {
      std::cout << "a[1] = " << pa[1] << std::endl;
      std::cout << "b[0] = " << pb[0] << std::endl;
      std::cout << "b[1] = " << pb[1] << std::endl;
      }
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool FirstOrderTf::setN1(const LCreal v)
{
   n1 = v;
   initialize();
   return true;
}

bool FirstOrderTf::setN2(const LCreal v)
{
   n2 = v;
   initialize();
   return true;
}

bool FirstOrderTf::setD1(const LCreal v)
{
   d1 = v;
   initialize();
   return true;
}

bool FirstOrderTf::setD2(const LCreal v)
{
   d2 = v;
   initialize();
   return true;
}

} // End LinearSystem namespace
} // End Eaagles namespace
