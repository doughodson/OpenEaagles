
#include "openeaagles/linearsystem/SecondOrderTf.h"

namespace oe {
namespace linearsystem {

//==============================================================================
// Class SecondOrderTf
//==============================================================================
IMPLEMENT_SUBCLASS(SecondOrderTf,"SecondOrderTf")
EMPTY_SLOTTABLE(SecondOrderTf)
EMPTY_SERIALIZER(SecondOrderTf)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SecondOrderTf::SecondOrderTf()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

SecondOrderTf::SecondOrderTf(const unsigned int ir,
         const LCreal in1, const LCreal in2, const LCreal in3,
         const LCreal id1, const LCreal id2, const LCreal id3
         ) : DiffEquation(ir)

{
   STANDARD_CONSTRUCTOR()

   initData();

   n1 = in1;
   n2 = in2;
   n3 = in3;
   d1 = id1;
   d2 = id2;
   d3 = id3;

   initialize();
}

//------------------------------------------------------------------------------
// initData() -- init member data
//------------------------------------------------------------------------------
void SecondOrderTf::initData()
{
   n1 = 0;
   n2 = 0;
   n3 = 0;
   d1 = 0;
   d2 = 0;
   d3 = 0;

   allocateMemory(ORDER);
   clearMemory();
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SecondOrderTf::copyData(const SecondOrderTf& org, const bool cc)
{
   if (cc) {
      allocateMemory(ORDER);
   }

   BaseClass::copyData(org);

   n1 = org.n1;
   n2 = org.n2;
   n3 = org.n3;
   d1 = org.d1;
   d2 = org.d2;
   d3 = org.d3;

   initialize();
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void SecondOrderTf::deleteData()
{
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool SecondOrderTf::isValid() const
{
   unsigned int orderN = 0;
   if (n1 != 0) orderN = 2;
   else if (n2 != 0) orderN = 1;

   unsigned int orderD = 0;
   if (d1 != 0) orderD = 2;
   else if (d2 != 0) orderD = 1;

   // Valid if we have a update rate and a value for the denominator, and the
   // order of the denominator is greater than or equal the numerator.
   bool valid = (
         n == ORDER &&
         rate > 0 &&
         (d1 != 0 || d2 != 0 || d3 != 0)
         && (orderD >= orderN)
      );

   return valid && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// initialize the system
//------------------------------------------------------------------------------
void SecondOrderTf::initialize()
{
   BaseClass::initialize();

   if (isValid()) {
      // ##### IN-WORK #####
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool SecondOrderTf::setN1(const LCreal v)
{
   n1 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setN2(const LCreal v)
{
   n2 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setN3(const LCreal v)
{
   n3 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setD1(const LCreal v)
{
   d1 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setD2(const LCreal v)
{
   d2 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setD3(const LCreal v)
{
   d3 = v;
   initialize();
   return true;
}

} // End linearsystem namespace
} // End oe namespace
