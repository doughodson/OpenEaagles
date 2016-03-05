
#include "openeaagles/linearsystem/LagFilter.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/units/Times.h"

namespace oe {
namespace linearsystem {

//==============================================================================
// Class LagFilter
//==============================================================================
IMPLEMENT_SUBCLASS(LagFilter,"LagFilter")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(LagFilter)
    "tau",      //  1: Filer time constant (Time or sec)
END_SLOTTABLE(LagFilter)

// Map slot table to handles
BEGIN_SLOT_MAP(LagFilter)
   ON_SLOT( 1, setSlotTau, base::Time)
   ON_SLOT( 1, setSlotTau, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
LagFilter::LagFilter()
{
   STANDARD_CONSTRUCTOR()

   tau = 1.0;
}

LagFilter::LagFilter(const unsigned int rate, const LCreal t)
                        : FirstOrderTf(rate, 0.0f, 1.0f, t, 1.0f)
{
   STANDARD_CONSTRUCTOR()

   tau = t;
}

// Copy data function
void LagFilter::copyData(const LagFilter& org, const bool)
{
    BaseClass::copyData(org);

    tau = org.tau;
}

EMPTY_DELETEDATA(LagFilter)

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool LagFilter::setTau(const LCreal v)
{
   tau = v;

   setN1(0.0f);
   setN2(1.0f);
   setD1(tau);
   setD2(1.0f);

   initialize();
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool LagFilter::setSlotTau(const base::Time* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      LCreal tsec = base::Seconds::convertStatic(*msg);
      if (tsec > 0) {
         setTau( tsec );
         ok = true;
      }
      else {
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LagFilter::setSlotTau() -- tau must be greater than zero!" << std::endl;
      }
   }
   }
   return ok;
}

bool LagFilter::setSlotTau(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      LCreal tsec = msg->getReal();
      if (tsec > 0) {
         setTau( tsec );
         ok = true;
      }
      else {
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LagFilter::setSlotTau() -- tau must be greater than zero!" << std::endl;
         }
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for LagFilter
//------------------------------------------------------------------------------
base::Object* LagFilter::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& LagFilter::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "tau:   " << getTau() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End linearsystem namespace
} // End oe namespace
